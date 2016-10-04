/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Timer.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "GameLogic/Timer.h"
#include "GameLogic/Stats.h"
#include "GameLogic/PlayerController.h"
#include "GameLogic/Flag.h"
#include "core/types/space/Space.h"
#include "core/types/object/Object.h"
#include "core/engine/core/Engine.h"

#include "physics/dynamics/RigidBody.h"
#include "audio/components/emitter/SoundEmitter.h"
#include "SkyeAwesomium/includes/SkyeAwesomium/JSBroadcaster.h"
#include "SkyeAwesomium/includes/SkyeAwesomium/SkyeAwesomium.h"
#include <Graphics/includes/GraphicsSystem/Fade.h>
#include <Graphics/includes/GraphicsSystem/Emissive.h>


#include <iostream>
#include <sstream>

namespace GameLogic
{
  Timer::Timer() :
  m_time(0.f)
  , m_startTime(4.0f)
  , m_endTime(2.5f)
  , m_nextLevel(0)
  , m_scoreP1(0)
  , m_scoreP2(0)
  , m_scoreP3(0)
  , m_scoreP4(0)
  , m_gameRunning(true)
  , m_uiBufferTime(0.0f)
  , winning(-1)
	{
	}

	void Timer::Update(float dt)
	{
    if (thePlayers.size() == 0)
      return;

    if (m_startTime <= 0)
    {
      if (m_time < 0)
      {
        if (m_endTime > 0.0f)
        {
          GameLogic::Stats* p1 = (GameLogic::Stats*)thePlayers[0]->GetComponent(eStats);
          p1->Death(true);
          m_scoreP1 = p1->m_score;
          p1->m_gameRunning = false;
          GameLogic::Stats* p2 = (GameLogic::Stats*)thePlayers[1]->GetComponent(eStats);
          p2->Death(true);
          m_scoreP2 = p2->m_score;
          p2->m_gameRunning = false;
          GameLogic::Stats* p3 = (GameLogic::Stats*)thePlayers[2]->GetComponent(eStats);
          p3->Death(true);
          m_scoreP3 = p3->m_score;
          p3->m_gameRunning = false;
          GameLogic::Stats* p4 = (GameLogic::Stats*)thePlayers[3]->GetComponent(eStats);
          p4->Death(true);
          m_scoreP4 = p4->m_score;
          p4->m_gameRunning = false;
        }

        if (m_gameRunning)
        {
          if (!Audio::AUDIOMANAGER->GetAllMute())
          {
            Audio::SoundInstance instance;
            instance.SetVolume(1.0f);
            Audio::AUDIOMANAGER->StopAll();
            Audio::AUDIOMANAGER->PlayEvent("GameEnd (2)", &instance);
          }
        }
        m_gameRunning = false;
        if (!m_gameRunning)
        {
          m_endTime -= dt;
          if (m_endTime <= 0.0f)
          {
            JSBroadcaster broadcaster;
            broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
            broadcaster.InvokeJSMethod("goMainMenu");

            Core::MessageUI m("ChangeLevel MainMenu");
            Core::ENGINE->SystemMessage(m); 
          }
          
        }
        std::string winnerColor = "";
        JSBroadcaster broadcaster;
        broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
        if (m_scoreP1 > m_scoreP2 && m_scoreP1 > m_scoreP3 && m_scoreP1 > m_scoreP4)
        {
          winnerColor = "Blue"; 
          broadcaster.AddArgument(winnerColor.c_str());
        }
        else if (m_scoreP2 > m_scoreP1 && m_scoreP2 > m_scoreP3 && m_scoreP2 > m_scoreP4)
        {
          winnerColor = "Purple";
          broadcaster.AddArgument(winnerColor.c_str());
        }
        else if (m_scoreP3 > m_scoreP2 && m_scoreP3 > m_scoreP1 && m_scoreP3 > m_scoreP4)
        {
          winnerColor = "Green";
          broadcaster.AddArgument(winnerColor.c_str());
        }
        else if (m_scoreP4 > m_scoreP2 && m_scoreP4 > m_scoreP3 && m_scoreP4 > m_scoreP1)
        {
          winnerColor = "Red";
          broadcaster.AddArgument(winnerColor.c_str());
        }
        else
        {
          broadcaster.AddArgument("Draw");
        }
        broadcaster.InvokeJSMethod("displayWinner");
      }
      m_time -= dt;

      GameLogic::Stats* p1 = (GameLogic::Stats*)thePlayers[0]->GetComponent(eStats);
      m_scoreP1 = p1->m_score;
      GameLogic::Stats* p2 = (GameLogic::Stats*)thePlayers[1]->GetComponent(eStats);
      m_scoreP2 = p2->m_score;
      GameLogic::Stats* p3 = (GameLogic::Stats*)thePlayers[2]->GetComponent(eStats);
      m_scoreP3 = p3->m_score;
      GameLogic::Stats* p4 = (GameLogic::Stats*)thePlayers[3]->GetComponent(eStats);
      m_scoreP4 = p4->m_score;
      int preWinning = winning;
      if (m_scoreP1 > m_scoreP2 && m_scoreP1 > m_scoreP3 && m_scoreP1 > m_scoreP4)
      {
        if (winning != 1) //notify hud
        {
          winning = 1;
          std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
          for each(auto sp in Spaces)
          {
            ObjectAllocator& objects = sp->GetObjectList();
            for (unsigned int i = 0; i < objects.Size(); ++i)
            {
              Core::GameObject* obj = reinterpret_cast<Core::GameObject*>(objects[i]);
              if (obj->GetName() != "Timer" && obj->GetName() != "Light" && obj->GetName() != "Player1" && obj->GetName() != "Player2" && obj->GetName() != "Player3" && obj->GetName() != "Player4" && obj->GetName() != "")
              {
                EmissiveComponent* pEmissive = reinterpret_cast<EmissiveComponent*>(obj->GetComponent(eEmissiveComponent));
                PlayerController* p1Controller = (PlayerController*)thePlayers[0]->GetComponent(ePlayerController);
                float4 newCol = p1Controller->m_Color;
                newCol.w = pEmissive->m_emittedColor.w;
                pEmissive->m_emittedColor = newCol;
              }
            }
          }
          JSBroadcaster broadcaster;
          broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
          broadcaster.AddArgument("blue");
          broadcaster.InvokeJSMethod("showLeading");
        }
      }
      else if (m_scoreP2 > m_scoreP1 && m_scoreP2 > m_scoreP3 && m_scoreP2 > m_scoreP4)
      {
        if (winning != 2) //notify hud
        {
          winning = 2;
          std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
          for each(auto sp in Spaces)
          {
            ObjectAllocator& objects = sp->GetObjectList();
            for (unsigned int i = 0; i < objects.Size(); ++i)
            {
              Core::GameObject* obj = reinterpret_cast<Core::GameObject*>(objects[i]);
              if (obj->GetName() != "Timer" && obj->GetName() != "Light" && obj->GetName() != "Player1" && obj->GetName() != "Player2" && obj->GetName() != "Player3" && obj->GetName() != "Player4" && obj->GetName() != "")
              {
                EmissiveComponent* pEmissive = reinterpret_cast<EmissiveComponent*>(obj->GetComponent(eEmissiveComponent));
                PlayerController* p2Controller = (PlayerController*)thePlayers[1]->GetComponent(ePlayerController);
                float4 newCol = p2Controller->m_Color;
                newCol.w = pEmissive->m_emittedColor.w;
                pEmissive->m_emittedColor = newCol;
              }
            }
          }

          JSBroadcaster broadcaster;
          broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
          broadcaster.AddArgument("purple");
          broadcaster.InvokeJSMethod("showLeading");
        }
      }
      else if (m_scoreP3 > m_scoreP2 && m_scoreP3 > m_scoreP1 && m_scoreP3 > m_scoreP4)
      {
        if (winning != 3) //notify hud
        {
          winning = 3;
          std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
          for each(auto sp in Spaces)
          {
            ObjectAllocator& objects = sp->GetObjectList();
            for (unsigned int i = 0; i < objects.Size(); ++i)
            {
              Core::GameObject* obj = reinterpret_cast<Core::GameObject*>(objects[i]);
              if (obj->GetName() != "Timer" && obj->GetName() != "Light" && obj->GetName() != "Player1" && obj->GetName() != "Player2" && obj->GetName() != "Player3" && obj->GetName() != "Player4" && obj->GetName() != "Player4" && obj->GetName() != "")
              {
                EmissiveComponent* pEmissive = reinterpret_cast<EmissiveComponent*>(obj->GetComponent(eEmissiveComponent));
                PlayerController* p3Controller = (PlayerController*)thePlayers[2]->GetComponent(ePlayerController);
                float4 newCol = p3Controller->m_Color;
                newCol.w = pEmissive->m_emittedColor.w;
                pEmissive->m_emittedColor = newCol;
              }
            }
          }

          JSBroadcaster broadcaster;
          broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
          broadcaster.AddArgument("green");
          broadcaster.InvokeJSMethod("showLeading");
        }
      }
      else if (m_scoreP4 > m_scoreP2 && m_scoreP4 > m_scoreP3 && m_scoreP4 > m_scoreP1)
      {
        if (winning != 4) //notify hud
        {
          winning = 4;
          std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
          for each(auto sp in Spaces)
          {
            ObjectAllocator& objects = sp->GetObjectList();
            for (unsigned int i = 0; i < objects.Size(); ++i)
            {
              Core::GameObject* obj = reinterpret_cast<Core::GameObject*>(objects[i]);
              if (obj->GetName() != "Timer" && obj->GetName() != "Light" && obj->GetName() != "Player1" && obj->GetName() != "Player2" && obj->GetName() != "Player3" && obj->GetName() != "Player4" && obj->GetName() != "Player4" && obj->GetName() != "" )
              {
                EmissiveComponent* pEmissive = reinterpret_cast<EmissiveComponent*>(obj->GetComponent(eEmissiveComponent));
                PlayerController* p4Controller = (PlayerController*)thePlayers[3]->GetComponent(ePlayerController);
                float4 newCol = p4Controller->m_Color;
                newCol.w = pEmissive->m_emittedColor.w;
                pEmissive->m_emittedColor = newCol;
              }
            }
          }

          JSBroadcaster broadcaster;
          broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
          broadcaster.AddArgument("red");
          broadcaster.InvokeJSMethod("showLeading");
        }
      }
      else
      {
        std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
        for each(auto sp in Spaces)
        {
          ObjectAllocator& objects = sp->GetObjectList();
          for (unsigned int i = 0; i < objects.Size(); ++i)
          {
            Core::GameObject* obj = reinterpret_cast<Core::GameObject*>(objects[i]);
            if (obj->GetName() != "Timer" && obj->GetName() != "Light" && obj->GetName() != "Player1" && obj->GetName() != "Player2" && obj->GetName() != "Player3" && obj->GetName() != "Player4" && obj->GetName() != "")
            {
               EmissiveComponent* pEmissive = reinterpret_cast<EmissiveComponent*>(obj->GetComponent(eEmissiveComponent));
               float4 newCol = float4(1.0f, 1.0f, 1.0f, 0.0f);
               newCol.w = pEmissive->m_emittedColor.w;
               pEmissive->m_emittedColor = newCol;
            }
          }
        }
      }
      if (preWinning != winning)
      {
        if (!Audio::AUDIOMANAGER->GetAllMute())
        {
          Audio::SoundInstance instance;
          instance.SetVolume(1.0f);
          Audio::AUDIOMANAGER->PlayEvent("MenuBack", &instance);
          instance.SetVolume(1.0f);
        }
      }

    }
    else if (m_gameRunning)
    {
      if (m_startTime >= 3.99f)
      {
        //(Fade*)thePlayers[0]->GetComponent("Fade");
        if (!Audio::AUDIOMANAGER->GetAllMute())
        {
          Audio::SoundInstance instance;
          instance.SetVolume(1.0f);
          Audio::AUDIOMANAGER->PlayEvent("GameStart", &instance);
          instance.SetVolume(1.0f);
        }
        Core::Transform* p1Trans = (Core::Transform*)thePlayers[0]->GetComponent(eTransform);

        Core::Transform* p2Trans = (Core::Transform*)thePlayers[1]->GetComponent(eTransform);

        Core::Transform* p3Trans = (Core::Transform*)thePlayers[2]->GetComponent(eTransform);

        Core::Transform* p4Trans = (Core::Transform*)thePlayers[3]->GetComponent(eTransform);

        PlayerController* p1Controller = (PlayerController*)thePlayers[0]->GetComponent(ePlayerController);

        PlayerController* p2Controller = (PlayerController*)thePlayers[1]->GetComponent(ePlayerController);

        PlayerController* p3Controller = (PlayerController*)thePlayers[2]->GetComponent(ePlayerController);

        PlayerController* p4Controller = (PlayerController*)thePlayers[3]->GetComponent(ePlayerController);

        Core::GameObject* flag = nullptr;
        std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
        for each(auto sp in Spaces)
        {
          flag = sp->GetFirstGameObjectNamed("Light");
          if (flag)
            break;
        }
        if (!flag)
          return;

        Core::Transform* flagTrans = (Core::Transform*)flag->GetComponent(eTransform);

        float4 newP1Dir = flagTrans->GetTranslation() - p1Trans->GetTranslation();
        float4 newP2Dir = flagTrans->GetTranslation() - p2Trans->GetTranslation();
        float4 newP3Dir = flagTrans->GetTranslation() - p3Trans->GetTranslation();
        float4 newP4Dir = flagTrans->GetTranslation() - p4Trans->GetTranslation();
        newP1Dir.y = 0.0f;
        newP2Dir.y = 0.0f;
        newP3Dir.y = 0.0f;
        newP4Dir.y = 0.0f;
        newP1Dir.Normalize();
        newP2Dir.Normalize();
        newP3Dir.Normalize();
        newP4Dir.Normalize();
        
        p1Controller->m_CameraForward = newP1Dir;
        p1Controller->m_DefaultForward = newP1Dir;
        p1Controller->m_PlayerForward = newP1Dir;
        p2Controller->m_CameraForward = newP2Dir;
        p2Controller->m_DefaultForward = newP2Dir;
        p2Controller->m_PlayerForward = newP2Dir;
        p3Controller->m_CameraForward = newP3Dir;
        p3Controller->m_DefaultForward = newP3Dir;
        p3Controller->m_PlayerForward = newP3Dir;
        p4Controller->m_CameraForward = newP4Dir;
        p4Controller->m_DefaultForward = newP4Dir;
        p4Controller->m_PlayerForward = newP4Dir;


      }
      m_startTime -= dt;

      //If time start time is at 3...
      if ( m_startTime >= 3.0f)
      {
        JSBroadcaster broadcaster;
        broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
        broadcaster.AddArgument(4);
        broadcaster.InvokeJSMethod("toggleAllPlayerDead");
        for (int i = 0; i < thePlayers.size(); ++i)
        {
          PlayerController* pPlayer = static_cast<PlayerController*>(thePlayers[i]->GetComponent(ePlayerController));
          pPlayer->UpdatePlayerStats();
        }
      }
      //If time start time is at 2...
      else if (m_startTime >= 2.0f)
      {
        JSBroadcaster broadcaster;
        broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
        broadcaster.AddArgument(5);
        broadcaster.InvokeJSMethod("toggleAllPlayerDead");
      }
      //If time start time is at 1...
      else if (m_startTime >= 1.0 && m_startTime > 0.0f)
      {
        JSBroadcaster broadcaster;
        broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
        broadcaster.AddArgument(6);
        broadcaster.InvokeJSMethod("toggleAllPlayerDead");
      }
      else if (m_startTime >= 0.0 && m_startTime > -1.0f)
      {
        JSBroadcaster broadcaster;
        broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
        broadcaster.AddArgument(7);
        broadcaster.InvokeJSMethod("toggleAllPlayerDead");
      }
      //else set the cross hair
      else
      {
        JSBroadcaster broadcaster;
        broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
        broadcaster.AddArgument(0);
        broadcaster.InvokeJSMethod("toggleAllPlayerDead");
      }
    }
    m_uiBufferTime -= dt;

    //only update the timers every time a second has passed instead of every frame
    if (m_uiBufferTime <= 0 && m_time >= 0)
    {
      JSBroadcaster broadcaster;
      broadcaster.SetCurrentGlobalWindow(AWESOMIUM->GetCurrentGlobalWindow());
      broadcaster.AddArgument((int)m_time);
      broadcaster.InvokeJSMethod("updateTimer");

      m_uiBufferTime = 1.0f;
    }
	}

  COMP_LUA_DEP(Timer);
}