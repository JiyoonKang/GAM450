/*******************************************************************************
* All content © 2015 DigiPen (USA) Corporation, all rights reserved.
* Reproduction or disclosure of this file or its contents without the prior
* written consent of DigiPen Institute of Technology is prohibited.
*
* File: Paintable.cpp
* Author:
* Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
******************************************************************************/
#include <pch\precompiled.h>
#include <Graphics\includes\GraphicsSystem\ParticleSystem.h>
#include <Graphics\includes\GraphicsSystem\SkyeGraphics.h>
#include <Graphics\includes\SkyeDX\Renderer.h>
#include <Core\types\object\Object.h>
#include <Base\util\math\MathUtil.h>

ParticleSystem::ParticleSystem(unsigned int c) :m_particleCount(c)
{
	m_particleLifeTime = 2.0f;
	m_particleLifeTimeVariance = 2.0;

  m_diffFromOwner = float3(0, 0, 0);

	m_particleSpin = float3(0, 0, 1);

	m_particleScale = float3(1, 1, 0);
	m_particleScaleVariance = float3(0.5, 0.5, 0.5);

	m_particleVel = float3(RandFloat(-3, 3), RandFloat(-3, 3), RandFloat(-3, 3));
	m_particleVelVariance = float3(1.5, 1.5, 1.5);

  m_particleEndTint = m_particleTint = float4(1, 1, 1, 1);

	m_systemIsVisible = true;
	m_systemIsPaused = false;
	m_systemIsLooping = true;
  m_useCustomDeltaTime = false;
  m_systemStopUpdating = false;
  m_customDeltaTime = 1.0f / 60.0f;
	m_systemLifeTime = 0;
	m_texture = m_indexBuffer = Skye::ObjectHandle::Null();
	m_TextureName = "";
	m_particleBlendMode = Skye::BlendMode::BLEND_MODE_ALPHA;

	m_particles.resize(MAX_PARTICLE_COUNT);

  renderedParticles = 0;
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Initialize()
{
}

void ParticleSystem::Remove()
{
}

void ParticleSystem::UpdateParticles(float dt)
{
	if (m_systemIsPaused)
		return;

  //if we want, then we can use custom delta time, instead of the engine's own delta time
  if (m_useCustomDeltaTime)
    dt = m_customDeltaTime;

	if (!m_systemIsLooping)
		m_systemLifeTime -= dt;

  renderedParticles = 0;
	for (int i = 0; i < m_particleCount /*m_particles.size()*/; ++i)
	{
		if (m_particles[i].m_isActive)
		{
			//if life has run out...
			if (m_particles[i].m_lifespan <= (m_particles[i].m_currentLife += dt))
			{
        m_particles[i].m_isActive = false;
				continue;
			}
			//else continue lifetime...
			else
			{
				//update position...
        m_particleConstBuffData.particles[renderedParticles].pos = m_particles[i].m_position += (m_particles[i].m_velocity * dt);
        m_particleConstBuffData.particles[renderedParticles].rot.z += m_particles[i].m_spin.z * dt;
        InterpolateScale(m_particleConstBuffData.particles[renderedParticles], m_particles[i].m_currentLife, m_particles[i].m_lifespan);
        InterpolateTint(m_particleConstBuffData.particles[renderedParticles], m_particles[i].m_currentLife, m_particles[i].m_lifespan);

        ++renderedParticles;
			}
		}

		else
		{
      if (!m_systemStopUpdating)
        ResetParticle(m_particles[i]);
		}
	}
}

float ParticleSystem::RandFloat(float min, float max) const
{
	//if the numbers are equal then just return any of them
	if (std::abs(min - max) <= 0.001f)
	{
		return min;
	}

	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));;
}

unsigned int ParticleSystem::GetParticleIndexCount() const
{
  return renderedParticles * 6;
}

void ParticleSystem::SetParticleCount(const unsigned int c)
{
	if (c <= MAX_PARTICLE_COUNT)
		m_particleCount = c;
}

Skye::ObjectHandle ParticleSystem::GetIndexBufferHandle()
{
  //create the index buffer
  if (!m_indexBuffer)
  {
    std::vector<unsigned int> m_indexData;

    m_indexData.resize(MAX_PARTICLE_COUNT * 6);
    for (unsigned i = 0; i < m_indexData.size(); ++i)
      m_indexData[i] = i;

    Skye::Renderer* context = GRAPHICS->GetRenderContext();
    context->CreateIndexBuffer(m_indexBuffer, Skye::USAGE_DEFAULT,
      m_indexData.size(), m_indexData.data());
  }

	return m_indexBuffer;
}

Skye::ObjectHandle ParticleSystem::GetTextureHandle() const
{
	return m_texture;
}

std::string ParticleSystem::GetTextureName() const
{
	return m_TextureName;
}

void ParticleSystem::SetTexture(std::string file)
{
	//get a copy of the render contex pointer
	Skye::Renderer* context = GRAPHICS->GetRenderContext();

	//create a temp. object handle to a file(if it exists)
	Skye::ObjectHandle tempNewTexture = GRAPHICS->GetTexture(file);

	// if the texture does not exist try to load it.
	if (!tempNewTexture)
	{
		//try to create the texture
		context->CreateTexture(tempNewTexture, file);

		//If texture was created successfully then change the texture
		if (tempNewTexture)
		{
			m_TextureName = file;
			m_texture = tempNewTexture;
		}
	}
	else
	{
		//if texture exists then change it
		if (tempNewTexture)
		{
			m_TextureName = file;
			m_texture = tempNewTexture;
		}
	}
}

void ParticleSystem::ResetParticle(Particle& p)
{
	Core::Transform* transform = reinterpret_cast<Core::Transform*>(GetOwner()->GetComponent(eTransform));
	p.m_currentLife = 0;
	p.m_spin = RandFloat3(m_particleSpin - m_particleSpinVariance, m_particleSpin + m_particleSpinVariance);
	p.m_lifespan = RandFloat(m_particleLifeTime - m_particleLifeTimeVariance, m_particleLifeTime + m_particleLifeTimeVariance);
	p.m_velocity = RandFloat3(m_particleVel - m_particleVelVariance, m_particleVel + m_particleVelVariance);
	p.m_scale = RandFloat3(m_particleScale - m_particleScaleVariance, m_particleScale + m_particleScaleVariance);
	p.m_position = transform->m_position.translation.xyz;
  p.m_position += m_diffFromOwner;
	p.m_isActive = true;
}

float3 ParticleSystem::RandFloat3(float3 min, float3 max) const
{
	float3 v;
	v.x = RandFloat(min.x, max.x);
	v.y = RandFloat(min.y, max.y);
	v.z = RandFloat(min.z, max.z);

	return v;
}

void ParticleSystem::InterpolateTint(Particle_HLSL& pHLSL, const float time, const float end_time)
{
  pHLSL.color = m_particleTint + ( (m_particleEndTint - m_particleTint) * (time / end_time) );
}

void ParticleSystem::InterpolateScale(Particle_HLSL& pHLSL, const float time, const float end_time)
{
  float2 t = (m_particleScale + ((m_particleEndScale - m_particleScale) * (time / end_time))).xy;
  pHLSL.scaleX = t.x;
  pHLSL.scaleY = t.y;
}

Skye::ObjectHandle ParticleSystem::m_indexBuffer = Skye::ObjectHandle();

Skye::ObjectHandle ParticleSystem::m_pixelShader = Skye::ObjectHandle();
Skye::ObjectHandle ParticleSystem::m_vertexShader = Skye::ObjectHandle();
Skye::ObjectHandle ParticleSystem::m_particleSystemConstBuff = Skye::ObjectHandle();

Particle_HLSLBuffer ParticleSystem::m_particleConstBuffData = Particle_HLSLBuffer();
