/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Frustum.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Frustum.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <Base\util\math\float4x4.h>
#include <Base\util\math\VectorMath.h>
//#include <includes\SkyeDX\DXTypes.h>
#include "Graphics/includes/SkyeDX/DXTypes.h"

namespace Skye
{
	class Camera;

	class Frustum
	{
		public:

			const float4& GetPlane(const FrustumPlane plane) const;
			void Update(const float4x4& planeMatrix, Camera* camera);

			float3* GetCorners(void);

		private:
			void GenerateCorners(Camera* camera);

			float4 m_Planes[6];
			float3 m_Corners[8];


	};
}