/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Viewport.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Viewport.cpp
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "pch\precompiled.h"
#include <includes\SkyeDX\Viewport.h>
namespace Skye
{
	Viewport::Viewport(void) : x(0), y(0), minZ(0.0f), maxZ(1.0f), size(0,0)
	{
		
	}

	Viewport::Viewport(int xx, int yy, const Area& Size, float zMin, float zMax) : x(xx), y(yy), minZ(zMin), maxZ(zMax), size(Size)
	{
		
	}

	void Viewport::SetViewport(int xx, int yy, const Area& Size, float zMin, float zMax)
	{
		x = xx;
		y = yy;
		minZ = zMin;
		maxZ = zMax;
		size = Size;
	}
}