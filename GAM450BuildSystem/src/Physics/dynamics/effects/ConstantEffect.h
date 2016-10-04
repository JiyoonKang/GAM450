/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: ConstantEffect.h 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_PHYSICS_DYNAMICS_EFFECTS_CONSTANT
#define H_PAINT_PHYSICS_DYNAMICS_EFFECTS_CONSTANT

#include "physics/dynamics/effects/Effect.h"
#include "core/components/base/Component.h"

namespace Physics
{
  class ConstantEffect : public Core::GameComponent, public Effect
  {
  public:
    float4 m_TorqueDirection;
    float m_fTorqueMagnitude;

    ConstantEffect(void);
    // used for creating a linear constant force
    ConstantEffect(float4 const &Direction, float fMagnitude);
    virtual ~ConstantEffect(void) final;

    //static void Serialize(File &file, Variable var);
    //static void Deserialize(File &file, Variable var);

    float4 GetTotalTorque(void) const;

    float4 const &GetTorqueDirection(void) const;
    void SetTorqueDirection(float4 const &Direction);

    float GetTorqueMagnitude(void) const;
    void SetTorqueMagnitude(float fMagnitude);
  };
}

#endif