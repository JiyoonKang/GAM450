/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Vertex.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Vertex.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <Base\util\math\VectorMath.h>

enum VertexType
{
	VertexTypeDefault,
	VertexTypeSkin
};

namespace Skye
{
	class Vertex3D
	{
		public:
			Vertex3D();

			Vertex3D(const float3& p, const float3& n, const float3& t,
				const float2& uv, const float4& c);

			Vertex3D(float px, float py, float pz, float nx, float ny, float nz,
				float tx, float ty, float tz, float u, float v, float cr,
				float cg, float cb, float ca);

			Vertex3D(float px, float py, float pz);

			float3 position;
			float3 normal;
			float3 tangent;
			float3 bitangent;
			float2 texture;
			float4 color;
			//unsigned int halfEdge;
	};
}