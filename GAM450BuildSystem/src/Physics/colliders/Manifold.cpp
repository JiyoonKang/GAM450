/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Manifold.cpp 
 * Author: Jonah Dube
 * Author: Daniel Oliveira
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

#include "physics/colliders/CollisionMgr.h"
#include "physics/dynamics/RigidBody.h"

#include "core/components/transform/CTransform.h"
#include "core/types/object/Object.h"
#include "core/types/space/Space.h"

#include <functional>

namespace
{
  using namespace Physics;

  void ComputeBasis(float4 const &n, float4 &t1, float4 &t2)
  {
    if (std::abs(n.x) >= 0.57735f)
      t1 = float4(n.y, -n.x, 0.f, 0.f);
    else
      t1 = float4(0.f, n.z, -n.y, 0.f);

    t1.Normalize();
    t2 = n.Cross(t1);
  }

  // Written by Daniel Oliveira
  std::function<float(float, float)> GetMixOperation(MixType type)
  {
    switch (type)
    {
      case MixType::MAXIMUM:
        return [](float first, float second)->float { return std::max(first, second); };

      case MixType::MULTIPLY:
        return [](float first, float second)->float { return (first * second); };

      case MixType::MINIMUM:
        return [](float first, float second)->float { return std::min(first, second); };

      case MixType::AVERAGE:
      default:
        return [](float first, float second)->float { return (first + second) / 2.0f; };
    }
  }
}

namespace Physics
{
  Manifold::Manifold(void)
    : m_ContactNormal()
    , m_ContactPoints()
    , m_Penetration()
    , m_fMixedRestitution(0.f)
    , m_fMixedStaticFriction(0.f)
    , m_fMixedDynamicFriction(0.f)
    , m_NormalImpulse()
    , m_TangentImpulseA()
    , m_TangentImpulseB()
    , m_TangentA()
    , m_TangentB()
    , m_pBodyA(nullptr)
    , m_pBodyB(nullptr)
  {
    m_ContactPoints.reserve(MAX_CONTACT_POINTS);
    m_Penetration.reserve(MAX_CONTACT_POINTS);
    m_NormalImpulse.resize(MAX_CONTACT_POINTS);
    m_TangentImpulseA.resize(MAX_CONTACT_POINTS);
    m_TangentImpulseB.resize(MAX_CONTACT_POINTS);
  }

  void Manifold::Step(void)
  {
    // TODO, actually calculate restitution of friction
    //m_fMixedRestitution = 0.2f;
    //m_fMixedDynamicFriction = 0.1f;
    //m_fMixedStaticFriction = 0.1f;

    float4 TangentA;
    float4 TangentB;
    ComputeBasis(m_ContactNormal, TangentA, TangentB);

    m_TangentA = TangentA;
    m_TangentB = TangentB;
  }

  void Manifold::ApplyNormalImpulse(float dt)
  {
    static float const fBiasFactor = 0.1f;
    static float const fSlop = 0.01f;

    if (!m_pBodyA || !m_pBodyB)
      return;

    // grab the position of the rigidbodies from the transform
    Core::Transform *pTransformA = 
      m_pBodyA->GetOwner()->GetComponent<Core::Transform>(eTransform);
    Core::Transform *pTransformB = 
      m_pBodyB->GetOwner()->GetComponent<Core::Transform>(eTransform);

    if (!pTransformA || !pTransformB)
      return;

    float4 PositionA = pTransformA->GetTranslation();
    float4 PositionB = pTransformB->GetTranslation();

    VelocityVector Velocity =
    {m_pBodyA->GetVelocity(), m_pBodyA->GetAngularVelocity(),
    m_pBodyB->GetVelocity(), m_pBodyB->GetAngularVelocity()};

    MassMatrix Masses =
    {m_pBodyA->GetInverseMass(), m_pBodyA->GetInverseInertiaTensorW(),
    m_pBodyB->GetInverseMass(), m_pBodyB->GetInverseInertiaTensorW()};

    unsigned nIdx = 0;
    for each(auto it in m_ContactPoints)
    {
      float4 rA = it - PositionA;
      float4 rB = it - PositionB;

      Jacobian NormalJacobian =
      {-m_ContactNormal, -(rA.Cross(m_ContactNormal)),
      m_ContactNormal, rB.Cross(m_ContactNormal)};

      // apply our baumgarte term
      float fBias = Clamp(fBiasFactor, 0.f, 1.f) * (1.f / dt);
      fBias = -fBias * std::max(m_Penetration[nIdx] - fSlop, 0.f);

      // apply our restitution term
      float4 RestitutionVec =
        -Velocity.m_LinA - Velocity.m_AngA.Cross(rA) +
        Velocity.m_LinB + Velocity.m_AngB.Cross(rB);
      float fRestitution = m_fMixedRestitution * RestitutionVec.Dot(m_ContactNormal);
      fBias += fRestitution;

      // calculate the effective mass of the constraint
      // JT * M^-1 * J
      float mc = Masses.GetEffectiveMass(NormalJacobian);

      // calculate our lamdba value
      float fLambda = -mc * (NormalJacobian * Velocity + fBias);

      // clamp the lamdba value
      float fImpulse = std::max(fLambda + m_NormalImpulse[nIdx], 0.f);
      fLambda = fImpulse - m_NormalImpulse[nIdx];
      m_NormalImpulse[nIdx] = fImpulse;

      // calculate my final impulse and apply it
      Jacobian Impulse = Masses.Solve(NormalJacobian) * fLambda;
      Velocity += Impulse;

      ++nIdx;
    }

    // apply our final velocity back to our rigidbodies
    m_pBodyA->SetVelocity(Velocity.m_LinA);
    m_pBodyA->SetAngularVelocity(Velocity.m_AngA);
    m_pBodyB->SetVelocity(Velocity.m_LinB);
    m_pBodyB->SetAngularVelocity(Velocity.m_AngB);
  }

  void Manifold::ApplyTangentImpulse(void)
  {
    if (!m_pBodyA || !m_pBodyB)
      return;
    // grab the position of the rigidbodies from the transform
    Core::Transform *pTransformA =
      m_pBodyA->GetOwner()->GetComponent<Core::Transform>(eTransform);
    Core::Transform *pTransformB =
      m_pBodyB->GetOwner()->GetComponent<Core::Transform>(eTransform);

    if (!pTransformA || !pTransformB)
      return;

    float4 PositionA = pTransformA->GetTranslation();
    float4 PositionB = pTransformB->GetTranslation();

    VelocityVector Velocity =
    {m_pBodyA->GetVelocity(), m_pBodyA->GetAngularVelocity(),
    m_pBodyB->GetVelocity(), m_pBodyB->GetAngularVelocity()};

    MassMatrix Masses =
    {m_pBodyA->GetInverseMass(), m_pBodyA->GetInverseInertiaTensorW(),
    m_pBodyB->GetInverseMass(), m_pBodyB->GetInverseInertiaTensorW()};

    unsigned nIdx = 0;
    for each(auto it in m_ContactPoints)
    {
      float4 rA = it - PositionA;
      float4 rB = it - PositionB;

      float fLimit = m_fMixedDynamicFriction * m_NormalImpulse[nIdx];

      // calculate the tangent jacobian
      Jacobian TangentJacobian =
      {-m_TangentA, -(rA.Cross(m_TangentA)),
      m_TangentA, rB.Cross(m_TangentA)};

      // calculate the effective mass
      float mc = Masses.GetEffectiveMass(TangentJacobian);
      float fLambda = -mc * (TangentJacobian * Velocity);

      float fImpulse = Clamp(fLambda + m_TangentImpulseA[nIdx], - fLimit, fLimit);
      fLambda = fImpulse - m_TangentImpulseA[nIdx];
      m_TangentImpulseA[nIdx] = fImpulse;

      // calculate the impulse and add it
      Jacobian Impulse = Masses.Solve(TangentJacobian) * fLambda;
      Velocity += Impulse;

      // calculate the second tangent impulse
      TangentJacobian =
      {-m_TangentB, -(rA.Cross(m_TangentB)),
      m_TangentB, rB.Cross(m_TangentB)};

      mc = Masses.GetEffectiveMass(TangentJacobian);
      fLambda = -mc * (TangentJacobian * Velocity);

      fImpulse = Clamp(fLambda + m_TangentImpulseB[nIdx], - fLimit, fLimit);
      fLambda = fImpulse - m_TangentImpulseB[nIdx];
      m_TangentImpulseB[nIdx] = fImpulse;

      Impulse = Masses.Solve(TangentJacobian) * fLambda;
      Velocity += Impulse;
    }

    m_pBodyA->SetVelocity(Velocity.m_LinA);
    m_pBodyA->SetAngularVelocity(Velocity.m_AngA);
    m_pBodyB->SetVelocity(Velocity.m_LinB);
    m_pBodyB->SetAngularVelocity(Velocity.m_AngB);
  }

  // Written by Daniel Oliveira
  void Manifold::MixMaterials(const Material& matA, const Material& matB) 
  {
    // Figure out what mix types to use
    MixType frictionMix = std::max(matA.GetFrictionMixture(), matB.GetFrictionMixture());
    MixType restitutionMix = std::max(matA.GetRestitutionMixture(), matB.GetRestitutionMixture());

    // Get operation based on mix type
    auto frictionOp = GetMixOperation(frictionMix);
    auto restitutionOp = GetMixOperation(restitutionMix);

    m_fMixedDynamicFriction = frictionOp(matA.GetDynamicFriction(), matB.GetDynamicFriction());
    m_fMixedStaticFriction = frictionOp(matA.GetStaticFriction(), matB.GetStaticFriction());
    m_fMixedRestitution = restitutionOp(matA.GetRestitution(), matB.GetRestitution());
  }

}