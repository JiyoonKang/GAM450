/*******************************************************************************
* All content © 2015 DigiPen (USA) Corporation, all rights reserved.
* Reproduction or disclosure of this file or its contents without the prior
* written consent of DigiPen Institute of Technology is prohibited.
*
* File: BulletMgr.cpp
* Author:
* Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
******************************************************************************/
#include "pch/precompiled.h"
#include "GameLogic/BulletMgr.h"
#include "Level.h"
#include "graphics/includes/GraphicsSystem/PointLight.h"
//#include "Base/util/HelperMacros.h"

#include "core/types/space/Space.h"
#include "core/types/object/Object.h"

namespace GameLogic
{
  BulletMgr::TBulletMap BulletMgr::m_Bullets;

  BulletMgr::BulletMgr(void)
  {

  }

  Core::GameObject* BulletMgr::CreateBullet(Core::GameObject *pObject,
    float4 const &pos, float4 const &bulletColor, Core::GameSpace *pSpace, 
    float fRadius, float fTime)
  {
    // create the bullet object
    ObjectBuilder builder(pSpace);
    Core::GameObject *pBullet = builder.CreateObject()
      .AddTransform(pos)
      .AddPointLight(bulletColor)
      .AddRigidBody()
      .AddBullet(fRadius, fTime)
	    .AddFlicker()
	    .AddLightDeath()
      .Finish();

    // create a weld joint between the bullet and the thing it hit
    // calculate the local positions and rotations for each body
    Core::Transform *pHitTransform = pObject->GetComponent<Core::Transform>(eTransform);
    ErrorIf(!pHitTransform);
    if (!pHitTransform)
      return pBullet;

    // find the local axis from our bullet to our original body
    float4 localPos = Physics::RotatePointIntoBasis(pHitTransform->GetRotationMatrix(), 
      pHitTransform->GetTranslation(), pos);

    Bullet *bullet = pBullet->GetComponent<Bullet>(eBullet);
    if (bullet)
    {
      bullet->m_LocalAxis = localPos;
      bullet->m_pAttachedObject = pObject;
    }
    ParticleSystem* pSystem = static_cast<ParticleSystem*>(pBullet->GetComponent(eParticleSystem));
    pSystem->m_systemIsPaused = false;
    return pBullet;
  }

  Core::GameObject* BulletMgr::CreateBullet(float4 const &pos, float4 const &facing, float4 const &bulletColor,
     Core::GameSpace *pSpace, float fRadius, float fTime, Core::GameObject* pPlayer)
  {
    ObjectBuilder builder(pSpace);
    Core::GameObject *pBullet = builder.CreateObject()
      .AddTransform(pos)
      .AddPointLight(bulletColor)
      .AddRigidBody(Physics::DynamicState::DYNAMIC)
      .AddBullet(fRadius, fTime)
      .AddFlicker()
      .AddLightDeath()
      .AddParticleSystem()
      .AddRenderable("Sphere", "Periwinkle", "Periwinkle")
      .AddEmissive(bulletColor)
      .AddBoxCollider()
      .Finish();

    //Modify the bullet's particle system to make it follow it.
    ParticleSystem* ps = static_cast<ParticleSystem*>(pBullet->GetComponent(eParticleSystem));
    Physics::RigidBody* rb = static_cast<Physics::RigidBody*>(pBullet->GetComponent(eRigidBody));

    //give 95% of the bullet's velocity to the particles so that they share the same direction
    // but they stay behind the bullet always
    ps->m_particleTint = bulletColor;
    ps->m_particleCount = 70;
    ps->m_particleVel.x = rb->GetVelocity().x * 0.95f;
    ps->m_particleVel.y = rb->GetVelocity().y * 0.95f;
    ps->m_particleVel.z = rb->GetVelocity().z * 0.95f;

    ps->m_particleVelVariance = float3(0.15, 0.15, 0.15);

    ps->m_systemStopUpdating = true;

    Physics::RigidBody* pBody = static_cast<Physics::RigidBody*>(pBullet->GetComponent(eRigidBody));
    pBody->m_bGhost = true;
    pBody->SetVelocity(facing * 100);
    Renderable* pRender = static_cast<Renderable*>(pBullet->GetComponent(eRenderable));
    pRender->SetEmissiveTexture("itstheremixtoemissives");
    pRender->m_IsVisible = false;
    Bullet* pBulletComp = static_cast<Bullet*>(pBullet->GetComponent(eBullet));
    pBulletComp->pBulletMgr = static_cast<GameLogic::BulletMgr*>(this);
    pBulletComp->pOwnedPlayer = pPlayer;
    return pBullet;
  }

  bool BulletMgr::AddDeath(float4 const &bulletColor, Core::GameSpace *pSpace, float4 const pos, float fRadius,
    float fMaxTime)
  {
    ObjectBuilder builder(pSpace);
    Core::GameObject *pDeath = builder.CreateObject()
      .AddTransform(pos)
      .AddBullet(2.0f, 0.5f)
      .AddParticleSystem()
      .AddPointLight(bulletColor)
      .AddFlicker()
      .AddLightDeath()
      .Finish();
    if (pDeath)
    {
    ParticleSystem* ps = static_cast<ParticleSystem*>(pDeath->GetComponent(eParticleSystem));

    if (!ps)
      return false;

    ps->m_particleCount = 200;
    ps->m_particleVel = float3(0.0f, 0.0f, 0.0f);
    ps->m_particleVelVariance = float3(3.0f, 3.0f, 3.0f);
    ps->m_particleTint = bulletColor;

    
      return true;
    }
    return false;
  }

  bool BulletMgr::AddBullet(Physics::RayCastResult const &result, 
    float4 const &bulletColor, Core::GameSpace *pSpace, float fRadius, float fMaxTime)
  {
    if (!result.m_FirstCollider)
      return false;

    float4 position = result.m_IntersectionPoint + result.m_Normal;

    Core::GameObject *pObject = result.m_FirstCollider->GetOwner();

    auto it = m_Bullets.find(pObject);
    if (it == m_Bullets.end())
    {
      // if we don't have this body yet, then just add a new body to the list
      TBulletList &list = m_Bullets[pObject];

      Core::GameObject* bullet = CreateBullet(pObject, position, bulletColor, pSpace, fRadius, fMaxTime);
      list.push_back(bullet);
      return true;
    }


    // otherwise this body exists in our map, look through every item in the list
    // and perform a squared distance check to see if we can insert the item

    // NOTE: if we change this to a tree then we will shoot the ray we have
    // in our result to our tree and see if we hit a leaf, perform squared
    // distance check, otherwise we can insert it into our tree
    TBulletList &list = it->second;
    for (TBulletList::iterator bullet = list.begin(); bullet != list.end(); ++bullet)
    {
      Core::Transform *pTransform = (*bullet)->GetComponent<Core::Transform>(eTransform);
      if (!pTransform)
        continue;

      Bullet *pBullet = (*bullet)->GetComponent<Bullet>(eBullet);
      if (!pBullet)
        continue;

      float4 diff = pTransform->GetTranslation() - position;
      // our new bullet is far enough away from this bullet
      if (diff.LengthSq() >= pBullet->m_fRadius * pBullet->m_fRadius)
        continue;
      else
      {
        // otherwise we are in the radius of the old bullet, if we are the same color
        // then just return false
        PointLight *pLight = (*bullet)->GetComponent<PointLight>(ePointLight);
        if (!pLight)
          return false;
        
        if (pLight->GetLightColor() == bulletColor)
          return false;
        else
        {
          pLight->SetLightColor(bulletColor);
          return true;
        }
      }
    }

    // I visited every bullet in my list, determined that we can create this bullet
    Core::GameObject* bullet = CreateBullet(pObject, position, bulletColor, pSpace, fRadius, fMaxTime);
    list.push_back(bullet);
    return true;
  }

  bool BulletMgr::AddBullet(float4 const &bulletColor, Core::GameSpace *pSpace, float4 const pos, float4 const& facing, Core::GameObject* pPlayer, float fRadius, float fMaxTime)
  {
    Core::GameObject *pBullet = CreateBullet(pos, facing, bulletColor, pSpace, fRadius, fMaxTime, pPlayer);
    if (!pBullet)
    {
      return false;
    }
    
    return true;
  }

  void BulletMgr::UpdateBullets(float dt)
  {
    std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
    for each(auto sp in Spaces)
    {
      ObjectAllocator *Bullets = sp->GetComponents(eBullet);
      for (auto it = Bullets->begin<Bullet>(); it != Bullets->end<Bullet>(); ++it)
      {
        if (!it)
          continue;

        // this bullet needs to be removed from our map
        if (it->UpdateBullet(dt))
        {
          Core::GameObject *pAttachedObject = it->m_pAttachedObject;
          if (!pAttachedObject)
            continue;

          TBulletList &list = m_Bullets[pAttachedObject];
          list.remove(it->GetOwner());
        }
      }
    }
  }

  void BulletMgr::RemoveAllBullets(void)
  {
    std::vector<Core::GameSpace*> &Spaces = Core::ENGINE->Spaces();
    for each(auto sp in Spaces)
    {
      ObjectAllocator *Bullets = sp->GetComponents(eBullet);
      for (auto it = Bullets->begin<Bullet>(); it != Bullets->end<Bullet>(); ++it)
      {
        if (!it)
          continue;

        it->RemoveBullet();
      }
    }

    m_Bullets.clear();
  }

  void BulletMgr::RemoveBulletsAttached(Core::GameObject *pObject)
  {
    if (!pObject)
      return;

    TBulletMap::iterator it = m_Bullets.find(pObject);
    if (it == m_Bullets.end())
      return;

    TBulletList &list = it->second;

    for (auto jt = list.begin(); jt != list.end(); ++jt)
    {
      Core::GameObject *bulletObj = *jt;
      if (!bulletObj)
        continue;

      Bullet *pBullet = bulletObj->GetComponent<Bullet>(eBullet);
      if (!pBullet)
        continue;

      pBullet->RemoveBullet();
    }

    m_Bullets.erase(it);
  }
}