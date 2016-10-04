/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Level.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "GameLogic/Level.h"

#define INIT_PLAYERS \
  if (!m_pSpace) \
  return; \
  \
  Core::GameObject *pTimerObj = nullptr; \
  \
  ObjectAllocator &objectAllocator = m_pSpace->GetObjectList(); \
  for (Core::GameObject *pObject = objectAllocator.begin<Core::GameObject>(); pObject != objectAllocator.end<Core::GameObject>(); ++pObject) \
      { \
    if (!pObject) \
      continue; \
   \
    if (pObject->GetName() == "Timer") \
            { \
      pTimerObj = pObject; \
      break; \
            } \
      } \
  \
  if (!pTimerObj) \
    return; \
    \
  Timer *pTimer = static_cast<Timer*>(pTimerObj->GetComponent(eTimer)); \
  if (!pTimer) \
    return; \
    \
  ObjectAllocator *pAlloc = m_pSpace->GetComponents(ePlayerController); \
  if (!pAlloc) \
    return; \
    \
  for (PlayerController *pPlayer = pAlloc->begin<PlayerController>(); pPlayer != pAlloc->end<PlayerController>(); ++pPlayer) \
      { \
    if (!pPlayer) \
      continue; \
    \
  pPlayer->UpdatePlayerStats(); \
    \
    Core::GameObject *pObject = pPlayer->GetOwner(); \
    pTimer->thePlayers.push_back(pObject); \
      } 

namespace GameLogic
{
  Core::GameSpace *GameLevel::m_pSpace = nullptr;

  void LoadSpace(Core::GameSpace *pSpace)
  {
    GameLevel::m_pSpace = pSpace;
  }

  GameLevel::GameLevel(std::string const &szLevel)
    : m_szLevel(szLevel)
  {
  }

  GameLevel::~GameLevel(void)
  {
    
  }

  void GameLevel::Initialize(void)
  {
    if (m_szLevel == std::string())
      return;

    Core::ENGINE->ObjectFactory.LoadLevel(m_szLevel.c_str(), LoadSpace);
  }

  void GameLevel::Update(float /*dt*/)
  {

  }

  void GameLevel::Deinitialize(void)
  {
    if (!m_pSpace)
      return;
    // run through and remove all objects from the game from our space
    m_pSpace->Clear();
    m_pSpace->Destroy();
    m_pSpace = nullptr;
  }

  MyScene::MyScene(std::string const &szLevel)
	  : GameLevel(szLevel)
  {
  }

  void MyScene::Initialize(void)
  {
	  __super::Initialize();

	  INIT_PLAYERS
  }

  /*GameLevel_1::GameLevel_1(std::string const &szLevel)
    : GameLevel(szLevel)
  {

  }



  void GameLevel_1::Initialize(void)
  {
    __super::Initialize();

    INIT_PLAYERS
  }

  GameLevel_2::GameLevel_2(std::string const &szLevel)
    : GameLevel(szLevel)
  {

  }



  void GameLevel_2::Initialize(void)
  {
    __super::Initialize();

    INIT_PLAYERS
  }

  GameLevel_3::GameLevel_3(std::string const &szLevel)
    : GameLevel(szLevel)
  {

  }



  void GameLevel_3::Initialize(void)
  {
    __super::Initialize();

    INIT_PLAYERS
  }

  GameLevel_4::GameLevel_4(std::string const &szLevel)
    : GameLevel(szLevel)
  {

  }



  void GameLevel_4::Initialize(void)
  {
    __super::Initialize();

    INIT_PLAYERS
  }

  GameLevel_5::GameLevel_5(std::string const &szLevel)
    : GameLevel(szLevel)
  {

  }

  void GameLevel_5::Initialize(void)
  {
    __super::Initialize();

    INIT_PLAYERS
  }
  
  GameLevel_6::GameLevel_6(std::string const &szLevel)
    : GameLevel(szLevel)
  {

  }

  void GameLevel_6::Initialize(void)
  {
    __super::Initialize();

    INIT_PLAYERS
  }*/
}