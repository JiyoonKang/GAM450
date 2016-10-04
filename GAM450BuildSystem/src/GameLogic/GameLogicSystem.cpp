/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: GameLogicSystem.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "GameLogic/GameLogicSystem.h"

#include "Audio/components/emitter/SoundEmitter.h"

#include "Base/util/debug/DebugAssist.h"
#include "base/util/math/MathUtil.h"

#include "Core/systems/input/Input.h"
#include "Core/types/space/Space.h"
#include "Core/engine/core/Engine.h"
#include "Core/components/transform/CTransform.h"
#include "Core/types/object/Object.h"
#include "core/components/base/Component.h"

#include "GameLogic/PlayerController.h"
#include "GameLogic/Timer.h"
#include "GameLogic/Stats.h"
#include "GameLogic/BulletComponent.h"
#include "GameLogic/Flag.h"

#include "Graphics/includes/GraphicsSystem/Renderable.h"
#include "Graphics/includes/GraphicsSystem/Paintable.h"

#include "physics/colliders/BoxCollider.h"
#include "Physics/colliders/CapsuleCollider.h"
#include "Physics/colliders/SphereCollider.h"
#include "Physics/dynamics/RigidBody.h"
#include "physics/dynamics/effects/GravityEffect.h"
#include "physics/dynamics/effects/DragEffect.h"
#include "physics/core/PhysicsSystem.h"

#include "GameLogic/levels/MainMenu.h"
#include "GameLogic/Level.h"
//#include "GameLogic/levels/Level2.h"
//#include "GameLogic/levels/Level3.h"

#include "start/config/Config.h"

#include<sstream>

namespace GameLogic
{
  GameLogicSystem *GAMELOGIC_SYSTEM = nullptr;

  void GameLogicSystem::DeInitCurrentLevel(void)
  {
    if (m_CurrentLevel == LevelID::Invalid)
      return;

    auto it = m_Levels.find(m_CurrentLevel);
    if (it == m_Levels.end())
      return;
    
    //Input::MouseEvent::SetScollDelta(WHEEL_DELTA * HIWORD(wParam));
    // remove all bullets from the current level
    m_BulletMgr.RemoveAllBullets();

    it->second->Deinitialize();
    Physics::PHYSICS->ClearRigidBodyMessages();
  }

  void GameLogicSystem::InitCurrentLevel(void)
  {
    if (m_CurrentLevel == LevelID::Invalid)
      return;

    auto it = m_Levels.find(m_CurrentLevel);
    if (it == m_Levels.end())
      return;

    it->second->Initialize();
  }

  void GameLogicSystem::UpdateCurrentLevel(float dt)
  {
    if (m_CurrentLevel == LevelID::Invalid)
      return;

    auto it = m_Levels.find(m_CurrentLevel);
    if (it == m_Levels.end())
      return;

    it->second->Update(dt);
  }
 
  GameLogicSystem::GameLogicSystem(void)
    : m_Levels()
    , m_CurrentLevel(LevelID::Invalid)
    , m_bPaused(false)
    , m_BulletMgr()
  {
    GAMELOGIC_SYSTEM = this;
  }

  GameLogicSystem::~GameLogicSystem(void)
  {
    for each(auto &it in m_Levels)
      delete it.second;

    m_Levels.clear();

    GAMELOGIC_SYSTEM = nullptr;

  }

  void GameLogicSystem::Initialize(void)
  {
    RegisterLevel(LevelID::Menu, new MainMenu());

	RegisterLevel(LevelID::MyScene, new MyScene("MyScene"));

    //RegisterLevel(LevelID::Level1, new GameLevel_1("Level1"));
    //RegisterLevel(LevelID::Level2, new GameLevel_2("Level2"));
    //RegisterLevel(LevelID::Level3, new GameLevel_3("Level3"));
    //// Example of loading in editor levels
    //// everything is the same, we can allocate a GameLevel object
    //// pass it the name of our level file
    //RegisterLevel(LevelID::Level4, new GameLevel_4("Level4"));
    //RegisterLevel(LevelID::Level5, new GameLevel_5("Level5"));
    //RegisterLevel(LevelID::Level6, new GameLevel_6("Level6"));
    
    //SwitchLevel(LevelID::Menu);
  }

  void GameLogicSystem::UpdatePlayerControllers(float dt)
  {
    // TODO: I should not update if we are paused or if we are in the menu
    if (m_CurrentLevel == LevelID::Menu)
      return;

    std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
    for each(auto sp in Spaces)
    {
      Base::ObjectAllocator *pPlayerArray = sp->GetComponents(ePlayerController);
      if (!pPlayerArray)
        continue;

      for (unsigned nIdx = 0; nIdx < pPlayerArray->Size(); ++nIdx)
      {
        //Input::MouseEvent::SetScollDelta(WHEEL_DELTA * HIWORD(wParam));
        Core::GameComponent *pComp = reinterpret_cast<Core::GameComponent*>((*pPlayerArray)[nIdx]);
        if (!pComp)
          continue;

        GameLogic::PlayerController *pPlayer = dynamic_cast<GameLogic::PlayerController*>(pComp);
        if (!pPlayer)
          continue;

        pPlayer->Update(dt, m_BulletMgr);
      }
    }
  }
  
  void GameLogicSystem::UpdateStats(float dt)
  {
    // TODO: When should not update these? Ask Weylin

    std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
    for each(auto sp in Spaces)
    {
      //Input::MouseEvent::SetScollDelta(WHEEL_DELTA * HIWORD(wParam));

      Base::ObjectAllocator *pStatsArray = sp->GetComponents(eStats);
      if (!pStatsArray)
        continue;

      for (unsigned nIdx = 0; nIdx < pStatsArray->Size(); ++nIdx)
      {
        Core::GameComponent *pComp = reinterpret_cast<Core::GameComponent*>((*pStatsArray)[nIdx]);
        if (!pComp)
          continue;

        GameLogic::Stats *pStats = dynamic_cast<GameLogic::Stats*>(pComp);
        if (!pStats)
          continue;

        pStats->Update(dt);
      }
    }
  }

  void GameLogicSystem::UpdateTimers(float dt)
  {
    // TODO: When should not update these? Ask Weylin

    std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
    for each(auto sp in Spaces)
    {
      Base::ObjectAllocator *pTimerArray = sp->GetComponents(eTimer);
      if (!pTimerArray)
        continue;

      //Input::MouseEvent::SetScollDelta(WHEEL_DELTA * HIWORD(wParam));
      for (unsigned nIdx = 0; nIdx < pTimerArray->Size(); ++nIdx)
      {
        Core::GameComponent *pComp = reinterpret_cast<Core::GameComponent*>((*pTimerArray)[nIdx]);
        if (!pComp)
          continue;

        GameLogic::Timer *pTimer = dynamic_cast<GameLogic::Timer*>(pComp);
        if (!pTimer)
          continue;

        pTimer->Update(dt);
      }
    }
  }

  void GameLogicSystem::UpdateFlag(float dt)
  {
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

        pFlag->Update(dt);
      }
    }
  }

  void GameLogicSystem::PlayMusic(std::string theTrack)
  {
    if (!Audio::AUDIOMANAGER->GetAllMute() && !Audio::AUDIOMANAGER->IsMusicMute())
    {
      Audio::AUDIOMANAGER->StopAll();
      //Determine which of the levels we'll be loading...
      Audio::SoundInstance si;
      Audio::AUDIOMANAGER->PlayEvent(theTrack, &si);

      si.SetVolume(1.f);
      Audio::AUDIOMANAGER->ResumeMusic();
    }
  }

  void GameLogicSystem::Update(float dt)  
  {
    // if we are currently paused
    if(GetPaused())
      return;

    UpdateCurrentLevel(dt);
    UpdatePlayerControllers(dt);
    UpdateTimers(dt);
    UpdateStats(dt);
    UpdateFlag(dt);
    // update the time on bullets
    m_BulletMgr.UpdateBullets(dt);
  }

  void GameLogicSystem::RegisterComponents(void)
  {
    REGISTER_COMPONENT(PlayerController, GameLogic::PlayerController);
    REGISTER_COMPONENT(Timer, GameLogic::Timer);
    REGISTER_COMPONENT(Stats, GameLogic::Stats);
    REGISTER_COMPONENT(Bullet, GameLogic::Bullet);
    REGISTER_COMPONENT(Flag, GameLogic::Flag);
  }

  void GameLogicSystem::RegisterLevel(LevelID levelId, GameLevel *pLevel)
  {
    if (!pLevel)
      return;

    // don't add this level again if it already exists
    if (m_Levels.find(levelId) != m_Levels.end())
      return;

    m_Levels[levelId] = pLevel;
  }

  void GameLogicSystem::SwitchLevel(LevelID levelId)
  {
    if (m_CurrentLevel != LevelID::Invalid)
      DeInitCurrentLevel();

    m_CurrentLevel = levelId;

    if (m_CurrentLevel != LevelID::Invalid)
      InitCurrentLevel();
  }
  
  GameLogic::LevelID GameLogicSystem::GetCurrentLevel()
  {
    return m_CurrentLevel;
  }


  void GameLogicSystem::ReceiveMessage(Core::Message& msg)
  {
    switch (msg.MessageId)
    {
      case Core::Message::UIMessage:
      {
        Core::MessageUI& m = dynamic_cast<Core::MessageUI&>(msg);

        //Split the string into tokens=================================
        std::string buf; // Have a buffer string
        std::stringstream ss(m.msg); // Insert the string into a stream
        std::vector<std::string> args; // Create vector to hold our words
        while (ss >> buf)
          args.push_back(buf);

        if (args.size() > 1 && strcmp("MuteMusic", args[0].c_str()) == 0) //CHECK FOR MUTE MUSIC MESSAGE
        {
          if (strcmp("true", args[1].c_str()) == 0)//MuteMusic is true
          {
            Audio::AUDIOMANAGER->MuteMusic();
            Audio::AUDIOMANAGER->StopAll();

          }
          else
          {
            Audio::SoundInstance si;
            Audio::AUDIOMANAGER->UnmuteMusic();
            Audio::AUDIOMANAGER->PlayEvent("MenuTrackLoopV1", &si);
            si.SetVolume(1.0f);
          }

        }
        else if (args.size() > 1 && strcmp("MuteAll", args[0].c_str()) == 0) //CHECK FOR MUTE ALL MESSAGE
        {
          if (strcmp("true", args[1].c_str()) == 0)//MuteAll is true
          {
            
            Audio::AUDIOMANAGER->SetAllMute(true);
            Audio::AUDIOMANAGER->SetMasterVolume(0.0f);
          }
          else
          {
            Audio::AUDIOMANAGER->SetAllMute(false);
            Audio::AUDIOMANAGER->SetMasterVolume(1.0f);
          }
        }


        //Check if the UI message is ChangeLevel and if we have a next argument
        if (args.size() > 1 && strcmp("ChangeLevel", args[0].c_str()) == 0)
        {
          
          std::string theTrack;
          if (strcmp("MainMenu", args[1].c_str()) == 0)
          {
            //Audio::AUDIOMANAGER->StopAll();
            SwitchLevel(LevelID::Menu);
            //theTrack = "MenuTrackLoopV1 (2)";
            //PlayMusic(theTrack);
            return;
          }
          else
          {
            theTrack = "MenuTrackLoopV1";
          }

		  if (strcmp("MyScene", args[1].c_str()) == 0)
		  {
			  SwitchLevel(LevelID::MyScene);
		  }

          /*if (strcmp("Level1", args[1].c_str()) == 0)
          {
            SwitchLevel(LevelID::Level1);
          }
          else if (strcmp("Level2", args[1].c_str()) == 0)
            SwitchLevel(LevelID::Level2);
          else if (strcmp("Level3", args[1].c_str()) == 0)
            SwitchLevel(LevelID::Level3);
          else if (strcmp("Level4", args[1].c_str()) == 0)
            SwitchLevel(LevelID::Level4);
          else if (strcmp("Level5", args[1].c_str()) == 0)
            SwitchLevel(LevelID::Level5);
          else if (strcmp("Level6", args[1].c_str()) == 0)
            SwitchLevel(LevelID::Level6);*/

          Audio::AUDIOMANAGER->StopAll();
          PlayMusic(theTrack);
          Audio::AUDIOMANAGER->ResumeAll();
        }

        break;
      }
      case Core::Message::CollisionEnter:
      {
        Core::CollisionEnterMessage& m = dynamic_cast<Core::CollisionEnterMessage&>(msg);

        std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
        for each(auto sp in Spaces)
        {
          //Core::GameObject* blag = sp->GetGameObject(m.body);

          Base::ObjectAllocator *pPlayerArray = sp->GetComponents(ePlayerController);
          if (!pPlayerArray)
            continue;

          for (unsigned nIdx = 0; nIdx < pPlayerArray->Size(); ++nIdx)
          {
            Core::GameComponent *pComp = reinterpret_cast<Core::GameComponent*>((*pPlayerArray)[nIdx]);
            if (!pComp)
              continue;

            GameLogic::PlayerController *pPlayer = dynamic_cast<GameLogic::PlayerController*>(pComp);
            if (!pPlayer)
              continue;
            pPlayer->RecieveMessage(m);
          }
        }
        for each(auto sp in Spaces)
        {
          //Core::GameObject* blag = sp->GetGameObject(m.body);

          Base::ObjectAllocator *pBulletArray = sp->GetComponents(eBullet);
          if (!pBulletArray)
            continue;

          for (unsigned nIdx = 0; nIdx < pBulletArray->Size(); ++nIdx)
          {
            Core::GameComponent *pComp = reinterpret_cast<Core::GameComponent*>((*pBulletArray)[nIdx]);
            if (!pComp)
              continue;

            GameLogic::Bullet *pBullet = dynamic_cast<GameLogic::Bullet*>(pComp);
            if (!pBullet)
              continue;
            pBullet->RecieveMessage(m);
          }
        }
        break;
      }
    }
  }
}