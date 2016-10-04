/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Material.h 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_PHYSICS_DYNAMICS_MATERIAL
#define H_PAINT_PHYSICS_DYNAMICS_MATERIAL

namespace Physics
{
  // The order of the mixtypes is relevant. The later ones have higher priority
  // than the earlier ones, meaning that between two different MixTypes, the
  // higher priority one is always used.
  enum class MixType
  {
    AVERAGE = 0,
    MINIMUM,
    MULTIPLY,
    MAXIMUM
  };

  class Material
  {
  private:
    float m_fStaticFriction;
    float m_fDynamicFriction;
    float m_fRestitution;
    float m_fDensity;
    
    MixType m_Friction;
    MixType m_Restitution;
  public:
    Material(void);

    float GetStaticFriction(void) const;
    void SetStaticFriction(float Friction);

    float GetDynamicFriction(void) const;
    void SetDynamicFriction(float Friction);

    float GetRestitution(void) const;
    void SetRestitution(float Restitution);

    float GetDensity(void) const;
    void SetDensity(float Density);

    MixType GetFrictionMixture(void) const;
    void SetFrictionMixture(MixType type);

    MixType GetRestitutionMixture(void) const;
    void SetRestitutionMixture(MixType type);
  };
}

#endif