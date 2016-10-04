/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Renderable.h 
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


class Renderable : public Core::GameComponent
{
	
	public:
		enum RenderableType
		{
			RENDERABLE_3D,
			RENDERABLE_POINT_LIGHT,
			RENDERABLE_DIRECTIONAL_LIGHT,
			RENDERABLE_SPOT_LIGHT,
			RENDERABLE_MAX,
		};

		struct WorldMatrix
		{
			float4x4 m_WorldMatrix;
		};

		Renderable(void);
		~Renderable(void);

		////Base Methods
		virtual void Initialize() override;
		virtual void Remove(void) override;
	
		void SetModel(const std::string& modelName);
		void SetTexture(const std::string& textureName);
		void SetNormalTexture(const std::string& textureName);
		void SetEmissiveTexture(const std::string& textureName);
		void SetBlendMode(const Skye::BlendMode& blendMode);
		void SetVisibility(const bool& visible);
		void ToggleVisibility(void);
		const bool& IsVisible(void);

		const std::string& GetModelName(void) const;
		const std::string& GetTextureName(void) const;
		const std::string& GetNormalTextureName(void) const;
		const std::string& GetEmissiveTextureName(void) const;
		const Skye::BlendMode& GetBlendMode(void) const;

	  Core::Transform* GetTransform(void);

    COMP_LUA_USABLE;

	  Base::Handle m_Transform;
	  std::string m_ModelName;
	  std::string m_TextureName;
	  std::string m_NormalTexture;
	  std::string m_EmissiveTexture;

	  WorldMatrix m_WorldMatrix;

	  bool m_IsVisible;

	protected:
	
    
	  Skye::BlendMode m_BlendMode;
};