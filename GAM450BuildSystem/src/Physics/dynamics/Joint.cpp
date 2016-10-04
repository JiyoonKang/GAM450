/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Joint.cpp 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "physics/dynamics/Joint.h"
#include "physics/dynamics/RigidBody.h"
#include "physics/util/Geometry.h"

#include "base/util/math/Float4x4.h"

#include "core/types/space/Space.h"
#include "core/types/object/Object.h"
#include "core/components/base/Component.h"
#include "core/components/transform/CTransform.h"

#include "graphics/includes/GraphicsSystem/SkyeGraphics.h"

namespace Physics
{
  Joint::Joint(Core::GameComponent *pBodyA, Core::GameComponent *pBodyB)
    : m_pBodyA(dynamic_cast<RigidBody*>(pBodyA))
    , m_pBodyB(dynamic_cast<RigidBody*>(pBodyB))
    , m_Constraints()
    , m_CachedMass()
  {

  }

  MassMatrix Joint::GetMassMatrix(void) const
  {
    MassMatrix mass = {0.f, float4x4(), 0.f, float4x4()};
    if (m_pBodyA)
    {
      mass.m_invMassA = m_pBodyA->GetInverseMass();
      mass.m_invTensorA = m_pBodyA->GetInverseInertiaTensorW();
    }
    if (m_pBodyB)
    {
      mass.m_invMassB = m_pBodyB->GetInverseMass();
      mass.m_invTensorB = m_pBodyB->GetInverseInertiaTensorW();
    }

    return mass;
  }

  VelocityVector Joint::GetVelocityVector(void) const
  {
    VelocityVector vel = {float4(), float4(), float4(), float4()};
    if (m_pBodyA)
    {
      vel.m_LinA = m_pBodyA->GetVelocity();
      vel.m_AngA = m_pBodyA->GetAngularVelocity();
    }
    if (m_pBodyB)
    {
      vel.m_LinB = m_pBodyB->GetVelocity();
      vel.m_AngB = m_pBodyB->GetAngularVelocity();
    }
    return vel;
  }

  void Joint::SetVelocity(VelocityVector const &v)
  {
    if (m_pBodyA)
    {
      m_pBodyA->SetVelocity(v.m_LinA);
      m_pBodyA->SetAngularVelocity(v.m_AngA);
    }
    if (m_pBodyB)
    {
      m_pBodyB->SetVelocity(v.m_LinB);
      m_pBodyB->SetAngularVelocity(v.m_AngB);
    }
  }

  void Joint::Step(float dt)
  {
    // compute the mass matrix for this frame and cache it
    m_CachedMass = GetMassMatrix();
    // apply the accumulated impulse, aka warm starting
    VelocityVector Velocity = GetVelocityVector();
    Init();

    // compute the term for each constraint
    for (unsigned nIdx = 0; nIdx < static_cast<unsigned>(m_Constraints.size()); ++nIdx)
    {
      Constraint &currConstraint = m_Constraints[nIdx];
      // compute the initial jacobian
      // should go to the proper ComputeJacobian?
      currConstraint.m_Jacobian = ComputeJacobian(nIdx);
      // compute the initial bias
      currConstraint.m_Bias = ComputeBias(nIdx, dt);

      Jacobian Impulse = m_CachedMass.Solve(currConstraint.m_Jacobian) *
        currConstraint.m_Lambda;
      Velocity += Impulse;
    }

    // apply back to our rigidbodies
    SetVelocity(Velocity);
  }

  void Joint::ApplyImpulse(void)
  {
    // grab our current velocity
    VelocityVector Velocity = GetVelocityVector();

    for (unsigned nIdx = 0; nIdx < static_cast<unsigned>(m_Constraints.size()); ++nIdx)
    {
      Constraint &currConstraint = m_Constraints[nIdx];
      // grab the effective mass
      float mc = m_CachedMass.GetEffectiveMass(currConstraint.m_Jacobian);
      // calculate lambda, our impulse scalar
      float fLambda = -mc * (currConstraint.m_Jacobian * Velocity + currConstraint.m_Bias);

      // calculate my final impulse to apply
      Jacobian Impulse = m_CachedMass.Solve(currConstraint.m_Jacobian) * fLambda;
      Velocity += Impulse;
      // update our cached lambda with our new calculated lambda
      currConstraint.m_Lambda += fLambda;
    }

    // apply back to our rigidbodies
    SetVelocity(Velocity);
  }

  Jacobian Joint::ComputeJacobian(unsigned /*nIdx*/) const
  {
    return Jacobian();
  }

  float Joint::ComputeBias(unsigned /*nIdx*/, float /*dt*/) const
  {
    return 0.f;
  }

  void Joint::SetBodyPtrs(Core::GameComponent *pBodyA, Core::GameComponent *pBodyB)
  {
    m_pBodyA = dynamic_cast<RigidBody*>(pBodyA);
    m_pBodyB = dynamic_cast<RigidBody*>(pBodyB);
  }

  void Joint::SetBodyAPtr(Core::GameComponent *pBodyA)
  {
    m_pBodyA = dynamic_cast<RigidBody*>(pBodyA);
  }

  void Joint::SetBodyBPtr(Core::GameComponent *pBodyB)
  {
    m_pBodyB = dynamic_cast<RigidBody*>(pBodyB);
  }

  PositionJoint::PositionJoint(float4 const &localA, float4 const &localB)
    : Joint()
    , m_LocalA(localA)
    , m_LocalB(localB)
    , m_WorldA()
    , m_WorldB()
    , m_rA()
    , m_rB()
  {
    // x y and z constraints
    m_Constraints.resize(3);
  }

  void PositionJoint::Init(void)
  {
    // for now just don't compute if we don't have a rigidbody for either one
    if (!m_pBodyA || !m_pBodyB)
      return;

    Core::Transform *pTransformA = m_pBodyA->GetOwner()->GetComponent<Core::Transform>(eTransform);
    Core::Transform *pTransformB = m_pBodyB->GetOwner()->GetComponent<Core::Transform>(eTransform);
    if (!pTransformA || !pTransformB)
      return;

    float4x4 rotA = pTransformA->GetRotationMatrix();
    float4x4 rotB = pTransformB->GetRotationMatrix();

    // grab rA and rB, can be computed by the local point to the origin
    // in the local space of each object, since the shapes origin is 0
    // we can just take the local position as our r vector, then rotate
    // the vector into world space
    m_rA = RotateVectorOutOfBasis(rotA, m_LocalA);
    m_rB = RotateVectorOutOfBasis(rotB, m_LocalB);

    // my two points in world space
    m_WorldA = RotatePointOutOfBasis(rotA, pTransformA->GetTranslation(), m_LocalA);
    m_WorldB = RotatePointOutOfBasis(rotB, pTransformB->GetTranslation(), m_LocalB);
  }

  Jacobian PositionJoint::ComputeJacobian(unsigned nIdx) const
  {
    // will either by x, y or z
    float4 axis;

    // compute our jacobian
    switch (nIdx)
    {
    case 0:
      axis.x = 1.f;
      break;
    case 1:
      axis.y = 1.f;
      break;
    case 2:
      axis.z = 1.f;
      break;
    }

    Jacobian result;
    result.m_LinA = -axis;
    result.m_AngA = (-m_rA).Cross(axis);
    result.m_LinB = axis;
    result.m_AngB = m_rB.Cross(axis);

    return result;
  }

  float PositionJoint::ComputeBias(unsigned nIdx, float dt) const
  {
    static float biasFactor = 0.1f;

    // compute the c term to feed back into the bias, aka baumgarte stabilization
    float c = 0.f;
    switch (nIdx)
    {
    case 0:
      c = m_WorldB.x - m_WorldA.x;
      break;
    case 1:
      c = m_WorldB.y - m_WorldA.y;
      break;
    case 2:
      c = m_WorldB.z - m_WorldA.z;
      break;
    }

    float fBias = Clamp(biasFactor, 0.f, 1.f) * (1.f / dt);
    return fBias * c;
  }

  void PositionJoint::Draw(void)
  {
  }

  float4 const &PositionJoint::GetLocalA(void) const
  {
    return m_LocalA;
  }

  float4 const &PositionJoint::GetLocalB(void) const
  {
    return m_LocalB;
  }

  void PositionJoint::SetLocalA(float4 const &a)
  {
    m_LocalA = a;
  }

  void PositionJoint::SetLocalB(float4 const &b)
  {
    m_LocalB = b;
  }

  FixedAngleJoint::FixedAngleJoint(quaternion const &a, quaternion const &b)
    : m_LocalA(a)
    , m_LocalB(b)
    , m_DecomposedAxis()
  {
    m_Constraints.resize(3);
  }

  void FixedAngleJoint::Init(void)
  {
    Core::Transform *pTransformA = m_pBodyA ? m_pBodyA->GetOwner()->GetComponent<Core::Transform>(eTransform) : nullptr;
    Core::Transform *pTransformB = m_pBodyB ? m_pBodyB->GetOwner()->GetComponent<Core::Transform>(eTransform) : nullptr;

    quaternion worldA;
    quaternion worldB;

    if (pTransformA)
      worldA = pTransformA->GetRotation() * m_LocalA * pTransformA->GetRotation().GetInverse();
    else
      worldA = m_LocalA;

    if (pTransformB)
      worldB = pTransformB->GetRotation() * m_LocalB * pTransformB->GetRotation().GetInverse();
    else
      worldB = m_LocalB;

    quaternion q = worldA.GetInverse() * worldB;
    float s = 0.f; // stub, not used
    q.Decompose(s, m_DecomposedAxis);
  }

  Jacobian FixedAngleJoint::ComputeJacobian(unsigned nIdx) const
  {
    float4 axis;
    switch (nIdx)
    {
    case 0:
      axis.x = 1.f;
      break;
    case 1:
      axis.y = 1.f;
      break;
    case 2:
      axis.z = 1.f;
      break;
    }

    Jacobian result;
    result.m_LinA = float4();
    result.m_AngA = -axis;
    result.m_LinB = float4();
    result.m_AngB = axis;
    return result;
  }

  float FixedAngleJoint::ComputeBias(unsigned nIdx, float dt) const
  {
    static float biasFactor = 0.1f;
    // our constraint value is 2v
    // v is our axis in the quaternion, q1^-1 * q2
    float c = 2 * m_DecomposedAxis[nIdx];
    float fBias = Clamp(biasFactor, 0.f, 1.f) * (1.f / dt);

    return fBias * c;
  }

  quaternion const &FixedAngleJoint::GetLocalA(void) const
  {
    return m_LocalA;
  }

  quaternion const &FixedAngleJoint::GetLocalB(void) const
  {
    return m_LocalB;
  }

  void FixedAngleJoint::SetLocalA(quaternion const &q)
  {
    m_LocalA = q;
  }

  void FixedAngleJoint::SetLocalB(quaternion const &q)
  {
    m_LocalB = q;
  }

  WeldJoint::WeldJoint(float4 const &pA, quaternion const &qA,
    float4 const &pB, quaternion const &qB)
    : m_LocalPosA(pA)
    , m_LocalPosB(pB)
    , m_LocalRotA(qA)
    , m_LocalRotB(qB)
  {
    // 3 for position, 3 for rotation
    m_Constraints.resize(6);
  }

  void WeldJoint::Init(void)
  {
    Core::Transform *pTransformA = m_pBodyA ? m_pBodyA->GetOwner()->GetComponent<Core::Transform>(eTransform) : nullptr;
    Core::Transform *pTransformB = m_pBodyB ? m_pBodyB->GetOwner()->GetComponent<Core::Transform>(eTransform) : nullptr;

    float4x4 rotA; rotA.Identity();
    float4 posA;
    quaternion quaA;
    if (pTransformA)
    {
      posA = pTransformA->GetTranslation();
      rotA = pTransformA->GetRotationMatrix();
      quaA = pTransformA->GetRotation();
    }
    float4x4 rotB; rotB.Identity();
    float4 posB;
    quaternion quaB;
    if(pTransformB)
    {
      posB = pTransformB->GetTranslation();
      rotB = pTransformB->GetRotationMatrix();
      quaB = pTransformB->GetRotation();
    }

    // grab rA and rB, can be computed by the local point to the origin
    // in the local space of each object, since the shapes origin is 0
    // we can just take the local position as our r vector, then rotate
    // the vector into world space
    m_rA = RotateVectorOutOfBasis(rotA, m_LocalPosA);
    m_rB = RotateVectorOutOfBasis(rotB, m_LocalPosB);

    // my two points in world space
    m_WorldPosA = RotatePointOutOfBasis(rotA, posA, m_LocalPosA);
    m_WorldPosB = RotatePointOutOfBasis(rotB, posB, m_LocalPosB);

    quaternion worldA = quaA * m_LocalRotA * quaA.GetInverse();
    quaternion worldB = quaB * m_LocalRotB * quaB.GetInverse();

    // computing rotation and storing the axis
    quaternion q = worldA.GetInverse() * worldB;
    float s = 0.f; // stub, not used
    q.Decompose(s, m_DecomposedAxis);

    // draw a sphere for each position on these two bodies
    float3 aTemp(m_WorldPosA.x, m_WorldPosA.y, m_WorldPosA.z);
    float3 bTemp(m_WorldPosB.x, m_WorldPosB.y, m_WorldPosB.z);
    GRAPHICS->GetDebugDrawer().DrawSphere(aTemp, 0.1f, float4(1.f, 0.f, 0.f, 1.f));
    GRAPHICS->GetDebugDrawer().DrawSphere(bTemp, 0.1f, float4(0.f, 0.f, 1.f, 1.f));
  }

  Jacobian WeldJoint::ComputeJacobian(unsigned nIdx) const
  {
    float4 axis;
    switch (nIdx)
    {
    case 0:
    case 3:
      axis.x = 1.f;
      break;
    case 1:
    case 4:
      axis.y = 1.f;
      break;
    case 2:
    case 5:
      axis.z = 1.f;
      break;
    }

    Jacobian result;

    switch (nIdx)
    {
    case 0:
    case 1:
    case 2:
      result.m_LinA = -axis;
      result.m_AngA = (-m_rA).Cross(axis);
      result.m_LinB = axis;
      result.m_AngB = m_rB.Cross(axis);
      break;

    case 3:
    case 4:
    case 5:
      result.m_LinA = float4();
      result.m_AngA = -axis;
      result.m_LinB = float4();
      result.m_AngB = axis;
      break;
    }

    return result;
  }

  float WeldJoint::ComputeBias(unsigned nIdx, float dt) const
  {
    static float fBiasFactor = 0.f;

    float c = 0.f;
    switch (nIdx)
    {
    case 0:
    case 1:
    case 2:
      c = m_WorldPosB[nIdx % 3] - m_WorldPosA[nIdx % 3];
      break;

    case 3:
    case 4:
    case 5:
      c = 2 * m_DecomposedAxis[nIdx % 3];
      break;
    }

    float fBias = Clamp(fBiasFactor, 0.f, 1.f) * (1.f / dt);
    return fBias * c;
  }

  float4 const &WeldJoint::GetLocalPositionA(void) const
  {
    return m_LocalPosA;
  }

  float4 const &WeldJoint::GetLocalPositionB(void) const
  {
    return m_LocalPosB;
  }

  quaternion const &WeldJoint::GetLocalRotationA(void) const
  {
    return m_LocalRotA;
  }

  quaternion const &WeldJoint::GetLocalRotationB(void) const
  {
    return m_LocalRotB;
  }

  void WeldJoint::SetLocalPositionA(float4 const &pA)
  {
    m_LocalPosA = pA;
  }

  void WeldJoint::SetLocalPositionB(float4 const &pB)
  {
    m_LocalPosB = pB;
  }

  void WeldJoint::SetLocalRotationA(quaternion const &qA)
  {
    m_LocalRotA = qA;
  }

  void WeldJoint::SetLocalRotationB(quaternion const &qB)
  {
    m_LocalRotB = qB;
  }
}