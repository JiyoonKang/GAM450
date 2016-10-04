/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: InputData.h 
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

#include <string>
#include <vector>


namespace Skye
{
	enum DataFormat;

	class InputData
	{
		public:
			InputData(const std::string& semanticName, const DataFormat& format, const bool instanceData = false);

			std::string semanticName;
			DataFormat format;
			bool instanceData;

		private:
			InputData(void);

	};

	typedef std::vector<InputData> InputLayout;
}