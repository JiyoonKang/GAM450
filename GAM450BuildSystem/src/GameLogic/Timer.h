/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Timer.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_ORBIT_GAME_TIMER
#define H_ORBIT_GAME_TIMER

#include "core/components/base/Component.h"
#include "core/systems/input/Gamepad.h"

#include "base/util/math/Float4.h"

namespace GameLogic
{
  class Timer : public Core::GameComponent
  {
  private:
    // the current number of seconds left in the round.
	  // the id of the next level to load
	  int m_nextLevel;
	  // the scores for the players
	  int m_scoreP1;
	  int m_scoreP2;
	  int m_scoreP3;
	  int m_scoreP4;
    // checking for if the game is running
    bool m_gameRunning;
  public:
    Timer(void);
	  //Timer(float startTime, int theNextLevel);
    std::vector<Core::GameObject*> thePlayers;
    void SetTime(float newTime) { m_time = newTime; }
    void Update(float dt);
    float m_time;
    float m_uiBufferTime;
    float m_startTime;
    float m_endTime;
    int winning;


    COMP_LUA_USABLE;
  };
}

#endif