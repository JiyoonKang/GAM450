/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Vertex.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file   Vertex3D.cpp
\project  Bring The Paint
\author Jorge Antonio Robles

Copyright (C) 2014 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch\precompiled.h"
#include <includes\GraphicsSystem\Vertex.h>

namespace Skye
{
	Vertex3D::Vertex3D() : position(0, 0, 0), normal(0, 0, 0),
		tangent(0, 0, 0), texture(0, 0),
		color(0, 0, 0, 0)
	{
		//halfEdge = -1;
	}

	Vertex3D::Vertex3D(const float3& p, const float3& n, const float3& t, const float2& uv, const float4& c) :
		position(p), normal(n), tangent(t), texture(uv), color(c)
	{
		//halfEdge = -1;
	}


	Vertex3D::Vertex3D(float px, float py, float pz, float nx, float ny, float nz,
		float tx, float ty, float tz, float u, float v, float cr,
		float cg, float cb, float ca) : position(px, py, pz),
		normal(nx, ny, nz), tangent(tx, ty, tz), texture(u, v),
		color(cr, cg, cb, ca)
	{
		//halfEdge = -1;
	}

	Vertex3D::Vertex3D(float px, float py, float pz) : position(px, py, pz),
		normal(0, 0, 0), tangent(0, 0, 0),
		texture(0, 0), color(0, 0, 0, 0)
	{
		//halfEdge = -1;
	}
}