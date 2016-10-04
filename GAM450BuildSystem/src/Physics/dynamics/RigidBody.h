/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: RigidBody.h 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_PHYSICS_DYNAMICS_RIGID_BODY
#define H_PAINT_PHYSICS_DYNAMICS_RIGID_BODY

#include "Base/util/math/float4.h"
#include "Base/util/math/float4x4.h"

#include "core/components/base/Component.h"

namespace Physics
{
  enum class DynamicState
  {
    DYNAMIC,
    STATIC,
    KINEMATIC,
    SLEEPING
  };

  class RigidBody : public Core::GameComponent
  {
    friend class PhysicsSystem;
  private:
    // quick way of knowing if the rigidbody has been initialized
    bool m_bInit;

    float4 m_Acceleration;
    // the temporary force that users can explictely add to a rigidbody, will only
    // be applied once to the rigidbody, if a continous force is desired see
    // ConstantEffect
    float4 m_AppliedForce;

    float4 m_AngularAcceleration;
    // temporary force that users can add to a rigidbody, if a constant torque
    // is desired see ConstantEffect
    float4 m_AppliedTorque;

    // we usually use the inverse mass more than the regular mass, saves us
    // from performing unnecessary operations, will be 0.f if the object is static
    float m_fInverseMass;
    // holds the inverse tensor instead of the regular tensor, saves from
    // having to perform unncessary inverse operations, the inverse tensor
    // will always be a diagonal matrix, if the object is static it will be
    // the zero matrix
    float4x4 m_InverseTensor;

  public:
    float4 m_Velocity;
    float4 m_AngularVelocity;

    // your current dynamic state
    DynamicState m_State;
    bool m_bStateDynamic;
    bool m_bGhost;
    
    // TODO, Should I implement these, ask Judy
    RigidBody(RigidBody const &) {}
    RigidBody &operator =(RigidBody const &) {return *this;}
  public:
    RigidBody(void);
    virtual ~RigidBody(void) override;
    // virtual from GameComponent, can I call Init here? ask Judy
    // can I register with the physics system here? :D
    virtual void Initialize(void) override;
    virtual void Remove(void) override;

    // serialiation routine
    //static void Serialize(File &file, Variable var);
    //static void Deserialize(File &file, Variable var);

    void Init(void);

    float4 const &GetVelocity(void) const;
    void SetVelocity(float4 const &Velocity);

    float4 const &GetAngularVelocity(void) const;
    void SetAngularVelocity(float4 const &AngularVelocity);

    float4 const &GetAcceleration(void) const;
    float4 const &GetAngularAcceleration(void) const;

    DynamicState GetCurrentState(void) const;
    void SetState(DynamicState state);

    float GetInverseMass(void);
    float GetMass(void);

    float4x4 GetInverseInertiaTensorW(void);
    float4x4 const &GetInverseInertiaTensorL(void);

    void ApplyImpulse(float4 const &Impulse, float4 const &ContactPoint);
    void ApplyLinearImpulse(float4 const &Impulse);
    void ApplyAngularImpulse(float4 const &Impulse);

    void AddForce(float4 const &Force);
    void AddTorque(float4 const &Torque);

    float4 const &GetForce(void) const;
    float4 const &GetTorque(void) const;

    void IntegrateAcceleration(float dt);
    void IntegrateVelocity(float dt);

    COMP_LUA_USABLE;
  };
}

#endif