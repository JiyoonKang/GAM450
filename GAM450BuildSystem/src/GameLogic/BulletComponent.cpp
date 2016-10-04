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
#include "GameLogic/BulletComponent.h"
#include "GameLogic/BulletMgr.h"
#include "GameLogic/Stats.h"
#include "GameLogic/PlayerController.h"
#include "core/types/object/Object.h"
#include "core/types/space/Space.h"
#include "core/components/transform/CTransform.h"
#include "physics/util/Geometry.h"
#include "physics/dynamics/RigidBody.h"
#include "core/engine/core/Engine.h"
#include "graphics/includes/GraphicsSystem/Emissive.h"
#include "graphics/includes/GraphicsSystem/Renderable.h"
#include "graphics/includes/GraphicsSystem/ParticleSystem.h"
#include "physics/colliders/BoxCollider.h"
#include <Graphics/includes/GraphicsSystem/LightDeath.h>

namespace GameLogic
{
  Bullet::Bullet(void)
    : m_bCanCollide(true)
    , m_pAttachedObject(nullptr)
    , m_nJointHandle(-1)
    , m_fCurrTime(0.f)
    , m_fMaxTime(DEFAULT_MAX_TIME)
    , m_fRadius(DEFAULT_RADIUS)
    , m_fTimeToStart(0.05f)
    , m_bKillParticles(false)
    , m_LocalAxis()
    , pOwnedPlayer(nullptr)
  {

  }

  bool Bullet::UpdateBullet(float dt)
  {
    m_fTimeToStart -= dt;
    ParticleSystem* pSystem = static_cast<ParticleSystem*>(GetOwner()->GetComponent(eParticleSystem));
    if (!pSystem)
      return false;
    if (m_fTimeToStart < 0)
    {
      pSystem->m_systemStopUpdating = false;
      Renderable* pRender = static_cast<Renderable*>(GetOwner()->GetComponent(eRenderable));
      if (pRender)
      {
        pRender->m_IsVisible = true;
      }
    }
    if (m_bKillParticles && m_fTimeToStart < -0.5)
    {
      pSystem->m_systemStopUpdating = true;
    }
    m_fCurrTime += dt;
    
    if (m_fCurrTime >= m_fMaxTime)
    {
      // remove this bullet from the game
	    LightDeath *death = GetOwner()->GetComponent<LightDeath>(eLightDeath);
	    
	    if (death)
	    {
		    death->Kill();
		    if (death->IsDead())
		    {
		    	RemoveBullet();
		    	return true;
		    }
	    }

	    else
	    {
		    RemoveBullet();
		    return true;
	    }
    }

    // otherwise update it
    Core::Transform *pTransform = GetOwner()->GetComponent<Core::Transform>(eTransform);
    if (!pTransform)
      return false;

    if (!m_pAttachedObject)
      return false;

    Core::Transform *pAttachedTransform = m_pAttachedObject->GetComponent<Core::Transform>(eTransform);
    if (!pAttachedTransform)
      return false;

    float4 attachedPosition = pAttachedTransform->GetTranslation();
    float4x4 attachedRotation = pAttachedTransform->GetRotationMatrix();

    float4 worldAxis = Physics::RotateVectorOutOfBasis(attachedRotation, m_LocalAxis);
    float4 newPos = attachedPosition + worldAxis;
    pTransform->SetTranslation(newPos);

    return false;
  }

  void Bullet::RemoveBullet(void)
  {
    GetOwner()->Destroy();
  }

  void Bullet::RecieveMessage(Core::CollisionEnterMessage& m)
  {
    if (m_bCanCollide)
    {
      std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
      for each(auto sp in Spaces)
      {
        auto thisObj = sp->GetGameObject(m.body);

        if (GetOwner() == thisObj)
        {
          const auto otherObj = sp->GetGameObject(m.other);
          if (!otherObj)
          {
            continue;
          }
          if (otherObj->GetName() == "" || otherObj->GetName() == "Light" || otherObj->GetName() == pOwnedPlayer->name)
          {
            return;
          }
          Physics::RigidBody* pBody = static_cast<Physics::RigidBody*>(thisObj->GetComponent(eRigidBody));

          Renderable* render = static_cast<Renderable*>(thisObj->GetComponent(eRenderable));
          EmissiveComponent* emissive = static_cast<EmissiveComponent*>(thisObj->GetComponent(eEmissiveComponent));

          Core::Transform* bulletTrans = static_cast<Core::Transform*>(thisObj->GetComponent(eTransform));
          Core::Transform* otherTrans = static_cast<Core::Transform*>(otherObj->GetComponent(eTransform));
          Physics::BoxCollider* pBox = static_cast<Physics::BoxCollider*>(thisObj->GetComponent(eBoxCollider));
          Physics::BoxCollider* pOtherBox = static_cast<Physics::BoxCollider*>(otherObj->GetComponent(eBoxCollider));

          Physics::RigidBody* pOtherBody = static_cast<Physics::RigidBody*>(otherObj->GetComponent(eRigidBody));

          ErrorIf(!pBody || !otherTrans || !bulletTrans || !pBox || !pOtherBox || !pOtherBody, "One of the bullet components does not exist.");

          if (!pBody || !otherTrans || !bulletTrans || !pBox || !pOtherBox || !pOtherBody)
            return;
          if (pOtherBody->m_bGhost == true)
          {
            return;
          }
          float4 posExt = otherTrans->GetTranslation() + pOtherBox->GetHalfExtents();
          float4 negExt = otherTrans->GetTranslation() - pOtherBox->GetHalfExtents();

          float4 posDist = posExt - bulletTrans->GetTranslation();
          float4 negDist = bulletTrans->GetTranslation() - negExt;

          bool posMask[3] = { false, false, false };
          bool negMask[3] = { false, false, false };

          float4 bulletVel = pBody->GetVelocity();
          for (int i = 0; i < 3; ++i)
          {
            if (bulletVel.v[i] > 0)
              negMask[i] = true;
            else
              posMask[i] = true;
          }

          float shortestDistance = FLT_MAX;
          int loc;
          bool posOrNeg;

          for (int i = 0; i < 3; ++i)
          {
            if (posDist.v[i] < shortestDistance && posMask[i] == true)
            {
              shortestDistance = posDist.v[i];
              loc = i;
              posOrNeg = true;
            }
          }
          for (int i = 0; i < 3; ++i)
          {
            if (negDist.v[i] < shortestDistance && negMask[i] == true)
            {
              shortestDistance = negDist.v[i];
              loc = i;
              posOrNeg = false;
            }
          }
          float4 disVec = float4(0.0f, 0.0f, 0.0f, 0.0f);
          if (posOrNeg)
          {
            disVec.v[loc] = 1.0f;
          }
          else
          {
            disVec.v[loc] = -1.0f;
          }
          float4 modTrans = disVec*(shortestDistance + pBox->GetDepth()) + bulletTrans->GetTranslation();
          bulletTrans->SetTranslation(modTrans);

          pBody->SetVelocity(float4(0.0f, 0.0f, 0.0f, 0.0f));


          // find the local axis from our bullet to our original body
          float4 localPos = Physics::RotatePointIntoBasis(otherTrans->GetRotationMatrix(),
            otherTrans->GetTranslation(), bulletTrans->GetTranslation());

          m_LocalAxis = localPos;
          m_pAttachedObject = otherObj;

          if (emissive)
            thisObj->DetatchComponent(emissive->typeID);
          
          if (render)
            thisObj->DetatchComponent(render->typeID);

          pBody->m_bGhost = true;

          Stats* pStats = static_cast<Stats*>(otherObj->GetComponent(eStats));
          PlayerController* pPlayer = static_cast<PlayerController*>(otherObj->GetComponent(ePlayerController));
          if (pStats && pPlayer && !pPlayer->m_bGodMode)
          {
            pPlayer->GetGamePad()->SetVibration(float2(0.5f, 0.5f));
            pPlayer->m_fCurBuzz = 0.0f;
            pStats->m_health -= 5;
            pPlayer->UpdatePlayerHealth();
            
            if (pStats->m_health <= 0)
            {
              Stats* pOwnedStats = static_cast<Stats*>(pOwnedPlayer->GetComponent(eStats));
              if (!pOwnedStats)
              {
                return;
              }
              
              
              PlayerController* pOwnedPlayerCont = static_cast<PlayerController*>(pOwnedPlayer->GetComponent(ePlayerController));
              if (!pOwnedPlayerCont)
              {
                return;
              }
              if (pOwnedPlayerCont->m_bHasFlag)
              {
                pOwnedStats->m_score += 100;
              }
              else
              {
                pOwnedStats->m_score += 50;
              }
              int isNotRing = 0;
              pOwnedPlayerCont->UpdatePlayerScore(isNotRing);
            }
          }

          auto it = pBulletMgr->m_Bullets.find(otherObj);
          if (it == pBulletMgr->m_Bullets.end())//NOT THERE
          {
            // if we don't have this body yet, then just add a new body to the list
            BulletMgr::TBulletList &list = pBulletMgr->m_Bullets[otherObj];
            list.push_back(GetOwner());
          }
          else
          {
            BulletMgr::TBulletList& list = it->second;
            for (BulletMgr::TBulletList::iterator bullet = list.begin(); bullet != list.end(); ++bullet)
            {
              Core::Transform *pTransform = (*bullet)->GetComponent<Core::Transform>(eTransform);
              if (!pTransform)
                continue;
            
              Bullet *pBullet = (*bullet)->GetComponent<Bullet>(eBullet);
              if (!pBullet)
                continue;
            
              float4 diff = pTransform->GetTranslation() - bulletTrans->GetTranslation();
              // our new bullet is far enough away from this bullet
              if (diff.LengthSq() >= pBullet->m_fRadius * pBullet->m_fRadius)
                continue;
            }
            list.push_back(GetOwner());
          }
          ParticleSystem* pSystem = static_cast<ParticleSystem*>(GetOwner()->GetComponent(eParticleSystem));
          if (!pSystem)
            return;
          //turn off particle system
          //pSystem->m_systemIsVisible = false;
          pSystem->m_systemStopUpdating = true;
          pSystem->m_particleVel = float3(0.0f, 0.0f, 0.0f);
          pSystem->m_particleVelVariance = float3(3.0f, 3.0f, 3.0f);
          m_fTimeToStart = 0.3f;
          m_fCurrTime = m_fMaxTime - 10.0f;
          m_bKillParticles = true;
          m_bCanCollide = false;
        }
      }
    }
  }

  //COMP_LUA_DEP(Bullet);
}