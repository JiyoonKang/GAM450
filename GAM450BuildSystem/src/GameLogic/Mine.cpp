/*******************************************************************************
* All content © 2015 DigiPen (USA) Corporation, all rights reserved.
* Reproduction or disclosure of this file or its contents without the prior
* written consent of DigiPen Institute of Technology is prohibited.
*
* File: BulletComponent.cpp
* Author:
* Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
******************************************************************************/
#include "pch/precompiled.h"
#include "GameLogic/Mine.h"
#include "core/types/object/Object.h"
#include "core/types/space/Space.h"
#include "core/components/transform/CTransform.h"
#include "physics/util/Geometry.h"

namespace GameLogic
{
  Mine::Mine(void)
    : m_pAttachedObject(nullptr)
    //, m_nJointHandle(-1)
    , m_fCurrTime(0.f)
    , m_fMaxTime(DEFAULT_MAX_TIME)
    //, m_fRadius(DEFAULT_RADIUS)
    , m_LocalAxis()
    , m_bActive(false)
  {

  }

  void Mine::UpdateMine(float dt)
  {
    m_fCurrTime += dt;
    if (m_bActive == false && m_fCurrTime >= m_fMaxTime)
    {
      m_bActive = true;
    }
    else if (m_bActive)
    {

    }
  }

  void Mine::RemoveMine(void)
  {
    GetOwner()->Destroy();
  }

  void Explode(void)
  {

  }

  COMP_LUA_DEP(Mine);
}