/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: RigidBody.cpp 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

#include "physics/dynamics/RigidBody.h"
#include "physics/core/PhysicsSystem.h"

#include "physics/dynamics/effects/ConstantEffect.h"
#include "physics/dynamics/effects/DragEffect.h"
#include "physics/dynamics/effects/GravityEffect.h"

#include "core/types/object/Object.h"
#include "Core/types/space/Space.h"
#include "core/components/transform/CTransform.h"

#include "physics/colliders/BoxCollider.h"
#include "colliders/SphereCollider.h"
#include "colliders/CapsuleCollider.h"

namespace
{
  using namespace Physics;

  float4 CalculateAcceleration(RigidBody *pBody)
  {
    // grab my force effects and calculate acceleration
    Core::GameObject* obj = pBody->GetOwner();
    ConstantEffect *constant = 
      obj->GetComponent<ConstantEffect>(eConstantEffect);
    GravityEffect *gravity =
      pBody->GetOwner()->GetComponent<GravityEffect>(eGravityEffect);
    DragEffect *drag =
      pBody->GetOwner()->GetComponent<DragEffect>(eDragEffect);

    float4 result;

    if (constant && constant->IsActive())
      result += constant->GetTotalForce();
    if (gravity && gravity->IsActive())
      result += gravity->GetTotalForce() * pBody->GetMass();
    if (drag && drag->IsActive())
      result += drag->GetTotalForce();

    result += pBody->GetForce();

    return result * pBody->GetInverseMass();
  }

  float4 CalculateAngularAcceleration(RigidBody *pBody)
  {
    ConstantEffect *constant =
      pBody->GetOwner()->GetComponent<ConstantEffect>(eConstantEffect);

    float4 result;

    if (constant && constant->IsActive())
      result += constant->GetTotalTorque();

    result += pBody->GetTorque();

    return pBody->GetInverseInertiaTensorW() * result;
  }

  void UpdateForces(RigidBody *pBody)
  {
    DragEffect *drag = pBody->GetOwner()->GetComponent<DragEffect>(eDragEffect);

    if (drag)
      drag->UpdateForce(pBody);
  }

  Collider *GetCollider(RigidBody *pBody)
  {
    Collider *pBox = dynamic_cast<Collider*>(pBody->GetOwner()->GetComponent<Core::GameComponent>(eBoxCollider));
    Collider *pSphere = dynamic_cast<Collider*>(pBody->GetOwner()->GetComponent<Core::GameComponent>(eSphereCollider));
    Collider *pCapsule = dynamic_cast<Collider*>(pBody->GetOwner()->GetComponent<Core::GameComponent>(eCapsuleCollider));

    if (pBox)
      return pBox;
    else if (pSphere)
      return pSphere;
    else if (pCapsule)
      return pCapsule;
    else
      return nullptr;
  }

  float CalculateInverseMass(RigidBody *pBody)
  {
    if (pBody->GetCurrentState() == DynamicState::STATIC)
      return 0.f;

    // if no collider, just have a mass of 1, arbitrary constant
    Collider *pCollider = GetCollider(pBody);
    if (!pCollider)
      return 1.f;

    float fMass = pCollider->GetMaterial().GetDensity() * pCollider->GetVolume();

    if (Equals(fMass, 0.f))
      return 0.f;
    else
      return 1.f / fMass;
  }

  float4x4 CalculateInverseTensor(RigidBody *pBody)
  {
    Collider *pCollider = GetCollider(pBody);

    if (!pCollider)
    {
      float4x4 tensor;
      tensor.Identity();
      return tensor * pBody->GetInverseMass();
    }

    return pCollider->GetInverseTensor() * pBody->GetInverseMass();
  }
}

namespace Physics
{
  RigidBody::RigidBody(void)
    : m_Velocity()
    , m_Acceleration()
    , m_AppliedForce()
    , m_AngularVelocity()
    , m_AngularAcceleration()
    , m_AppliedTorque()
    , m_fInverseMass(0.f)
    , m_InverseTensor()
    , m_State(DynamicState::DYNAMIC)
    , m_bStateDynamic(true)
    , m_bInit(false)
    , m_bGhost(false)
  {
    m_InverseTensor.Identity();
  }

  RigidBody::~RigidBody(void)
  {
    if (PHYSICS)
      PHYSICS->RemoveRigidBodyMessages(this);
  }

  void RigidBody::Initialize(void)
  {
    //PhysicsSystem::RegisterRigidBody(self);

    Init();
  }

  void RigidBody::Remove(void)
  {
    if (PHYSICS)
      PHYSICS->RemoveRigidBodyMessages(this);
  }

  //void RigidBody::Serialize(File &file, Variable var)
  //{
  //  RigidBody *pBody = var.GetValue<RigidBody*>();
  //
  //  // get a pointer to the serializer
  //  Base::Serializer *s = Base::Serializer::Get();
  //
  //  TypeInfo const *pInfo = var.GetTypeInfo();
  //
  //  // variagle for members
  //  Variable mem;
  //
  //  // write to the file the name of the type
  //  file.Write("%s\n", pInfo->Name());
  //
  //  // increase padding
  //  s->Padding(file, s->GetPadLevel()++);
  //
  //  // make a bracket and new line
  //  file.Write("{\n");
  //
  //  // inverse mass
  //  s->Padding(file, s->GetPadLevel());
  //  file.Write("%s", "Inverse Mass");
  //  mem = Variable(pBody->m_fInverseMass);
  //  mem.Serialize(file);
  //
  //  // inverse tensor
  //  s->Padding(file, s->GetPadLevel());
  //  file.Write("%s", "Inverse Tensor");
  //  mem = Variable(pBody->m_InverseTensor);
  //  mem.Serialize(file);
  //
  //  // the state of the body, dynamic, static
  //  s->Padding(file, s->GetPadLevel());
  //  file.Write("%s", "Dynamic State");
  //  mem = Variable(pBody->m_State);
  //  mem.Serialize(file);
  //
  //  // done with members
  //  s->Padding(file, --s->GetPadLevel());
  //
  //  // write a bracket
  //  file.Write("}\n");
  //}
  //
  //void RigidBody::Deserialize(File &file, Variable var)
  //{
  //  RigidBody *pBody = var.GetValue<RigidBody*>();
  //  Base::Serializer *s = Base::Serializer::Get();
  //
  //  // peek function to figure out the starting level?
  //  int nStartLevel = s->GetPadLevel();
  //
  //  std::string szLine = file.GetLine('{');
  //  char buffer[1024] = {0};
  //
  //  // read for inverse mass
  //  file.Read("%s", &buffer);
  //  float invMass = 0.f;
  //  GET_TYPE(float)->Deserialize(file, invMass);
  //
  //  // read for inverse tensor
  //  file.Read("%s", &buffer);
  //  float4x4 invTensor;
  //  GET_TYPE(float4x4)->Deserialize(file, invTensor);
  //  
  //  // read for dynamic state
  //  file.Read("%s", &buffer);
  //  DynamicState state;
  //  GET_TYPE(DynamicState)->Deserialize(file, state);
  //
  //  // after I am done grabbing my data, set it in the body
  //  pBody->m_fInverseMass = invMass;
  //  pBody->m_InverseTensor = invTensor;
  //  pBody->m_State = state;
  //
  //  // not sure why these are necessary?
  //  file.GetLine('\n');
  //  file.GetLine('\n');
  //}

  float4 const &RigidBody::GetVelocity(void) const
  {
    return m_Velocity;
  }

  void RigidBody::SetVelocity(float4 const &Velocity)
  {
    m_Velocity = Velocity;
  }

  float4 const &RigidBody::GetAngularVelocity(void) const
  {
    return m_AngularVelocity;
  }

  void RigidBody::SetAngularVelocity(float4 const &AngularVelocity)
  {
    m_AngularVelocity = AngularVelocity;
  }

  float4 const &RigidBody::GetAcceleration(void) const
  {
    return m_Acceleration;
  }

  float4 const &RigidBody::GetAngularAcceleration(void) const
  {
    return m_AngularAcceleration;
  }

  DynamicState RigidBody::GetCurrentState(void) const
  {
    if (m_bStateDynamic)
      return DynamicState::DYNAMIC;
    else
      return DynamicState::STATIC;
  }

  void RigidBody::SetState(DynamicState state)
  {
    if (state == m_State)
      return;

    m_State = state;

    if (m_State == DynamicState::DYNAMIC)
      m_bStateDynamic = true;
    else
      m_bStateDynamic = false;

    // the very first init should be by the Initialize function
    if (m_bInit)
      Init();
  }

  float RigidBody::GetInverseMass(void)
  {
    if (!m_bInit)
      Init();

    return m_fInverseMass;
  }

  float RigidBody::GetMass(void)
  {
    if (!m_bInit)
      Init();

    // mgiht change this to a EPSILON check
    return m_fInverseMass == 0.f ? 0.f : 1 / m_fInverseMass;
  }

  float4x4 RigidBody::GetInverseInertiaTensorW(void)
  {
    if (!m_bInit)
      Init();

    Core::Transform *pTransform = 
      GetOwner()->GetComponent<Core::Transform>(eTransform);
    if (!pTransform)
      return float4x4();

    float4x4 Transform = pTransform->GetRotationMatrix();

    return Transform * (m_InverseTensor * Transform.Transpose());
  }

  float4x4 const &RigidBody::GetInverseInertiaTensorL(void)
  {
    if (!m_bInit)
      Init();

    return m_InverseTensor;
  }

  void RigidBody::ApplyImpulse(float4 const &Impulse, float4 const &ContactPoint)
  {
    m_Velocity += Impulse * GetInverseMass();
    m_AngularVelocity += GetInverseInertiaTensorW() * ContactPoint.Cross(Impulse);
  }

  void RigidBody::ApplyLinearImpulse(float4 const &Impulse)
  {
    m_Velocity += Impulse * GetInverseMass();
  }

  void RigidBody::ApplyAngularImpulse(float4 const &Impulse)
  {
    m_AngularVelocity += GetInverseInertiaTensorW() * Impulse;
  }

  void RigidBody::AddForce(float4 const &Force)
  {
    m_AppliedForce += Force;
  }

  void RigidBody::AddTorque(float4 const &Torque)
  {
    m_AppliedTorque += Torque;
  }

  float4 const &RigidBody::GetForce(void) const
  {
    return m_AppliedForce;
  }

  float4 const &RigidBody::GetTorque(void) const
  {
    return m_AppliedTorque;
  }

  void RigidBody::IntegrateAcceleration(float dt)
  {
    m_Acceleration = CalculateAcceleration(this);
    m_AngularAcceleration = CalculateAngularAcceleration(this);

    m_Velocity += m_Acceleration * dt;
    m_AngularVelocity += m_AngularAcceleration * dt;

    // goes through the entity and finds all effects and calls their update force functions
    UpdateForces(this);

    m_AppliedForce = float4(0.f, 0.f, 0.f, 0.f);
    m_AppliedTorque = float4(0.f, 0.f, 0.f, 0.f);
  }

  void RigidBody::IntegrateVelocity(float dt)
  {
    static float k = 0.98f;
   
    Core::Transform *pTransform = 
      GetOwner()->GetComponent<Core::Transform>(eTransform);
    if (!pTransform)
      return;

    float4 Position = pTransform->GetTranslation();
    Position += m_Velocity * dt * k;
    pTransform->SetTranslation(Position);

    quaternion Angular(0.f, m_AngularVelocity.x, m_AngularVelocity.y, m_AngularVelocity.z);
    quaternion Rotation = pTransform->GetRotation();
    Rotation += (1.f / 2.f) * Angular * Rotation * dt;
    pTransform->SetRotation(Rotation);
  }

  void RigidBody::Init(void)
  {
    m_fInverseMass = CalculateInverseMass(this);
    m_bInit = true;
    m_InverseTensor = CalculateInverseTensor(this);
  }

  COMP_LUA_DEP(RigidBody);
}