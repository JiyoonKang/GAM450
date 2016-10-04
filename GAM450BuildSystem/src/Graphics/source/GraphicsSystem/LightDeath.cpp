#include <pch/precompiled.h>
#include <Graphics/includes/GraphicsSystem/LightDeath.h>
#include <Graphics/includes/GraphicsSystem/PointLight.h>
#include <Core/types/space/Space.h>

LightDeath::LightDeath() : animationLength(3.0f), m_isDead(false), m_rangeAttenuationOffset(float2()), m_Kill(false), m_rangeAttenuation(float2())
{
}

LightDeath::~LightDeath()
{
}

void LightDeath::Initialize()
{
	//Get the handle to the owner object
	if (space->GetComponent(ePointLight, owner))
		m_pointLight = space->GetComponent(ePointLight, owner)->self;

	//If we have an owner
	if (m_pointLight != Handle::null)
	{
		//Get the point light component attached to the owner
		PointLight* parent = space->GetHandles().GetAs<PointLight>(m_pointLight);

		//If the owner has a point light component
		if (parent)
			//Get the range attenuation from said component
			m_rangeAttenuation = parent->GetRangeAttenuation();
	}
}

void LightDeath::Remove()
{
}

void LightDeath::Update(const float& dt)
{
	if (m_Kill)
	{
		float timeInterpolation = dt / animationLength;

		//Calculate how much needs to be offset this frame
		float2 addedOffset = float2(m_rangeAttenuation.x * timeInterpolation, m_rangeAttenuation.y * timeInterpolation);

		//Decrement it from the currante range offset
		m_rangeAttenuationOffset -= addedOffset;


		//once it is done interpolating (range is 0), kill the light
		float diff = m_rangeAttenuation.x + m_rangeAttenuationOffset.x;

		if (diff <= 0)
		{
			m_isDead = true;
			m_Kill = false;
		}
	}
}

void LightDeath::Kill()
{
	m_Kill = true;
}

bool LightDeath::IsDead()
{
	return m_isDead;
}

float2 LightDeath::GetOffset()
{
	return m_rangeAttenuationOffset;
}

void LightDeath::SetAnimationLength(float length)
{
	animationLength = length;
}