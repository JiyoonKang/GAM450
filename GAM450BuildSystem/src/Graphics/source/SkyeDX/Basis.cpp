/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Basis.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Basis.cpp
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch\precompiled.h"
#include <includes\SkyeDX\Basis.h>

namespace Skye
{
	Basis::Basis(void) : m_AxisX(1, 0, 0, 0), m_AxisY(0, 1, 0, 0), m_AxisZ(0, 0, 1, 0)
	{
	}

	Basis::Basis(Basis& basis)
	{
		m_AxisX = basis.GetAxisX();
		m_AxisY = basis.GetAxisY();
		m_AxisZ = basis.GetAxisZ();
	}

	float4& Basis::GetAxisX(void)
	{
		return m_AxisX;
	}

	float4& Basis::GetAxisY(void)
	{
		return m_AxisY;
	}

	float4& Basis::GetAxisZ(void)
	{
		return m_AxisZ;
	}

	void Basis::SetAxisX(float4 axis)
	{
		m_AxisX = axis;
	}

	void Basis::SetAxisY(float4 axis)
	{
		m_AxisY = axis;
	}

	void Basis::SetAxisZ(float4 axis)
	{
		m_AxisZ = axis;
	}
}