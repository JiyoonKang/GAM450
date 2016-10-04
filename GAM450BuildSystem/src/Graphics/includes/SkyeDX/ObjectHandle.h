/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: ObjectHandle.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  ObjectHandle.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <Graphics\includes\SkyeDX\DXTypes.h>

namespace Skye
{
	class ObjectHandle
	{
	public:
		ObjectHandle(void);

		ObjectType GetType(void) const;

		//Conversion operator
		operator bool(void) const;

		//Operator overloads
		const int operator*(void) const;
		bool operator==(const ObjectHandle& rhs) const;
		bool operator!=(const ObjectHandle& rhs) const;

		//Returning a null handle
		static const ObjectHandle& Null(void);

		//Setters
		void SetType(ObjectType type);
		void SetHandle(int handle);		

	private:
		ObjectType m_Type;
		int m_Handle;

		static ObjectHandle m_NullHandle;
		friend class Renderer;
	};
}