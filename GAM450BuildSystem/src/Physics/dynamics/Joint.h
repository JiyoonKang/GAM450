/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Joint.h 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_PHYSICS_DYNAMICS_JOINT
#define H_PAINT_PHYSICS_DYNAMICS_JOINT

#include "physics/colliders/CollisionMgr.h"
#include "base/util/math/float4.h"
#include "base/util/math/Quaternion.h"

namespace Physics
{
  class RigidBody;

  struct Constraint
  {
    Jacobian m_Jacobian;
    float m_Bias;
    float m_Lambda;

    Constraint(void)
      : m_Jacobian()
      , m_Bias(0.f)
      , m_Lambda(0.f)
    {

    }
  };

  class IJoint
  {
  private:

  public:
    virtual ~IJoint(void)
    {
    }
    virtual void Step(float dt) = 0;
    virtual void Init(void)
    {
    }
    virtual void ApplyImpulse(void) = 0;
    virtual Jacobian ComputeJacobian(unsigned nIdx) const = 0;
    virtual float ComputeBias(unsigned nIdx, float dt) const = 0;
    virtual void Draw(void) const
    {
    }
  };

  class Joint : public IJoint
  {
  protected:
    RigidBody *m_pBodyA;
    RigidBody *m_pBodyB;

    // used for warmstarting
    std::vector<Constraint> m_Constraints;

    // computed in step, so we only compute once per frame
    MassMatrix m_CachedMass;

    MassMatrix GetMassMatrix(void) const;
    VelocityVector GetVelocityVector(void) const;
    void SetVelocity(VelocityVector const &v);

  public:
    Joint(Core::GameComponent *pBodyA = nullptr, Core::GameComponent *pBodyB = nullptr);
    virtual ~Joint(void) override {}
    // step will be the same for every joint as well, no changes besides
    // bias term and jacobian
    virtual void Step(float dt) final;
    // apply impulse should be the same for all? all that changes is what
    // bias term and what jacobian we use
    virtual void ApplyImpulse(void) final;
    // nIdx is the current constraint to compute the proper jacobian and bias
    // for, which is determined by each joint individually, at most
    // will probably only have 6 constraints per joint, for each
    // degree of freedom we want to limit, we need a constraint
    virtual Jacobian ComputeJacobian(unsigned nIdx) const override;
    virtual float ComputeBias(unsigned nIdx, float dt) const override;

    void SetBodyPtrs(Core::GameComponent *pBodyA, Core::GameComponent *pBodyB);
    void SetBodyAPtr(Core::GameComponent *pBodyA);
    void SetBodyBPtr(Core::GameComponent *pBodyB);
  };

  class PositionJoint : public Joint
  {
  private:
    // these two local positions will be locked in place to each other
    // they will be equal to the same point in world space
    float4 m_LocalA;
    float4 m_LocalB;

    // caching this data
    float4 m_WorldA;
    float4 m_WorldB;

    // caching this data
    float4 m_rA;
    float4 m_rB;

  public:
  public:
    PositionJoint(float4 const &localA = float4(),
      float4 const &localB = float4());

    virtual void Init(void) override;
    virtual Jacobian ComputeJacobian(unsigned nIdx) const override;
    virtual float ComputeBias(unsigned nIdx, float dt) const override;
    // will use it's debug guid to create/update the debug draw for this
    // joint, position joint will simply draw a point at the position the two
    // local points are at in world space
    virtual void Draw(void);

    float4 const &GetLocalA(void) const;
    float4 const &GetLocalB(void) const;
    void SetLocalA(float4 const &a);
    void SetLocalB(float4 const &b);
  };

  class FixedAngleJoint : public Joint
  {
  private:
    // local rotations of two bodies that should remain the same rotation in world space
    quaternion m_LocalA;
    quaternion m_LocalB;

    // caching this data, used when calculating baumgarte
    float4 m_DecomposedAxis;

  public:
    FixedAngleJoint(quaternion const &a = quaternion(), quaternion const &b = quaternion());

    virtual void Init(void) override;
    virtual Jacobian ComputeJacobian(unsigned nIdx) const override;
    virtual float ComputeBias(unsigned nIdx, float dt) const override;

    quaternion const &GetLocalA(void) const;
    quaternion const &GetLocalB(void) const;
    void SetLocalA(quaternion const &q);
    void SetLocalB(quaternion const &q);
  };

  class WeldJoint : public Joint
  {
  private:
    float4 m_LocalPosA;
    float4 m_LocalPosB;
    quaternion m_LocalRotA;
    quaternion m_LocalRotB;

    // caching this data
    float4 m_WorldPosA;
    float4 m_WorldPosB;

    float4 m_rA;
    float4 m_rB;

    float4 m_DecomposedAxis;

  public:
    WeldJoint(float4 const &pA = float4(), quaternion const &qA = quaternion(),
      float4 const &pB = float4(), quaternion const &qB = quaternion());

    virtual void Init(void) override;
    virtual Jacobian ComputeJacobian(unsigned nIdx) const override;
    virtual float ComputeBias(unsigned nIdx, float dt) const override;

    float4 const &GetLocalPositionA(void) const;
    float4 const &GetLocalPositionB(void) const;
    quaternion const &GetLocalRotationA(void) const;
    quaternion const &GetLocalRotationB(void) const;

    void SetLocalPositionA(float4 const &pA);
    void SetLocalPositionB(float4 const &pB);
    void SetLocalRotationA(quaternion const &qA);
    void SetLocalRotationB(quaternion const &qB);
  };
}

#endif