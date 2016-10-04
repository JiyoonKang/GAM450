/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Collider.cpp 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include <pch\precompiled.h>
#include <colliders\Collider.h>

namespace Physics
{
  Collider::Collider(void)
    : m_Material()
  {

  }

  Collider::~Collider(void)
  {

  }

  std::string Collider::GetGuid(void) const
  {
    return m_szGuid;
  }

  //void Collider::Initialize(void)
  //{
  //
  //}
  //
  //void Collider::Remove(void)
  //{
  //
  //}

  Material const &Collider::GetMaterial(void) const
  {
    return m_Material;
  }

  Material &Collider::GetMaterial(void)
  {
    return m_Material;
  }
}