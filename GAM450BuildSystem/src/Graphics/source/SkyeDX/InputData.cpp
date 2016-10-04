/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: InputData.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  InputData.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "pch/precompiled.h"
#include <includes\SkyeDX\InputData.h>
#include <includes\SkyeDX\DXTypes.h>

namespace Skye
{
	InputData::InputData(const std::string& semanticName_, const DataFormat& format_, const bool instanceData_)
		: semanticName(semanticName_), format(format_), instanceData(instanceData_)
	{
	}
	
	InputData::InputData(void)
	{
	
	}
}