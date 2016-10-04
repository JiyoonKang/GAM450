/******************************************************************************/
/*!
\file  ModelLoader.cpp
\project  CS460
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
//#include <Graphics/ModelLoader/ModelLoader.h>
//#include <Graphics/Animation/AnimatedModel.h>
//#include <Graphics/Animation/Skeleton.h>
//#include <Graphics/Animation/Animation.h>
//#include <FBX/FilePath.h>

#include <pch/precompiled.h>
#include <Graphics/includes/ModelLoader/ModelLoader.h>
#include <includes/ModelLoader/FilePath.h>
#include <Graphics/includes/GraphicsSystem/SkyeGraphics.h>


void ModelLoader::Initialize()
{

}

Model* ModelLoader::LoadModel(const std::string& modelName)
{
	ChunkReader reader;
	bool result = reader.Open(modelName);
	if (!result)
	{
		return nullptr;
	}

	GChunk fileChunk = reader.ReadChunkHeader();

	if (fileChunk.Type != FileMark)
	{
		return nullptr;
	}

	ModelType type;
	reader.Read(type);

	Model* model;

	if (type == ModelType::MODEL_STATIC)
	{
		model = new Model();
	}
	else
	{
		//model = new AnimatedModel();
	}

	fileChunk = reader.PeekChunk();

	while (fileChunk.Type != 0)
	{
		switch (fileChunk.Type)
		{
			case MarkMesh:
			{
				LoadMesh(model, reader);
				model->m_DataList.push_back(model->m_ModelData);
				model->GenerateBuffers(GRAPHICS->GetRenderContext());
				break;
			}

			//case MarkSkel:
			//{
			//	LoadSkeleton(model, reader);
			//	break;
			//}
			//
			//case MarkAnimation:
			//{
			//	LoadAnimation(model, reader);
			//	break;
			//}
			
			default:
			{
				reader.SkipChunk(fileChunk);
				break;
			}
		}

		fileChunk = reader.PeekChunk();
	}

	reader.Close();

	return model;
}

void ModelLoader::LoadMesh(Model* model, ChunkReader& reader)
{
	//model->m_Mesh = new Mesh();
	//Mesh* mesh = model->m_Mesh;

	GChunk meshChunk = reader.ReadChunkHeader();

	reader.Read(model->m_VertexType);

	unsigned numIndices = 0;
	unsigned numVerts = 0;

	//Reads the number of indices in the mesh
	reader.Read(numIndices);

	model->m_ModelData.Indices.resize(numIndices);

	//Reads in all the indices in the mesh
	reader.ReadArray(model->m_ModelData.Indices.data(), numIndices);

	//Read number of verts
	reader.Read(numVerts);

	//Read the verts depending of animation or not
	if (model->m_VertexType == VertexType::VertexTypeDefault)
	{
		model->m_ModelData.Vertices.resize(numVerts);
		reader.ReadArray(model->m_ModelData.Vertices.data(), numVerts);
	}
	else
	{
		//mesh->m_MeshData.m_SkinVertices.resize(numVerts);
		//reader.ReadArray(mesh->m_MeshData.m_SkinVertices.data(), numVerts);
	}

}

//void ModelLoader::LoadSkeleton(Model* model, ChunkReader& reader)
//{
//	AnimatedModel* animatedModel = reinterpret_cast<AnimatedModel*>(model);
//
//	animatedModel->m_Skeleton = new Skeleton();
//
//	GChunk skelChunk = reader.ReadChunkHeader();
//	unsigned numBones = 0;
//	reader.Read(numBones);
//
//	//Reserve array for the number of bones
//	animatedModel->m_Skeleton->m_BoneList.resize(numBones);
//
//	//Read all the bone data for each bone in the skeleton
//	for (unsigned i = 0; i < numBones; ++i)
//	{
//		Bone& bone = animatedModel->m_Skeleton->m_BoneList[i];
//
//		reader.Read(bone.m_Name);
//		reader.Read(bone.m_ParentIndex);
//		reader.Read(bone.m_BindPose);
//		reader.Read(bone.m_BoneTransformation);
//	}
//}
//
//void ModelLoader::LoadAnimation(Model* model, ChunkReader& reader)
//{
//	AnimatedModel* animatedModel = reinterpret_cast<AnimatedModel*>(model);
//
//	GChunk animChunk = reader.ReadChunkHeader();
//
//	unsigned numAnimations = 0;
//	unsigned numTracks = 0;
//	unsigned numKeys = 0;
//
//	reader.Read(numAnimations);
//
//	for (unsigned i = 0; i < numAnimations; ++i)
//	{
//		Animation* animation = new Animation();
//
//		reader.Read(animation->m_AnimationName);
//
//		reader.Read(animation->m_Duration);
//
//		reader.Read(numTracks);
//
//		animation->m_TrackList.resize(numTracks);
//
//		for (unsigned j = 0; j < numTracks; ++j)
//		{
//			reader.Read(numKeys);
//
//			animation->m_TrackList[j].m_KeyFrameList.resize(numKeys);
//			reader.ReadArray(&animation->m_TrackList[j].m_KeyFrameList[0], numKeys);
//		}
//
//		animatedModel->m_Animations.push_back(animation);
//	}
//}

ModelLoader* ModelLoader::getInstance()
{
  static ModelLoader* instance = 0;

  if (!instance)
    instance = new ModelLoader();

  return instance;
}