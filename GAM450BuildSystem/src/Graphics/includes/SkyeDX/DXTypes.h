/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: DXTypes.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  DXTypes.h
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
	enum ObjectType
	{
		NONE = -1,
		BACK_BUFFER,
		DEPTH_BUFFER,
		TEXTURE,
		RENDER_TARGET,
		SHADER_RESOURCE_VIEW,
		VERTEX_SHADER,
		PIXEL_SHADER,
		GEOMETRY_SHADER,
		COMPUTE_SHADER,
		VERTEX_BUFFER,
		INDEX_BUFFER,
		CONSTANT_BUFFER,
    FONT_FACTORY,
    FONT_WRAPPER
	};

	enum DataFormat
	{
		R_UNORM = 0,
		RG_UNORM,
		//RGB_UNORM,
		RGBA_UNORM,
		BGRA_UNORM,
		FLOAT4,
		FLOAT3,
		FLOAT2,
		FLOAT1,
		UNSIGNED1,
		UNSIGNED2,
		UNSIGNED3,
		UNSIGNED4,
		INT1,
		INT2,
		INT3,
		INT4,

	};

	enum FeatureLevel
	{
		FEATURE_LEVEL_DX9_1 = 0,
		FEATURE_LEVEL_DX9_2,
		FEATURE_LEVEL_DX9_3,
		FEATURE_LEVEL_DX10_0,
		FEATURE_LEVEL_DX10_1,
		FEATURE_LEVEL_DX11_0,
		FEATURE_LEVEL_DX11_1,
		FEATURE_LEVEL_DX12_0,
		FEATURE_LEVEL_DX12_1
	};

	enum PrimitiveTopology
	{
		PRIMITIVE_TOPOLOGY_UNDEFINED = 0,
		PRIMITIVE_TOPOLOGY_POINTLIST = 1,
		PRIMITIVE_TOPOLOGY_LINELIST = 2,
		PRIMITIVE_TOPOLOGY_LINESTRIP = 3,
		PRIMITIVE_TOPOLOGY_TRIANGLELIST = 4,
		PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = 5,
	};

	enum BufferUsage
	{
		USAGE_DEFAULT = 0,
		USAGE_IMMUTABLE,
		USAGE_DYNAMIC,
		USAGE_STAGING,
	};

	enum FillMode
	{
		FILL_SOLID = 0,
		FILL_WIREFRAME,
	};

	enum CullMode
	{
		CULL_MODE_NONE = 0,
		CULL_MODE_FRONT_FACE,
		CULL_MODE_BACK_FACE,
		CULL_MODE_SCISSOR,
		CULL_MODE_IMGUI,
	};

	enum BlendMode
	{
		BLEND_MODE_NOT_SET = -1,
		BLEND_MODE_NONE,
		BLEND_MODE_ALPHA,
		BLEND_MODE_ADDITIVE,
		BLEND_MODE_ADDITIVE_RENDER_TARGET,
	};

	enum TextureFilter
	{
		FILTER_POINT = 0,
		FILTER_BILINEAR,
		FILTER_TRILINEAR,
		FILTER_ANISOTROPIC,
	};

	enum TextureAdressing
	{
		CLAMP = 0,
		MIRROR,
		WRAP,
	};

	enum Anisotropy
	{
		ANISOTROPY_1X = 0,
		ANISOTROPY_2X,
		ANISOTROPY_4X,
		ANISOTROPY_8X,
		ANISOTROPY_16X
	};

	enum MSAALevel
	{
		MSAA_LEVEL_NONE = 0,
		MSAA_LEVEL_2X,
		MSAA_LEVEL_4X,
		MSAA_LEVEL_8X,
		MSAA_LEVEL_16X,
		MSAA_LEVEL_32X
	};

	enum FrustumPlane
	{
		NEAR_PLANE,
		FAR_PLANE,
		LEFT_PLANE,
		RIGHT_PLANE,
		TOP_PLANE,
		BOTTOM_PLANE,
	};

	struct SkyeRect
	{
		long left;
		long right;
		long top;
		long bottom;
	};
}