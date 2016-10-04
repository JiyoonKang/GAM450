/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: PrimitiveGenerator.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file   PrimitiveGenerator.cpp
\project  Bring The Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch\precompiled.h"
#include <includes\Bezier\Bezier.h>
#include <includes\SkyeDX\PrimitiveGenerator.h>

namespace Skye
{
	PrimitiveGenerator::PrimitiveGenerator(void)
	{

	}

	PrimitiveGenerator::~PrimitiveGenerator(void)
	{

	}

	void PrimitiveGenerator::CreateBox(float width, float height, float depth, ModelData& ModelData)
	{
		//
		// Create the vertices.
		//

		//Vertex3D v[24];

		std::vector<Vertex3D> v;
		v.reserve(24);

		float w2 = 0.5f*width;
		float h2 = 0.5f*height;
		float d2 = 0.5f*depth;

		//// Fill in the front face Vertex3D data.
		//v[0] = Vertex3D(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f); //White
		//v[1] = Vertex3D(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); //Black
		//v[2] = Vertex3D(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f); //Red
		//v[3] = Vertex3D(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f); //Green

		//// Fill in the back face Vertex3D data.
		//v[4] = Vertex3D(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f); //Blue
		//v[5] = Vertex3D(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f); //Magenta
		//v[6] = Vertex3D(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f); //Cyan
		//v[7] = Vertex3D(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f); //Yellow

		//// Fill in the top face Vertex3D data.
		//v[8] = Vertex3D(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f); //Black
		//v[9] = Vertex3D(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f); //Yellow
		//v[10] = Vertex3D(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f); //Cyan
		//v[11] = Vertex3D(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f); //Red

		//// Fill in the bottom face Vertex3D data.
		//v[12] = Vertex3D(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f); //White
		//v[13] = Vertex3D(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f); //Green
		//v[14] = Vertex3D(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f); //Magenta
		//v[15] = Vertex3D(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f); //Blue

		//// Fill in the left face Vertex3D data.
		//v[16] = Vertex3D(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f); //Blue
		//v[17] = Vertex3D(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f); //Yellow
		//v[18] = Vertex3D(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); //Black
		//v[19] = Vertex3D(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f); //White

		//// Fill in the right face Vertex3D data.
		//v[20] = Vertex3D(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f); //Green
		//v[21] = Vertex3D(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f); //Red
		//v[22] = Vertex3D(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f); //Cyan
		//v[23] = Vertex3D(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f); //Magenta

		// Fill in the front face Vertex3D data.
		v.push_back(Vertex3D(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f)); //White
		v.push_back(Vertex3D(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)); //Black
		v.push_back(Vertex3D(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f)); //Red
		v.push_back(Vertex3D(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f)); //Green

		// Fill in the back face Vertex3D data.
		v.push_back(Vertex3D(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f)); //Blue
		v.push_back(Vertex3D(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f)); //Magenta
		v.push_back(Vertex3D(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f)); //Cyan
		v.push_back(Vertex3D(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f)); //Yellow

		// Fill in the top face Vertex3D data.
		v.push_back(Vertex3D(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f)); //Black
		v.push_back(Vertex3D(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f)); //Yellow
		v.push_back(Vertex3D(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f)); //Cyan
		v.push_back(Vertex3D(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f)); //Red

		// Fill in the bottom face Vertex3D data.
		v.push_back(Vertex3D(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f)); //White
		v.push_back(Vertex3D(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f)); //Green
		v.push_back(Vertex3D(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f)); //Magenta
		v.push_back(Vertex3D(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f)); //Blue

		// Fill in the left face Vertex3D data.
		v.push_back(Vertex3D(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f)); //Blue
		v.push_back(Vertex3D(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f)); //Yellow
		v.push_back(Vertex3D(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)); //Black
		v.push_back(Vertex3D(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f)); //White

		// Fill in the right face Vertex3D data.
		v.push_back(Vertex3D(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f)); //Green
		v.push_back(Vertex3D(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f)); //Red
		v.push_back(Vertex3D(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f)); //Cyan
		v.push_back(Vertex3D(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f)); //Magenta


		ModelData.Vertices.assign(v.begin(), v.end());

		//
		// Create the indices.
		//

		unsigned i[36];

		// Fill in the front face index data
		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 0; i[4] = 2; i[5] = 3;

		// Fill in the back face index data
		i[6] = 4; i[7] = 5; i[8] = 6;
		i[9] = 4; i[10] = 6; i[11] = 7;

		// Fill in the top face index data
		i[12] = 8; i[13] = 9; i[14] = 10;
		i[15] = 8; i[16] = 10; i[17] = 11;

		// Fill in the bottom face index data
		i[18] = 12; i[19] = 13; i[20] = 14;
		i[21] = 12; i[22] = 14; i[23] = 15;

		// Fill in the left face index data
		i[24] = 16; i[25] = 17; i[26] = 18;
		i[27] = 16; i[28] = 18; i[29] = 19;

		// Fill in the right face index data
		i[30] = 20; i[31] = 21; i[32] = 22;
		i[33] = 20; i[34] = 22; i[35] = 23;

		ModelData.Indices.assign(&i[0], &i[36]);
	}

	void PrimitiveGenerator::CreateSphere(float radius, unsigned sliceCount, unsigned stackCount, ModelData& ModelData)
	{
		ModelData.Vertices.clear();
		ModelData.Indices.clear();

		//
		// Compute the vertices stating at the top pole and moving down the stacks.
		//

		// Poles: note that there will be texture coordinate distortion as there is
		// not a unique point on the texture map to assign to the pole when mapping
		// a rectangular texture onto a sphere.
		Vertex3D topVertex3D(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);   //WHITE
		Vertex3D bottomVertex3D(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);//BLACK

		ModelData.Vertices.push_back(topVertex3D);

		float phiStep = DirectX::XM_PI / static_cast<float>(stackCount);
		float thetaStep = 2.0f * DirectX::XM_PI / static_cast<float>(sliceCount);

		float4 colorIncrement = (bottomVertex3D.color - topVertex3D.color) / static_cast<float>(sliceCount);

		float4 ringcolor = topVertex3D.color + colorIncrement;

		// Compute vertices for each stack ring (do not count the poles as rings).
		for (unsigned i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i*phiStep;

			ringcolor += colorIncrement;

			// Vertices of ring.
			for (unsigned j = 0; j <= sliceCount; ++j)
			{
				float theta = j*thetaStep;

				Vertex3D v;

				v.color = ringcolor;

				// spherical to cartesian
				v.position.x = radius*sinf(phi)*cosf(theta);
				v.position.y = radius*cosf(phi);
				v.position.z = radius*sinf(phi)*sinf(theta);

				// Partial derivative of P with respect to theta
				v.tangent.x = -radius*sinf(phi)*sinf(theta);
				v.tangent.y = 0.0f;
				v.tangent.z = +radius*sinf(phi)*cosf(theta);
				
				v.tangent.Normalize();

				//DirectX::XMVECTOR T = XMLoadFloat3(&v.tangent);
				//DirectX::XMStoreFloat3(&v.tangent, float3normalize(T));

				//DirectX::XMVECTOR p = XMLoadFloat3(&v.position);
				//DirectX::XMStoreFloat3(&v.normal, float3normalize(p));

				float3 p = v.position;
				p.Normalize();
				v.normal = p;

				v.texture.x = theta / DirectX::XM_2PI;
				v.texture.y = phi / DirectX::XM_PI;

				ModelData.Vertices.push_back(v);
			}
		}

		ModelData.Vertices.push_back(bottomVertex3D);

		//
		// Compute indices for top stack.  The top stack was written first to the Vertex3D buffer
		// and connects the top pole to the first ring.
		//

		for (unsigned i = 1; i <= sliceCount; ++i)
		{
			ModelData.Indices.push_back(0);
			ModelData.Indices.push_back(i + 1);
			ModelData.Indices.push_back(i);
		}

		//
		// Compute indices for inner stacks (not connected to poles).
		//

		// Offset the indices to the index of the first Vertex3D in the first ring.
		// This is just skipping the top pole Vertex3D.
		unsigned baseIndex = 1;
		unsigned ringVertex3DCount = sliceCount + 1;
		for (unsigned i = 0; i < stackCount - 2; ++i)
		{
			for (unsigned j = 0; j < sliceCount; ++j)
			{
				ModelData.Indices.push_back(baseIndex + i*ringVertex3DCount + j);
				ModelData.Indices.push_back(baseIndex + i*ringVertex3DCount + j + 1);
				ModelData.Indices.push_back(baseIndex + (i + 1)*ringVertex3DCount + j);

				ModelData.Indices.push_back(baseIndex + (i + 1)*ringVertex3DCount + j);
				ModelData.Indices.push_back(baseIndex + i*ringVertex3DCount + j + 1);
				ModelData.Indices.push_back(baseIndex + (i + 1)*ringVertex3DCount + j + 1);
			}
		}

		//
		// Compute indices for bottom stack.  The bottom stack was written last to the Vertex3D buffer
		// and connects the bottom pole to the bottom ring.
		//

		// South pole Vertex3D was added last.
		unsigned southPoleIndex = (unsigned)ModelData.Vertices.size() - 1;

		// Offset the indices to the index of the first Vertex3D in the last ring.
		baseIndex = southPoleIndex - ringVertex3DCount;

		for (unsigned i = 0; i < sliceCount; ++i)
		{
			ModelData.Indices.push_back(southPoleIndex);
			ModelData.Indices.push_back(baseIndex + i);
			ModelData.Indices.push_back(baseIndex + i + 1);
		}
	}

	void PrimitiveGenerator::Subdivide(ModelData& modelData)
	{
		// Save a copy of the input geometry.
		ModelData inputCopy = modelData;


		modelData.Vertices.resize(0);
		modelData.Indices.resize(0);

		//       v1
		//       *
		//      / \
				//     /   \
				//  m0*-----*m1
		//   / \   / \
				//  /   \ /   \
				// *-----*-----*
		// v0    m2     v2

		unsigned numTris = inputCopy.Indices.size() / 3;
		for (unsigned i = 0; i < numTris; ++i)
		{
			Vertex3D v0 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 0]];
			Vertex3D v1 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 1]];
			Vertex3D v2 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 2]];

			//
			// Generate the midpoints.
			//

			Vertex3D m0, m1, m2;

			// For subdivision, we just care about the position component.  We derive the other
			// Vertex3D components in CreateGeosphere.

			m0.position = float3(
				0.5f*(v0.position.x + v1.position.x),
				0.5f*(v0.position.y + v1.position.y),
				0.5f*(v0.position.z + v1.position.z));

			m1.position = float3(
				0.5f*(v1.position.x + v2.position.x),
				0.5f*(v1.position.y + v2.position.y),
				0.5f*(v1.position.z + v2.position.z));

			m2.position = float3(
				0.5f*(v0.position.x + v2.position.x),
				0.5f*(v0.position.y + v2.position.y),
				0.5f*(v0.position.z + v2.position.z));

			//
			// Add new geometry.
			//

			modelData.Vertices.push_back(v0); // 0
			modelData.Vertices.push_back(v1); // 1
			modelData.Vertices.push_back(v2); // 2
			modelData.Vertices.push_back(m0); // 3
			modelData.Vertices.push_back(m1); // 4
			modelData.Vertices.push_back(m2); // 5
			
			modelData.Indices.push_back(i * 6 + 0);
			modelData.Indices.push_back(i * 6 + 3);
			modelData.Indices.push_back(i * 6 + 5);
			
			modelData.Indices.push_back(i * 6 + 3);
			modelData.Indices.push_back(i * 6 + 4);
			modelData.Indices.push_back(i * 6 + 5);
			
			modelData.Indices.push_back(i * 6 + 5);
			modelData.Indices.push_back(i * 6 + 4);
			modelData.Indices.push_back(i * 6 + 2);
			
			modelData.Indices.push_back(i * 6 + 3);
			modelData.Indices.push_back(i * 6 + 1);
			modelData.Indices.push_back(i * 6 + 4);
		}
	}

	void PrimitiveGenerator::CreateGeosphere(float radius, unsigned numSubdivisions, ModelData& modelData)
	{
		// Put a cap on the number of subdivisions.
		numSubdivisions = Min(numSubdivisions, 5u);

		// Approximate a sphere by tessellating an icosahedron.

		const float X = 0.525731f;
		const float Z = 0.850651f;

		float3 pos[12] =
		{
			float3(-X, 0.0f, Z), float3(X, 0.0f, Z),
			float3(-X, 0.0f, -Z), float3(X, 0.0f, -Z),
			float3(0.0f, Z, X), float3(0.0f, Z, -X),
			float3(0.0f, -Z, X), float3(0.0f, -Z, -X),
			float3(Z, X, 0.0f), float3(-Z, X, 0.0f),
			float3(Z, -X, 0.0f), float3(-Z, -X, 0.0f)
		};

		unsigned long k[60] =
		{
			1, 4, 0, 4, 9, 0, 4, 5, 9, 8, 5, 4, 1, 8, 4,
			1, 10, 8, 10, 3, 8, 8, 3, 5, 3, 2, 5, 3, 7, 2,
			3, 10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6, 1, 0,
			10, 1, 6, 11, 0, 9, 2, 11, 9, 5, 2, 9, 11, 2, 7
		};

		modelData.Vertices.resize(12);
		modelData.Indices.resize(60);

		for (unsigned i = 0; i < 12; ++i)
			modelData.Vertices[i].position = pos[i];

		for (unsigned i = 0; i < 60; ++i)
			modelData.Indices[i] = k[i];

		for (unsigned i = 0; i < numSubdivisions; ++i)
			Subdivide(modelData);

		float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

		float4 colorIncrement = (float4(0.0f, 0.0f, 0.0f, 1.0f) - color) / static_cast<float>(modelData.Vertices.size());



		// Project vertices onto sphere and scale.
		for (unsigned i = 0; i < modelData.Vertices.size(); ++i)
		{
			// Project onto unit sphere.
			//DirectX::XMVECTOR n = float3normalize(XMLoadFloat3(&modelData.Vertices[i].position));
			float3 p = modelData.Vertices[i].position;
			p.Normalize();
			
			float3 n = p;

			// Project onto sphere.
			p = n * radius;

			//XMStoreFloat3(&ModelData.Vertices[i].position, p);
			//XMStoreFloat3(&ModelData.Vertices[i].normal, n);

			modelData.Vertices[i].position = p;
			modelData.Vertices[i].normal = n;

			// Derive texture coordinates from spherical coordinates.
			float theta = AngleFromXY(
				modelData.Vertices[i].position.x,
				modelData.Vertices[i].position.z);

			float phi = acosf(modelData.Vertices[i].position.y / radius);

			modelData.Vertices[i].texture.x = theta / DirectX::XM_2PI;
			modelData.Vertices[i].texture.y = phi / DirectX::XM_PI;

			// Partial derivative of P with respect to theta
			modelData.Vertices[i].tangent.x = -radius*sinf(phi)*sinf(theta);
			modelData.Vertices[i].tangent.y = 0.0f;
			modelData.Vertices[i].tangent.z = +radius*sinf(phi)*cosf(theta);

			modelData.Vertices[i].tangent.Normalize();

			//DirectX::XMVECTOR T = XMLoadFloat3(&modelData.Vertices[i].tangent);
			//XMStoreFloat3(&modelData.Vertices[i].tangent, float3normalize(T));

			color += colorIncrement;

			modelData.Vertices[i].color = color;
		}
	}

	void PrimitiveGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, unsigned sliceCount, unsigned stackCount, ModelData& modelData)
	{
		modelData.Vertices.clear();
		modelData.Indices.clear();

		//
		// Build Stacks.
		// 

		float stackHeight = height / stackCount;

		// Amount to increment radius as we move up each stack level from bottom to top.
		float radiusStep = (topRadius - bottomRadius) / stackCount;

		unsigned ringCount = stackCount + 1;

		float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

		float4 colorInccrement = (float4(0.0f, 0.0f, 0.0f, 1.0f) - float4(1.0f, 1.0f, 1.0f, 1.0f)) / static_cast<float>(ringCount);

		float4 bottomcolor = color;

		// Compute vertices for each stack ring starting at the bottom and moving up.
		for (unsigned i = 0; i < ringCount; ++i)
		{
			float y = -0.5f*height + i*stackHeight;
			float r = bottomRadius + i*radiusStep;

			color += colorInccrement;

			// vertices of ring
			float dTheta = 2.0f* DirectX::XM_PI / sliceCount;
			for (unsigned j = 0; j <= sliceCount; ++j)
			{
				Vertex3D Vertex3D;



				Vertex3D.color = color;

				float c = cosf(j*dTheta);
				float s = sinf(j*dTheta);

				Vertex3D.position = float3(r*c, y, r*s);

				Vertex3D.texture.x = (float)j / sliceCount;
				Vertex3D.texture.y = 1.0f - (float)i / stackCount;

				// Cylinder can be parameterized as follows, where we introduce v
				// parameter that goes in the same direction as the v tex-coord
				// so that the bitangent goes in the same direction as the v tex-coord.
				//   Let r0 be the bottom radius and let r1 be the top radius.
				//   y(v) = h - hv for v in [0,1].
				//   r(v) = r1 + (r0-r1)v
				//
				//   x(t, v) = r(v)*cos(t)
				//   y(t, v) = h - hv
				//   z(t, v) = r(v)*sin(t)
				// 
				//  dx/dt = -r(v)*sin(t)
				//  dy/dt = 0
				//  dz/dt = +r(v)*cos(t)
				//
				//  dx/dv = (r0-r1)*cos(t)
				//  dy/dv = -h
				//  dz/dv = (r0-r1)*sin(t)

				// This is unit length.
				Vertex3D.tangent = float3(-s, 0.0f, c);

				float dr = bottomRadius - topRadius;
				float3 bitangent(dr*c, -height, dr*s);

				//DirectX::XMVECTOR T = XMLoadFloat3(&Vertex3D.tangent);

				float3 t = Vertex3D.tangent;
				//DirectX::XMVECTOR B = XMLoadFloat3(&bitangent);

				float3 b = bitangent;

				//DirectX::XMVECTOR N = float3normalize(float3Cross(T, B));

				float3 n = t.Cross(b);
				n.Normalize();

				//XMStoreFloat3(&Vertex3D.normal, N);
				Vertex3D.normal = n;

				modelData.Vertices.push_back(Vertex3D);
			}
		}

		// Add one because we duplicate the first and last Vertex3D per ring
		// since the texture coordinates are different.
		unsigned ringVertex3DCount = sliceCount + 1;

		// Compute indices for each stack.
		for (unsigned i = 0; i < stackCount; ++i)
		{
			for (unsigned j = 0; j < sliceCount; ++j)
			{
				modelData.Indices.push_back(i*ringVertex3DCount + j);
				modelData.Indices.push_back((i + 1)*ringVertex3DCount + j);
				modelData.Indices.push_back((i + 1)*ringVertex3DCount + j + 1);

				modelData.Indices.push_back(i*ringVertex3DCount + j);
				modelData.Indices.push_back((i + 1)*ringVertex3DCount + j + 1);
				modelData.Indices.push_back(i*ringVertex3DCount + j + 1);
			}
		}

		BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, modelData, color);
		BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, modelData, bottomcolor);
	}

	void PrimitiveGenerator::BuildCylinderTopCap(float bottomRadius, float topRadius, float height,
		unsigned sliceCount, unsigned stackCount, ModelData& ModelData,
		float4 color)
	{
		unsigned baseIndex = (unsigned)ModelData.Vertices.size();

		float y = 0.5f*height;
		float dTheta = 2.0f* DirectX::XM_PI / sliceCount;

		// Duplicate cap ring vertices because the texture coordinates and normals differ.
		for (unsigned i = 0; i <= sliceCount; ++i)
		{
			float x = topRadius*cosf(i*dTheta);
			float z = topRadius*sinf(i*dTheta);

			// Scale down by the height to try and make top cap texture coord area
			// proportional to base.
			float u = x / height + 0.5f;
			float v = z / height + 0.5f;

			ModelData.Vertices.push_back(Vertex3D(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v, color.x, color.y, color.z, color.w));
		}

		// Cap center Vertex3D.
		ModelData.Vertices.push_back(Vertex3D(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, color.x, color.y, color.z, color.w));

		// Index of center Vertex3D.
		unsigned centerIndex = (unsigned)ModelData.Vertices.size() - 1;

		for (unsigned i = 0; i < sliceCount; ++i)
		{
			ModelData.Indices.push_back(centerIndex);
			ModelData.Indices.push_back(baseIndex + i + 1);
			ModelData.Indices.push_back(baseIndex + i);
		}
	}

	void PrimitiveGenerator::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height,
		unsigned sliceCount, unsigned stackCount, ModelData& ModelData, float4 color)
	{
		// 
		// Build bottom cap.
		//

		unsigned baseIndex = (unsigned)ModelData.Vertices.size();
		float y = -0.5f*height;

		// vertices of ring
		float dTheta = 2.0f* DirectX::XM_PI / sliceCount;
		for (unsigned i = 0; i <= sliceCount; ++i)
		{
			float x = bottomRadius*cosf(i*dTheta);
			float z = bottomRadius*sinf(i*dTheta);

			// Scale down by the height to try and make top cap texture coord area
			// proportional to base.
			float u = x / height + 0.5f;
			float v = z / height + 0.5f;

			ModelData.Vertices.push_back(Vertex3D(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v, color.x, color.y, color.z, color.w));
		}

		// Cap center Vertex3D.
		ModelData.Vertices.push_back(Vertex3D(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, color.x, color.y, color.z, color.w));

		// Cache the index of center Vertex3D.
		unsigned centerIndex = (unsigned)ModelData.Vertices.size() - 1;

		for (unsigned i = 0; i < sliceCount; ++i)
		{
			ModelData.Indices.push_back(centerIndex);
			ModelData.Indices.push_back(baseIndex + i);
			ModelData.Indices.push_back(baseIndex + i + 1);
		}
	}

	void PrimitiveGenerator::CreateGrid(float width, float depth, unsigned m, unsigned n, ModelData& ModelData)
	{
		unsigned Vertex3DCount = m*n;
		unsigned faceCount = (m - 1)*(n - 1) * 2;

		//
		// Create the vertices.
		//

		float halfWidth = 0.5f*width;
		float halfDepth = 0.5f*depth;

		float dx = width / (n - 1);
		float dz = depth / (m - 1);

		float du = 1.0f / (n - 1);
		float dv = 1.0f / (m - 1);

		float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

		float4 colorInccrement = (float4(0.0f, 0.0f, 0.0f, 1.0f) - float4(1.0f, 1.0f, 1.0f, 1.0f)) / static_cast<float>(m);

		ModelData.Vertices.resize(Vertex3DCount);
		for (unsigned i = 0; i < m; ++i)
		{
			color += colorInccrement;

			float z = halfDepth - i*dz;
			for (unsigned j = 0; j < n; ++j)
			{
				float x = -halfWidth + j*dx;

				ModelData.Vertices[i*n + j].position = float3(x, 0.0f, z);
				ModelData.Vertices[i*n + j].normal = float3(0.0f, 1.0f, 0.0f);
				ModelData.Vertices[i*n + j].tangent = float3(1.0f, 0.0f, 0.0f);

				// Stretch texture over grid.
				ModelData.Vertices[i*n + j].texture.x = j*du;
				ModelData.Vertices[i*n + j].texture.y = i*dv;

				ModelData.Vertices[i*n + j].color = color;
			}
		}

		//
		// Create the indices.
		//

		ModelData.Indices.resize(faceCount * 3); // 3 indices per face

		// Iterate over each quad and compute indices.
		unsigned k = 0;
		for (unsigned i = 0; i < m - 1; ++i)
		{
			for (unsigned j = 0; j < n - 1; ++j)
			{
				ModelData.Indices[k] = i*n + j;
				ModelData.Indices[k + 1] = i*n + j + 1;
				ModelData.Indices[k + 2] = (i + 1)*n + j;

				ModelData.Indices[k + 3] = (i + 1)*n + j;
				ModelData.Indices[k + 4] = i*n + j + 1;
				ModelData.Indices[k + 5] = (i + 1)*n + j + 1;

				k += 6; // next quad
			}
		}
	}

	//void PrimitiveGenerator::CreateSamplingPlane(std::vector<float3>& Vertex3Dpositions, float4x4 view, ModelData& modelData)
	//{
	//
	//	std::vector<float3> viewSpacePoints;
	//	viewSpacePoints.resize(Vertex3Dpositions.size());
	//	float viewSpaceZ = 0.0f;
	//
	//	//transforms the vertices to view space
	//	for (unsigned int i = 0; i < viewSpacePoints.size(); ++i)
	//	{
	//		viewSpacePoints[i] = view * Vertex3Dpositions[i];
	//		viewSpaceZ += viewSpacePoints[i].z;
	//	}
	//
	//	viewSpaceZ /= Vertex3Dpositions.size();
	//
	//	//Calculates the hull in view space
	//	std::vector<float2> hull = FindConvexHull(viewSpacePoints);
	//
	//	std::vector<float3> worldSpaceVerts;
	//	worldSpaceVerts.resize(hull.size());
	//
	//	//Transforms the vertices back to world space
	//	for (unsigned int i = 0; i < hull.size(); ++i)
	//	{
	//		worldSpaceVerts[i] = Inverse(view) * float3(hull[i].x, hull[i].y, viewSpaceZ);
	//	}
	//
	//	modelData.Indices.resize((hull.size() - 2) * 3);
	//	for (unsigned int i = 0, x = 1; i < modelData.Indices.size(); ++x, i += 3)
	//	{
	//		modelData.Indices[i] = 0;
	//		modelData.Indices[i + 1] = x;
	//		modelData.Indices[i + 2] = x + 1;
	//	}
	//
	//	modelData.Vertices.resize(hull.size());
	//	for (unsigned int i = 0; i < modelData.Vertices.size(); ++i)
	//	{
	//		Vertex3D v;
	//		v.position = worldSpaceVerts[i];
	//		v.color = float4(1, 1, 1, 1);
	//
	//		modelData.Vertices[i] = v;
	//	}
	//
	//}
	//
	//std::vector<float2> PrimitiveGenerator::FindConvexHull(const std::vector<float3>& Vertex3Dpositions)
	//{
	//	std::vector<float2> corners2D;
	//	corners2D.resize(Vertex3Dpositions.size());
	//
	//	for (unsigned int i = 0; i < corners2D.size(); ++i)
	//	{
	//		corners2D[i] = float2(Vertex3Dpositions[i].x, Vertex3Dpositions[i].y);
	//	}
	//
	//	int mostLeftIndex = -1;
	//	float minX = 9999999999999999999999999999999999.9f;
	//
	//	for (unsigned int x = 0; x < corners2D.size(); ++x)
	//	{
	//		if (corners2D[x].x < minX)
	//		{
	//			mostLeftIndex = x;
	//			minX = corners2D[x].x;
	//		}
	//	}
	//
	//
	//	std::vector<int> pathIndices;
	//
	//	float2 srcLine(0, 1);
	//	int currentPointIndex = mostLeftIndex;
	//	pathIndices.push_back(currentPointIndex);
	//
	//	bool keepLooking = true;
	//
	//	while (true)
	//	{
	//		float maxDot = -99999999999999999999999999999999.9f;
	//		int maxDotIndex = -1;
	//
	//		for (unsigned int x = 0; x < corners2D.size(); ++x)
	//		{
	//
	//			float2 testLine = normalize(corners2D[x] - corners2D[currentPointIndex]);
	//
	//			if (testLine.x == 0.0f && testLine.y == 0.0f)
	//				continue;
	//
	//			float dot = Dot(srcLine, testLine);
	//
	//			if (dot > maxDot)
	//			{
	//				maxDot = dot;
	//				maxDotIndex = x;
	//			}
	//		}
	//
	//		if (maxDotIndex == pathIndices[0])
	//		{
	//			break;
	//		}
	//
	//
	//		else
	//		{
	//			pathIndices.push_back(maxDotIndex);
	//			srcLine = normalize(corners2D[maxDotIndex] - corners2D[currentPointIndex]);
	//			currentPointIndex = maxDotIndex;
	//		}
	//
	//
	//	}
	//
	//	std::vector<float2> hullPoints;
	//	hullPoints.resize(pathIndices.size());
	//	for (unsigned int i = 0; i < hullPoints.size(); ++i)
	//	{
	//		hullPoints[i] = corners2D[pathIndices[i]];
	//	}
	//
	//	return hullPoints;
	//}

	void PrimitiveGenerator::CreateFullscreenQuad(ModelData& ModelData)
	{
		ModelData.Vertices.resize(4);
		ModelData.Indices.resize(6);

		// position coordinates specified in NDC space.
		ModelData.Vertices[0] = Vertex3D(
			-1.0f, -1.0f, 0.0f,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f);

		ModelData.Vertices[1] = Vertex3D(
			-1.0f, +1.0f, 0.0f,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 1.0f);

		ModelData.Vertices[2] = Vertex3D(
			+1.0f, +1.0f, 0.0f,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 1.0f);

		ModelData.Vertices[3] = Vertex3D(
			+1.0f, -1.0f, 0.0f,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		ModelData.Indices[0] = 0;
		ModelData.Indices[1] = 1;
		ModelData.Indices[2] = 2;

		ModelData.Indices[3] = 0;
		ModelData.Indices[4] = 2;
		ModelData.Indices[5] = 3;
	}

	void PrimitiveGenerator::CreateTeapot(const unsigned n, ModelData& ModelData)
	{
		CreateTeapot(1, n, ModelData);
	}

	// Tessellates the specified bezier patch.
	void _fastcall PrimitiveGenerator::TessellatePatch(ModelData& ModelData, TeapotPatch const& patch, size_t tessellation, DirectX::FXMVECTOR scale, bool isMirrored, float4 color)
	{
		// Look up the 16 control points for this patch.
	//	DirectX::XMVECTOR controlPoints[16];
	//
	//	for (int i = 0; i < 16; i++)
	//	{
	//		controlPoints[i] = TeapotControlPoints[patch.indices[i]] * scale;
	//	}
	//
	//	// Create the index data.
	//	size_t vbase = ModelData.Vertices.size();
	//	Bezier::CreatePatchIndices(tessellation, isMirrored, [&](size_t index)
	//	{
	//		ModelData.Indices.push_back(vbase + index);
	//	});
	//
	//	// Create the Vertex3D data.
	//	Bezier::CreatePatchVertices(controlPoints, tessellation, isMirrored, [&](DirectX::FXMVECTOR position, DirectX::FXMVECTOR normal, DirectX::FXMVECTOR textureCoordinate)
	//	{
	//		ModelData.Vertices.push_back(Vertex3D(position.m128_f32[2], position.m128_f32[1], position.m128_f32[0],
	//			normal.m128_f32[2], normal.m128_f32[1], normal.m128_f32[0],
	//			0.0f, 1.0f, 0.0f,
	//			textureCoordinate.m128_f32[0], textureCoordinate.m128_f32[1],
	//			color.x, color.y, color.z, color.w));
	//	});
	}


	// Creates a teapot primitive.
	void PrimitiveGenerator::CreateTeapot(float size, size_t tessellation, ModelData& ModelData)
	{
		ModelData.Vertices.clear();
		ModelData.Indices.clear();

		if (tessellation < 1)
			throw std::out_of_range("tesselation parameter out of range");

		DirectX::XMVECTOR scaleVector = DirectX::XMVectorReplicate(size);

		DirectX::XMVECTOR scaleNegateX = scaleVector * DirectX::g_XMNegateX;
		DirectX::XMVECTOR scaleNegateZ = scaleVector * DirectX::g_XMNegateZ;
		DirectX::XMVECTOR scaleNegateXZ = scaleVector * DirectX::g_XMNegateX * DirectX::g_XMNegateZ;

		float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

		float4 colorIncrement = (float4(0.0f, 0.0f, 0.0f, 1.0f) - color) / (sizeof(TeapotPatches) / sizeof(TeapotPatches[0]));

		for (int i = 0; i < sizeof(TeapotPatches) / sizeof(TeapotPatches[0]); i++)
		{
			TeapotPatch const& patch = TeapotPatches[i];

			color += colorIncrement;

			// Because the teapot is symmetrical from left to right, we only store
			// data for one side, then tessellate each patch twice, mirroring in X.
			TessellatePatch(ModelData, patch, tessellation, scaleVector, false, color);
			TessellatePatch(ModelData, patch, tessellation, scaleNegateX, true, color);

			if (patch.mirrorZ)
			{
				color += colorIncrement;
				// Some parts of the teapot (the body, lid, and rim, but not the
				// handle or spout) are also symmetrical from front to back, so
				// we tessellate them four times, mirroring in Z as well as X.
				TessellatePatch(ModelData, patch, tessellation, scaleNegateZ, true, color);
				TessellatePatch(ModelData, patch, tessellation, scaleNegateXZ, false, color);
			}
		}

		int i = ModelData.Vertices.size();
		int j = ModelData.Indices.size();
	}
}