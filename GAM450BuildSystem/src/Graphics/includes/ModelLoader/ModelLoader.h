/******************************************************************************/
/*!
\file  ModelLoader.h
\project  CS460
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
//#include <Graphics/ModelLoader/ChunkReader.hpp>
//#include <Engine/Utilities.h>
//#include <Graphics/Skye/Model.h>

#include <Graphics/includes/ModelLoader/ChunkReader.hpp>
#include <Graphics/includes/GraphicsSystem/Model.h>

using namespace Skye;
//using namespace Math;

class ModelLoader
{
	public:

		void Initialize();
		Model* LoadModel(const std::string& modelName);
		void LoadMesh(Model* model, ChunkReader& reader);
		void LoadSkeleton(Model* model, ChunkReader& reader);
		void LoadAnimation(Model* model, ChunkReader& reader);
		
		static ModelLoader* getInstance();
};
