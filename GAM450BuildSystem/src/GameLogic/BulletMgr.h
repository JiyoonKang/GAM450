/*******************************************************************************
* All content © 2015 DigiPen (USA) Corporation, all rights reserved.
* Reproduction or disclosure of this file or its contents without the prior
* written consent of DigiPen Institute of Technology is prohibited.
*
* File: BulletMgr.h
* Author:
* Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
******************************************************************************/
#pragma once
#ifndef H_PAINT_GAME_LOGIC_BULLET_MGR
#define H_PAINT_GAME_LOGIC_BULLET_MGR

#include "core/types/space/Space.h"
#include "core/types/object/Object.h"
#include "Physics/colliders/RayCastMgr.h"
#include "base/util/math/float4.h"
#include "base/modules/handle/Handle.h"

#include "GameLogic/BulletComponent.h"

#include <unordered_map>

namespace GameLogic
{
  // manages all bullets in the game, holds every single bullet that was fired
  // and automatically removes bullets when they either time out or a new
  // bullet that is a different color is placed on top of it
  class BulletMgr
  {
  private:
    // the map of gameobjects that have a bullet attached to it as the key
    // the value is a list or tree of bullet objects

    Core::GameObject* CreateBullet(Core::GameObject *pObject, float4 const &pos,
      float4 const &bulletColor, Core::GameSpace *pSpace, float fRadius, float fTime);

    Core::GameObject* CreateBullet(float4 const &pos, float4 const& facing, float4 const &bulletColor,
      Core::GameSpace *pSpace, float fRadius, float fTime, Core::GameObject* pPlayer);
  public:
    typedef std::list<Core::GameObject*> TBulletList;
    typedef std::unordered_map<Core::GameObject*, TBulletList> TBulletMap;
    static TBulletMap m_Bullets;
    BulletMgr(void);
    
    // we add a new bullet if it is not too close to the other bullets on the body
    // if one is too close but a different color, just replace the color with the new one
    // removes an old bullet if it reaches over max count
    bool AddBullet(Physics::RayCastResult const &result, 
      float4 const &bulletColor, Core::GameSpace *pSpace, float fRadius = DEFAULT_RADIUS, 
      float fMaxTime = DEFAULT_MAX_TIME);

    bool AddBullet(float4 const &bulletColor, Core::GameSpace *pSpace, float4 const pos, float4 const& facing, Core::GameObject* pPlayer,
      float fRadius = DEFAULT_RADIUS, float fMaxTime = DEFAULT_MAX_TIME);

    bool AddDeath(float4 const &bulletColor, Core::GameSpace *pSpace, float4 const pos, float fRadius = DEFAULT_RADIUS,
      float fMaxTime = DEFAULT_MAX_TIME);

    void UpdateBullets(float dt);

    void RemoveAllBullets(void);

    // removes all bullets attached to this game object
    static void RemoveBulletsAttached(Core::GameObject *pObject);
  };
}

#endif