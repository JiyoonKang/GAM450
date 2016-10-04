/******************************************************************************/
/*!
\file  ParticleSystem.h
\project  Bring the Paint
\author Esteban Maldonado Cabán

Copyright (C) 2016 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma  once

#include "Graphics/includes/GraphicsSystem/Renderable.h"
#include "Graphics/includes/GraphicsSystem/Particle.h"
#include "Graphics/includes/SkyeDX/ObjectHandle.h"

#define INDEX_COUNT DEFAULT_PARTICLE_COUNT * 6

class ParticleSystem : public Core::GameComponent
{
public:
  ParticleSystem(unsigned int c = DEFAULT_PARTICLE_COUNT);
  ~ParticleSystem();

  //Inherited Methods
  virtual void Initialize() override;
  virtual void Remove() override;

  //NOTE: I need to ask later on for a way to get the DeltaTime from the frame.
  void UpdateParticles(float dt);

  //Getters
  static Skye::ObjectHandle GetIndexBufferHandle();
  unsigned int GetParticleIndexCount()const;
  Skye::ObjectHandle GetTextureHandle()const;
  std::string GetTextureName()const;

  //Setters
  void SetParticleCount(const unsigned int c);
  void SetTexture(std::string file);

  //System properties
  int m_particleCount;
  bool m_systemIsVisible;
  bool m_systemIsPaused;
  bool m_systemIsLooping;
  float m_systemLifeTime;
  bool m_systemStopUpdating;
  bool m_useCustomDeltaTime;
  float m_customDeltaTime;
  int m_particleBlendMode;

  //For each particle properties
  //Lifetime
  float m_particleLifeTime;
  float m_particleLifeTimeVariance;

  //Difference from owner's own location
  float3 m_diffFromOwner;

  //Spin
  float3 m_particleSpin;
  float3 m_particleSpinVariance;

  //Scale
  float3 m_particleScale;
  float3 m_particleScaleVariance;
  float3 m_particleEndScale;

  //Velocity
  float3 m_particleVel;
  float3 m_particleVelVariance;

  //Acceleration
  float3 m_particleAcc;
  float3 m_particleAccVariance;

  float4 m_particleTint;
  float4 m_particleEndTint;

  static Particle_HLSLBuffer m_particleConstBuffData;
private:

  int renderedParticles = 0;
  //getters
  float RandFloat(float min = 0.0f, float max = 1.0f)const;
  float3 RandFloat3(float3 min, float3 max) const;
  
  //setters
  void ResetParticle(Particle& p);
  void InterpolateTint(Particle_HLSL& pHLSL, const float time, const float end_time);
  void InterpolateScale(Particle_HLSL& pHLSL, const float time, const float end_time);

  //inner data
  Skye::ObjectHandle m_texture;
  std::string m_TextureName;
  std::vector<Particle> m_particles;
  std::vector<Particle_HLSL> m_hlslParticles;

  //static data
  static Skye::ObjectHandle m_particleSystemConstBuff;
  static Skye::ObjectHandle m_indexBuffer;
  static Skye::ObjectHandle m_vertexShader;
  static Skye::ObjectHandle m_pixelShader;
};