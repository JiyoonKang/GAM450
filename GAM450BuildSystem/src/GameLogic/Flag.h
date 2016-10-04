/*******************************************************************************
* All content © 2015 DigiPen (USA) Corporation, all rights reserved.
* Reproduction or disclosure of this file or its contents without the prior
* written consent of DigiPen Institute of Technology is prohibited.
*
* File: BulletComponent.h
* Author:
* Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
******************************************************************************/
#pragma once
#ifndef H_PAINT_GAME_LOGIC_FLAG_COMPONENT
#define H_PAINT_GAME_LOGIC_FLAG_COMPONENT

#include "core/components/base/Component.h"
#include "base/util/math/float4.h"
#include "base/modules/handle/Handle.h"

#define DEFAULT_MAX_TIME 10.f
#define DEFAULT_RADIUS 2.f

namespace GameLogic
{
  class Flag : public Core::GameComponent
  {
  public:
    //Core::GameObject *m_pAttachedObject;
    Base::Handle m_hAttachedObject;
    bool m_bActive;
    float m_fTimeTillPoints;
    float m_fGettingPoints;
    float m_fTimeTillReturn;
    float m_fWaiting;
    float4 m_startPos;

    Flag(void);

    // and possibly removes itself if it's time is over the limit
    // true if it removed itself
    void Update(float dt);
  };
}

#endif