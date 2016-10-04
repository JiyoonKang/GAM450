/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: BoxCollider.h 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_PHYSICS_BOX_COLLIDER
#define H_PAINT_PHYSICS_BOX_COLLIDER

#include "physics/colliders/Collider.h"
#include "physics/util/Geometry.h"

#include "core/components/base/Component.h"

namespace Physics
{
  class BoxCollider : public Core::GameComponent, public Collider
  {
  public:
    Box m_Box;

    BoxCollider(void);
    virtual ~BoxCollider(void) override;
    static std::string GetGuid(void);

    // from GameComponent
    virtual void Initialize(void) override;
    virtual void Remove(void) override;

    COMP_LUA_USABLE;

    // serialization routine
    //static void Serialize(File &file, Variable var);
    //static void Deserialize(File &file, Variable var);

    float4 GetHalfExtents(void) const 
    { return m_Box.m_HalfExtents; }
    
    float4 GetExtents(void) const 
    { return m_Box.GetExtents(); }
    
    void SetHalfExtents(float4 const &halfExtents) 
    { m_Box.m_HalfExtents = halfExtents; }
    
    void SetExtents(float4 const &halfExtents) 
    { m_Box.m_HalfExtents = halfExtents; m_Box.m_HalfExtents *= (1.f / 2.f); }
    
    float  GetWidth(void) const 
    { return m_Box.GetWidth(); }
    
    float  GetHeight(void) const 
    { return m_Box.GetHeight(); }
    
    float  GetDepth(void) const 
    { return m_Box.GetDepth(); }

    void   SetWidth(float fWidth) 
    { if (fWidth <= 0.f) return; m_Box.SetWidth(fWidth); }
    
    void   SetHeight(float fHeight) 
    { if (fHeight <= 0.f) return; m_Box.SetHeight(fHeight); }
    
    void   SetDepth(float fDepth) 
    { if (fDepth <= 0.f) return; m_Box.SetDepth(fDepth); }

    virtual float  GetVolume(void) const override 
    { return m_Box.GetVolume(); }
    
    virtual float4x4 GetInverseTensor(void) const override 
    { return m_Box.GetInverseInertiaTensor(); }

    Box const &GetShape(void) const
    { return m_Box; }
  };
}

#endif