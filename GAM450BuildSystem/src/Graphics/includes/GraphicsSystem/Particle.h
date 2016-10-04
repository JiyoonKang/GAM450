/******************************************************************************/
/*!
\file  Particle.h
\project  Bring the Paint
\author Esteban Maldonado Cabán

Copyright (C) 2016 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma  once

#include <Base/util/math/VectorMath.h>
//#include <Base/util/math/MathUtil.h>

#define DEFAULT_PARTICLE_COUNT 100
//#define MAX_PARTICLE_COUNT 1024
const int MAX_PARTICLE_COUNT = 200;

struct Particle_HLSL
{
  Particle_HLSL() : pos(0, 0, 0), scaleX(1.0f), rot(0,0,0), scaleY(1.0f), color(1.0f, 1.0f, 1.0f, 1.0f){}
  
  float3 pos;
  float scaleX;

  float3 rot;
  float scaleY;
  
  float4 color;

};

struct Particle_HLSLBuffer
{
  Particle_HLSL particles[MAX_PARTICLE_COUNT];

  int particle_count; //4
  float padding[3]; //12
};

struct Particle
{
public:
  Particle(){}
  ~Particle(){}

  float3 m_position;

  float3 m_spin;
  float3 m_end_spin;

  float3 m_scale;
  float3 m_end_scale;

  float3 m_velocity;
  float3 m_acceleration;

  float m_lifespan;
  float m_currentLife;

  bool m_isActive;
};