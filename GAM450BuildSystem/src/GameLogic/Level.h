/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Level.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_GAME_LOGIC_LEVEL
#define H_PAINT_GAME_LOGIC_LEVEL

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

#include "Graphics/includes/GraphicsSystem/Renderable.h"
#include "Graphics/includes/GraphicsSystem/Paintable.h"
#include "Graphics/includes/GraphicsSystem/Flicker.h"
#include "Graphics/includes/GraphicsSystem/ParticleSystem.h"
#include "Graphics/includes/GraphicsSystem/PointLight.h"
#include "Graphics/includes/GraphicsSystem/ParticleSystem.h"
#include "Graphics/includes/GraphicsSystem/Emissive.h"

#include "physics/colliders/BoxCollider.h"
#include "Physics/colliders/CapsuleCollider.h"
#include "Physics/colliders/SphereCollider.h"
#include "Physics/dynamics/RigidBody.h"
#include "physics/dynamics/effects/GravityEffect.h"
#include "physics/dynamics/effects/DragEffect.h"
#include "Base/util/math/VectorMath.h"

#define DEFAULT_NORMAL_TEXTURE "Periwinkle"
#define DEFAULT_TEXTURE "Square"
#include <Graphics/includes/GraphicsSystem/LightDeath.h>

namespace GameLogic
{
  class ObjectBuilder
  {
    Core::GameSpace *m_pSpace;
    std::string m_szName;
    Core::GameObject *m_pCurrObject;

  public:
    ObjectBuilder(Core::GameSpace *pSpace)
      : m_pCurrObject(nullptr)
      , m_pSpace(pSpace)
      , m_szName()
    {
    }

    ObjectBuilder &CreateObject(std::string const &szName = "")
    {
      ErrorIf(!m_pSpace);
      if (!m_pSpace)
        return *this;

      m_pCurrObject = m_pSpace->CreateEmptyObject();
      m_szName = szName;
      return *this;
    }

    Core::GameObject *Finish(void)
    {
      Core::GameObject *pTemp = m_pCurrObject;
      m_pCurrObject = nullptr;
      pTemp->Initialize();
      pTemp->name = m_szName;
      return pTemp;
    }

  ObjectBuilder &AddEmissive(float4 const &color = float4(0.f, 0.f, 0.f, 0.f))
  {
    ErrorIf(!m_pSpace);
    ErrorIf(!m_pCurrObject);
    if (!m_pCurrObject || !m_pSpace)
      return *this;

    EmissiveComponent* emissive = static_cast<EmissiveComponent*>(m_pSpace->CreateComponent(eEmissiveComponent));
    m_pCurrObject->AddComponent(emissive);
    emissive->SetEmittedcolor(color);
    return *this;
  }

  ObjectBuilder &AddParticles()
  {
    ErrorIf(!m_pSpace);
    ErrorIf(!m_pCurrObject);
    if (!m_pCurrObject || !m_pSpace)
      return *this;

    ParticleSystem* particleSystem = static_cast<ParticleSystem*>(m_pSpace->CreateComponent(eParticleSystem));
    m_pCurrObject->AddComponent(particleSystem);
    return *this;
  }

	ObjectBuilder &AddPointLight(
		float4 const &lightColor = float4(1.0f, 0.0f, 0.8f, 1.0f),
		float2 const &rangeAttenuation = float2(10.0f, 3.0f))
    {
		ErrorIf(!m_pSpace);
		ErrorIf(!m_pCurrObject);
		if (!m_pCurrObject || !m_pSpace)
			return *this;

		PointLight* light = static_cast<PointLight*>(m_pSpace->CreateComponent(ePointLight));
		m_pCurrObject->AddComponent(light);
		light->SetLightColor(lightColor);
		light->SetRangeAttenuation(rangeAttenuation);

		return *this;
    }

    ObjectBuilder &AddTransform(
      float4 const &position = float4(0.f, 0.f, 0.f, 0.f),
      float4 const &scale = float4(1.f, 1.f, 1.f, 0.f),
      quaternion const &orientation = quaternion(1.f, 0.f, 0.f, 0.f))
    {
      ErrorIf(!m_pSpace);
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject || !m_pSpace)
        return *this;

      Core::Transform *pTransform = 
        static_cast<Core::Transform*>(m_pSpace->CreateComponent(eTransform));
      m_pCurrObject->AddComponent(pTransform);
      pTransform->SetTranslation(position);
      pTransform->SetRotation(orientation);
      pTransform->SetScale(scale);

      return *this;
    }

    ObjectBuilder &AddBoxCollider(float4 const &extents = float4(1.f, 1.f, 1.f, 0.f))
    {
      ErrorIf(!m_pSpace);
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject || !m_pSpace)
        return *this;

      Physics::BoxCollider *pBox = 
        static_cast<Physics::BoxCollider*>(m_pSpace->CreateComponent(eBoxCollider));
      m_pCurrObject->AddComponent(pBox);
      pBox->SetExtents(extents);
      return *this;
    }

    ObjectBuilder &AddSphereCollider(float radius = 0.5f)
    {
      ErrorIf(!m_pSpace);
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject || !m_pSpace)
        return *this;

      Physics::SphereCollider *pSphere = 
        static_cast<Physics::SphereCollider*>(m_pSpace->CreateComponent(eSphereCollider));
      m_pCurrObject->AddComponent(pSphere);
      pSphere->SetRadius(radius);
      return *this;
    }

    ObjectBuilder &AddCapsuleCollider(float height = 1.f, float radius = 0.5f)
    {
      ErrorIf(!m_pSpace);
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject || !m_pSpace)
        return *this;

      Physics::CapsuleCollider *pCapsule = 
        static_cast<Physics::CapsuleCollider*>(m_pSpace->CreateComponent(eCapsuleCollider));
      m_pCurrObject->AddComponent(pCapsule);
      pCapsule->SetHeight(height);
      pCapsule->SetRadius(radius);
      return *this;
    }

    ObjectBuilder &AddRenderable(std::string const &szModel, std::string const &texture, std::string const normalTexture)
    {
      ErrorIf(!m_pSpace);
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject || !m_pSpace)
        return *this;

      Renderable *pRender = 
        static_cast<Renderable*>(m_pSpace->CreateComponent(eRenderable));
      m_pCurrObject->AddComponent(pRender);
      pRender->SetModel(szModel);
      pRender->SetTexture(texture);
      pRender->SetNormalTexture(normalTexture);
      return *this;
    }

    ObjectBuilder &AddRigidBody(Physics::DynamicState state = Physics::DynamicState::DYNAMIC)
    {
      ErrorIf(!m_pSpace);
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject || !m_pSpace)
        return *this;

      Physics::RigidBody *pBody = 
        static_cast<Physics::RigidBody*>(m_pSpace->CreateComponent(eRigidBody));
      m_pCurrObject->AddComponent(pBody);
      pBody->SetState(state);
      return *this;
    }

    ObjectBuilder &AddGravityEffect(float4 const &direction = float4(0.f, -1.f, 0.f, 0.f),
      float fMag = 9.81f)
    {
      ErrorIf(!m_pSpace);
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject || !m_pSpace)
        return *this;

      Physics::GravityEffect *pGravity = 
        static_cast<Physics::GravityEffect*>(m_pSpace->CreateComponent(eGravityEffect));
      m_pCurrObject->AddComponent(pGravity);
      pGravity->SetDirection(direction);
      pGravity->SetMagnitude(fMag);
      return *this;
    }

    ObjectBuilder &AddDragEffect(float b = 1.f)
    {
      ErrorIf(!m_pSpace);
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject || !m_pSpace)
        return *this;

      Physics::DragEffect *pDrag = 
        static_cast<Physics::DragEffect*>(m_pSpace->CreateComponent(eDragEffect));
      m_pCurrObject->AddComponent(pDrag);
      pDrag->SetDampingFactor(b);
      return *this;
    }

    ObjectBuilder &AddSoundEmitter(void)
    {
      ErrorIf(!m_pSpace);
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject || !m_pSpace)
        return *this;

      Audio::SoundEmitter *pEmitter = 
        static_cast<Audio::SoundEmitter*>(m_pSpace->CreateComponent(eSoundEmitter));
      m_pCurrObject->AddComponent(pEmitter);
      return *this;
    }

    ObjectBuilder &AddPlayerController(unsigned nGamePad, float4 const &color = float4(1.f, 0.f, 0.f, 1.f))
    {
      ErrorIf(!m_pSpace);
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject || !m_pSpace)
        return *this;

      GameLogic::PlayerController *pController = 
        static_cast<GameLogic::PlayerController*>(m_pSpace->CreateComponent(ePlayerController));
      pController->SetGamePad(nGamePad);
      pController->SetColor(color);
      m_pCurrObject->AddComponent(pController);
      return *this;
    }

    ObjectBuilder &AddTimer(float startTime)
    {
      ErrorIf(!m_pSpace);
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject || !m_pSpace)
        return *this;

      GameLogic::Timer *pTimer = 
        static_cast<GameLogic::Timer*>(m_pSpace->CreateComponent(eTimer));
      pTimer->SetTime(startTime);
      m_pCurrObject->AddComponent(pTimer);
      return *this;
    }

    ObjectBuilder &AddStats( float4 const &position = float4(0.f, 0.f, 0.f, 0.f) )
    {
      ErrorIf(!m_pSpace);
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject || !m_pSpace)
        return *this;

      GameLogic::Stats *pStats =
        static_cast<GameLogic::Stats*>(m_pSpace->CreateComponent(eStats));
      pStats->m_respawnPos = position;
      pStats->m_deathPos = position;
      pStats->m_deathPos.y += 100.0f;
      m_pCurrObject->AddComponent(pStats);
      return *this;
    }

    ObjectBuilder &AddPaintable(void)
    {
      ErrorIf(!m_pSpace);
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject || !m_pSpace)
        return *this;

      Paintable *pPaintable = 
        static_cast<Paintable*>(m_pSpace->CreateComponent(ePaintable));
      m_pCurrObject->AddComponent(pPaintable);
      return *this;
    }

    ObjectBuilder &AddBullet(float fRadius = DEFAULT_RADIUS, float fMaxTime = DEFAULT_MAX_TIME)
    {
      ErrorIf(!m_pSpace);
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject || !m_pSpace)
        return *this;

      Bullet *bullet = static_cast<Bullet*>(m_pSpace->CreateComponent(eBullet));
      bullet->m_fRadius = fRadius;
      bullet->m_fMaxTime = fMaxTime;

      m_pCurrObject->AddComponent(bullet);
      return *this;
    }

	ObjectBuilder &AddFlicker(void)
	{
		ErrorIf(!m_pSpace);
		ErrorIf(!m_pCurrObject);
		if (!m_pCurrObject || !m_pSpace)
			return *this;

		Flicker *flicker = static_cast<Flicker*>(m_pSpace->CreateComponent(eFlicker));

		m_pCurrObject->AddComponent(flicker);
		return *this;
	}

	ObjectBuilder &AddLightDeath(void)
	{
		ErrorIf(!m_pSpace);
		ErrorIf(!m_pCurrObject);
		if (!m_pCurrObject || !m_pSpace)
			return *this;

		LightDeath *death = static_cast<LightDeath*>(m_pSpace->CreateComponent(eLightDeath));

		m_pCurrObject->AddComponent(death);
		return *this;
	}

    ObjectBuilder &AddParticleSystem()
    {
      ErrorIf(!m_pSpace);
      ErrorIf(!m_pCurrObject);
      if (!m_pCurrObject || !m_pSpace)
        return *this;

      ParticleSystem* particleSystem = static_cast<ParticleSystem*>(m_pSpace->CreateComponent(eParticleSystem));
      m_pCurrObject->AddComponent(particleSystem);
      return *this;

    }
  };

  enum class LevelID
  {
    // all level should have a unique id between here
      Menu
	, MyScene
   /* , Level1
    , Level2
    , Level3
    , Level4
    , Level5
    , Level6*/
    // and here, Count and Invalid MUST be the last two types
    , Count
    , Invalid
  };

  class GameLevel
  {
  private:
    // the current level file associated if one exists
    std::string m_szLevel;

  public:
    // we should never have multiple levels running at the same time,
    // one level means we can have one shared space pointer
    static Core::GameSpace *m_pSpace;

    GameLevel(std::string const &szLevel = std::string());
    virtual ~GameLevel(void);
    // will load in any game objects from the level file if one exists
    // or if this is an inherited game level will load in any hardcoded c++
    // objects we have
    virtual void Initialize(void);
    // if default base class we will run any scripts associated with this level?
    // if this is an inherited class we will just run any hardcoded update code
    virtual void Update(float dt);

    // if default base class, this will run through and remove any objects
    // currently in the game from the level, this is so we can switch levels
    // at runtime easily
    // if inherited will run the hardcoded user code
    virtual void Deinitialize(void);
  };

  class MyScene : public GameLevel
  {
  public:
	  MyScene(std::string const &szLevel);

	  virtual void Initialize(void) override;
  };

  /*class GameLevel_1 : public GameLevel
  {
  public:
    GameLevel_1(std::string const &szLevel);

    virtual void Initialize(void) override;
  };
  class GameLevel_2 : public GameLevel
  {
  public:
    GameLevel_2(std::string const &szLevel);

    virtual void Initialize(void) override;
  };
  class GameLevel_3 : public GameLevel
  {
  public:
    GameLevel_3(std::string const &szLevel);

    virtual void Initialize(void) override;
  };

  class GameLevel_4 : public GameLevel
  {
  public:
    GameLevel_4(std::string const &szLevel);

    virtual void Initialize(void) override;
  };

  class GameLevel_5 : public GameLevel
  {
  public:
    GameLevel_5(std::string const &szLevel);

    virtual void Initialize(void) override;
  };

  class GameLevel_6 : public GameLevel
  {
  public:
    GameLevel_6(std::string const &szLevel);
    virtual void Initialize(void) override;

  };*/
}

#endif