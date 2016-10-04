/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Viewport.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Viewport.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <Graphics\includes\SkyeDX\Area.h>
#include <Base\util\math\float4x4.h>

namespace Skye
{
	class Viewport
	{
		public:
			Viewport(void);
			Viewport(int x, int y, const Area& size, float minZ = 0.0f, float maxZ = 1.0f);
			void SetViewport(int x, int y, const Area& size, float minZ = 0.0f, float maxZ = 1.0f);

			int x, y;
			float minZ, maxZ;

			Area size;
			float4x4 matrix;

	};
}