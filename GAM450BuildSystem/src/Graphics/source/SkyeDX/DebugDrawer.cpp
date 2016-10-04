/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: DebugDrawer.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  DebugDrawer.cpp
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "pch\precompiled.h"
#include "includes/SkyeDX/DebugDrawer.h"
#include "includes/SkyeDX/SkyeDX11.h"
#include "includes/SkyeDX/Camera.h"
#include "Core/components/transform/CTransform.h"

#define PI 3.14159265359f
#define RGB_TO_FLOAT(x) x / 255.0f
namespace Skye
{
	// Predefined Colors
	const float4 Color::s_Red = float4(RGB_TO_FLOAT(255), 0, 0, 1);
	const float4 Color::s_Green = float4(0, RGB_TO_FLOAT(255), 0, 1);
	const float4 Color::s_Blue = float4(0, 0, RGB_TO_FLOAT(255), 1);
	const float4 Color::s_Yellow = float4(RGB_TO_FLOAT(255), RGB_TO_FLOAT(255), 0, 1);
	const float4 Color::s_Cyan = float4(0, RGB_TO_FLOAT(255), RGB_TO_FLOAT(255), 1);
	const float4 Color::s_Black = float4(0, 0, 0, 1);
	const float4 Color::s_White = float4(RGB_TO_FLOAT(255), RGB_TO_FLOAT(255), RGB_TO_FLOAT(255), 1);

	DebugDrawer::LineVertex::LineVertex() : position(float4()), color(1.0f, 1.0f, 1.0f, 1.0f)
	{
		
	}

	DebugDrawer::LineVertex::LineVertex(const float4& position, const float4& color) : position(position), color(color)
	{
		
	}

	DebugDrawer::DebugDrawer(unsigned capacity) : m_renderContext(nullptr), m_EditorCamera(nullptr), m_Size(0), m_Capacity(capacity), m_VertexList(new LineVertex[m_Capacity])
	{
	}

	DebugDrawer::~DebugDrawer()
	{
		delete[] m_VertexList;
	}

	void DebugDrawer::Initialize(Renderer* renderContext, Camera* editorCamera)
	{
		//Save render context pointer
		m_renderContext = renderContext;

		m_EditorCamera = editorCamera;

		//Create vertex buffer
		m_renderContext->CreateVertexBuffer(m_VertexBuffer, BufferUsage::USAGE_DYNAMIC, m_Capacity * sizeof(LineVertex));
	}

	void DebugDrawer::Draw()
	{
		m_renderContext->CopyData(m_VertexBuffer, m_VertexList, m_Size * sizeof(LineVertex));
		m_renderContext->BindVertexBuffer(m_VertexBuffer, sizeof(LineVertex));
		m_renderContext->Draw(m_Size);
	}

	void DebugDrawer::Clear()
	{
		m_Size = 0;
	}

	void DebugDrawer::DrawLine(const float3& pos1, const float3& pos2, const float4& color)
	{
		Push(pos1, color);
		Push(pos2, color);
	}

	unsigned DebugDrawer::GetSize() const
	{
		return m_Size;
	}

	unsigned DebugDrawer::GetCapacity() const
	{
		return m_Capacity;
	}

	void DebugDrawer::SetEditorCamera(Camera* editorCamera)
	{
		m_EditorCamera = editorCamera;
	}

	void DebugDrawer::DrawDiscHelper(const float3& position, float3& n, const float& radius, const float4& color)
	{
		float fullCircle = PI * 2.0f;
		float slice = PI / 40.0f;

		float3 u, v;

		if (n.x != 0 || n.z != 0)
		{
			u = n.Cross(float3(0.0f, 1.0f, 0.0f));
			u.Normalize();
		}
		else
		{
			u = n.Cross(float3(0.0f, 0.0f, 1.0f));
			u.Normalize();
		}

		v = u.Cross(n);
		v.Normalize();

		float3 w = v * std::cosf(0) + u * std::sinf(0);
		float3 p = position + w * radius;
		Push(p, color);

		for (float i = slice; i <= fullCircle; i += slice)
		{
			w = v * std::cos(i) + u * std::sin(i);
			p = position + w * radius;
			Push(p, color);
		}
	}

	void DebugDrawer::DrawHorizonDisc(const float3& position, const float& radius, const float4& color)
	{
		float3 cameraPosition = m_EditorCamera->GetPosition();
		float3 cameraToShphere = position - cameraPosition;

		float d = cameraToShphere.Length();
		float l = std::sqrt((d * d) - (radius * radius));
		float h = l * (radius / d);
		float z = std::sqrt((radius * radius) - (h * h));

		cameraToShphere.Normalize();

		float3 centerOffset = position - cameraToShphere * z;

		DrawDiscHelper(centerOffset, cameraToShphere, h, color);
	}

	void DebugDrawer::DrawSphere(const float3& position, const float& radius, const float4& color)
	{
		DrawHorizonDisc(position, radius, color);

		float3 v(1.0f, 0.0f, 0.0f);

		DrawDiscHelper(position, v, radius, color);

		v = float3(0.0f, 1.0f, 0.0f);

		DrawDiscHelper(position, v, radius, color);

		v = float3(0.0f, 0.0f, 1.0f);

		DrawDiscHelper(position, v, radius, color);
	}

	void DebugDrawer::DrawCapsuleCylinder(const float3& p0, const float3& p1, const float3& n, const float& radius, const float4& color)
	{

		float3 normal = n;

		float numSlices = 6.0f;
		float3 verticalStep = (p1 - p0) / numSlices;

		float3 tempPos = p0;
		float3 tempEnd = p1;
		//DrawDiscHelper(p0, normal, radius, color);
		//DrawDiscHelper((p0 + p1) / 2, normal, radius, color);
		//DrawDiscHelper(p1, normal, radius, color);

		while (tempPos != tempEnd)
		{
			DrawDiscHelper(tempPos, normal, radius, color);
			tempPos += verticalStep;
		}

		DrawCapsuleVerticals(p0, p1, normal, radius, color);

	}

	void DebugDrawer::DrawCapsuleVerticals(const float3& p0, const float3& p1, const float3& normal, const float& radius, const float4& color)
	{
		const int numSlices = 8;

		float3 topPoints[numSlices];
		float3 bottomPoints[numSlices];

		//Circle slices
		float fullCircle = PI * 2.0f;
		float slice = fullCircle / static_cast<float>(numSlices);

		//Calculate orthonormal basis for the circles on the cylinder
		float3 u, v;

		if (normal.x != 0 || normal.z != 0)
		{
			u = normal.Cross(float3(0.0f, 1.0f, 0.0f));
			u.Normalize();
		}
		else
		{
			u = normal.Cross(float3(0.0f, 0.0f, 1.0f));
			u.Normalize();
		}

		v = u.Cross(normal);
		v.Normalize();

		//Initial point on the top circle
		float3 w = v * std::cosf(0) + u * std::sinf(0);
		w.Normalize();
		float3 p = p0 + w * radius;

		topPoints[0] = p;

		//Calculate thre remaining points on top circle
		int j = 1;
		for (float i = slice; j < numSlices; i += slice, ++j)
		{
			w = v * std::cos(i) + u * std::sin(i);
			topPoints[j] = p0 + w * radius;
		}


		//Calculate the points at the bottom of the cylinder

		//Initial point on the circle
		w = v * std::cosf(0) + u * std::sinf(0);
		p = p1 + w * radius;
		bottomPoints[0] = p;
		
		//Calculate the remaining points on the circle
		j = 1;
		for (float i = slice; j < numSlices; i += slice, ++j)
		{
			w = v * std::cos(i) + u * std::sin(i);
			bottomPoints[j] = p1 + w * radius;
		}

		//Draw lines from top to bottom
		for (int i = 0; i < numSlices; ++i)
		{
			DrawLine(topPoints[i], bottomPoints[i], color);
		}
	}

	void DebugDrawer::DrawCapsule(const float4& endPoint0, const float4& endPoint1, const float& radius, const float4& color)
	{
		float4 n = endPoint0 - endPoint1;
		float3 normal = n.xyz;
		normal.Normalize();

		float3 p0(endPoint0.x, endPoint0.y, endPoint0.z);
		float3 p1(endPoint1.x, endPoint1.y, endPoint1.z);

		DrawCapsuleCylinder(p0, p1, normal, radius, color);

		DrawSemiSphere(p0, normal, radius, color);
		DrawSemiSphere(p1, -normal, radius, color);

	}

	void DebugDrawer::DrawFrustum(const float3* corners, const float4& color)
	{
		DrawLine(corners[0], corners[1], color);
		DrawLine(corners[1], corners[3], color);
		DrawLine(corners[2], corners[3], color);
		DrawLine(corners[2], corners[0], color);

		DrawLine(corners[4], corners[5], color);
		DrawLine(corners[5], corners[7], color);
		DrawLine(corners[6], corners[7], color);
		DrawLine(corners[6], corners[4], color);

		DrawLine(corners[0], corners[4], color);
		DrawLine(corners[1], corners[5], color);
		DrawLine(corners[2], corners[6], color);
		DrawLine(corners[3], corners[7], color);

	}

	void DebugDrawer::DrawSemiSphere(const float3& position, const float3& direction, const float& radius, const float4& color)
	{
		DrawHalfHorizonDisc(position, direction, radius, color);

		float3 v(1.0f, 0.0f, 0.0f);

		DrawHalfDisc(position, v, direction, radius, color);

		v = float3(0.0f, 1.0f, 0.0f);

		DrawHalfDisc(position, v, direction, radius, color);

		v = float3(0.0f, 0.0f, 1.0f);

		DrawHalfDisc(position, v, direction, radius, color);
	}

	void DebugDrawer::DrawHalfDisc(const float3& position, float3& n, const float3& direction, const float& radius, const float4& color)
	{
		float fullCircle = PI;
		float slice = PI / 40.0f;

		float3 u, v;

		u = n.Cross(direction);
		u.Normalize();

		v = u.Cross(n);
		v.Normalize();

		float3 w = u * std::cosf(0) + v * std::sinf(0);
		float3 p = position + w * radius;
		Push(p, color);

		for (float i = slice; i <= fullCircle; i += slice)
		{
			w = u * std::cos(i) + v * std::sin(i);
			p = position + w * radius;
			Push(p, color);
		}
	}

	void DebugDrawer::DrawHalfHorizonDisc(const float3& position, const float3& direction, const float& radius, const float4& color)
	{
		float3 cameraPosition = m_EditorCamera->GetPosition();
		float3 cameraToShphere = position - cameraPosition;

		float d = cameraToShphere.Length();
		float l = std::sqrt((d * d) - (radius * radius));
		float h = l * (radius / d);
		float z = std::sqrt((radius * radius) - (h * h));

		cameraToShphere.Normalize();

		float3 centerOffset = position - cameraToShphere * z;

		DrawHalfDisc(centerOffset, cameraToShphere, direction, h, color);
	}

	void DebugDrawer::DrawBox(const float4& scale, const Core::Transform* transform, const float4& color)
	{
		float4 position = transform->GetTranslation();

		float3 halfScale(scale.x, scale.y, scale.z);

		halfScale /= 2;

		float3 pos = position.xyz;

		float4x4 rotation = transform->GetRotationMatrix();

		float3 points[8];

		//TOP VERTICES
		points[0] = float3(halfScale.x, halfScale.y, halfScale.z);
		points[1] = float3(halfScale.x, halfScale.y, -halfScale.z);
		points[2] = float3(-halfScale.x, halfScale.y, halfScale.z);
		points[3] = float3(-halfScale.x, halfScale.y, -halfScale.z);

		//BOTTOM VERTICES
		points[4] = float3(halfScale.x, -halfScale.y, halfScale.z);
		points[5] = float3(halfScale.x, -halfScale.y, -halfScale.z);
		points[6] = float3(-halfScale.x, -halfScale.y, halfScale.z);
		points[7] = float3(-halfScale.x, -halfScale.y, -halfScale.z);

		//ROTATE POINTS
		for (int i = 0; i < 8; ++i)
		{
			points[i] = rotation * points[i];
		}

		//TRANSLATE POINTS
		for (int i = 0; i < 8; ++i)
		{
			points[i] = points[i] + pos;
		}

		//TOP FACE
		DrawLine(points[0], points[1], color);
		DrawLine(points[1], points[3], color);
		DrawLine(points[2], points[3], color);
		DrawLine(points[2], points[0], color);	

		//BOTTOM FACE
		DrawLine(points[4], points[5], color);
		DrawLine(points[5], points[7], color);
		DrawLine(points[6], points[7], color);
		DrawLine(points[6], points[4], color);

		//BOTTOM TO TOP CONNECTIONS
		DrawLine(points[0], points[4], color);
		DrawLine(points[1], points[5], color);
		DrawLine(points[2], points[6], color);
		DrawLine(points[3], points[7], color);
	}

	void DebugDrawer::DrawLine(const float4& pos1, const float4& pos2, const float4& color)
	{
		DrawLine(float3(pos1.x, pos1.y, pos1.z), float3(pos2.x, pos2.y, pos2.z), color);
	}

	void DebugDrawer::Push(const float3& vertex, const float4& color)
	{
		//If there is room in the buffer, add the vertex
		if (m_Size < m_Capacity)
		{
			//Add the vertex at the end of the buffer
			m_VertexList[m_Size++] = LineVertex(float4(vertex.x, vertex.y, vertex.z, 1.0f), color);
		}

	}

	void  DebugDrawer::Axis(const float4& pos, const float scale)
	{
		//Draw X axis
		DrawLine(pos, float4(pos.x + scale, pos.y, pos.z, pos.w), Color::s_Red);

		//Draw Y axis
		DrawLine(pos, float4(pos.x, pos.y + scale, pos.z, pos.w), Color::s_Green);

		//Draw Z axis
		DrawLine(pos, float4(pos.x, pos.y, pos.z + scale, pos.w), Color::s_Blue);

	}
	void  DebugDrawer::DrawTranslateGizmo(const float4& pos, const float scale)
	{
		//Draw X axis
		DrawLine(pos, float4(pos.x + scale, pos.y, pos.z, pos.w), Color::s_Red);

		//Draw Y axis
		DrawLine(pos, float4(pos.x, pos.y + scale, pos.z, pos.w), Color::s_Green);

		//Draw Z axis
		DrawLine(pos, float4(pos.x, pos.y, pos.z + scale, pos.w), Color::s_Blue);
	}
	void  DebugDrawer::DrawRotateGizmo(const float4& pos, const float scale)
	{

	}
	void  DebugDrawer::DrawScaleGizmo(const float4& pos, const float scale)
	{
		//Draw X axis
		DrawLine(pos, float4(pos.x + scale, pos.y, pos.z, pos.w), Color::s_Red);

		//Draw Y axis
		DrawLine(pos, float4(pos.x, pos.y + scale, pos.z, pos.w), Color::s_Green);

		//Draw Z axis
		DrawLine(pos, float4(pos.x, pos.y, pos.z + scale, pos.w), Color::s_Blue);
	}
}