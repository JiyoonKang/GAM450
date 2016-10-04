/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Camera.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Camera.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
//#include <includes\SkyeDX\Basis.h> //Includes VectorMath.h
#include "Graphics/includes/SkyeDX/Basis.h"
#include <Base\util\math\Float4x4.h>
//#include <includes\SkyeDX\Frustum.h>
#include "Graphics/includes/SkyeDX/Frustum.h"

namespace Skye
{
	class Area;

	class Camera
	{
		public:
			Camera(void);
			~Camera(void);

			/*void Resize(float fov,
				float aspectratio,
				float near,
				float far);

			float4x4 GetView();
			float4x4 GetProjection();

			void Strafe(float dt);
			void Walk(float dt);
			void Elevate(float dt);
			void Pitch(float angle);
			void RotateY(float angle);
			void Update(void);
			void ResetAxis(void);

			float GetFar();
			float GetNear();
			void SetMovementSpeed(float speed);
			void SetPosition(float3& pos);
			void SetLookAt(float3& lookAt);
			float3 GetPosition() const;
			float3 GetLookAt(void) const;
			float3 GetUp(void) const;
			float& GetSpeed(void);
			float GetAspectRatio(void) const;
			float GetFOV(void) const;*/

			/*===============================================================================*/
			/*	SETTERS
			/*===============================================================================*/
			void SetPosition(const float4 & position);
			void SetPosition(const float3& position);
			void SetPosition(float x, float y, float z);

			void SetViewMatrix(const float4x4& viewMatrix);
			void SetViewMatrix(void);

			void SetProjectionMatrix(const float4x4& projectionMatrix);
			void SetProjectionMatrix(float fovY, float aspectRatio, float n, float f);
			void SetProjectionMatrix(const Area& size, float n, float f);
			void SetOrthographicProjection(float left, float right, float zN, float zF);

			void SetLookAt(float4 & lookAt);
			void SetUp(float4& up);
			void SetWidth(float width);
			void SetHeight(float height);
			void SetNear(float nearPlane);
			void SetFar(float farPlane);
			void SetFOV(float FOV);

			/*===============================================================================*/
			/*	GETTERS
			/*===============================================================================*/
			float GetFOV(void);
			float GetAspectRatio(void);
			float GetNear(void);
			float GetFar(void);
			float4 GetLookAt(void);
			float4 GetUp(void);
			float3 GetPosition(void);
			const float4x4& GetViewMatrix(void) const;
			const float4x4& GetProjectionMatrix(void) const;
			Frustum GetFrustum(void);

			/*===============================================================================*/
			/*	UTILITY
			/*===============================================================================*/
			void Update(void);
			float4x4 WorldToCamera(void);
			void RotateAlongX(float angle);
			void RotateAlongY(float angle);
			void RotateAlongZ(float angle);
			void MoveAlongX(float offset);
			void MoveAlongY(float offset);
			void MoveAlongZ(float offset);
			void ResetBasis(void);
			void StabilizeCamera(void);
			void Translate(float x, float y, float z);

		private:
			float4x4 m_ViewMatrix;
			float4x4 m_ProjectionMatrix;
			float4 m_Position;
			float4 m_LookAt;
			float4 m_Up;
			Basis m_Basis;
			Frustum m_Frustum;
			float m_FOV;
			float m_Width;
			float m_Height;
			float m_Near;
			float m_Far;

			/*float3 m_Position;
			float3 m_LookAt;
			float3 m_Up;
			float3 m_Right;

			float m_NearWindowHeight;
			float m_FarWindowHeight;
			float m_AspectRatio;
			float m_Near;
			float m_Far;
			float m_FOV;
			float m_Speed;

			float4x4 m_View;
			float4x4 m_Projection;
*/
		};
}