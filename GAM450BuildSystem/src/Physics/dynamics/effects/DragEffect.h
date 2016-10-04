/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: DragEffect.h 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_PHYSICS_DYNAMICS_EFFECTS_DRAG
#define H_PAINT_PHYSICS_DYNAMICS_EFFECTS_DRAG

#include "physics/dynamics/effects/Effect.h"
#include "core/components/base/Component.h"

namespace Physics
{
  class RigidBody;

  class DragEffect : public Core::GameComponent, public Effect
  {
  public:
    // the damping factor in -bv
    float m_fBTerm;

    DragEffect(void);
    virtual ~DragEffect(void) final;

    //static void Serialize(File &file, Variable var);
    //static void Deserialize(File &file, Variable var);

    void UpdateForce(RigidBody *pBody);
    
    float GetDampingFactor(void) const;
    void SetDampingFactor(float fBTerm);
  };
}

#endif