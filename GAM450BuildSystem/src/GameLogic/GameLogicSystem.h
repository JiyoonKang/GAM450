/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: GameLogicSystem.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_GAME_LOGIC_SYSTEM
#define H_PAINT_GAME_LOGIC_SYSTEM

#include "physics/colliders/CollisionMgr.h"
#include "Core/systems/System.h"
#include "base/modules/handle/Handle.h"

#include "GameLogic/Level.h"

#include "GameLogic/BulletMgr.h"

namespace GameLogic
{
  class GameLogicSystem : public Core::ISystem
  {
  private:
    // all possible levels to be accessed in the game
    std::unordered_map<LevelID, GameLevel*> m_Levels;

    // the current level that we will be updating
    LevelID m_CurrentLevel;

    // whether we are paused or unpaused
    bool m_bPaused;

    // the manager of all bullets in the game
    

    void DeInitCurrentLevel(void);
    void InitCurrentLevel(void);
    void UpdateCurrentLevel(float dt);
    void UpdatePlayerControllers(float dt);
    void UpdateTimers(float dt);
    void UpdateStats(float dt);
    void UpdateFlag(float dt);
    

  public:
    GameLogicSystem(void);
    virtual ~GameLogicSystem(void) override;
    virtual std::string GetName(void) const override { return "Game Logic System"; }
    virtual void Initialize(void) override;
    virtual void Update(float dt) override;
    // will register player controllers and will update them
    virtual void RegisterComponents(void) override;
    LevelID GetCurrentLevel();
    // must take in a new level, can be an inherited version
    // or can be a default base class type level
    void RegisterLevel(LevelID levelId, GameLevel *pLevel);
    // will deintialize the current level if any exists, and initalize the new
    // level if any exists
    void SwitchLevel(LevelID levelId);
    void PlayMusic(std::string theTrack);
    virtual void ReceiveMessage(Core::Message& msg) override;

    void SetPaused(bool bPaused) { m_bPaused = bPaused; }
    bool GetPaused(void) const { return m_bPaused; }

    BulletMgr m_BulletMgr;
  };

  extern GameLogicSystem *GAMELOGIC_SYSTEM;
}

#endif