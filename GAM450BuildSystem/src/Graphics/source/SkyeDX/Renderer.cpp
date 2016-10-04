/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Renderer.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Renderer.cpp
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch\precompiled.h"
#include <includes\SkyeDX\Renderer.h>
#include <Base/util/debug/DebugAssist.h>

namespace Skye
{
	Renderer* Renderer::m_Instance(nullptr);

	Renderer::Renderer(void) : m_hwnd(nullptr), m_ScreenColor(0.0f, 0.0f, 0.0f, 0.0f)
	{
		m_BackBuffer.SetType(ObjectType::BACK_BUFFER);
		m_BackBuffer.SetHandle(0);

		if (m_Instance == nullptr)
		{
			m_Instance = this;
		}
		else
		{
			ErrorIf(true, "ERROR: A renderer object already exists");
		}
	}

	Renderer::~Renderer(void)
	{
		m_Instance = nullptr;
	}

	Renderer* Renderer::Instance(void)
	{
		return m_Instance;
	}

	void Renderer::SetWindow(const HWND& hwnd)
	{
		m_hwnd = hwnd;
	}

	void Renderer::SetBackColor(const float4& color)
	{
		m_ScreenColor = color;
	}

	void Renderer::SetMSAALevel(const MSAALevel level)
	{
		m_MSAALevel = level;
	}

	void Renderer::SetVsync(bool vSync)
	{
		m_Vsync = vSync;
	}

	const ObjectHandle& Renderer::GetBackBuffer(void) const
	{
		return m_BackBuffer;
	}

	bool Renderer::GetVsync(void) const
	{
		return m_Vsync;
	}

	ObjectHandle Renderer::CreateHandle(const ObjectType type, const int handle)
	{
		ObjectHandle newHandle;
		newHandle.SetType(type);
		newHandle.SetHandle(handle);

		return newHandle;
	}
}