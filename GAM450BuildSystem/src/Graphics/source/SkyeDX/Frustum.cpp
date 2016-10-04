/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Frustum.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Frustum.cpp
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch\precompiled.h"
#include <Base\util\math\Float4.h>
//#include <includes\SkyeDX\Frustum.h>
#include <includes\SkyeDX\Camera.h>

namespace Skye
{
	const float4& Frustum::GetPlane(const FrustumPlane plane) const
	{
		return m_Planes[plane];
	}

	void Frustum::Update(const float4x4& planeMatrix, Camera* camera)
	{
		float4 r0(planeMatrix.m00, planeMatrix.m01, planeMatrix.m02, planeMatrix.m03);
		float4 r1(planeMatrix.m10, planeMatrix.m11, planeMatrix.m12, planeMatrix.m13);
		float4 r2(planeMatrix.m20, planeMatrix.m21, planeMatrix.m22, planeMatrix.m23);
		float4 r3(planeMatrix.m30, planeMatrix.m31, planeMatrix.m32, planeMatrix.m33);

		m_Planes[FrustumPlane::NEAR_PLANE]    = -r2 - r3;
		m_Planes[FrustumPlane::FAR_PLANE]     =  r2 - r3;
		m_Planes[FrustumPlane::LEFT_PLANE]    = -r0 - r3;
		m_Planes[FrustumPlane::RIGHT_PLANE]   =  r0 - r3;
		m_Planes[FrustumPlane::TOP_PLANE]     =  r1 - r3;
		m_Planes[FrustumPlane::BOTTOM_PLANE]  = -r1 - r3;

		GenerateCorners(camera);
	}

	float3* Frustum::GetCorners()
	{
		return m_Corners;
	}

	void Frustum::GenerateCorners(Camera* camera)
	{
		float3 position = camera->GetPosition().xyz;
		float3 lookAt = camera->GetLookAt().xyz;
		float3 up = camera->GetUp().xyz;
		float3 w = lookAt.Cross(up);

		float near = camera->GetNear();
		float far = camera->GetFar();
		float aspectRatio = camera->GetAspectRatio();
		float fov = camera->GetFOV();

		float hNear, wNear, hFar, wFar;

		hNear = 2 * tan(fov / 2) * near;
		wNear = hNear * aspectRatio;

		hFar = 2 * tan(fov / 2) * far;
		wFar = hFar * aspectRatio;

		float3 nearCenter = position + lookAt * near;
		float3 farCenter = position + lookAt * far;

		m_Corners[0] = nearCenter + (up * (hNear / 2)) - (w * (wNear / 2));
		m_Corners[1] = nearCenter + (up * (hNear / 2)) + (w * (wNear / 2));
		m_Corners[2] = nearCenter - (up * (hNear / 2)) - (w * (wNear / 2));
		m_Corners[3] = nearCenter - (up * (hNear / 2)) + (w * (wNear / 2));
		m_Corners[4] = farCenter + (up * (hFar / 2)) - (w * (wFar / 2));
		m_Corners[5] = farCenter + (up * (hFar / 2)) + (w * (wFar / 2));
		m_Corners[6] = farCenter - (up * (hFar / 2)) - (w * (wFar / 2));
		m_Corners[7] = farCenter - (up * (hFar / 2)) + (w * (wFar / 2));
	}
}