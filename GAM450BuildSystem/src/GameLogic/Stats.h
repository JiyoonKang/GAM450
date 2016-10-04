/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Stats.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_ORBIT_STATS
#define H_ORBIT_STATS

#include "core/components/base/Component.h"
#include "core/systems/input/Gamepad.h"
#include "graphics/includes/GraphicsSystem/SkyeGraphics.h"

#include "base/util/math/Float4.h"

namespace GameLogic
{
  class Stats : public Core::GameComponent
  {
  private:
    int m_kills;
    int m_killStreak;
    int m_successiveKills;
    float m_timeOfLastKill;
    float m_timeTillRevive;
    bool m_isAlive;

  public:
    Stats(void);
    void Update(float dt);
    bool GetAlive() { return m_isAlive; }
    void SetAlive(bool newAlive) { m_isAlive = newAlive; }
    float GetRevive() { return m_timeTillRevive; }
    void SetRevive(float newTime) { m_timeTillRevive = newTime; }
    void IncreaseKills() { ++m_kills; ++m_successiveKills; }
    void IncreaseKillStreak() { ++m_killStreak; }
    int GetSuccessiveKills() { return m_successiveKills; }
    float GetTimeOfLastKill() { return m_timeOfLastKill; }
    void GotAKill();
    void Respawn();
    void Death(bool endOfGame);
    float4 m_deathPos;
    float4 m_respawnPos;
    int m_health;
    int m_score;
    bool m_gameRunning;

    COMP_LUA_USABLE;
  };
}

#endif