/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: SphereCollider.h 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_PHYSICS_SPHERE_COLLIDER
#define H_PAINT_PHYSICS_SPHERE_COLLIDER

#include "physics/Colliders/Collider.h"
#include "physics/util/Geometry.h"

#include "core/components/base/Component.h"

namespace Physics
{
  class SphereCollider : public Core::GameComponent, public Collider
  {
  public:
    Sphere m_Sphere;

    SphereCollider(void);
    virtual ~SphereCollider(void) override;
    static std::string GetGuid(void);

    // from GameComponent
    virtual void Initialize(void) override;
    virtual void Remove(void) override;

    // serialization Routine
    //static void Serialize(File &file, Variable var);
    //static void Deserialize(File &file, Variable var);

    float GetRadius(void) const 
    { return m_Sphere.m_fRadius; }
    
    void SetRadius(float fRadius) 
    { m_Sphere.m_fRadius = fRadius; }
    
    virtual float GetVolume(void) const override 
    { return m_Sphere.GetVolume(); }
    
    virtual float4x4 GetInverseTensor(void) const override 
    { return m_Sphere.GetInverseInertiaTensor(); }

    Sphere const &GetShape(void) const
    { return m_Sphere; }
  };
}

#endif