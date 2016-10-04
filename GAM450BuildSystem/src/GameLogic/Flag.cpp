/*******************************************************************************
* All content © 2015 DigiPen (USA) Corporation, all rights reserved.
* Reproduction or disclosure of this file or its contents without the prior
* written consent of DigiPen Institute of Technology is prohibited.
*
* File: BulletComponent.cpp
* Author:
* Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
******************************************************************************/
#include "pch/precompiled.h"
#include "GameLogic/Flag.h"
#include "GameLogic/Stats.h"
#include "GameLogic/Timer.h"
#include "GameLogic/PlayerController.h"
#include "core/types/object/Object.h"
#include "core/types/space/Space.h"
#include "core/components/transform/CTransform.h"
#include "physics/util/Geometry.h"
#include "physics/dynamics/RigidBody.h"
#include "Core/engine/core/Engine.h"
#include "Graphics/includes/GraphicsSystem/Emissive.h"
#include "graphics/includes/GraphicsSystem/PointLight.h"

namespace GameLogic
{
  Flag::Flag(void)
    : m_hAttachedObject(Base::Handle())
    , m_bActive(false)
    , m_fTimeTillPoints(1.0f)
    , m_fGettingPoints(1.0f)
    , m_fTimeTillReturn(3.0f)
    , m_fWaiting(3.0f)
    , m_startPos(float4(0.0f, 0.0f, 0.0f, 0.0f))
  {

  }

  void Flag::Update(float dt)
  {
    Core::Transform* pos = reinterpret_cast<Core::Transform*>(GetOwner()->GetComponent(eTransform));
    bool xClose = pos->GetTranslation().x >= m_startPos.x - 0.05 && pos->GetTranslation().x <= m_startPos.x + 0.05;
    bool yClose = pos->GetTranslation().y >= m_startPos.y - 0.05 && pos->GetTranslation().y <= m_startPos.y + 0.05;
    bool zClose = pos->GetTranslation().z >= m_startPos.z - 0.05 && pos->GetTranslation().z <= m_startPos.z + 0.05;
    if (m_bActive)
    {

      Core::GameObject* pAttachedObject = GetOwner()->space->GetGameObject(m_hAttachedObject);
      Core::Transform* playerPos = reinterpret_cast<Core::Transform*>(pAttachedObject->GetComponent(eTransform));
      PlayerController* pPlayer = reinterpret_cast<PlayerController*>(pAttachedObject->GetComponent(ePlayerController));
      if (!playerPos || !pPlayer)
        return;

      //auto newPos = playerPos->GetTranslation() + playerPos->GetForwardVector() * 10;
      auto newPos = playerPos->GetTranslation();
      newPos.y += 5.f;
      pos->SetTranslation(newPos);

      if (m_fTimeTillPoints < m_fGettingPoints)
      {
        Core::GameObject* timer = nullptr;
        std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
        for each(auto sp in Spaces)
        {
          timer = sp->GetFirstGameObjectNamed("Timer");
          if (timer)
            break;
        }
        if (!timer)
          return;
        Timer* theTimer = dynamic_cast<Timer*>(timer->GetComponent(eTimer));
        if (!theTimer)
          return;
        m_fGettingPoints = 0.0f;
        Stats* stats = reinterpret_cast<Stats*>(pAttachedObject->GetComponent("Stats"));
        if (!stats)
          return;
        if (theTimer->m_time > 45.0f)
        {
          stats->m_score += 100;
        }
        else if (theTimer->m_time > 10.0f)
        {
          stats->m_score += 200;
        }
        else if (theTimer->m_time > 0.0f)
        {
          stats->m_score += 300;
        }
        
        if(theTimer->m_time > 0.0f)
        {
          std::string msg = std::string("JSMethod updatePlayerScore ") + std::to_string(reinterpret_cast<PlayerController*>(playerPos->GetOwner()->GetComponent(ePlayerController))->m_nGamePadID + 1) + std::string(" ") + std::to_string(stats->m_score) + std::string(" ") + std::to_string(1);
  
          Core::MessageUI uiM(msg);
  
          Core::ENGINE->SystemMessage(uiM);
        }
      }
      m_fGettingPoints += dt;
      m_fWaiting = 3.0f;
    }
    else if (xClose && yClose && zClose)
    {
      Physics::RigidBody* pBody = static_cast<Physics::RigidBody*>(GetOwner()->GetComponent(eRigidBody));
      if (!pos || !pBody)
        return;
      pBody->SetVelocity(float4(0.0f, 0.0f, 0.0f, 0.0f));
    }
    else if (m_fWaiting < 0)
    {
      //Core::Transform* pos = reinterpret_cast<Core::Transform*>(GetOwner()->GetComponent(eTransform));
      Physics::RigidBody* pBody = static_cast<Physics::RigidBody*>(GetOwner()->GetComponent(eRigidBody));
      if (!pos || !pBody)
        return;
      float4 newVel = m_startPos - pos->GetTranslation();
      newVel.Normalize();
      pBody->SetVelocity(newVel * 10);
      //m_fWaiting = 3.0f;
    }
    else
    {
      m_fWaiting -= dt;
      EmissiveComponent* pEmissive = reinterpret_cast<EmissiveComponent*>(GetOwner()->GetComponent(eEmissiveComponent));
      PointLight* pLight = reinterpret_cast<PointLight*>(GetOwner()->GetComponent(ePointLight));
      if (!pLight || !pEmissive)
        return;
      pEmissive->m_emittedColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
      pLight->m_LightColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
  }
}