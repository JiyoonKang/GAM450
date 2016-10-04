/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Area.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Area.cpp
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch\precompiled.h"
#include <includes\SkyeDX\Area.h>

namespace Skye
{
  Area Area::m_NullArea;

	Area::Area(void) : width(0), height(0)
	{

	}

	Area::Area(const unsigned width_, const unsigned height_) : width(width_), height(height_)
	{

	}

	Area::~Area(void)
	{

	}

	bool Area::operator==(const Area& rhs) const
	{
		return (width == rhs.width && height == rhs.height);
	}

	bool Area::operator!=(const Area& rhs) const
	{
		return (width != rhs.width || height != rhs.height);
	}

  const Area& Area::Null(void)
  {
    return m_NullArea;
  }
}