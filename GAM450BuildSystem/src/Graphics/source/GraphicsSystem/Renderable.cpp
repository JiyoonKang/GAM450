/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Renderable.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Renderable.cpp
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch\precompiled.h"
#include <includes\GraphicsSystem\Renderable.h>
#include "includes/GraphicsSystem/SkyeGraphics.h"
#include "Core/types/space/Space.h"


Renderable::Renderable() : m_TextureName(""), m_NormalTexture("Periwinkle.png"), m_EmissiveTexture("NoGlow"), m_IsVisible(true), m_BlendMode(Skye::BlendMode::BLEND_MODE_NONE)
{
}

Renderable::~Renderable()
{
}

void Renderable::Initialize()
{
	m_Transform = space->GetComponent(eTransform, owner)->self;
  SkyeGraphics::RegisterRenderable(self);
}

void Renderable::Remove()
{
	SkyeGraphics::UnregisterRenderable(self);
}

void Renderable::SetModel(const std::string& modelName)
{
	m_ModelName = modelName;
}

void Renderable::SetTexture(const std::string& textureName)
{
	m_TextureName = textureName;
}

void Renderable::SetNormalTexture(const std::string& textureName)
{
	m_NormalTexture = textureName;
}

void Renderable::SetEmissiveTexture(const std::string& textureName)
{
	m_EmissiveTexture = textureName;
}

void Renderable::SetBlendMode(const Skye::BlendMode& blendMode)
{
	m_BlendMode = blendMode;
}

void Renderable::SetVisibility(const bool& visible)
{
	m_IsVisible = visible;
}

void Renderable::ToggleVisibility()
{
	m_IsVisible = !m_IsVisible;
}

const bool& Renderable::IsVisible()
{
	return m_IsVisible;
}

const std::string& Renderable::GetModelName() const
{
	return m_ModelName;
}

const std::string& Renderable::GetTextureName() const
{
	return m_TextureName;
}

const std::string& Renderable::GetNormalTextureName() const
{
	return m_NormalTexture;
}

const std::string& Renderable::GetEmissiveTextureName() const
{
	return m_EmissiveTexture;
}

const Skye::BlendMode& Renderable::GetBlendMode() const
{
	return m_BlendMode;
}

Core::Transform* Renderable::GetTransform()
{
	return space->GetHandles().GetAs<Core::Transform>(m_Transform);
}

COMP_LUA_DEP(Renderable);
