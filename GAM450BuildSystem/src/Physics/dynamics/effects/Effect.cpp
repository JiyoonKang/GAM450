/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Effect.cpp 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include <pch\precompiled.h>
#include <dynamics\effects\Effect.h>

namespace Physics
{
  Effect::Effect(void)
    : m_Direction()
    , m_fMagnitude(0.f)
    , m_bActive(true)
    , m_szGuid("{E63A1309-3920-4763-9FB3-2396416FBF87}")
  {
  }

  Effect::Effect(float4 const &Direction, float fMagnitude)
    : m_Direction(Direction)
    , m_fMagnitude(fMagnitude)
    , m_bActive(true)
    , m_szGuid("{E63A1309-3920-4763-9FB3-2396416FBF87}")
  {

  }

  Effect::~Effect(void)
  {

  }

  float4 const &Effect::GetDirection(void) const
  {
    return m_Direction;
  }

  void Effect::SetDirection(float4 const &Direction)
  {
    m_Direction = Direction;
  }

  float Effect::GetMagnitude(void) const
  {
    return m_fMagnitude;
  }

  void Effect::SetMagnitude(float fMagnitude)
  {
    if (fMagnitude < 0.f)
      return;

    m_fMagnitude = fMagnitude;
  }

  float4 Effect::GetTotalForce(void) const
  {
    return m_Direction * m_fMagnitude;
  }

  bool Effect::IsActive(void) const
  {
    return m_bActive;
  }

  void Effect::SetActive(bool bActive)
  {
    m_bActive = bActive;
  }
}