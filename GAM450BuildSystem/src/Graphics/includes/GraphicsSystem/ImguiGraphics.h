/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: ImguiGraphics.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  ImguiGraphics.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "Base/util/math/VectorMath.h"
#include "Graphics/includes/SkyeDX/DXTypes.h"
#include "Graphics/includes/SkyeDX/ObjectHandle.h"

namespace Skye
{
	class Renderer;
}

class ImguiGraphics
{
	public:
		struct ImguiVertex
		{

			float3 position;
			float4 color;
			float2 uv;
		};

		struct ImguiRect
		{
			Skye::SkyeRect rect;
			unsigned offset;
		};

		struct RectData
		{
			ImguiRect* rects;
			unsigned rectCount;
			RectData(void) : rects(nullptr), rectCount(0) {}
		};

		ImguiGraphics(void);
		~ImguiGraphics(void);
		
		void Initialize(Skye::Renderer* renderContext);
		void Update(unsigned vertexCount, unsigned rectCount);
		void Draw(void);

		void CreateBuffer(void* data, unsigned size);

		void SetRectData(ImguiRect* rects);
		void SetTexture(const Skye::ObjectHandle& texture);

	private:

		RectData m_rectData;
		Skye::ObjectHandle m_VertexBuffer;
		Skye::ObjectHandle m_Texture;

		unsigned m_VertexCount;
		Skye::Renderer* m_renderContext;

		void* m_Data;

};
