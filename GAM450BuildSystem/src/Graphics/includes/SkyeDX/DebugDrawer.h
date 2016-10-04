/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: DebugDrawer.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  DebugDrawer.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "Base/util/math/VectorMath.h"
#include "ObjectHandle.h"

namespace Core{
	class Transform;
}

namespace Skye
{
	class Renderer;
	class Camera;

	class DebugDrawer
	{
		public:

			struct LineVertex
			{
				LineVertex(void);
				LineVertex(const float4& position, const float4& color);
				float4 position;
				float4 color;
			};

			DebugDrawer(unsigned capacity);
			~DebugDrawer(void);

			void Initialize(Renderer* renderContext, Camera* editorCamera);
			void Draw(void);

			void Clear(void);

			void DrawLine(const float3& pos1, const float3& pos2, const float4& color);
			void DrawLine(const float4& pos1, const float4& pos2, const float4& color);

			void DrawSphere(const float3& position, const float& radius, const float4& color);
			void DrawBox(const float4& scale, const Core::Transform* transform, const float4& color);
			void DrawCapsule(const float4& endPoint0, const float4& endPoint1, const float& radius, const float4& color);
			void DrawFrustum(const float3* corners, const float4& color);

			unsigned GetSize(void) const;
			unsigned GetCapacity(void) const;

			void SetEditorCamera(Camera* editorCamera);

			void Axis(const float4& pos, const float scale);
			void DrawTranslateGizmo(const float4& pos, const float scale);
			void DrawRotateGizmo(const float4& pos, const float scale);
			void DrawScaleGizmo(const float4& pos, const float scale);


		private:

			void DrawDiscHelper(const float3& position, float3& v, const float& radius, const float4& color);
			void DrawHorizonDisc(const float3& position, const float& radius, const float4& color);
			void DrawCapsuleCylinder(const float3& p0, const float3& p1, const float3& normal, const float& radius, const float4& color);
			void DrawCapsuleVerticals(const float3& p0, const float3& p1, const float3& normal, const float& radius, const float4& color);
			void DrawSemiSphere(const float3& position, const float3& direction, const float& radius, const float4& color);
			void DrawHalfDisc(const float3& position, float3& v, const float3& direction, const float& radius, const float4& color);
			void DrawHalfHorizonDisc(const float3& position, const float3& direction, const float& radius, const float4& color);
			void Push(const float3& vertex, const float4& color);

			Renderer* m_renderContext;
			Camera* m_EditorCamera;
			ObjectHandle m_VertexBuffer;

			unsigned m_Size;
			unsigned m_Capacity;

			LineVertex* m_VertexList;
	};

	struct Color
	{
		// Predefined Colors
		static const float4 s_Red;
		static const float4 s_Green;
		static const float4 s_Blue;
		static const float4 s_Yellow;
		static const float4 s_Cyan;
		static const float4 s_Black;
		static const float4 s_White;
	};
	
}