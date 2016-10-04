/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: ObjectHandle.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  ObjectHandle.cpp
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch/precompiled.h"
#include <includes\SkyeDX\ObjectHandle.h>

namespace Skye
{
	ObjectHandle ObjectHandle::m_NullHandle;

	ObjectHandle::ObjectHandle(void) : m_Type(ObjectType::NONE), m_Handle(-1)
	{

	}

	ObjectType ObjectHandle::GetType(void) const
	{
		return m_Type;
	}

	ObjectHandle::operator bool(void) const
	{
		return (m_Type != ObjectType::NONE && m_Handle != -1);
	}

	const int ObjectHandle::operator*(void) const
	{
		return m_Handle;
	}

	bool ObjectHandle::operator==(const ObjectHandle& rhs) const
	{
		return (m_Type == rhs.m_Type && m_Handle == rhs.m_Handle);
	}

	bool ObjectHandle::operator!=(const ObjectHandle& rhs) const
	{
		return (m_Type != rhs.m_Type || m_Handle != rhs.m_Handle);
	}

	const ObjectHandle& ObjectHandle::Null(void)
	{
		return m_NullHandle;
	}

	void ObjectHandle::SetType(ObjectType type)
	{
		m_Type = type;
	}

	void ObjectHandle::SetHandle(int handle)
	{
		m_Handle = handle;
	}
}