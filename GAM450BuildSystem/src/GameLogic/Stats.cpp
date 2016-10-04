/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Stats.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "GameLogic/Stats.h"
#include "core/types/space/Space.h"
#include "core/types/object/Object.h"
#include "core/components/transform/CTransform.h"
//#include "core/components/base/Component.h"

#include "physics/dynamics/RigidBody.h"
#include "physics/core/PhysicsSystem.h"
#include "physics/dynamics/Joint.h"
#include "physics/util/Geometry.h"

#include "graphics/includes/GraphicsSystem/PointLight.h"

#include "GameLogic/Level.h"
#include "GameLogic/BulletMgr.h"
#include "GameLogic/Flag.h"
#include "GameLogic/GameLogicSystem.h"

#include "SkyeAwesomium/includes/SkyeAwesomium/JSBroadcaster.h"
#include "SkyeAwesomium/includes/SkyeAwesomium/SkyeAwesomium.h"

#include <iostream>
#include <sstream>

namespace GameLogic
{
  Stats::Stats() : m_kills(0), m_killStreak(0), m_successiveKills(0), m_timeOfLastKill(0.0), m_timeTillRevive(0.0), m_isAlive(true), m_health(25), m_score(0), m_gameRunning(true)
  {
    
  }

  void Stats::Update(float dt)
  {
    if (m_gameRunning)
    {
      if (m_isAlive == false && m_timeTillRevive > 0.0f)
      {
        PlayerController* pPlayer = static_cast<PlayerController*>(GetOwner()->GetComponent(ePlayerController));
        m_timeTillRevive -= dt;
        if (m_timeTillRevive >= 2.0f && m_timeTillRevive <= 3.0f)
        {
          JSBroadcaster broadcaster;
          broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
          broadcaster.AddArgument(pPlayer->m_nGamePadID + 1);
          broadcaster.AddArgument(4);
          broadcaster.InvokeJSMethod("togglePlayerDead");
        }
        //If time start time is at 2...
        else if (m_timeTillRevive >= 1.0f && m_timeTillRevive <= 3.0f)
        {
          JSBroadcaster broadcaster;
          broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
          broadcaster.AddArgument(pPlayer->m_nGamePadID + 1);
          broadcaster.AddArgument(5);
          broadcaster.InvokeJSMethod("togglePlayerDead");
        }
        //If time start time is at 1...
        else if (m_timeTillRevive <= 1.0 && m_timeTillRevive > 0)
        {
          JSBroadcaster broadcaster;
          broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
          broadcaster.AddArgument(pPlayer->m_nGamePadID + 1);
          broadcaster.AddArgument(6);
          broadcaster.InvokeJSMethod("togglePlayerDead");
        }
      }
      else if (m_isAlive == false && m_timeTillRevive < 0.0f)
      {
        Respawn();
      }
      if (m_health <= 0)
      {
        PlayerController* thePC = reinterpret_cast<PlayerController*>(GetOwner()->GetComponent("PlayerController"));
        if (thePC->m_bHasFlag)
        {
          thePC->m_bHasFlag = false;
          std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
          for each(auto sp in Spaces)
          {
            Base::ObjectAllocator *pFlagArray = sp->GetComponents(eFlag);
            if (!pFlagArray)
              continue;

            //Input::MouseEvent::SetScollDelta(WHEEL_DELTA * HIWORD(wParam));
            for (unsigned nIdx = 0; nIdx < pFlagArray->Size(); ++nIdx)
            {
              Core::GameComponent *pComp = reinterpret_cast<Core::GameComponent*>((*pFlagArray)[nIdx]);
              if (!pComp)
                continue;

              GameLogic::Flag *pFlag = dynamic_cast<GameLogic::Flag*>(pComp);
              if (!pFlag)
                continue;

              pFlag->m_bActive = false;
              pFlag->m_hAttachedObject = Base::Handle();
              pFlag->m_fWaiting = pFlag->m_fTimeTillReturn;
              Renderable* render = static_cast<Renderable*>(pFlag->GetOwner()->GetComponent(eRenderable));
              if (!render)
                continue;
              render->m_IsVisible = true;
            }
          }
        }
        Death(false);
      }
      if (m_timeOfLastKill > 0.0f)
      {
        m_timeOfLastKill -= dt;
      }
      else
      {
        m_successiveKills = 0;
      }
    }
  }

  void Stats::Respawn()
  {
    m_isAlive = true;
    PlayerController *pController = GetOwner()->GetComponent<PlayerController>(ePlayerController);

    char buff[50] = { 0 };
    //get the game pad id in the buffer
    itoa((int)pController->m_nGamePadID + 1, buff, 10);
    std::string msg = std::string("JSMethod updatePlayerHealth ") + std::string(buff) + std::string(" ");
    itoa(m_health, buff, 10);
    msg += std::string(buff);

    Core::MessageUI uiM(msg);

    Core::ENGINE->SystemMessage(uiM);
    Core::Transform* myTransform = reinterpret_cast<Core::Transform*>(GetOwner()->GetComponent("Transform"));
    myTransform->SetTranslation(m_respawnPos);
    pController->m_fOverHeat = 0.0f;


    JSBroadcaster broadcaster;
    broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
    broadcaster.AddArgument(pController->m_nGamePadID + 1);
    broadcaster.AddArgument(0);
    broadcaster.InvokeJSMethod("togglePlayerDead");
    Physics::RigidBody* pBody = static_cast<Physics::RigidBody*>(GetOwner()->GetComponent(eRigidBody));
    if (!pBody)
      return;
    pBody->SetVelocity(float4(0.0f, 0.0f, 0.0f, 0.0f));

  }

  void Stats::Death(bool endOfGame)
  {
    m_isAlive = false;
    m_health = 25;
    m_timeTillRevive = 5.0f;
    //m_score += (m_killStreak * m_killStreak) * 100;
    m_killStreak = 0;
    Core::Transform* myTransform = reinterpret_cast<Core::Transform*>(GetOwner()->GetComponent("Transform"));
    PlayerController *pController = GetOwner()->GetComponent<PlayerController>(ePlayerController);
    Physics::RigidBody* pBody = static_cast<Physics::RigidBody*>(GetOwner()->GetComponent(eRigidBody));
    if (!myTransform || !pController || !pBody)
      return;
    if (!endOfGame)
    {
      GAMELOGIC_SYSTEM->m_BulletMgr.AddDeath(pController->m_Color, space, myTransform->GetTranslation());
      if (!Audio::AUDIOMANAGER->GetAllMute())
      {
        Audio::SoundInstance instance;
        instance.SetVolume(1.0f);
        Audio::AUDIOMANAGER->PlayEvent("Death", &instance);
        instance.SetVolume(1.0f);
      }
      JSBroadcaster broadcaster;
      broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
      broadcaster.AddArgument(pController->m_nGamePadID + 1);
      broadcaster.AddArgument(1);
      broadcaster.InvokeJSMethod("togglePlayerDead");
    }

    pBody->SetVelocity(float4(0.0f, 0.0f, 0.0f, 0.0f));
    myTransform->SetTranslation(m_deathPos);

    // remove any bullets attached to this player
    BulletMgr::RemoveBulletsAttached(GetOwner());
    
    pController->m_CameraForward = pController->m_DefaultForward;
    pController->m_PlayerForward = pController->m_DefaultForward;
    myTransform->SetForwardVector(pController->m_PlayerForward);
    pController->m_fCurrPhi = 0.0f;
    pController->m_fCurrTheta = 0.0f;
  }

  void Stats::GotAKill()
  {
    IncreaseKills();
    IncreaseKillStreak();
    //m_timeOfLastKill = 1.0f;
    //m_score += 100;
  }

  COMP_LUA_DEP(Stats);
}