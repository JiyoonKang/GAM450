/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: ModelManager.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  ModelManager.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

//#include <GraphicsSystem\Model.h>
#include <unordered_map>


namespace Skye
{
	class Model;
	class Renderer;

	class ModelManager
	{
		public:
			ModelManager(void);
			~ModelManager(void);

			void AddModel(const std::string& modelName, Model* model);
			void LoadModel(const std::string& modelName, Renderer* renderContext);
			Model* GetModel(const std::string& modelName);

			void FreeModel(const std::string& modelName, Renderer* renderContext);
			void Clear(Renderer* renderContext);


		private:
			std::unordered_map<std::string, Model*> m_Models;
	};
}