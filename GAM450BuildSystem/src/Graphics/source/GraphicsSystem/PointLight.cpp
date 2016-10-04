/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: PointLight.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  PointLight.cpp
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch\precompiled.h"
#include "Graphics/includes/GraphicsSystem/PointLight.h"
#include "Core/types/space/Space.h"

PointLight::PointLight() : m_offset(0.0f)
{
}

PointLight::~PointLight()
{
}

void PointLight::Initialize()
{
	m_Transform = space->GetComponent(eTransform, owner)->self;
}

const float4& PointLight::GetLightColor() const
{
	return m_LightColor;
}

const float2& PointLight::GetRangeAttenuation() const
{
	return m_RangeAttenuation;
}

Core::Transform* PointLight::GetTransform()
{
	return space->GetHandles().GetAs<Core::Transform>(m_Transform);
}

void PointLight::SetLightColor(const float4& color)
{
	m_LightColor = color;
}

void PointLight::SetRangeAttenuation(const float2& rangeAttenuation)
{
	m_RangeAttenuation = rangeAttenuation;
}

void PointLight::SetOffset(const float& offset)
{
	m_offset = offset;
}

void PointLight::SetDeathOffset(const float2& deathOffset)
{
	m_deathOffset = deathOffset;
}

const float& PointLight::GetOffset()
{
	return m_offset;
}

const float2& PointLight::GetDeathOffset()
{
	return m_deathOffset;
}

COMP_LUA_DEP(PointLight);