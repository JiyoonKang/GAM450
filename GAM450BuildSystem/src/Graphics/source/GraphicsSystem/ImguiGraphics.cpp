/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: ImguiGraphics.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file   ImguiGraphics.cpp
\project  Bring The Paint
\author Jorge Antonio Robles

Copyright (C) 2014 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch\precompiled.h"
#include "includes/GraphicsSystem/ImguiGraphics.h"
#include "Graphics\includes\GraphicsSystem\SkyeGraphics.h"
#include "includes/SkyeDX/SkyeDX11.h"

ImguiGraphics::ImguiGraphics() : m_VertexBuffer(Skye::ObjectHandle::Null()), m_Texture(Skye::ObjectHandle::Null()), 
								 m_VertexCount(0), m_renderContext(nullptr), m_Data(nullptr)
{
}

ImguiGraphics::~ImguiGraphics()
{
}

void ImguiGraphics::Initialize(Skye::Renderer* renderContext)
{
	m_renderContext = renderContext;
}

void ImguiGraphics::Update(unsigned vertexCount, unsigned rectCount)
{
	m_VertexCount = vertexCount;
	m_rectData.rectCount = rectCount;

	m_renderContext->CopyData(m_VertexBuffer, m_Data, m_VertexCount * sizeof(ImguiVertex));
}

void ImguiGraphics::Draw()
{
	m_renderContext->BindTexture(0, m_Texture);
	m_renderContext->BindVertexBuffer(m_VertexBuffer, sizeof(ImguiVertex));

	unsigned count = 0;
	if (m_rectData.rects && m_rectData.rectCount > 0)
	{
		for (unsigned i = 0; i < m_rectData.rectCount; ++i)
		{
			m_renderContext->SetScissorRect(1, &m_rectData.rects[i].rect);
			m_renderContext->Draw(m_rectData.rects[i].offset, count);
			count += m_rectData.rects[i].offset;
		}
	}
}

void ImguiGraphics::CreateBuffer(void* data, unsigned size)
{
	m_renderContext->CreateVertexBuffer(m_VertexBuffer, Skye::BufferUsage::USAGE_DYNAMIC, size, data);
	m_Data = data;
}

void ImguiGraphics::SetRectData(ImguiRect* rects)
{
	m_rectData.rects = rects;
}

void ImguiGraphics::SetTexture(const Skye::ObjectHandle& texture)
{
	m_Texture = texture;
}