/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: PhysicsSystem.cpp 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include <pch/precompiled.h>

#include "physics/core/PhysicsSystem.h"
#include "physics/util/Geometry.h"

#include "physics/dynamics/RigidBody.h"
#include "physics/colliders/Collider.h"
#include "physics/colliders/CollisionMgr.h"
#include "physics/colliders/BoxCollider.h"
#include "physics/colliders/SphereCollider.h"
#include "physics/colliders/CapsuleCollider.h"

#include "core/engine/factory/Factory.h"
#include "core/engine/core/Engine.h"
#include "core/types/space/Space.h"
#include "core/types/object/Object.h"
#include "core/types/message/Message.h"

#include "Physics/dynamics/effects/ConstantEffect.h"
#include "Physics/dynamics/effects/GravityEffect.h"
#include "Physics/dynamics/effects/DragEffect.h"

#include "Editor/InGameEditor/InGameEditor.h"


namespace
{
  using namespace Physics;

  static RayCastMgr *s_RayCastMgr = nullptr;
  static JointMgr *s_JointMgr = nullptr;
}

namespace Physics
{
  PhysicsSystem *PHYSICS = nullptr;
  PhysicsSystem::TSpaceColliderVec PhysicsSystem::s_Colliders;

  PhysicsSystem::PhysicsSystem(void)
    : ISystem()
    , m_bFrameStepping(false)
    , m_bCanStep(false)
    , m_nIterations(7)
    , m_CollisionMgr()
    , m_RayCastMgr()
    , m_JointMgr()
  {
    // update the global pointer
    PHYSICS = this;
    s_RayCastMgr = &m_RayCastMgr;
    s_JointMgr = &m_JointMgr;
  }

  PhysicsSystem::~PhysicsSystem(void)
  {
    PHYSICS = nullptr;
  }

  void PhysicsSystem::Initialize(void)
  {
    // The CollisionMgr SetCollisionCallbacks defines what happens when
    // collisions enter, continue, and exit. I am defining the callbacks
    // right here as lambdas. Each one sends a different type of message
    // through the Engine's messaging system.
    m_CollisionMgr.SetCollisionCallbacks(
      [](RigidBody* a, RigidBody* b)->void // Send message on CollisionEnter
      { 
        Core::ENGINE->SystemMessage(Core::CollisionEnterMessage(a->owner, b->owner));
      }, 
      [](RigidBody* a, RigidBody* b)->void // Send message on Collision
      { 
        Core::ENGINE->SystemMessage(Core::CollisionMessage(a->owner, b->owner));
      }, 
      [](RigidBody* a, RigidBody* b)->void  // Send message on CollisionExit
      { 
        Core::ENGINE->SystemMessage(Core::CollisionExitMessage(a->owner, b->owner));
      }
    );
  }

  void PhysicsSystem::RegisterComponents(void)
  {
    REGISTER_COMPONENT(RigidBody, RigidBody);
    REGISTER_COMPONENT(BoxCollider, BoxCollider);
    REGISTER_COMPONENT(SphereCollider, SphereCollider);
    REGISTER_COMPONENT(CapsuleCollider, CapsuleCollider);
    REGISTER_COMPONENT(GravityEffect, GravityEffect);
    REGISTER_COMPONENT(ConstantEffect, ConstantEffect);
    REGISTER_COMPONENT(DragEffect, DragEffect);
  }

  std::string PhysicsSystem::GetName(void) const
  {
    return "Physics";
  }

  void PhysicsSystem::Update(float fDelta)
  {
    if (!InGameEditor::IsEditorActive())
    {
      static float fAccumulator = 0.f;
      static float dt = (1.f / 60.f);

      fAccumulator += fDelta;
      fAccumulator = Clamp(fAccumulator, 0.f, (1.f / 30.f));

      while (fAccumulator >= dt)
      {
        Step(dt);
        fAccumulator -= dt;
      }
    }
  }

  void PhysicsSystem::Step(float dt)
  {
    UpdateColliderMap();
    ClearContacts();
    RunBroadPhase();
    RunNarrowPhase();

    if (m_bFrameStepping)
    {
      if (m_bCanStep)
      {
        IntegrateAcceleration(dt);
        ResolveCollisions(dt);
        IntegrateVelocity(dt);

        ResetAcceleration();
        
        CacheContacts();
        ManageCollisionMessages();
        m_bCanStep = false;
      }
    }
    else
    {
      IntegrateAcceleration(dt);
      ResolveCollisions(dt);
      IntegrateVelocity(dt);

      ResetAcceleration();

      CacheContacts();
      ManageCollisionMessages();
    }
  }

  void PhysicsSystem::ClearContacts(void)
  {
    m_CollisionMgr.ClearContacts();
  }

  void PhysicsSystem::RunBroadPhase(void)
  {

  }

  void PhysicsSystem::RunNarrowPhase(void)
  {
    // run through every space with colliders in it, do an n^2 check between
    // every dynamic object, then check every dynamic object to every static
    // object in the same space
    for each(auto sp in s_Colliders)
    {
      Core::GameSpace *pSpace = sp.first;
      if (!pSpace)
        continue;

      TColliderPair &colliders = sp.second;
      TColliderVec &dynamicObjects = colliders.first;
      TColliderVec &staticObjects = colliders.second;
      
      for (TColliderVec::iterator it = dynamicObjects.begin(); it != dynamicObjects.end(); ++it)
      {
        // do an n^2 check with all other dynamic objects, start at nIdx + 1
        if (*it == nullptr)
          continue;

        for (auto jt = it;;)
        {
          ++jt;
          if (jt == dynamicObjects.end())
            break;

          if (*jt == nullptr || it == jt)
            continue;

          CheckCollision(*it, *jt);
        }
      }

      // now check every dynamic object against every static object
      for (TColliderVec::iterator it = dynamicObjects.begin(); it != dynamicObjects.end(); ++it)
      {
        if (*it == nullptr)
          continue;

        for (TColliderVec::iterator jt = staticObjects.begin(); jt != staticObjects.end(); ++jt)
        {
          if (*jt == nullptr)
            continue;

          CheckCollision(*it, *jt);
        }
      }
    }
  }

  void PhysicsSystem::IntegrateAcceleration(float dt)
  {
    // go through every space
    std::vector<Core::GameSpace*>& Spaces = Core::ENGINE->Spaces();
    for each(auto sp in Spaces)
    {
      Base::ObjectAllocator *Bodies = sp->GetComponents(eRigidBody);

      for (auto it = Bodies->begin<RigidBody>(); it != Bodies->end<RigidBody>(); ++it)
      {
        if (!it)
          continue;

        it->IntegrateAcceleration(dt);
      }
    }
  }

  void PhysicsSystem::ResolveCollisions(float dt)
  {
    // prestep collisions
    m_CollisionMgr.PrestepCollisionResolver();

    m_JointMgr.Step(dt);

    // perform impulses on collisions
    m_CollisionMgr.ResolveCollisions(dt, m_nIterations);

    m_JointMgr.ApplyImpulse(m_nIterations);
  }
  
  void PhysicsSystem::IntegrateVelocity(float dt)
  {
    // go through every space
    std::vector<Core::GameSpace*>& Spaces = Core::ENGINE->Spaces();
    for each(auto sp in Spaces)
    {
      Base::ObjectAllocator *Bodies = sp->GetComponents(eRigidBody);

      for (auto it = Bodies->begin<RigidBody>(); it != Bodies->end<RigidBody>(); ++it)
      {
        if (!it)
          continue;

        it->IntegrateVelocity(dt);
      }
    }
    
  }

  void PhysicsSystem::ResetAcceleration(void)
  {
    std::vector<Core::GameSpace*>& Spaces = Core::ENGINE->Spaces();
    for each(auto sp in Spaces)
    {
      Base::ObjectAllocator *Bodies = sp->GetComponents(eRigidBody);

      for (auto it = Bodies->begin<RigidBody>(); it != Bodies->end<RigidBody>(); ++it)
      {
        if (!it)
          continue;

        it->m_Acceleration = float4();
        it->m_AngularAcceleration = float4();
      }
    }
  }

  void PhysicsSystem::CacheContacts(void)
  {
    m_CollisionMgr.CacheContacts();
  }

  void PhysicsSystem::ManageCollisionMessages(void)
  {
    m_CollisionMgr.ManageCollisionMessages();
  }

  void PhysicsSystem::RemoveRigidBodyMessages(RigidBody* pRigidBody)
  {
    m_CollisionMgr.RemoveRigidBodyMessages(pRigidBody);
  }

  void PhysicsSystem::ClearRigidBodyMessages()
  {
    m_CollisionMgr.ClearRigidBodyMessages();
  }

	void PhysicsSystem::CheckCollision(Core::GameComponent *pColliderA, Core::GameComponent *pColliderB)
  {
    m_CollisionMgr.Collide(pColliderA, pColliderB);
  }

  bool PhysicsSystem::IsFrameStepping(void) const
  {
    return m_bFrameStepping;
  }

  void PhysicsSystem::SetFrameStepping(bool bFrameStepping)
  {
    m_bFrameStepping = bFrameStepping;
  }

  void PhysicsSystem::SetCanStep(bool bCanStep)
  {
    m_bCanStep = bCanStep;
  }

  bool PhysicsSystem::CastRay(float4 const &Start, float4 const &Direction,
    Core::GameComponent *pCollider, float fMaxDistance)
  {
    if (!s_RayCastMgr)
      return false;

    Ray newRay;
    newRay.m_Start = Start;
    newRay.m_Direction = Direction;
    RayCastResult result;
    return s_RayCastMgr->CastRay(newRay, pCollider, result, fMaxDistance);
  }

  bool PhysicsSystem::CastRay(Ray &ray, Core::GameComponent *pCollider, float fMaxDistance)
  {
    if (!s_RayCastMgr)
      return false;

    RayCastResult result;
    return s_RayCastMgr->CastRay(ray, pCollider, result, fMaxDistance);
  }

  bool PhysicsSystem::CastRay(float4 const &Start, float4 const &Direction,
    Core::GameComponent *pCollider, RayCastResult &result, float fMaxDistance)
  {
    if (!s_RayCastMgr)
      return false;

    Ray newRay;
    newRay.m_Start = Start;
    newRay.m_Direction = Direction;
    return s_RayCastMgr->CastRay(newRay, pCollider, result, fMaxDistance);
  }

  bool PhysicsSystem::CastRay(Ray &ray, RayCastResult &result, 
    Core::GameComponent *pCollider, float fMaxDistance)
  {
    if (!s_RayCastMgr)
      return false;

    return s_RayCastMgr->CastRay(ray, pCollider, result, fMaxDistance);
  }

  bool PhysicsSystem::CastRay(Core::GameSpace *pSpace, float4 const &Start, 
    float4 const &Direction, RayCastResult &result, float fMaxDistance)
  {
    if (!s_RayCastMgr)
      return false;

    Ray newRay;
    newRay.m_Start = Start;
    newRay.m_Direction = Direction;

    // grab my pair of dynamic and static colliders from the space
    TColliderPair &objects = s_Colliders[pSpace];
    bool dynamicResult = s_RayCastMgr->CastRay(newRay, objects.first, result, fMaxDistance);
    bool staticResult = s_RayCastMgr->CastRay(newRay, objects.second, result, fMaxDistance);
    return dynamicResult || staticResult;
  }

  bool PhysicsSystem::CastRay(Core::GameSpace *pSpace, Ray &ray, 
    RayCastResult &result, float fMaxDistance)
  {
    if (!s_RayCastMgr)
      return false;

    TColliderPair &objects = s_Colliders[pSpace];
    bool dynamicResult = s_RayCastMgr->CastRay(ray, objects.first, result, fMaxDistance);
    bool staticResult = s_RayCastMgr->CastRay(ray, objects.second, result, fMaxDistance);
    return dynamicResult || staticResult;
  }

  bool PhysicsSystem::CastRayStatic(Core::GameSpace *pSpace, float4 const &Start, 
    float4 const &Direction, RayCastResult &result, float fMaxDistance)
  {
    if (!s_RayCastMgr)
      return false;

    Ray newRay;
    newRay.m_Start = Start;
    newRay.m_Direction = Direction;

    TColliderPair &objects = s_Colliders[pSpace];
    return s_RayCastMgr->CastRay(newRay, objects.second, result, fMaxDistance);
  }

  bool PhysicsSystem::CastRayStatic(Core::GameSpace *pSpace, Ray &ray, 
    RayCastResult &result, float fMaxDistance)
  {
    if (!s_RayCastMgr)
      return false;

    TColliderPair &objects = s_Colliders[pSpace];
    return s_RayCastMgr->CastRay(ray, objects.second, result, fMaxDistance);
  }

  bool PhysicsSystem::CastRayFiltered(Core::GameSpace *pSpace, float4 const &Start,
    float4 const &Direction, RayCastResult &result,
    std::vector<Core::GameComponent*> const &filterList, float fMaxDistance)
  {
    if (!s_RayCastMgr)
      return false;

    Ray newRay;
    newRay.m_Start = Start;
    newRay.m_Direction = Direction;

    TColliderPair &objects = s_Colliders[pSpace];
    bool dynamicResult = s_RayCastMgr->CastRay(newRay, objects.first, result, fMaxDistance, filterList);
    bool staticResult = s_RayCastMgr->CastRay(newRay, objects.second, result, fMaxDistance, filterList);
    return dynamicResult || staticResult;
  }

  bool PhysicsSystem::CastRayFiltered(Core::GameSpace *pSpace, Ray &ray,
    RayCastResult &result, std::vector<Core::GameComponent*> const &filterList,
    float fMaxDistance)
  {
    if (!s_RayCastMgr)
      return false;

    TColliderPair &objects = s_Colliders[pSpace];
    bool dynamicResult = s_RayCastMgr->CastRay(ray, objects.first, result, fMaxDistance, filterList);
    bool staticResult = s_RayCastMgr->CastRay(ray, objects.second, result, fMaxDistance, filterList);
    return dynamicResult || staticResult;
  }

  bool PhysicsSystem::CastRayFilteredStatic(Core::GameSpace *pSpace, 
    float4 const &Start, float4 const &Direction, RayCastResult &result, 
    std::vector<Core::GameComponent*> const &filterList, float fMaxDistance /* = FLT_MAX */)
  {
    if (!s_RayCastMgr)
      return false;

    Ray newRay;
    newRay.m_Start = Start;
    newRay.m_Direction = Direction;

    TColliderPair &objects = s_Colliders[pSpace];
    return s_RayCastMgr->CastRay(newRay, objects.second, result, fMaxDistance, filterList);
  }

  bool PhysicsSystem::CastRayFilteredStatic(Core::GameSpace *pSpace, Ray &ray, 
    RayCastResult &result, std::vector<Core::GameComponent*> const &filterList, 
    float fMaxDistance /* = FLT_MAX */)
  {
    if (!s_RayCastMgr)
      return false;

    TColliderPair &objects = s_Colliders[pSpace];
    return s_RayCastMgr->CastRay(ray, objects.second, result, fMaxDistance, filterList);
  }

  unsigned PhysicsSystem::AddJoint(Joint *pJoint)
  {
    if (!s_JointMgr)
      return static_cast<unsigned>(-1);

    return s_JointMgr->AddJoint(pJoint);
  }

  Joint const *PhysicsSystem::GetConstJoint(unsigned nIdx)
  {
    if (!s_JointMgr)
      return nullptr;

    return s_JointMgr->GetJoint(nIdx);
  }

  Joint *PhysicsSystem::GetJoint(unsigned nIdx)
  {
    if (!s_JointMgr)
      return nullptr;

    return s_JointMgr->GetJoint(nIdx);
  }

  bool PhysicsSystem::RemoveJoint(unsigned nIdx)
  {
    if (!s_JointMgr)
      return false;

    return s_JointMgr->RemoveJoint(nIdx);
  }

  void PhysicsSystem::UpdateColliderMap(void)
  {
    s_Colliders.clear();

    // run through every space in the game, grab all collider types
    std::vector<Core::GameSpace*>&Spaces = Core::ENGINE->Spaces();
    for each(auto sp in Spaces)
    {
      TColliderVec dynamicObjects;
      TColliderVec staticObjects;

      // grab my array of object allocators for this space
      Base::ObjectAllocator *Colliders[NUM_COLLIDERS] = {nullptr};
      Colliders[0] = sp->GetComponents(eBoxCollider);
      Colliders[1] = sp->GetComponents(eSphereCollider);
      Colliders[2] = sp->GetComponents(eCapsuleCollider);

      for (unsigned nIdx = 0; nIdx < NUM_COLLIDERS; ++nIdx)
      {
        for (unsigned nJdx = 0; nJdx < Colliders[nIdx]->Size(); ++nJdx)
        {
          Core::GameComponent *pComp = static_cast<Core::GameComponent*>((*Colliders[nIdx])[nJdx]);
          if (!pComp)
            continue;

          // we'll implicitely trust this is a pointer of type collider
          //Collider *pCollider = dynamic_cast<Collider*>(pComp);
          //if (!pCollider)
          //  continue;

          // does this collider have a rigidbody?
          // if not, it's static
          RigidBody *pBody = pComp->GetOwner()->GetComponent<RigidBody>(eRigidBody);
          if (!pBody)
            staticObjects.push_back(pComp);
          else if (pBody->GetCurrentState() == DynamicState::STATIC)
            staticObjects.push_back(pComp);
          else
            dynamicObjects.push_back(pComp);
        }
      }

      // I went through every collider in this space, store this object
      s_Colliders[sp] = std::make_pair(dynamicObjects, staticObjects);
    }
  }
}