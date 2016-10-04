/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: ModelManager.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  ModelManager.cpp
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
#include <includes\GraphicsSystem\ModelManager.h>

namespace Skye
{
	ModelManager::ModelManager()
	{
	}

	ModelManager::~ModelManager()
	{
		
	}

	void ModelManager::AddModel(const std::string& modelName, Model* model)
	{
		m_Models.insert(std::pair<std::string, Model*>(modelName, model));
	}

	void ModelManager::LoadModel(const std::string& modelName, Renderer* renderContext)
	{
		Model* model = new Model();

		if (model->Load(renderContext, modelName))
		{
			m_Models.insert(std::pair<std::string, Model*>(modelName, model));
		}
	}

	Model* ModelManager::GetModel(const std::string& modelName)
	{
		auto it = m_Models.find(modelName);

		if (it != m_Models.end())
			return it->second;

		return nullptr;
	}

	void ModelManager::FreeModel(const std::string& modelName, Renderer* renderContext)
	{
		auto it = m_Models.find(modelName);
		
		if (it != m_Models.end())
		{
			renderContext->Release(it->second->GetVertexBuffer());
			renderContext->Release(it->second->GetIndexBuffer());
			delete it->second;
			m_Models.erase(it);
		}
	}

	void ModelManager::Clear(Renderer* renderContext)
	{
		for (auto model : m_Models)
		{
			renderContext->Release(model.second->GetVertexBuffer());
			renderContext->Release(model.second->GetIndexBuffer());
			delete model.second;
		}

		m_Models.clear();
	}
}