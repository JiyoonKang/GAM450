/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: CollisionMgr.h 
 * Author: Jonah Dube
 * Author: Daniel Oliveira
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_PHYSICS_COLLISION_MGR
#define H_PAINT_PHYSICS_COLLISION_MGR

#include <vector>
#include <unordered_map>
#include <functional>

#include "Base/util/math/float4.h"
#include "Physics/colliders/Collider.h"
#include "Physics/util/Geometry.h"
#include "core/components/base/Component.h"

// TODO, make this 4 and if we go over, then consolidate down to 4
#define MAX_CONTACT_POINTS 8

namespace Physics
{
  class RigidBody;

  struct Jacobian
  {
    float4 m_LinA;
    float4 m_AngA;
    float4 m_LinB;
    float4 m_AngB;

    Jacobian &operator *=(float fScalar)
    {
      m_LinA *= fScalar;
      m_LinB *= fScalar;
      m_AngA *= fScalar;
      m_AngB *= fScalar;
      return *this;
    }

    Jacobian &operator +=(Jacobian const &rhs)
    {
      m_LinA += rhs.m_LinA;
      m_LinB += rhs.m_LinB;
      m_AngA += rhs.m_AngA;
      m_AngB += rhs.m_AngB;
      return *this;
    }

    float operator *(Jacobian const &rhs) const
    {
      return
        m_LinA.Dot(rhs.m_LinA) +
        m_AngA.Dot(rhs.m_AngA) +
        m_LinB.Dot(rhs.m_LinB) +
        m_AngB.Dot(rhs.m_AngB);
    }

    Jacobian operator *(float fScalar)
    {
      Jacobian result = *this;
      result *= fScalar;
      return result;
    }
  };

  struct MassMatrix
  {
    float m_invMassA;
    float4x4 m_invTensorA;
    float m_invMassB;
    float4x4 m_invTensorB;

    Jacobian Solve(Jacobian const &rhs)
    {
      return {rhs.m_LinA * m_invMassA, m_invTensorA * rhs.m_AngA,
        rhs.m_LinB * m_invMassB, m_invTensorB * rhs.m_AngB};
    }

    float GetEffectiveMass(Jacobian const &rhs)
    {
      float mc =
        rhs.m_LinA.Dot(rhs.m_LinA) * m_invMassA +
        rhs.m_AngA.Dot(m_invTensorA * rhs.m_AngA) +
        rhs.m_LinB.Dot(rhs.m_LinB) * m_invMassB +
        rhs.m_AngB.Dot(m_invTensorB * rhs.m_AngB);

      if (Equals(mc, 0.f))
        return 0.f;

      return 1.f / mc;
    }
  };

  typedef Jacobian VelocityVector;

  struct Manifold
  {
    // the entities that collided
    RigidBody *m_pBodyA;
    RigidBody *m_pBodyB;

    // gathered from collision detection algorithms
    float4 m_ContactNormal;
    std::vector<float4> m_ContactPoints;
    std::vector<float> m_Penetration;

    // gathered when initializing this collision
    float m_fMixedRestitution;
    float m_fMixedStaticFriction;
    float m_fMixedDynamicFriction;

    // used when calculating the response for each contact point
    std::vector<float> m_NormalImpulse;
    std::vector<float> m_TangentImpulseA;
    std::vector<float> m_TangentImpulseB;
    float4 m_TangentA;
    float4 m_TangentB;

    Manifold(void);

    void Step(void);
    void ApplyNormalImpulse(float dt);
    void ApplyTangentImpulse(void);

    // Mixes two material's properties and applies it to this manifold.
    void MixMaterials(const Material& matA, const Material& matB);
  };

  // Written by Daniel Oliveira
  // The CollisionMessageMgr is just a container used for keeping track of
  // currently colliding rigid bodies over multiple frames so that messages
  // can be sent when collisions start, continue, and end.
  //
  // When this is created it needs a maxAge value. That represents how many
  // frames of non-collision are needed for a collision to be exited. This
  // is to prevent slight jittering from making too many collisionEnter and
  // collisionExit messages in short periods of time
  typedef void (*TCollisionCallback)(RigidBody*, RigidBody*);

  enum MessageType
  {
    ENTER,
    CONTINUE,
    EXIT
  };
  struct MessageHold
  {
    MessageType type;
    RigidBody* a;
    RigidBody* b;
  };

  class CollisionMessageMgr
  {
    public:
      CollisionMessageMgr(int maxAge);
      
      // Enter a collision into the container.
      //
      // If the collision is new, then enterCallback is called with the
      //   colliding objects as params.
      //
      // Note: This looks at a ONE WAY edge. That is, it'll note a collision
      //       on A->B, but not B->A. It must be called again with A and B
      //       switched in order to handle that.
      void NoteCollision(RigidBody* a, RigidBody* b, std::vector<MessageHold>& messages);

      // Steps through container aging all collisions and removing collisions
      // older than the maxAge chosen on this class's construction.
      //
      // The callback param is called on all object pairs that aren't too old.
      // The exitCallback is called with object pairs that are too old.
      void AgeCollisions(std::vector<MessageHold>& messages);

      void ClearBodies();

      void RemoveBody(RigidBody* body);

    private:
      typedef std::unordered_map<RigidBody*, std::vector<std::pair<RigidBody*, int>>> TMessagingMap;
      TMessagingMap m_MessageMap;
      int m_MaxAge;
  };

  class CollisionMgr
  {
  private:
    friend class PhysicsSystem;
    // the contacts that occured this frame
    std::vector<Manifold> m_Contacts;

    typedef bool(*CollisionHelperFunc)(Core::GameComponent*, Core::GameComponent*, Manifold &m);
    typedef std::unordered_map<Base::EComponent, std::unordered_map<Base::EComponent, CollisionHelperFunc>> TCollisionMap;
    TCollisionMap m_CollisionPicker;

    // need to keep all contacts that are unique this frame, need an easy way
    // to access each potential contact between bodies, hold the guid
    typedef std::unordered_map<RigidBody*, Manifold> TInternalContactMap;
    typedef std::unordered_map<RigidBody*, TInternalContactMap> TContactMap;
    TContactMap m_CachedContacts;

    CollisionMessageMgr m_MessageMgr; // Keep track of collisions for messages
    TCollisionCallback m_EnterCallback, m_Callback, m_ExitCallback;

    bool CompareContactPoints(Manifold &cachedContact, Manifold &contact, unsigned nIdx);
    void WarmStart(Manifold &cachedContact, Manifold &contact, unsigned nIdx);

  public:
    CollisionMgr(void);

    bool Collide(Core::GameComponent *a, Core::GameComponent *b);
    void ClearContacts(void);
    void PrestepCollisionResolver(void);
    void ResolveCollisions(float dt, unsigned nIterations);
    void CacheContacts(void);

    void SetCollisionCallbacks(const TCollisionCallback& enterCallback,
      const TCollisionCallback& callback, const TCollisionCallback& exitCallback);
    void ManageCollisionMessages(void);
    void RemoveRigidBodyMessages(RigidBody* pRigidBody);
    void ClearRigidBodyMessages();

    static bool BoxToBox(Core::GameComponent *a, Core::GameComponent *b, Manifold &m);
    static bool BoxToSphere(Core::GameComponent *a, Core::GameComponent *b, Manifold &m);
    static bool BoxToCapsule(Core::GameComponent *a, Core::GameComponent *b, Manifold &m);
    static bool SphereToSphere(Core::GameComponent *a, Core::GameComponent *b, Manifold &m);
    static bool SphereToCapsule(Core::GameComponent *a, Core::GameComponent *b, Manifold &m);
    static bool SphereToBox(Core::GameComponent *a, Core::GameComponent *b, Manifold &m);
    static bool CapsuleToCapsule(Core::GameComponent *a, Core::GameComponent *b, Manifold &m);
    static bool CapsuleToBox(Core::GameComponent *a, Core::GameComponent *b, Manifold &m);
    static bool CapsuleToSphere(Core::GameComponent *a, Core::GameComponent *b, Manifold &m);
  };
}

#endif