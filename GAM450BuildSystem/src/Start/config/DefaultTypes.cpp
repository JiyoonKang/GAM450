/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: DefaultTypes.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

#include "Core/components/transform/CTransform.h"
//#include "components/lua/CLuaComponent.h"
#include "Base/util/math/Float2.h"
#include "Base/util/math/Float3.h"
#include "Base/util/math/Float4.h"
#include "Base/util/math/VectorLua.h"
#include "Base/util/Macros.h"
#include "base/util/math/Quaternion.h"

#include "Audio/components/emitter/SoundEmitter.h"

// physics includes
#include "physics/colliders/BoxCollider.h"
#include "physics/colliders/CapsuleCollider.h"
#include "physics/colliders/SphereCollider.h"
#include "physics/dynamics/RigidBody.h"
#include "physics/dynamics/effects/ConstantEffect.h"
#include "physics/dynamics/effects/DragEffect.h"
#include "physics/dynamics/effects/GravityEffect.h"

// game logic includes
#include "GameLogic/PlayerController.h"
#include "GameLogic/Timer.h"
#include "GameLogic/Stats.h"
#include "GameLogic/BulletComponent.h"
#include "GameLogic/Flag.h"

//Graphics includes
#include<Graphics/includes/GraphicsSystem/Renderable.h>
#include<Graphics/includes/GraphicsSystem/Paintable.h>
#include<Graphics/includes/GraphicsSystem/ParticleSystem.h>
#include "graphics/includes/GraphicsSystem/PointLight.h"
#include <Graphics/includes/GraphicsSystem/Emissive.h>
#include <Graphics/includes/GraphicsSystem/Fade.h>
#include <Graphics/includes/GraphicsSystem/Levitate.h>
#include <Graphics/includes/GraphicsSystem/Flicker.h>
#include <Graphics/includes/GraphicsSystem/LightDeath.h>

#include "Lua/components/LuaComponent.h"

namespace Core
{
  using namespace Audio;
  using namespace Physics;
  using namespace GameLogic;
  using namespace Lua;

#define REGISTER_POD_FULL( TYPE ) \
  TYPE_REGISTER_POD( TYPE ); \
  TYPE_SET_SERIALIZER( TYPE, SerializePOD<TYPE> );\
  TYPE_SET_DESERIALIZER( TYPE, DeserializePOD<TYPE> )

  void TypeRegistration()
  {
    TYPE_REGISTER_POD(int);
    TYPE_SET_SERIALIZER(int, SerializePOD<int>);
    TYPE_SET_DESERIALIZER(int, DeserializePOD<int>);



    TYPE_REGISTER_POD(unsigned int);
    TYPE_SET_SERIALIZER(unsigned int, SerializePOD<unsigned int>);
    TYPE_SET_DESERIALIZER(unsigned int, DeserializePOD<unsigned int>);

    TYPE_REGISTER_POD(void*);

    TYPE_REGISTER_POD(const char*);
    TYPE_SET_SERIALIZER(const char*, SerializePOD<char*>);
    TYPE_SET_DESERIALIZER(const char*, DeserializePOD<char*>);

    TYPE_REGISTER_POD(float);
    TYPE_SET_SERIALIZER(float, SerializePOD<float>);
    TYPE_SET_DESERIALIZER(float, DeserializePOD<float>);

    TYPE_REGISTER_POD(bool);
    TYPE_SET_SERIALIZER(bool, SerializePOD<bool>);
    TYPE_SET_DESERIALIZER(bool, DeserializePOD<bool>);

    TYPE_REGISTER(std::vector<int>);
    TYPE_SET_SERIALIZER(std::vector<int>, SerializeArray<int>);
    TYPE_SET_DESERIALIZER(std::vector<int>, DeserializeArray<int>);

    TYPE_REGISTER(std::string);
    TYPE_SET_SERIALIZER(std::string, SerializeString);
    TYPE_SET_DESERIALIZER(std::string, DeserializeString);

    TYPE_REGISTER(Handle);

    TYPE_REGISTER(Generic);
    TYPE_REGISTER_PTR(Generic*);

    TYPE_REGISTER(Member);
    TYPE_REGISTER_PTR(Member*);

    TYPE_REGISTER(float2);
    TYPE_ADD_MEMBER(float2, x);
    TYPE_ADD_MEMBER(float2, y);
    TYPE_REGISTER(float3);
    TYPE_ADD_MEMBER(float3, x);
    TYPE_ADD_MEMBER(float3, y);
    TYPE_ADD_MEMBER(float3, z);
    TYPE_REGISTER(float4);
    TYPE_ADD_MEMBER(float4, x);
    TYPE_ADD_MEMBER(float4, y);
    TYPE_ADD_MEMBER(float4, z);
    TYPE_ADD_MEMBER(float4, w);

    TYPE_REGISTER(quaternion);
    TYPE_ADD_MEMBER(quaternion, x);
    TYPE_ADD_MEMBER(quaternion, y);
    TYPE_ADD_MEMBER(quaternion, z);
    TYPE_ADD_MEMBER(quaternion, w);

    TYPE_REGISTER(Position);
    TYPE_ADD_MEMBER(Position, translation);
    TYPE_ADD_MEMBER(Position, scale);
    TYPE_ADD_MEMBER(Position, rotation);

    TYPE_REGISTER(GameComponent);
    TYPE_REGISTER_PTR(GameComponent*);

    //TYPE_REGISTER(LuaProxy);

    TYPE_REGISTER(GameObject);
    TYPE_ADD_MEMBER(GameObject, name);
    TYPE_ADD_MEMBER(GameObject, archetype);
    TYPE_SET_SERIALIZER(GameObject, GameObject::Serialize);
    TYPE_SET_DESERIALIZER(GameObject, GameObject::Deserialize);

    TYPE_REGISTER_PTR(GameObject*);

    TYPE_REGISTER(Archetype);
    TYPE_ADD_MEMBER(Archetype, name);
    TYPE_ADD_MEMBER(Archetype, archetype);
    TYPE_SET_SERIALIZER(Archetype, Archetype::Serialize);
    TYPE_SET_DESERIALIZER(Archetype, Archetype::Deserialize);

    TYPE_REGISTER(GameSpace);
    TYPE_SET_SERIALIZER(GameSpace, GameSpace::Serialize);
    TYPE_SET_DESERIALIZER(GameSpace, GameSpace::Deserialize);
    TYPE_REGISTER_PTR(GameSpace*);

    TYPE_REGISTER(LuaComponent);
    TYPE_ADD_MEMBER(LuaComponent, Name);
    TYPE_SET_SERIALIZER(LuaComponent, LuaComponent::Serialize);
    TYPE_SET_DESERIALIZER(LuaComponent, LuaComponent::Deserialize);

    TYPE_REGISTER(Transform);
	  TYPE_ADD_MEMBER(Transform, m_position);
    TYPE_ADD_MEMBER(Transform, m_right);
    TYPE_ADD_MEMBER(Transform, m_up);
    TYPE_ADD_MEMBER(Transform, m_forward);
    //TYPE_ADD_MEMBER(Transform, m_translation);
    //TYPE_ADD_MEMBER(Transform, m_rotation);
    //TYPE_ADD_MEMBER(Transform, m_scale);
    //TYPE_SET_SERIALIZER(Transform, Transform::Serialize);
    //TYPE_SET_DESERIALIZER(Transform, Transform::Deserialize);

    // Audio Components
    TYPE_REGISTER(SoundEmitter);
    TYPE_ADD_MEMBER(SoundEmitter, m_currentSong);
    TYPE_ADD_MEMBER(SoundEmitter, m_volume);
    TYPE_ADD_MEMBER(SoundEmitter, m_pitch);
    TYPE_ADD_MEMBER(SoundEmitter, m_isPlaying);
    TYPE_ADD_MEMBER(SoundEmitter, m_isLoop);

    // Physics Components

    // This is how you serialize enums
    TYPE_REGISTER_POD(DynamicState);
    TYPE_SET_SERIALIZER(DynamicState, SerializePOD<int>);
    TYPE_SET_DESERIALIZER(DynamicState, DeserializePOD<int>);

    TYPE_REGISTER(RigidBody);
    TYPE_ADD_MEMBER(RigidBody, m_State);
    TYPE_ADD_MEMBER(RigidBody, m_bStateDynamic);
    TYPE_ADD_MEMBER(RigidBody, m_bGhost);
   
    // should I add the member for my internal shape?
    TYPE_REGISTER(Box);
    TYPE_ADD_MEMBER(Box, m_HalfExtents);

    TYPE_REGISTER(BoxCollider);
    TYPE_ADD_MEMBER(BoxCollider, m_Box);

    TYPE_REGISTER(Sphere);
    TYPE_ADD_MEMBER(Sphere, m_fRadius);

    TYPE_REGISTER(SphereCollider);
    TYPE_ADD_MEMBER(SphereCollider, m_Sphere);

    TYPE_REGISTER(Capsule);
    TYPE_ADD_MEMBER(Capsule, m_TopPoint);
    TYPE_ADD_MEMBER(Capsule, m_BottomPoint);
    TYPE_ADD_MEMBER(Capsule, m_fRadius);

    TYPE_REGISTER(CapsuleCollider);
    TYPE_ADD_MEMBER(CapsuleCollider, m_Capsule);

    TYPE_REGISTER(GravityEffect);
    TYPE_ADD_MEMBER(GravityEffect, m_Direction);
    TYPE_ADD_MEMBER(GravityEffect, m_fMagnitude);
    TYPE_ADD_MEMBER(GravityEffect, m_bActive);

    TYPE_REGISTER(ConstantEffect);
    TYPE_ADD_MEMBER(ConstantEffect, m_Direction);
    TYPE_ADD_MEMBER(ConstantEffect, m_fMagnitude);
    TYPE_ADD_MEMBER(ConstantEffect, m_bActive);
    TYPE_ADD_MEMBER(ConstantEffect, m_TorqueDirection);
    TYPE_ADD_MEMBER(ConstantEffect, m_fTorqueMagnitude);

    TYPE_REGISTER(DragEffect);
    TYPE_ADD_MEMBER(DragEffect, m_fBTerm);

    // game logic
    TYPE_REGISTER(PlayerController);
    TYPE_ADD_MEMBER(PlayerController, m_nGamePadID);
    TYPE_ADD_MEMBER(PlayerController, m_WorldUp);
    TYPE_ADD_MEMBER(PlayerController, m_fOverHeatUpdate);
    TYPE_ADD_MEMBER(PlayerController, m_fJumpSpeed);
    TYPE_ADD_MEMBER(PlayerController, m_fJumpCancel);
    TYPE_ADD_MEMBER(PlayerController, m_fMaxMoveSpeed);
    TYPE_ADD_MEMBER(PlayerController, m_fJumpSpeed);
    TYPE_ADD_MEMBER(PlayerController, m_DefaultForward);
    TYPE_ADD_MEMBER(PlayerController, m_fTimeToShoot);
    TYPE_ADD_MEMBER(PlayerController, m_Color);

    TYPE_REGISTER(Stats);
    TYPE_ADD_MEMBER(Stats, m_health);
    TYPE_ADD_MEMBER(Stats, m_deathPos);
    TYPE_ADD_MEMBER(Stats, m_respawnPos);

    TYPE_REGISTER(Timer);
    TYPE_ADD_MEMBER(Timer, m_time);
    TYPE_ADD_MEMBER(Timer, m_startTime);

    TYPE_REGISTER(Bullet);
    TYPE_ADD_MEMBER(Bullet, m_fMaxTime);
    TYPE_ADD_MEMBER(Bullet, m_fRadius);

    TYPE_REGISTER(Flag);
    TYPE_ADD_MEMBER(Flag, m_startPos);

    //  Graphics
    TYPE_REGISTER(Renderable);
	//TYPE_ADD_MEMBER(Renderable, m_Transform);

    TYPE_ADD_MEMBER(Renderable, m_ModelName);
    TYPE_ADD_MEMBER(Renderable, m_TextureName);
	TYPE_ADD_MEMBER(Renderable, m_NormalTexture);
	TYPE_ADD_MEMBER(Renderable, m_EmissiveTexture);
	TYPE_ADD_MEMBER(Renderable, m_IsVisible);

    TYPE_REGISTER(Paintable);

    //Esteban's change - Adding ParticleSystem class and its members
    TYPE_REGISTER(ParticleSystem);
    TYPE_ADD_MEMBER(ParticleSystem, m_systemIsVisible);
    TYPE_ADD_MEMBER(ParticleSystem, m_systemIsPaused);
    TYPE_ADD_MEMBER(ParticleSystem, m_systemIsLooping);
    TYPE_ADD_MEMBER(ParticleSystem, m_particleCount);
    TYPE_ADD_MEMBER(ParticleSystem, m_systemLifeTime);
    TYPE_ADD_MEMBER(ParticleSystem, m_particleLifeTime);
    TYPE_ADD_MEMBER(ParticleSystem, m_particleLifeTimeVariance);
    TYPE_ADD_MEMBER(ParticleSystem, m_useCustomDeltaTime);
    TYPE_ADD_MEMBER(ParticleSystem, m_customDeltaTime);
    TYPE_ADD_MEMBER(ParticleSystem, m_diffFromOwner);
    //TYPE_ADD_MEMBER(ParticleSystem, m_blendMode); <- needs to be an int, not an enum
    TYPE_ADD_MEMBER(ParticleSystem, m_particleSpin);
    TYPE_ADD_MEMBER(ParticleSystem, m_particleSpinVariance);
    TYPE_ADD_MEMBER(ParticleSystem, m_particleScale);
    TYPE_ADD_MEMBER(ParticleSystem, m_particleScaleVariance);
    TYPE_ADD_MEMBER(ParticleSystem, m_particleEndScale);
    TYPE_ADD_MEMBER(ParticleSystem, m_particleVel);
    TYPE_ADD_MEMBER(ParticleSystem, m_particleVelVariance);
    TYPE_ADD_MEMBER(ParticleSystem, m_particleBlendMode);
    TYPE_ADD_MEMBER(ParticleSystem, m_particleTint);
    TYPE_ADD_MEMBER(ParticleSystem, m_particleEndTint);
    //TYPE_ADD_MEMBER(ParticleSystem, );


    TYPE_REGISTER(PointLight);
    TYPE_ADD_MEMBER(PointLight, m_LightColor);
    TYPE_ADD_MEMBER(PointLight, m_RangeAttenuation);


	TYPE_REGISTER(EmissiveComponent);
	TYPE_ADD_MEMBER(EmissiveComponent, m_emittedColor); 
		
	TYPE_REGISTER(Fade);
	TYPE_ADD_MEMBER(Fade, m_opacity);
	TYPE_ADD_MEMBER(Fade, m_timer);
	TYPE_ADD_MEMBER(Fade, m_timeBetweenPulses);
	TYPE_ADD_MEMBER(Fade, m_fluctuate);
	TYPE_ADD_MEMBER(Fade, m_update);
	TYPE_ADD_MEMBER(Fade, m_pulse);


	TYPE_REGISTER(Levitate);
	TYPE_ADD_MEMBER(Levitate, m_offsetRange);
	TYPE_ADD_MEMBER(Levitate, m_speed);

	TYPE_REGISTER(Flicker);
	TYPE_ADD_MEMBER(Flicker, m_offsetRange);
	TYPE_ADD_MEMBER(Flicker, m_speed);

	TYPE_REGISTER(LightDeath);
	TYPE_ADD_MEMBER(LightDeath, animationLength);

    RegisterEnums();
  }
}