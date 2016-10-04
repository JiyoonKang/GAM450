/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: CapsuleCollider.h 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_PHYSICS_CAPSULE_COLLIDER
#define H_PAINT_PHYSICS_CAPSULE_COLLIDER

#include "physics/colliders/Collider.h"
#include "physics/util/Geometry.h"

#include "core/components/base/Component.h"

namespace Physics
{
  class CapsuleCollider : public Core::GameComponent, public Collider
  {
  public:
    Capsule m_Capsule;

    CapsuleCollider(void);
    virtual ~CapsuleCollider(void) override;
    static std::string GetGuid(void);

    // from GameComponent
    virtual void Initialize(void) override;
    virtual void Remove(void) override;

    COMP_LUA_USABLE;

    // serialization routine
    //static void Serialize(File &file, Variable var);
    //static void Deserialize(File &file, Variable var);

    float4 GetTopPoint(void) const 
    { return m_Capsule.m_TopPoint; }
    
    float4 GetBottomPoint(void) const 
    { return m_Capsule.m_BottomPoint; }
    
    float  GetRadius(void) const 
    { return m_Capsule.m_fRadius; }

    float GetHeight(void) const 
    { return m_Capsule.GetHeight(); }

    void SetHeight(float fHeight)
    { return m_Capsule.SetHeight(fHeight); }

    void SetRadius(float fRadius)
    { m_Capsule.m_fRadius = fRadius; }
    
    virtual float GetVolume(void) const override 
    { return m_Capsule.GetVolume(); }
    
    virtual float4x4 GetInverseTensor(void) const override 
    { return m_Capsule.GetInvereInertiaTensor(); }

    Capsule const &GetShape(void) const
    { return m_Capsule; }
  };
}

#endif