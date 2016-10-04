/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Model.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Model.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <includes\SkyeDX\ObjectHandle.h>
#include <includes\GraphicsSystem\Vertex.h>

namespace Skye
{
	class Renderer;

	enum ModelType
	{
		MODEL_STATIC,
		MODEL_SKINNED,
		ONLY_ANIMATION
	};

	struct ModelDXData
	{
		ObjectHandle m_VertexBuffer;
		ObjectHandle m_IndexBuffer;
	};

	struct ModelData
	{
		std::vector<Vertex3D> Vertices;
		std::vector<unsigned int> Indices;
	};

	typedef std::vector<ModelData> ModelDataList;
	typedef std::vector<ModelDXData> DXDataList;

	class Model
	{
		public:
			Model(void);
			Model(ModelData data);
			~Model(void);

			bool Load(Renderer* renderContext, const std::string& fileName);

			void GenerateBuffers(Renderer* renderContext);

			const ObjectHandle& GetVertexBuffer(bool pointsOnly = false) const;
			unsigned GetVertexCount(void) const;

			const ObjectHandle& GetIndexBuffer(void) const;
			unsigned GetIndexCount(unsigned ID) const;

		public:
			ModelDXData m_D3DModelData;
			ModelData m_ModelData;

			ModelDataList m_DataList;
			DXDataList m_DXDataList;

			VertexType m_VertexType;
			
		private:
			

			unsigned m_VertexCount;
			unsigned m_IndexCount;
	};
}