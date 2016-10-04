/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Camera.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Camera.pp
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "pch\precompiled.h"
#include <includes\SkyeDX\Camera.h>
#include <includes\SkyeDX\Area.h>
#include <complex>

#define _XM_NO_INTRINSICS_
#include <DirectXMath.h>

namespace Skye
{
	//Camera::Camera() : m_Position(0.0f, 3.0f, -10.0f),
	//	m_LookAt(0.0f, 0.0f, 1.0f),
	//	m_Up(0.0f, 1.0f, 0.0f),
	//	m_Right(1.0f, 0.0f, 0.0f),
	//	m_NearWindowHeight(0.0f),
	//	m_FarWindowHeight(0.0f),
	//	m_AspectRatio(0.0f),
	//	m_Near(0.0f),
	//	m_Far(0.0f),
	//	m_FOV(0.0f),
	//	m_Speed(1.0f)
	//{
	//	m_View.Identity();
	//	m_Projection.Identity();
	//}

	//Camera::~Camera()
	//{
	//}

	//void Camera::Resize(float fov, float aspectratio, float n, float f)
	//{
	//	m_AspectRatio = aspectratio;
	//	m_Near = n;
	//	m_Far = f;
	//	m_FOV = fov;

	//	m_NearWindowHeight = 2.0f * m_Near * tanf(0.5f * m_FOV);
	//	m_FarWindowHeight = 2.0f * m_Far * tanf(0.5f * m_FOV);

	//	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, m_Near, m_Far);
	//	//m_Projection = float4x4(&(P.m[0][0]));
	//}

	//float4x4 Camera::GetView()
	//{
	//	return m_View;
	//}

	//float4x4 Camera::GetProjection()
	//{
	//	return m_Projection;
	//}

	//void Camera::Strafe(float dt)
	//{
	//	m_Position += m_Right * dt * m_Speed;
	//}

	//void Camera::Walk(float dt)
	//{
	//	m_Position += m_LookAt * dt * m_Speed;
	//}

	//void Camera::Elevate(float dt)
	//{
	//	m_Position += m_Up * dt * m_Speed;
	//}

	//void Camera::Pitch(float angle)
	//{
	//	float4x4 rotation;
	//	rotation.Rotate(m_Right, angle);
	//	//m_Up = rotation.TransformNormal(rotation, m_Up);
	//	//m_LookAt = rotation.TransformNormal(rotation, m_LookAt);
	//}

	//void Camera::RotateY(float angle)
	//{
	//	float4x4 rotation;
	//	rotation.Rotate(float3::cYAxis, angle);
	//	//m_Right = rotation.TransformNormal(rotation, m_Right);
	//	//m_Up = rotation.TransformNormal(rotation, m_Up);
	//	//m_LookAt = rotation.TransformNormal(rotation, m_LookAt);
	//}

	//void Camera::Update()
	//{
	//	// Keep camera's axes orthogonal to each other and of unit length.
	//	m_LookAt.Normalize();
	//	m_Up = m_LookAt.Cross(m_Right).Normalize();

	//	// U, L already ortho-normal, so no need to normalize cross product.
	//	m_Right = (m_Up.Cross(m_LookAt));

	//	// Fill in the view matrix entries.
	//	float x = -m_Position.Dot(m_Right);
	//	float y = -m_Position.Dot(m_Up);
	//	float z = -m_Position.Dot(m_LookAt);

	//	m_View.m[0][0] = m_Right.x;
	//	m_View.m[1][0] = m_Right.y;
	//	m_View.m[2][0] = m_Right.z;
	//	m_View.m[3][0] = x;

	//	m_View.m[0][1] = m_Up.x;
	//	m_View.m[1][1] = m_Up.y;
	//	m_View.m[2][1] = m_Up.z;
	//	m_View.m[3][1] = y;

	//	m_View.m[0][2] = m_LookAt.x;
	//	m_View.m[1][2] = m_LookAt.y;
	//	m_View.m[2][2] = m_LookAt.z;
	//	m_View.m[3][2] = z;

	//	m_View.m[0][3] = 0.0f;
	//	m_View.m[1][3] = 0.0f;
	//	m_View.m[2][3] = 0.0f;
	//	m_View.m[3][3] = 1.0f;
	//}

	//void Camera::ResetAxis()
	//{
	//	m_LookAt = float3(0, 0, 1);
	//	m_Up = float3(0, 1, 0);
	//	m_Right = float3(1, 0, 0);
	//}

	//float Camera::GetFar()
	//{
	//	return m_Far;
	//}

	//float Camera::GetNear()
	//{
	//	return m_Near;
	//}

	//void Camera::SetMovementSpeed(float speed)
	//{
	//	m_Speed = speed;
	//}

	//void Camera::SetPosition(float3& pos)
	//{
	//	m_Position = pos;
	//}

	//void Camera::SetLookAt(float3& lookAt)
	//{
	//	m_LookAt = lookAt;
	//	m_LookAt.Normalize();
	//}

	//float3 Camera::GetPosition() const
	//{
	//	return m_Position;
	//}

	//float3 Camera::GetLookAt(void) const
	//{
	//	return m_LookAt;
	//}
	//float3 Camera::GetUp(void) const
	//{
	//	return m_Up;
	//}

	//float& Camera::GetSpeed()
	//{
	//	return  m_Speed;
	//}

	//float Camera::GetAspectRatio(void) const
	//{
	//	return  m_AspectRatio;
	//}

	//float Camera::GetFOV(void) const
	//{
	//	return m_FOV;
	//}


	Camera::Camera() : m_Position(float4()), m_FOV(0.0f), m_Width(0.0f), m_Height(0.0f), m_Near(0.0f), m_Far(0.0f)
	{
		m_LookAt = m_Basis.GetAxisZ();
		m_Up = m_Basis.GetAxisY();
		SetViewMatrix();
	}

	Camera::~Camera()
	{
	}

	void Camera::SetPosition(const float4& position)
	{
		m_Position = position;
		SetViewMatrix();
	}

	void Camera::SetPosition(const float3& position)
	{
		m_Position = float4(position, 0);
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		m_Position.x = x;
		m_Position.y = y;
		m_Position.z = z;
		SetViewMatrix();
	}

	void Camera::SetViewMatrix(const float4x4& viewMatrix)
	{
		m_ViewMatrix = viewMatrix;

		m_Frustum.Update((m_ProjectionMatrix * m_ViewMatrix), this);
	}

	void Camera::SetViewMatrix(void)
	{
		float3 xAxis = m_Basis.GetAxisX().xyz;
		float3 yAxis = m_Basis.GetAxisY().xyz;
		float3 zAxis = m_Basis.GetAxisZ().xyz;
		
		/*m_ViewMatrix = float4x4(xAxis.x, xAxis.y, xAxis.z,  -xAxis.Dot(m_Position.xyz),
								  yAxis.x, yAxis.y, yAxis.z,  -yAxis.Dot(m_Position.xyz),
								  zAxis.x, zAxis.y, zAxis.z,  -zAxis.Dot(m_Position.xyz),
								   0.0f,    0.0f,    0.0f,                      1.0f);*/
		
		//m_ViewMatrix = float4x4(xAxis.x, yAxis.x, zAxis.x, 0.0f,
		//						xAxis.y, yAxis.y, zAxis.y, 0.0f,
		//						xAxis.z, yAxis.z, zAxis.z, 0.0f,
		//						-xAxis.Dot(m_Position.xyz), -yAxis.Dot(m_Position.xyz), -zAxis.Dot(m_Position.xyz), 1.0f);

		m_ViewMatrix = WorldToCamera();

		m_Frustum.Update((m_ProjectionMatrix * m_ViewMatrix), this);
	}

	void Camera::SetProjectionMatrix(const float4x4& projectionMatrix)
	{
		m_ProjectionMatrix = projectionMatrix;

		m_Frustum.Update((m_ProjectionMatrix * m_ViewMatrix), this);
	}

	void Camera::SetProjectionMatrix(float fovY, float aspectRatio, float n, float f)
	{
		//float fovy2 = fovY / 2.0f;
		//
		m_ProjectionMatrix.Identity();
		//
		//m_ProjectionMatrix.m00 = 1.0f / (aspectRatio * std::tan(fovy2));
		//m_ProjectionMatrix.m11 = 1.0f / std::tan(fovy2);
		//m_ProjectionMatrix.m22 = f / (n - f);
		//m_ProjectionMatrix.m23 = (f * n) / (n - f);
		//m_ProjectionMatrix.m32 = -1;
		//m_ProjectionMatrix.m33 = 0.0f;
		//
		//

		float yScale = 1 / std::tan(fovY / 2.0f);
		float xScale = yScale / aspectRatio;

		m_ProjectionMatrix.m00 = xScale;
		m_ProjectionMatrix.m11 = yScale;
		m_ProjectionMatrix.m22 = f / (n - f);
		m_ProjectionMatrix.m23 = -1;
		m_ProjectionMatrix.m32 = (f * n) / (n - f);
		m_ProjectionMatrix.m33 = 0.0f;

		m_Frustum.Update((m_ProjectionMatrix * m_ViewMatrix), this);
	}

	void Camera::SetProjectionMatrix(const Area& size, float n, float f)
	{
		m_ProjectionMatrix.m00 = 2.0f / static_cast<float>(size.width);
		m_ProjectionMatrix.m11 = 2.0f / static_cast<float>(size.height);
		m_ProjectionMatrix.m22 = 1.0f / (n - f);
		m_ProjectionMatrix.m33 = n / (n - f);

		m_Frustum.Update(m_ProjectionMatrix * m_ViewMatrix, this);
	}

	void Camera::SetLookAt(float4& lookAt)
	{
		m_LookAt = lookAt;
		m_LookAt.Normalize();

		float4 right = float4(0, 1, 0, 0).Cross(m_LookAt);
		right.w = 0;
		right.Normalize();

		m_Up = m_LookAt.Cross(right);
		m_Up.w = 0;
		m_Up.Normalize();

		m_Basis.SetAxisX(right);
		m_Basis.SetAxisY(m_Up);
		m_Basis.SetAxisZ(m_LookAt);
	}

	void Camera::SetUp(float4& up)
	{
		m_Up = up;
		m_Up.Normalize();
		m_Basis.SetAxisY(m_Up);
	}

	void Camera::SetWidth(float width)
	{
		m_Width = width;
	}

	void Camera::SetHeight(float height)
	{
		m_Height = height;
	}

	void Camera::SetNear(float nearPlane)
	{
		m_Near = nearPlane;
	}

	void Camera::SetFar(float farPlane)
	{
		m_Far = farPlane;
	}

	void Camera::SetFOV(float FOV)
	{
		m_FOV = FOV;
	}

	float Camera::GetFOV()
	{
		return m_FOV;
	}

	float Camera::GetAspectRatio()
	{
		return m_Width / m_Height;
	}

	float Camera::GetNear()
	{
		return m_Near;
	}

	float Camera::GetFar()
	{
		return m_Far;
	}

	float4 Camera::GetLookAt()
	{
		return m_LookAt;
	}

	float4 Camera::GetUp()
	{
		return m_Up;
	}

	float3 Camera::GetPosition()
	{
		return m_Position.xyz;
	}

	const float4x4& Camera::GetViewMatrix() const
	{
		return m_ViewMatrix;
	}

	const float4x4& Camera::GetProjectionMatrix() const
	{
		return m_ProjectionMatrix;
	}

	Frustum Camera::GetFrustum()
	{
		return m_Frustum;
	}

	void Camera::Update()
	{
		m_LookAt = m_Basis.GetAxisZ();
		m_Up = m_Basis.GetAxisY();
		SetViewMatrix();
	}

	float4x4 Camera::WorldToCamera()
	{
		float4x4 translate, align;

		translate.Identity();
		align.Identity();

		float4 cross = m_Up.Cross(m_LookAt);

		translate.m[0][3] = -m_Position.x;
		translate.m[1][3] = -m_Position.y;
		translate.m[2][3] = -m_Position.z;

		align.m[0][0] = cross.x;
		align.m[0][1] = cross.y;
		align.m[0][2] = cross.z;

		align.m[1][0] = m_Up.x;
		align.m[1][1] = m_Up.y;
		align.m[1][2] = m_Up.z;

		align.m[2][0] = -m_LookAt.x;
		align.m[2][1] = -m_LookAt.y;
		align.m[2][2] = -m_LookAt.z;

		//m_ViewMatrix = Transpose(align * translate);

		return Transpose(align * translate);
	}

	void Camera::RotateAlongX(float angle)
	{
		RotateVectorByAxis(m_Basis.GetAxisY(), m_Basis.GetAxisZ(), m_Basis.GetAxisX(), angle);
	}

	void Camera::RotateAlongY(float angle)
	{
		RotateVectorByAxis(m_Basis.GetAxisX(), m_Basis.GetAxisZ(), m_Basis.GetAxisY(), angle);
	}

	void Camera::RotateAlongZ(float angle)
	{
		RotateVectorByAxis(m_Basis.GetAxisX(), m_Basis.GetAxisY(), m_Basis.GetAxisZ(), angle);
	}

	void Camera::MoveAlongX(float offset)
	{
		m_Position += m_Basis.GetAxisX() * offset;
	}

	void Camera::MoveAlongY(float offset)
	{
		m_Position += m_Basis.GetAxisY() * offset;
	}

	void Camera::MoveAlongZ(float offset)
	{
		m_Position += m_Basis.GetAxisZ() * offset;
	}

	void Camera::ResetBasis()
	{
		Basis reset;
		m_Basis = reset;
	}

	void Camera::StabilizeCamera()
	{
		float4 right = float4(0, 1, 0, 0).Cross(m_LookAt);
		right.w = 0;
		right.Normalize();

		m_Up = m_LookAt.Cross(right);

		m_Up.w = 0;

		m_Up.Normalize();

		m_Basis.SetAxisX(right);
		m_Basis.SetAxisY(m_Up);
		m_Basis.SetAxisZ(m_LookAt);
	}

	void Camera::Translate(float x, float y, float z)
	{
		m_Position.x += x;
		m_Position.y += y;
		m_Position.z += z;
	}
}