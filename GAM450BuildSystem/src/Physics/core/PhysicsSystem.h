/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: PhysicsSystem.h 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_PHYSICS_CORE_PHYSICS_SYSTEM
#define H_PAINT_PHYSICS_CORE_PHYSICS_SYSTEM

#include <vector>
#include <unordered_map>

#include "physics/colliders/CollisionMgr.h"
#include "core/systems/System.h"

#include "base/modules/handle/Handle.h"

#include "physics/colliders/RayCastMgr.h"
#include "physics/dynamics/JointMgr.h"

class Core::GameComponent;

namespace Physics
{
  struct Manifold;
  class Collider;
  class RigidBody;

  class PhysicsSystem : public Core::ISystem
  {
  public:
    typedef std::vector<Core::GameComponent*> TColliderVec;
    // first is dynamic objects, second is static objects     
    typedef std::pair<TColliderVec, TColliderVec> TColliderPair;
    typedef std::unordered_map<Core::GameSpace*, TColliderPair> TSpaceColliderVec;

    void RemoveRigidBodyMessages(RigidBody* pRigidBody);
    void ClearRigidBodyMessages();
  private:
    void Step(float dt);

    // helper functions that are used to update the physics system
    void ClearContacts(void);
    void RunBroadPhase(void);
    void RunNarrowPhase(void);

    void IntegrateAcceleration(float dt);
    void ResolveCollisions(float dt);
    void IntegrateVelocity(float dt);
    void ResetAcceleration(void);

    void CacheContacts(void);
    void ManageCollisionMessages(void);
    void CheckCollision(Core::GameComponent *pColliderA, Core::GameComponent *pColliderB);

    CollisionMgr m_CollisionMgr;

    // handles all ray casting
    RayCastMgr m_RayCastMgr;

    JointMgr m_JointMgr;

    // my list of dynamic and static colliders in the game, is re updated each
    // frame since it is not a good idea to store these components in memory
    // also holds which space this items belongs to
    static TSpaceColliderVec s_Colliders;

    void UpdateColliderMap(void);

    // true marks frame stepping on, false will continously run the entire physics system
    bool m_bFrameStepping;
    // only used when frame stepping is currently on
    bool m_bCanStep;

    // we run our resolver multiple times to decrease error
    unsigned m_nIterations;

  public:
    PhysicsSystem(void);
    virtual ~PhysicsSystem(void) override;
    virtual std::string  GetName(void) const override;
    virtual void Initialize(void) override;
    virtual void Update(float dt) override;
    virtual void RegisterComponents(void) override;

    bool IsFrameStepping(void) const;
    void SetFrameStepping(bool bFrameStepping);
    void SetCanStep(bool bCanStep);

    static bool CastRay(float4 const &Start, float4 const &Direction,
      Core::GameComponent *pCollider, float fMaxDistance = FLT_MAX);
    static bool CastRay(Ray &ray, Core::GameComponent *pCollider, float fMaxDistance = FLT_MAX);
    static bool CastRay(float4 const &Start, float4 const &Direction,
      Core::GameComponent *pCollider, RayCastResult &result, float fMaxDistance = FLT_MAX);
    static bool CastRay(Ray &ray, RayCastResult &result,
      Core::GameComponent *pCollider, float fMaxDistance = FLT_MAX);

    static bool CastRay(Core::GameSpace *pSpace, float4 const &Start, 
      float4 const &Direction, RayCastResult &result, float fMaxDistance = FLT_MAX);
    static bool CastRay(Core::GameSpace *pSpace, Ray &ray, RayCastResult &result, 
      float fMaxDistance = FLT_MAX);

    // only check against static colliders
    static bool CastRayStatic(Core::GameSpace *pSpace, 
      float4 const &Start, float4 const &Direction,
      RayCastResult &result, float fMaxDistance = FLT_MAX);
    static bool CastRayStatic(Core::GameSpace *pSpace, Ray &ray, RayCastResult &result, 
      float fMaxDistance = FLT_MAX);


    static bool CastRayFiltered(Core::GameSpace *pSpace, float4 const &Start, 
      float4 const &Direction, RayCastResult &result, 
      std::vector<Core::GameComponent*> const &filterList, 
      float fMaxDistance = FLT_MAX);
    static bool CastRayFiltered(Core::GameSpace *pSpace, Ray &ray, 
      RayCastResult &result, std::vector<Core::GameComponent*> const &filterList,
      float fMaxDistance = FLT_MAX);

    // only check against static colliders
    static bool CastRayFilteredStatic(Core::GameSpace *pSpace,
      float4 const &Start, float4 const &Direction,
      RayCastResult &result, std::vector<Core::GameComponent*> const &filterList,
      float fMaxDistance = FLT_MAX);
    static bool CastRayFilteredStatic(Core::GameSpace *pSpace, Ray &ray, RayCastResult &result,
      std::vector<Core::GameComponent*> const &filterList, float fMaxDistance = FLT_MAX);


    // adding a joint function
    // joint mgr will take ownership of this pointer
    static unsigned AddJoint(Joint *pJoint);
    static Joint const *GetConstJoint(unsigned nIdx);
    static Joint *GetJoint(unsigned nIdx);
    static bool RemoveJoint(unsigned nIdx);
  };

  extern PhysicsSystem *PHYSICS;
}

#endif