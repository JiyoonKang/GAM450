/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Basis.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Basis.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <Base\util\math\VectorMath.h>

namespace Skye
{
	class Basis
	{
	public:
		Basis(void);
		Basis(Basis& basis);
		float4& GetAxisX(void);
		float4& GetAxisY(void);
		float4& GetAxisZ(void);
	
		void SetAxisX(float4 axis);
		void SetAxisY(float4 axis);
		void SetAxisZ(float4 axis);
	
	private:
		float4 m_AxisX;
		float4 m_AxisY;
		float4 m_AxisZ;
	
	};
}