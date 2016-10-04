/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: PointLight.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Renderable.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

//#include <includes\SkyeDX\DXTypes.h>
#include "Graphics/includes/SkyeDX/DXTypes.h"
#include <Core\components\base\Component.h>
#include <Core\components\transform\CTransform.h>


class PointLight : public Core::GameComponent
{
	
	public:
		PointLight(void);
		~PointLight(void);

		virtual void Initialize() override;

		const float4& GetLightColor(void) const;
		const float2& GetRangeAttenuation(void) const;
		Core::Transform* GetTransform(void);

		void SetLightColor(const float4& color);
		void SetRangeAttenuation(const float2& rangeAttenuation);
		void SetOffset(const float& offset);
		void SetDeathOffset(const float2& deathOffset);

		const float& GetOffset(void);
		const float2& GetDeathOffset(void);

    COMP_LUA_USABLE;

	public:

		float4 m_LightColor;
		float2 m_RangeAttenuation;

  private:
	  Base::Handle m_Transform;
	  float m_offset;
	  float2 m_deathOffset;
};