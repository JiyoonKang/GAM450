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
#ifndef H_PAINT_GAME_LOGIC_MINE_COMPONENT
#define H_PAINT_GAME_LOGIC_MINE_COMPONENT

#include "core/components/base/Component.h"
#include "base/util/math/float4.h"

#define DEFAULT_MAX_TIME 10.f
#define DEFAULT_RADIUS 2.f

namespace GameLogic
{
  class Mine : public Core::GameComponent
  {
  public:
    Core::GameObject *m_pAttachedObject;

    //int m_nJointHandle; // the joint between the bullet and the other body
    float m_fCurrTime; // the current time of a bullet, possibly will time out and remove itself when time
    float m_fMaxTime; // if current time is greater than max time we kill the bullet
    //float m_fRadius; // the max radius that another bullet is allowed to hit on
    float4 m_LocalAxis; // the vector from our origin to our position in local space
    bool m_bActive;

    Mine(void);

    COMP_LUA_USABLE;

    // and possibly removes itself if it's time is over the limit
    // true if it removed itself
    void UpdateMine(float dt);
    void RemoveMine(void);
    void Explode(void);
  };
}

#endif