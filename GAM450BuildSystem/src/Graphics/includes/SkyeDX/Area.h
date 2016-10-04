/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Area.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Area.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

namespace Skye
{

	class Area
	{
		public:
			Area(void);
			Area(const unsigned width, const unsigned height);
			~Area(void);

			bool operator==(const Area& rhs) const;
			bool operator!=(const Area& rhs) const;

      //Returning a null handle
      static const Area& Null(void);

			unsigned width, height;

		private:
      static Area m_NullArea;
	};
}