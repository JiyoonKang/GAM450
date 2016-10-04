/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Model.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Model.cpp
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch\precompiled.h"
#include <includes\SkyeDX\Renderer.h>
#include <includes\GraphicsSystem\Model.h>

namespace Skye
{
	Model::Model() : m_VertexType(VertexType::VertexTypeDefault), m_VertexCount(0), m_IndexCount(0)
	{
	}

	Model::Model(ModelData data) : m_ModelData(data), m_VertexType(VertexType::VertexTypeDefault),
		m_VertexCount(m_ModelData.Vertices.size()), m_IndexCount(m_ModelData.Indices.size())
	{
		//m_VertexCount = m_ModelData.Vertices.size();
		//m_IndexCount = m_ModelData.Indices.size();
	}

	Model::~Model()
	{
		m_ModelData.Vertices.clear();
		m_ModelData.Indices.clear();
	}

	bool Model::Load(Renderer* renderContext, const std::string& fileName)
	{
		return true;
	}

	void Model::GenerateBuffers(Renderer* renderContext)
	{
		m_VertexCount = m_ModelData.Vertices.size();
		m_IndexCount = m_ModelData.Indices.size();

		for (unsigned i = 0; i < m_DataList.size(); ++i)
		{
			ModelDXData data;

			renderContext->CreateVertexBuffer(data.m_VertexBuffer, BufferUsage::USAGE_IMMUTABLE, sizeof(Vertex3D) * m_DataList[i].Vertices.size(), m_DataList[i].Vertices.data());
			renderContext->CreateIndexBuffer(data.m_IndexBuffer, BufferUsage::USAGE_IMMUTABLE, sizeof(unsigned int) * m_DataList[i].Indices.size(), m_DataList[i].Indices.data());

			m_DXDataList.push_back(data);
		}
	}

	const ObjectHandle& Model::GetVertexBuffer(bool pointsOnly) const
	{
		return m_D3DModelData.m_VertexBuffer;

	}

	unsigned Model::GetVertexCount() const
	{
		return m_VertexCount;
	}

	const ObjectHandle& Model::GetIndexBuffer() const
	{
		return m_D3DModelData.m_IndexBuffer;
	}

	unsigned Model::GetIndexCount(unsigned ID) const
	{
		if (ID >= m_DataList.size())
			return 0;

		return m_DataList[ID].Indices.size();
	}
}