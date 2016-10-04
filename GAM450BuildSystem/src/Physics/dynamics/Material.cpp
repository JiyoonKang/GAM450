/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Material.cpp 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include <pch\precompiled.h>
#include <Dynamics\Material.h>
#include <algorithm>

namespace Physics
{
  Material::Material(void)
    : m_fStaticFriction(0.0f)
    , m_fDynamicFriction(0.0f)
    , m_fRestitution(0.f)
    , m_fDensity(1.f)
    , m_Friction(MixType::AVERAGE)
    , m_Restitution(MixType::AVERAGE)
  {

  }

  float Material::GetStaticFriction(void) const
  {
    return m_fStaticFriction;
  }

  void Material::SetStaticFriction(float Friction)
  {
    if (Friction < 0.f)
      return;

    m_fStaticFriction = Friction;
  }

  float Material::GetDynamicFriction(void) const
  {
    return m_fDynamicFriction;
  }

  void Material::SetDynamicFriction(float Friction)
  {
    if (Friction < 0.f)
      return;

    m_fDynamicFriction = Friction;
  }

  float Material::GetRestitution(void) const
  {
    return m_fRestitution;
  }

  void Material::SetRestitution(float Restitution)
  {
    // TODO, change this out for a clamp function
    if (Restitution < 0.f || Restitution > 1.f)
      return;

    m_fRestitution = Restitution;
  }

  float Material::GetDensity(void) const
  {
    return m_fDensity;
  }

  void Material::SetDensity(float Density)
  {
    if (Density <= 0.f)
      return;

    m_fDensity = Density;
  }

  MixType Material::GetFrictionMixture(void) const
  {
    return m_Friction;
  }

  void Material::SetFrictionMixture(MixType type)
  {
    m_Friction = type;
  }

  MixType Material::GetRestitutionMixture(void) const
  {
    return m_Restitution;
  }

  void Material::SetRestitutionMixture(MixType type)
  {
    m_Restitution = type;
  }
}