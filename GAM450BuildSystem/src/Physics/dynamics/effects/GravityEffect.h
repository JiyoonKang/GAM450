/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: GravityEffect.h 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once 
#ifndef H_PAINT_PHYSICS_DYNAMICS_EFFECTS_GRAVITY
#define H_PAINT_PHYSICS_DYNAMICS_EFFECTS_GRAVITY

#include "physics/dynamics/effects/Effect.h"
#include "core/components/base/Component.h"
#include "Base/util/file/File.h"
#include "base/modules/introspection/variable/Variable.h"

namespace Physics
{
  class GravityEffect : public Core::GameComponent, public Effect
  {
  private:

  public:
    GravityEffect(void);
    GravityEffect(float4 const &Direction, float fMagnitude);
    virtual ~GravityEffect(void) final;

    //static void Serialize(File &file, Variable var);
    //static void Deserialize(File &file, Variable var);
  };
}

#endif