/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Collider.h 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_PHYSICS_COLLIDER
#define H_PAINT_PHYSICS_COLLIDER

#include <string>
#include "physics/dynamics/Material.h"
#include "base/util/math/Float4x4.h"
#include "core/components/base/Component.h"

#define NUM_COLLIDERS 3

namespace Physics
{
  class ICollider
  {
  private:

  public:
    virtual ~ICollider(void) {}

    virtual std::string GetGuid(void) const = 0;

    virtual float GetVolume(void) const = 0;
    virtual float4x4 GetInverseTensor(void) const = 0;
  };

  class Collider : public ICollider//, public Core::GameComponent
  {
  private:

  protected:
    // a unique guid that is used to identify the type of the collider
    std::string m_szGuid;
    Material m_Material;

  public:
    Collider(void);
    virtual ~Collider(void) override;
    virtual std::string GetGuid(void) const final;

    // from GameComponent
    //virtual void Initialize(void) override;
    //virtual void Remove(void) override;

    Material const &GetMaterial(void) const;
    Material &GetMaterial(void);

    virtual float GetVolume(void) const override { return 0.f; }
    virtual float4x4 GetInverseTensor(void) const override { return float4x4(); }
  };
}

#endif