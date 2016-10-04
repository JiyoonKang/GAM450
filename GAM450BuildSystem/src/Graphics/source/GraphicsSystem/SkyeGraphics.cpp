/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: SkyeGraphics.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  SkyeGraphics.cpp
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "pch\precompiled.h"
#include <includes\GraphicsSystem\SkyeGraphics.h>
#include <Base\util\debug\DebugAssist.h>
#include <includes\SkyeDX\PrimitiveGenerator.h>
#include <includes\GraphicsSystem\Vertex.h>
#include <Core\systems\window\Window.h>
#include <includes\SkyeDX\SkyeDX11.h>
#include <Core/systems/input/KeyboardManager.h>

#include "core/engine/factory/Factory.h"
#include "core/engine/core/Engine.h"
#include "core/types/space/Space.h"
#include <base/util/math/MathUtil.h>

#include "Physics/colliders/CapsuleCollider.h"
#include "Physics/colliders/SphereCollider.h"
#include "Physics/colliders/BoxCollider.h"
#include "Core/types/object/Object.h"
#include "Graphics/includes/GraphicsSystem/PointLight.h"
#include <Graphics/includes/GraphicsSystem/ParticleSystem.h>

//#include "Editor/InGameEditor/InGameEditor.h"

#include <Graphics/includes/ModelLoader/ModelLoader.h>

#include <Graphics/includes/GraphicsSystem/Emissive.h>
#include <Graphics/includes/GraphicsSystem/Fade.h>
#include <Graphics/includes/GraphicsSystem/Levitate.h>
#include <Graphics/includes/GraphicsSystem/Flicker.h>
#include <Graphics/includes/GraphicsSystem/LightDeath.h>

#include "start/config/Config.h"

#include "Core/engine/frameratecontroller/FrameRateController.h"

#include "GameLogic/GameLogicSystem.h"

#include "Editor/InGameEditor/imgui_impl_dx11.h"

using namespace Skye; 

//const static float PI = 3.1415926535897932384626433832795f;

#define MAX_LIGHTS 1000

const static float SCREEN_FAR = 100000.0f;
const static float SCREEN_NEAR = .1f;


SkyeGraphics* GRAPHICS = nullptr;

SkyeGraphics::RenderableList SkyeGraphics::m_RenderableObjects;

static std::vector<PointLight*> PointLightList;

SkyeGraphics::SkyeGraphics(const HWND hwnd) 
	: m_hwnd(hwnd), 
	  m_renderContext(new Skye::SkyeDX11()),
	  m_DebugDrawer(90000),
	  m_FovY(0.0f),
	  m_FullscreenAspectRatio(1),
	  m_OrthographicProjectionSize(1, 1),
	  m_MultipleViewports(true),
	  m_DrawDebug(false)
{
	GRAPHICS = this;
	m_DrawImgui = false;
	m_Imgui = new InGameEditor(hwnd);
}

SkyeGraphics::~SkyeGraphics()
{
	FreeAllModels();
	FreeAllTextures();
}

void SkyeGraphics::Initialize()
{

	REGISTER_COMPONENT(Renderable, Renderable);
	REGISTER_COMPONENT(PointLight, PointLight);
	REGISTER_COMPONENT(EmissiveComponent, EmissiveComponent);
	REGISTER_COMPONENT(Fade, Fade);
	REGISTER_COMPONENT(Levitate, Levitate);
	REGISTER_COMPONENT(Flicker, Flicker);
	REGISTER_COMPONENT(LightDeath, LightDeath);

	m_renderContext->SetBackColor(float4(0.3f, 0.6f, 0.7f, 1.f));
	//m_renderContext->SetBackColor(float4(0.0f, 0.0f, 0.0f, 1.0f));

	SetFOV(60.0f);
	
	m_renderContext->SetWindow(m_hwnd);
	
	m_renderContext->Initialize();

	SetOrthographicProjectionSize(float2(static_cast<float>(WINDOWSSYSTEM->GetScreenWidth()), static_cast<float>(WINDOWSSYSTEM->GetScreenHeight())));

	m_CameraManager.AddCamera();

	/*m_CameraManager.GetActiveCamera().SetLookAt(float3(0, 0, -1));
	m_CameraManager.GetActiveCamera().SetPosition(float3(0, 0, 0));*/

	m_CameraManager.AddViewport(0, 0, Skye::Area());

	ResetScreen();

	InitializeConstantBuffers();

	//At this point enough is initialized to render loading texture
	LoadingScreen();

	m_renderContext->SwapBuffers();

	//Finish initializing and loading the remainding assets
	InitializeDefaultAssets();
	InitializeRenderTargets();

	LoadVertexShaders();
	LoadGeometryShaders();
	LoadPixelShaders();

	ID3D11Device* device = reinterpret_cast<ID3D11Device*>(m_renderContext->GetDevice());
	ID3D11DeviceContext* device_context = reinterpret_cast<ID3D11DeviceContext*>(m_renderContext->GetDeviceContext());
	ImGui_ImplDX11_Init(WINDOWSSYSTEM->hWnd, device, device_context);

	m_Imgui->Initialize();
	m_DebugDrawer.Initialize(m_renderContext.get(), &m_CameraManager.GetActiveCamera());
	
	InitializeDefaultFontWrappers();

	LoadAssets();

	m_renderContext->SetPrimitiveTopology(PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	SetViewPlanes(1.0f, 20000.0f);
}

void SkyeGraphics::Shutdown()
{
	
}

void SkyeGraphics::OnResize()
{
	//m_CameraManager.GetActiveCamera().Resize(0.25 * PI, 3.f /4 /*Window::GetInstance()->GetAspectRatio()*/, SCREEN_NEAR, SCREEN_FAR);
}

void SkyeGraphics::Update(float dt)
{
	m_renderContext->ClearBuffer();

	UpdateCameraManager();
	
	ClearBuffers();
  
	BindConstantBuffers();

	m_renderContext->SetDepthTesting(true);
	m_renderContext->SetDepthWriting(true);

	m_renderContext->BindDepthBuffer(m_CameraManager.m_DepthBuffer);

	m_renderContext->SetBlendMode(Skye::BlendMode::BLEND_MODE_NONE);
	m_renderContext->SetFillMode(Skye::FillMode::FILL_SOLID);
	m_renderContext->SetCullMode(Skye::CullMode::CULL_MODE_BACK_FACE);

	m_renderContext->SetPrimitiveTopology(PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_renderContext->BindRenderTarget(m_renderContext->GetBackBuffer());

	RenderDebug();

	//m_DebugDrawer.Clear();

	Render3DObjects();
	RenderDebug();

	m_Imgui->Update(dt);
	RenderEditor();

	m_renderContext->SwapBuffers();

	//m_DebugDrawer.DrawCapsule(float4(0, 15, 100, 0), float4(0, 25, 90, 0), 5, Color::s_Red);
  //
	//m_DebugDrawer.DrawSphere(float3(50.0f, 50.0f, 150.0f), 1.0f, Color::s_White);
  
	//m_DebugDrawer.Axis(float4(), 1);

	//SUPER TERRIBLE, I HATE THIS BUT AT THIS POINT... IT WORKS... STILL GWUMPY ABOUT IT -.-
	//if (Config::installer && !GameLogic::GAMELOGIC_SYSTEM->GetPaused())
	//{
	//	//Update components
	//	for (auto it = Core::ENGINE->Spaces().begin(); it != Core::ENGINE->Spaces().end(); ++it)
	//	{
	//		Base::ObjectAllocator* fadeObjects = (*it)->GetComponents(eFade);
	//		Base::ObjectAllocator* levitations = (*it)->GetComponents(eLevitate);
	//		Base::ObjectAllocator* flickers = (*it)->GetComponents(eFlicker);
	//		Base::ObjectAllocator* lightDeaths = (*it)->GetComponents(eLightDeath);

	//		//Update all the levitation and fade components 

	//		for (auto rt = levitations->begin<Levitate>(); rt != levitations->end<Levitate>(); ++rt)
	//		{
	//			rt->Update(dt);
	//		}

	//		for (auto rt = fadeObjects->begin<Fade>(); rt != fadeObjects->end<Fade>(); ++rt)
	//		{
	//			rt->Update(dt);
	//		}

	//		for (auto rt = flickers->begin<Flicker>(); rt != flickers->end<Flicker>(); ++rt)
	//		{
	//			rt->Update(dt);
	//		}

	//		for (auto rt = lightDeaths->begin<LightDeath>(); rt != lightDeaths->end<LightDeath>(); ++rt)
	//		{
	//			rt->Update(dt);
	//		}
	//	}
	//}

	
	
}

void SkyeGraphics::ReceiveMessage(Core::Message& msg)
{
	switch (msg.MessageId)
	{
		case Core::Message::WindowResize:
		{
			ResizeMessage* m = reinterpret_cast<ResizeMessage*>(&msg);
			m_renderContext->OnResize(m->wParam, m->lParam);
			break;
		}
	}
}

CameraManager::CameraManager() : m_ActiveCamera(0)
{
	m_Cameras.push_back(Skye::Camera());
}

CameraManager::~CameraManager()
{
}

CameraID CameraManager::AddCamera()
{
	m_Cameras.push_back(Skye::Camera());

	return m_Cameras.size() - 1;
}

void CameraManager::AddViewport(int x, int y, Skye::Area size)
{
	m_Viewports.push_back(Skye::Viewport(x, y, size));
}

CameraID CameraManager::GetActiveCameraID()
{
	return m_ActiveCamera;
}

Skye::Camera& CameraManager::GetActiveCamera()
{
	return m_Cameras[m_ActiveCamera];
}

const Skye::Viewport& CameraManager::GetViewport() const
{
	return m_Viewport;
}

const float2& CameraManager::GetOrthographicProjectionSize() const
{
	return m_OrthographicProjectionSize;
}

void CameraManager::SetActiveCamera(CameraID cam)
{
	m_ActiveCamera = cam;
}

void CameraManager::SetActiveViewport(unsigned ID)
{
	m_Viewport = m_Viewports[ID];
}

const HWND& SkyeGraphics::GetWindowHandle() const
{
	return m_hwnd;
}

void SkyeGraphics::LoadAssets()
{
	LoadModelPrimitives();

	LoadModels();


	LoadTextures();
}

void SkyeGraphics::LoadModelPrimitives()
{
	PrimitiveGenerator geoGen;

	//CUBE
	Model* cube = new Model();

	geoGen.CreateBox(1, 1, 1, cube->m_ModelData);

	cube->m_DataList.push_back(cube->m_ModelData);

	cube->GenerateBuffers(m_renderContext.get());

	m_ModelManager.AddModel("Cube", cube);

	InGameEditor::AddModelNameOnList("Cube");

	//QUAD
	Model* quad = new Model();

	geoGen.CreateFullscreenQuad(quad->m_ModelData);

	quad->m_DataList.push_back(quad->m_ModelData);

	quad->GenerateBuffers(m_renderContext.get());


	m_ModelManager.AddModel("Quad", quad);

	InGameEditor::AddModelNameOnList("Quad");

	//GRID
	Model* grid = new Model();

	geoGen.CreateGrid(1.0, 1.0, 10, 10, grid->m_ModelData);

	grid->m_DataList.push_back(grid->m_ModelData);

	grid->GenerateBuffers(m_renderContext.get());

	m_ModelManager.AddModel("Grid", grid);

	InGameEditor::AddModelNameOnList("Grid");

	//SPHERE
	Model* sphere = new Model();

	geoGen.CreateSphere(1.0f, 20, 20, sphere->m_ModelData);

	sphere->m_DataList.push_back(sphere->m_ModelData);

	sphere->GenerateBuffers(m_renderContext.get());

	m_ModelManager.AddModel("Sphere", sphere);

	InGameEditor::AddModelNameOnList("Sphere");

	//GEOSPHERE
	Model* geoSphere = new Model();

	geoGen.CreateGeosphere(1.0f, 2, geoSphere->m_ModelData);

	geoSphere->m_DataList.push_back(geoSphere->m_ModelData);

	geoSphere->GenerateBuffers(m_renderContext.get());

	m_ModelManager.AddModel("GeoShpere", geoSphere);

	InGameEditor::AddModelNameOnList("GeoSphere");

	//CYLINDER
  Model *cylinder = new Model();

  geoGen.CreateCylinder(0.5f, 0.5f, 1.f, 20, 20, cylinder->m_ModelData);

  cylinder->m_DataList.push_back(cylinder->m_ModelData);

  cylinder->GenerateBuffers(m_renderContext.get());

  m_ModelManager.AddModel("Cylinder", cylinder);

  InGameEditor::AddModelNameOnList("Cylinder");
}

std::string RemoveFileExtension(const std::string & file)
{
	size_t lastdot = file.find_last_of(".");
	if (lastdot == std::string::npos)
		return file;
	return file.substr(0, lastdot);
}

void SkyeGraphics::LoadTextures()
{
	
	LoadDiffuseTextures();
	LoadNormalTextures();
	LoadEmissiveTextures();
}

void SkyeGraphics::LoadDiffuseTextures()
{
	WIN32_FIND_DATA fd[256];
	int count = 1;
	std::string name = "textures/diffuse/";
	FILE_INFO_BY_HANDLE_CLASS fileInfo = FILE_INFO_BY_HANDLE_CLASS::FileNameInfo;
	HANDLE h = FindFirstFile((name + "*.png").c_str(), &fd[0]);
	if (!h) --count;
	while (FindNextFile(h, &fd[count]))
	{
		//COUNTING FILES
		count++;
	}
	FindClose(h);
	
	for (int i = 0; i < count; ++i)
	{
		LoadTexture(fd[i].cFileName, "diffuse/");
		std::string strippedName = RemoveFileExtension(fd[i].cFileName);
	
		InGameEditor::AddTextureNameOnList(strippedName);
	}

	count = 1;
	h = FindFirstFile((name + "*.jpg").c_str(), &fd[0]);
	if (!h) --count;
	while (FindNextFile(h, &fd[count]))
	{
		//COUNTING FILES.
		count++;
	}
	FindClose(h);

	for (int i = 0; i < count; ++i)
	{
		LoadTexture(fd[i].cFileName, "diffuse/");
		std::string strippedName = RemoveFileExtension(fd[i].cFileName);

		InGameEditor::AddTextureNameOnList(strippedName);
	}

}

void SkyeGraphics::LoadNormalTextures()
{
	WIN32_FIND_DATA fd[256];
	int count = 1;
	std::string name = "textures/normal/";
	FILE_INFO_BY_HANDLE_CLASS fileInfo = FILE_INFO_BY_HANDLE_CLASS::FileNameInfo;
	HANDLE h = FindFirstFile((name + "*.png").c_str(), &fd[0]);

	while (FindNextFile(h, &fd[count++]))
	{
		//COUNTING FILES
	}
	--count;
	FindClose(h);

	for (int i = 0; i < count; ++i)
	{
		LoadTexture(fd[i].cFileName, "normal/");
		std::string strippedName = RemoveFileExtension(fd[i].cFileName);

		InGameEditor::AddNormalTextureNameOnList(strippedName);
	}

	count = 1;
	h = FindFirstFile((name + "*.jpg").c_str(), &fd[0]);

	while (FindNextFile(h, &fd[count++]))
	{
		//COUNTING FILES
	}
	--count;
	FindClose(h);

	for (int i = 0; i < count; ++i)
	{
		LoadTexture(fd[i].cFileName, "normal/");
		std::string strippedName = RemoveFileExtension(fd[i].cFileName);

		InGameEditor::AddNormalTextureNameOnList(strippedName);
	}
}

void SkyeGraphics::LoadEmissiveTextures()
{
	WIN32_FIND_DATA fd[256];
	int count = 1;
	std::string name = "textures/emissive/";
	FILE_INFO_BY_HANDLE_CLASS fileInfo = FILE_INFO_BY_HANDLE_CLASS::FileNameInfo;
	HANDLE h = FindFirstFile((name + "*.png").c_str(), &fd[0]);

	while (FindNextFile(h, &fd[count++]))
	{
		//COUNTING FILES
	}
	--count;
	FindClose(h);

	for (int i = 0; i < count; ++i)
	{
		LoadTexture(fd[i].cFileName, "emissive/");

		std::string strippedName = RemoveFileExtension(fd[i].cFileName);

		InGameEditor::AddEmissiveTextureNameOnList(strippedName);
	}
}

Skye::Camera& SkyeGraphics::GetCamera()
{
	return m_CameraManager.GetActiveCamera();
}

Skye::Camera* SkyeGraphics::GetCamera(unsigned ID)
{
	return &m_CameraManager.m_Cameras[ID];
}

float2 SkyeGraphics::GetScreenSize()
{
	return m_CameraManager.GetOrthographicProjectionSize();
}

Skye::Renderer* SkyeGraphics::GetRenderContext()
{
	return m_renderContext.get();
}

void SkyeGraphics::SetUITexture(Skye::ObjectHandle& texture)
{
	m_UITexture = texture;
}

void SkyeGraphics::LoadTexture(const std::string& fileName, const std::string& folder)
{
	Skye::ObjectHandle texture;

	std::string name = "textures/" + folder + fileName;

	m_renderContext->CreateTexture(texture, name);

	if (texture)
	{
		std::string strippedName = RemoveFileExtension(fileName);
		m_Textures.insert(std::pair<std::string, Skye::ObjectHandle>(strippedName, texture));
	}
}

void SkyeGraphics::LoadTextureFromMemory(const std::string& fileName, const void* data, unsigned dataSize)
{
	Skye::ObjectHandle texture;

	m_renderContext->CreateTextureFromMemory(texture, data, dataSize);

	if (texture)
		m_Textures.insert(std::pair<std::string, Skye::ObjectHandle>(fileName, texture));
}

void SkyeGraphics::LoadTextureFromMemory(const std::string& fileName, const void* data, Skye::Area dataSize, DataFormat format)
{
  Skye::ObjectHandle texture;

  m_renderContext->CreateTexture(texture, data, dataSize, format);

  if (texture)
    m_Textures.insert(std::pair<std::string, Skye::ObjectHandle>(fileName, texture));
}


const Skye::ObjectHandle& SkyeGraphics::GetTexture(const std::string& textureName)
{
	auto it = m_Textures.find(textureName);

	if (it != m_Textures.end())
	{
		return it->second;
	}

	return Skye::ObjectHandle::Null();
}
const Skye::Area& SkyeGraphics::GetTextureSize(const std::string& textureName)
{
  auto it = m_Textures.find(textureName);

  if (it != m_Textures.end())
  {
    return m_renderContext->GetTextureSize(it->second);
  }
  
  return Skye::Area::Null();
}

void SkyeGraphics::LoadModelFromFile(const std::string& fileName, Skye::Model& model)
{
	std::string name = std::string("models/") + fileName;

	name.append(".bin");

	FILE* file = fopen(name.c_str(), "rb");

	ErrorIf(!file, "CouldNot open Model %s", name.c_str());

	if (!file)
		return;

	unsigned numMeshes;
	DWORD sizeV, sizeI;

	fread(&numMeshes, sizeof(unsigned), 1, file);

	for (unsigned j = 0; j < numMeshes; ++j)
	{
		ModelData data;
		fread(&sizeV, sizeof(DWORD), 1, file);
		fread(&sizeI, sizeof(DWORD), 1, file);

		for (DWORD i = 0; i < sizeV; ++i)
		{
			Vertex3D temp;
			fread(&temp, sizeof(Vertex3D), 1, file);
			data.Vertices.push_back(temp);
		}

		for (DWORD i = 0; i < sizeI; ++i)
		{
			int temp;
			fread(&temp, sizeof(int), 1, file);
			data.Indices.push_back(temp);
		}

		model.m_DataList.push_back(data);
	}
	fclose(file);

}

void SkyeGraphics::LoadModels()
{

	WIN32_FIND_DATA fd[256];
	int count = 1;
	std::string name = "models/";
	FILE_INFO_BY_HANDLE_CLASS fileInfo = FILE_INFO_BY_HANDLE_CLASS::FileNameInfo;
  HANDLE h = FindFirstFile((name + "*.skye").c_str(), &fd[0]);

	while (FindNextFile(h, &fd[count++]))
	{
		//COUNTING FILES
	}
	--count;
	FindClose(h);

	for (int i = 0; i < count; ++i)
	{

		std::string fileName = RemoveFileExtension(fd[i].cFileName);

		std::string filePath = "models/" + fileName + ".skye";

		Model* modelToLoad = ModelLoader::getInstance()->LoadModel(filePath);


		m_ModelManager.AddModel(fileName, modelToLoad);

		InGameEditor::AddModelNameOnList(fileName);
	}

}

void SkyeGraphics::FreeTexture(const std::string& textureName)
{
	auto it = m_Textures.find(textureName);

	if (it != m_Textures.end())
	{
		m_renderContext->Release(it->second);
		m_Textures.erase(it);
	}
}

void SkyeGraphics::FreeAllTextures()
{
	for (auto texture : m_Textures)
	{
		m_renderContext->Release(texture.second);
	}

	m_Textures.clear();
}

void SkyeGraphics::LoadModel(const std::string& fileName)
{
	Skye::Model* model = new Skye::Model();

	if (model->Load(m_renderContext.get(), fileName))
	{
		m_ModelManager.AddModel(fileName, model);
	}
}

Skye::Model* SkyeGraphics::GetModel(const std::string& modelName)
{
	return m_ModelManager.GetModel(modelName);
}

void SkyeGraphics::FreeModel(const std::string& modelName)
{
	m_ModelManager.FreeModel(modelName, m_renderContext.get());
}

void SkyeGraphics::FreeAllModels()
{
	m_ModelManager.Clear(m_renderContext.get());
}

CameraManager& SkyeGraphics::GetCameraManager()
{
	return m_CameraManager;
}

void SkyeGraphics::SetOrthographicProjectionSize(const float2& size)
{
	m_OrthographicProjectionSize = size;
}

void SkyeGraphics::SetFOV(float fov)
{
	m_FovY = fov * (PI / 180.0f);
}

float SkyeGraphics::GetFOV()
{
	return m_FovY;
}

void SkyeGraphics::SetViewPlanes(float nearPlane, float farPlane)
{
	m_EffectConstants.viewPlanes = float2(nearPlane, farPlane);
}

float SkyeGraphics::GetNearPlane()
{
	return m_EffectConstants.viewPlanes.x;
}

float SkyeGraphics::GetFarPlane()
{
	return m_EffectConstants.viewPlanes.y;
}

void SkyeGraphics::SetFullScreen(bool fullScreen)
{

}

bool SkyeGraphics::GetFullScreen()
{
	return true;
}

void SkyeGraphics::SetVSync(bool vSync)
{
	m_renderContext->SetVsync(vSync);
}

bool SkyeGraphics::GetVSync()
{
	return m_renderContext->GetVsync();
}

void SkyeGraphics::DrawEditor(bool isEditorActive)
{
	m_DrawImgui = isEditorActive;
}

void SkyeGraphics::RenderEditor()
{
	m_renderContext->SetCullMode(Skye::CullMode::CULL_MODE_IMGUI);

	//TURNING OFF ALL KINDS OF DEPTH TESTING ******************************
	m_renderContext->SetDepthTesting(false);
	m_renderContext->SetDepthWriting(false);

	m_renderContext->BindDepthBuffer(m_CameraManager.m_DepthBuffer);
	m_renderContext->BindRenderTarget(m_renderContext->GetBackBuffer());
	//TURNING OFF ALL KINDS OF DEPTH TESTING ******************************


	m_Imgui->Render();


	//TURNING ON ALL KINDS OF DEPTH TESTING ******************************
	m_renderContext->SetDepthTesting(true);
	m_renderContext->SetDepthWriting(true);

	m_renderContext->BindDepthBuffer(m_CameraManager.m_DepthBuffer);
	m_renderContext->BindRenderTarget(ObjectHandle::Null());
	//TURNING ON ALL KINDS OF DEPTH TESTING ******************************

	m_renderContext->SetCullMode(Skye::CullMode::CULL_MODE_NONE);
}

void SkyeGraphics::RenderDebug()
{
	//Clear and bind the debug draw render target
	//m_renderContext->ClearRenderTarget(m_RenderTargets.find("DEBUGDRAW")->second, float4());
	//m_renderContext->BindRenderTarget(m_RenderTargets.find("DEBUGDRAW")->second);

	m_renderContext->BindRenderTarget(m_renderContext->GetBackBuffer());

	//Bind the debug draw shaders
	m_renderContext->BindVertexShader(m_VertexShaders["LineVS.hlsl"]);
	m_renderContext->BindPixelShader(m_PixelShaders["LinePS.hlsl"]);

	//Set topology to be lines
	m_renderContext->SetPrimitiveTopology(PrimitiveTopology::PRIMITIVE_TOPOLOGY_LINELIST);

	//Remove culling, blending and depth testing
	m_renderContext->SetCullMode(CullMode::CULL_MODE_NONE);
	m_renderContext->SetBlendMode(BlendMode::BLEND_MODE_NONE);

	m_renderContext->SetDepthTesting(false);
	m_renderContext->SetDepthWriting(false);

	//Render any lines that are in the debug drawer
	if (m_DebugDrawer.GetSize())
	{
		m_renderContext->SetViewport(m_CameraManager.m_Viewports[Viewports::FullScreen]);
		SetCameraConstantBuffer(Viewports::FullScreen);

		m_DebugDrawer.Draw();
		/*if (Editor::InGameEditor::IsEditorActive())
		{
			m_renderContext->SetViewport(m_CameraManager.m_Viewports[Viewports::FullScreen]);
			SetCameraConstantBuffer(Viewports::Player1);

			m_DebugDrawer.Draw();
		}

		else
		{
			m_renderContext->SetViewport(m_CameraManager.m_Viewports[Viewports::Player1]);
			SetCameraConstantBuffer(Viewports::Player1);

			m_DebugDrawer.Draw();

			m_renderContext->SetViewport(m_CameraManager.m_Viewports[Viewports::Player2]);
			SetCameraConstantBuffer(Viewports::Player2);

			m_DebugDrawer.Draw();

			m_renderContext->SetViewport(m_CameraManager.m_Viewports[Viewports::Player3]);
			SetCameraConstantBuffer(Viewports::Player3);

			m_DebugDrawer.Draw();

			m_renderContext->SetViewport(m_CameraManager.m_Viewports[Viewports::Player4]);
			SetCameraConstantBuffer(Viewports::Player4);

			m_DebugDrawer.Draw();
		}*/
	}

	//Set things back to normal rendering
	//Triangle topology
	m_renderContext->SetPrimitiveTopology(PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Activate culling, blending and depth testing
	m_renderContext->SetCullMode(CullMode::CULL_MODE_BACK_FACE);
	m_renderContext->SetBlendMode(BlendMode::BLEND_MODE_NONE);

	m_renderContext->BindVertexShader(ObjectHandle::Null());
	m_renderContext->BindPixelShader(ObjectHandle::Null());

	m_renderContext->SetDepthTesting(true);
	m_renderContext->SetDepthWriting(true);
}

void SkyeGraphics::SetFillMode(const Skye::FillMode fillMode)
{
	m_renderContext->SetFillMode(fillMode);
}

void SkyeGraphics::RegisterRenderable(Base::Handle const& handle)
{
	m_RenderableObjects.push_back(handle);
}

void SkyeGraphics::UnregisterRenderable(Base::Handle const& handle)
{
	auto newEnd = std::remove_if(m_RenderableObjects.begin(), m_RenderableObjects.end(), [&](Base::Handle const& currHandle)
	{
		if (currHandle == handle)
			return true;
		else
			return false;
	});

	m_RenderableObjects.erase(newEnd, m_RenderableObjects.end());
}

Skye::DebugDrawer& SkyeGraphics::GetDebugDrawer()
{
	return m_DebugDrawer;
}

void SkyeGraphics::ToggleDebugDraw()
{
	m_DrawDebug = !m_DrawDebug;
}

bool SkyeGraphics::IsDebugDrawOn(void)
{
	return m_DrawDebug;
}

void SkyeGraphics::ClearBuffers()
{
	m_renderContext->ClearDepthBuffer(m_CameraManager.m_DepthBuffer);
	m_renderContext->ClearDepthBuffer(m_CameraManager.m_DepthBufferCopy);
}

void SkyeGraphics::UpdateCameraManager()
{
	UpdateCamera(m_CameraManager.GetActiveCameraID());
	m_CameraManager.m_Mat2DBuffer.projection.SetOrthographicProjection(m_CameraManager.m_OrthographicProjectionSize.x, m_CameraManager.m_OrthographicProjectionSize.y, 0.0f, 1.0f);

	float aspectRatio = static_cast<float>(m_CameraManager.m_Viewport.size.width) / static_cast<float>(m_CameraManager.m_Viewport.size.height);


	for (unsigned i = 0; i < m_CameraManager.m_Cameras.size(); ++i)
	{
		float fovY = 2.0f * atan(tan(m_FovY / 2.0f) / aspectRatio);
		m_CameraManager.m_Cameras[i].SetProjectionMatrix(fovY, aspectRatio, m_EffectConstants.viewPlanes.x, m_EffectConstants.viewPlanes.y);
		m_CameraManager.m_Cameras[i].SetWidth(static_cast<float>(m_CameraManager.m_Viewport.size.width));
		m_CameraManager.m_Cameras[i].SetHeight(static_cast<float>(m_CameraManager.m_Viewport.size.height));
		m_CameraManager.m_Cameras[i].SetFOV(m_FovY);
		m_CameraManager.m_Cameras[i].SetNear(m_EffectConstants.viewPlanes.x);
		m_CameraManager.m_Cameras[i].SetFar(150.0);
	}

	Skye::Camera& camera = m_CameraManager.GetActiveCamera();

	m_renderContext->SetViewport(m_CameraManager.GetViewport());
}

void SkyeGraphics::UpdateCamera(unsigned ID)
{
	Skye::Camera& camera = m_CameraManager.m_Cameras[ID];

	m_DebugDrawer.SetEditorCamera(&camera);

	////Draw debug data for cameras
	//for (unsigned i = 0; i < m_CameraManager.m_Cameras.size(); ++i)
	//{
	//	if (m_CameraManager.GetActiveCameraID() != i)
	//	{
	//		m_DebugDrawer.DrawSphere(m_CameraManager.m_Cameras[i].GetPosition(), 3.0f, Color::s_Blue);
	//
	//		
	//		m_DebugDrawer.DrawFrustum(m_CameraManager.m_Cameras[i].GetFrustum().GetCorners(), Color::s_Yellow);
	//		float3 look = float3(m_CameraManager.m_Cameras[i].GetLookAt().x, m_CameraManager.m_Cameras[i].GetLookAt().y, m_CameraManager.m_Cameras[i].GetLookAt().z);
	//		m_DebugDrawer.DrawLine(m_CameraManager.m_Cameras[i].GetPosition(), m_CameraManager.m_Cameras[i].GetPosition() + look * 5, Color::s_Yellow);
	//		
	//	}
	//}
	//
	camera.Update();

  // if we are turning in the game with an installer
  // we shouldn't let the camera take control, player must have control
  if (Config::installer || !InGameEditor::IsEditorActive())
    return;

  //if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_W))
  //{
	 // camera.Walk(.1);
  //}

  //if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_S))
  //{
	 // camera.Walk(-.1);
  //}

  //if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_A))
  //{
	 // camera.Strafe(-.1);
  //}

  //if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_D))
  //{
	 // camera.Strafe(.1);
  //}

  //if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_Q))
  //{
	 // camera.Elevate(.1);
  //}

  //if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_E))
  //{
	 // camera.Elevate(-.1);
  //}

  //if (Input::MouseEvent::IsRightMouseButtonHeld())
  //{
	 // // Make each pixel correspond to a quarter of a degree.
	 // float dx = DirectX::XMConvertToRadians(0.25f*static_cast<float>(Input::MouseEvent::GetMouseDelta().x));
	 // float dy = DirectX::XMConvertToRadians(0.25f*static_cast<float>(Input::MouseEvent::GetMouseDelta().y));

	 // m_CameraManager.GetActiveCamera().Pitch(dy);
	 // m_CameraManager.GetActiveCamera().RotateY(dx);
  //}


	if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_1))
	{
		m_CameraManager.SetActiveCamera(0);
	}

	if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_2))
	{
		m_CameraManager.SetActiveCamera(1);
	}

	if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_3))
	{
		m_CameraManager.SetActiveCamera(2);
	}

	if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_4))
	{
		m_CameraManager.SetActiveCamera(3);
	}

	if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_5))
	{
		m_CameraManager.SetActiveCamera(4);
	}

  if(Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_W))
  {
  	camera.MoveAlongZ(1);
  }
  
  if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_S))
  {
  	camera.MoveAlongZ(-1);
  }
  
  if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_A))
  {
  	camera.MoveAlongX(-1);
  }
  
  if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_D))
  {
  	camera.MoveAlongX(1);
  }
  
  if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_Q))
  {
  	camera.MoveAlongY(1);
  }
  
  if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_E))
  {
  	camera.MoveAlongY(-1);
  }
  
  if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_Z))
  {
  	camera.RotateAlongY(0.05f);
  }
  
  if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_X))
  {
  	camera.RotateAlongY(-0.05f);
  }
  
  if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_C))
  {
  	camera.RotateAlongX(0.05f);
  }
  
  if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_V))
  {
  	camera.RotateAlongX(-0.05f);
  }
  
  if (Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_SPACE))
  {
  	camera.StabilizeCamera();
  }
}

void SkyeGraphics::SetCameraConstantBuffer(unsigned ID)
{
	Skye::Camera camera = m_CameraManager.m_Cameras[ID];

	m_Constants.eyePosition = camera.GetPosition();

	m_CameraManager.m_Mat3DBuffer.view = m_CameraManager.m_Mat3DBuffer.viewInverse = camera.WorldToCamera();
	m_CameraManager.m_Mat3DBuffer.projection = m_CameraManager.m_Mat3DBuffer.projectionInverse = camera.GetProjectionMatrix();


	/*m_CameraManager.m_Mat3DBuffer.view = m_CameraManager.m_Mat3DBuffer.viewInverse = camera.GetView();
	m_CameraManager.m_Mat3DBuffer.projection = m_CameraManager.m_Mat3DBuffer.projectionInverse = camera.GetProjection();
*/
	//m_CameraManager.m_Mat3DBuffer.view.Identity();
	m_CameraManager.m_Mat3DBuffer.viewProjection = m_CameraManager.m_Mat3DBuffer.projection * m_CameraManager.m_Mat3DBuffer.view;

	m_renderContext->CopyData(m_ConstantBuffers[ConstantBuffer::Matrix2D], &m_CameraManager.m_Mat2DBuffer, sizeof(ConstantBuffer::Matrix2DBuffer));
	m_renderContext->CopyData(m_ConstantBuffers[ConstantBuffer::Matrix3D], &m_CameraManager.m_Mat3DBuffer, sizeof(ConstantBuffer::Matrix3DBuffer));
}

void SkyeGraphics::ResetScreen()
{
	bool fullscreen = true;

	const Skye::Area resolution = Skye::Area(WINDOWSSYSTEM->GetScreenWidth(), WINDOWSSYSTEM->GetScreenHeight());


	const Skye::Area splitScreenResolution = Skye::Area(WINDOWSSYSTEM->GetScreenWidth() / 2, WINDOWSSYSTEM->GetScreenHeight() / 2);

	float aspectRatio = static_cast<float>(resolution.width) / static_cast<float>(resolution.height);

	m_CameraManager.m_OrthographicProjectionSize.y = m_OrthographicProjectionSize.y;

	m_CameraManager.m_OrthographicProjectionSize.x = m_OrthographicProjectionSize.y * aspectRatio;

	m_CameraManager.m_Viewport.SetViewport(0, 0, resolution);

	//TOP LEFT
	//m_CameraManager.m_Viewports[Viewports::Player1].SetViewport(0, 0, splitScreenResolution);

	//TOP RIGHT
	//m_CameraManager.m_Viewports[Viewports::Player2].SetViewport(splitScreenResolution.width, 0, splitScreenResolution);

	//BOTTOM LEFT
	//m_CameraManager.m_Viewports[Viewports::Player3].SetViewport(0, splitScreenResolution.height, splitScreenResolution);

	//BOTTOM RIGHT
	//m_CameraManager.m_Viewports[Viewports::Player4].SetViewport(splitScreenResolution.width, splitScreenResolution.height, splitScreenResolution);

	//FULL SCREEN
	m_CameraManager.m_Viewports[Viewports::FullScreen].SetViewport(0, 0, resolution);
	
	OnResize();
}

void SkyeGraphics::Render3DObjects()
{

	//if (InGameEditor::IsEditorActive())
	{
		RenderSingleScreen();
	}
	//else
	{
	//	RenderSplitScreen();
	}
}

void SkyeGraphics::RenderPass()
{
	for (auto it = Core::ENGINE->Spaces().begin(); it != Core::ENGINE->Spaces().end(); ++it)
	{
		//Esteban - just a suggestion for rendering, basically if a space is meant to be hidden then
		// don't render it
		//////////////////////////////////////////////////////////////////////////
		if ((*it)->Hidden())
		{
			continue;
		}
		//////////////////////////////////////////////////////////////////////////

		Base::ObjectAllocator* renderables = (*it)->GetComponents(eRenderable);
		
		Base::ObjectAllocator* capsuleColliders = (*it)->GetComponents(eCapsuleCollider);
		
		Base::ObjectAllocator* sphereColliders = (*it)->GetComponents(eSphereCollider);
		
		Base::ObjectAllocator* boxColliders = (*it)->GetComponents(eBoxCollider);

		if (m_DrawDebug)
		{
			//DRAW BOX COLLIDERS
			for (auto rt = boxColliders->begin<Physics::BoxCollider>(); rt != boxColliders->end < Physics::BoxCollider>(); ++rt)
			{
				Core::Transform* trans = dynamic_cast<Core::Transform*>(rt->GetOwner()->GetComponent(eTransform));
				if (trans)
				{
					m_DebugDrawer.DrawBox(rt->GetExtents(), trans, Color::s_Yellow);
				}
			}
			
			//DRAW SPHERE COLLIDERS
			for (auto rt = sphereColliders->begin<Physics::SphereCollider>(); rt != sphereColliders->end < Physics::SphereCollider>(); ++rt)
			{
				Core::Transform* trans = dynamic_cast<Core::Transform*>(rt->GetOwner()->GetComponent(eTransform));
				if (trans)
				{
					float3 position = float3(trans->GetTranslation().x, trans->GetTranslation().y, trans->GetTranslation().z);
					
					m_DebugDrawer.DrawSphere(position, rt->GetRadius(), Color::s_Yellow);
				}
			}
			
			//DRAW CAPSULE COLLIDERS
			for (auto rt = capsuleColliders->begin<Physics::CapsuleCollider>(); rt != capsuleColliders->end < Physics::CapsuleCollider>(); ++rt)
			{
				Core::Transform* trans = dynamic_cast<Core::Transform*>(rt->GetOwner()->GetComponent(eTransform));
				
				if (trans)
				{
					float4x4 world = trans->GetRotationMatrix();
					
					float4 point0 = (world * rt->GetBottomPoint()) + trans->GetTranslation();
					float4 point1 = (world * rt->GetTopPoint()) + trans->GetTranslation();
					
					m_DebugDrawer.DrawCapsule(point0, point1, rt->GetRadius(), Color::s_Yellow);
				}
			}
		}
			
		//RENDER OBJECTS
		for (auto rt = renderables->begin<Renderable>(); rt != renderables->end<Renderable>(); ++rt)
		{
			if (!rt->IsVisible())
				continue;

			Skye::Model* model = m_ModelManager.GetModel(rt->GetModelName());
			
			if (model)
			{
				//If there is an emissive component, set it's color in the constant buffer
				EmissiveComponent* emissive = dynamic_cast<EmissiveComponent*>(rt->GetOwner()->GetComponent(eEmissiveComponent));
				if (emissive)
				{

					emissive->Update();

					Fade* fade = dynamic_cast<Fade*>(rt->GetOwner()->GetComponent(eFade));

					if (fade)
						m_Constants.emissiveColor = emissive->GetEmittedColor() * fade->GetOpacity();
					else
						m_Constants.emissiveColor = emissive->GetEmittedColor();
				}
				else
					m_Constants.emissiveColor = float4();

				Levitate* levitation = dynamic_cast<Levitate*>(rt->GetOwner()->GetComponent(eLevitate));

				//Add the levitation offset
				if (levitation)
					m_Constants.offset = levitation->GetOffset();
				else
					m_Constants.offset = 0.0f;

				Core::Transform* trans = rt->GetTransform();
				
				Renderable::WorldMatrix matrix;
				
				trans->UpdateWorldMatrix();
				matrix.m_WorldMatrix = trans->GetWorldMatrix();
				
				for (unsigned i = 0; i < model->m_DXDataList.size(); ++i)
				{
					m_renderContext->SetViewport(m_CameraManager.m_Viewports[Viewports::FullScreen]);
					SetCameraConstantBuffer(Viewports::FullScreen);

					m_renderContext->BindVertexBuffer(model->m_DXDataList[i].m_VertexBuffer, sizeof(Vertex3D));
					m_renderContext->BindIndexBuffer(model->m_DXDataList[i].m_IndexBuffer);

					m_renderContext->BindTexture(0, GetTexture(rt->GetTextureName()));
					m_renderContext->BindTexture(1, GetTexture(rt->GetNormalTextureName()));
					m_renderContext->BindTexture(2, GetTexture(rt->GetEmissiveTextureName()));
					
					m_Constants.isSkydome = 0.0f;

					m_renderContext->CopyData(m_ConstantBuffers[ConstantBuffer::Object3D], &matrix, sizeof(Renderable::WorldMatrix));
					m_renderContext->CopyData(m_ConstantBuffers[ConstantBuffer::Constants], &m_Constants, sizeof(ConstantBuffer::ConstantsItems));
					m_renderContext->DrawIndexed(model->GetIndexCount(i));
				}
			}
		}
		RenderFontWrapperDrawCalls();
	}
}

void SkyeGraphics::RenderHelper(RenderableList& renderableList)
{
}

void SkyeGraphics::DeferredPass()
{

	m_renderContext->BindVertexShader(m_VertexShaders["Graphics3DVS.hlsl"]);
	m_renderContext->BindPixelShader(m_PixelShaders["Graphics3DPS.hlsl"]);


	//Array of texture handles for deferred pass
	ObjectHandle targets[]
	{
		m_RenderTargets.find("POSITION")->second,
		m_RenderTargets.find("NORMAL")->second,
		m_RenderTargets.find("DIFFUSED")->second,
		m_RenderTargets.find("SPECULAR")->second,
		m_RenderTargets.find("EMITTED")->second,
		m_RenderTargets.find("SKYDOME")->second,
	};

	//Clearing and binding textures as render targets;
	m_renderContext->ClearRenderTargets(6, targets, float4(0.0f, 0.0f, 0.0f, 0.0f));
	m_renderContext->BindRenderTargets(6, targets);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//No culling to draw the inside of the sphere
	m_renderContext->SetCullMode(CullMode::CULL_MODE_NONE);

	//Getting a sphere model
	Skye::Model* model = m_ModelManager.GetModel("Sphere");
	Core::Transform trans;

	//Set the transform for the skydome
	trans.SetTranslation(float4(0, 0, 0, 1));
	trans.SetScale(float4(10000, 10000, 10000, 0));
	float4x4 rotation;
	rotation.Identity();
	Renderable::WorldMatrix matrix;

	//float4 m_apexColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 m_apexColor = float4(0.0f, 0.05f, 0.6f, 1.0f);
	float4 m_centerColor = float4(0.0f, 0.5f, 0.8f, 1.0f);

	/*float4 m_apexColor = float4(0.0f, 0.15f, 0.66f, 1.0f);
	float4 m_centerColor = float4(0.81f, 0.38f, 0.66f, 1.0f);*/

	matrix.m_WorldMatrix = trans.GetWorldMatrix();

	//Binding vertex and index buffers of the model
	m_renderContext->BindVertexBuffer(model->m_DXDataList[0].m_VertexBuffer, sizeof(Vertex3D));

	m_renderContext->BindIndexBuffer(model->m_DXDataList[0].m_IndexBuffer);

	m_renderContext->BindTexture(0, GetTexture(""));

	m_Constants.isSkydome = 1.0f;

	m_Constants.apexColor = m_apexColor;

	m_Constants.centerColor = m_centerColor;
	//Pass the object data and constant buffers to the shader
	m_renderContext->CopyData(m_ConstantBuffers[ConstantBuffer::Object3D], &matrix, sizeof(Renderable::WorldMatrix));

	m_renderContext->CopyData(m_ConstantBuffers[ConstantBuffer::Constants], &m_Constants, sizeof(ConstantBuffer::ConstantsItems));

	m_renderContext->DrawIndexed(model->GetIndexCount(0));

	//Setting the culling to backface for the rest of the objects
	m_renderContext->SetCullMode(CullMode::CULL_MODE_BACK_FACE); 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	RenderPass();

	Skye::ObjectHandle nullTextures[6] = { Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null() };
	m_renderContext->BindRenderTargets(6, nullTextures);
}

void SkyeGraphics::CompositePass()
{
	m_renderContext->BindVertexShader(m_VertexShaders["CompositeVS.hlsl"]);

  if (!Config::installer)
	  m_renderContext->BindPixelShader(m_PixelShaders["CompositePS.hlsl"]);
  else
    m_renderContext->BindPixelShader(m_PixelShaders["CompositePSDark.hlsl"]);


	m_renderContext->BindRenderTarget(m_renderContext->GetBackBuffer());

	m_renderContext->SetViewport(m_CameraManager.m_Viewports[Viewports::FullScreen]);

	Skye::Model* quad = m_ModelManager.GetModel("Quad");

	float4x4 world, scale, translate;
	world.Identity();
	
	m_renderContext->BindVertexBuffer(quad->m_DXDataList[0].m_VertexBuffer, sizeof(Vertex3D));

	m_renderContext->BindIndexBuffer(quad->m_DXDataList[0].m_IndexBuffer);

	m_renderContext->BindTexture(0, m_RenderTargets.find("POSITION")->second);
	m_renderContext->BindTexture(1, m_RenderTargets.find("NORMAL")->second);
	m_renderContext->BindTexture(2, m_RenderTargets.find("DIFFUSED")->second);
	m_renderContext->BindTexture(3, m_RenderTargets.find("SKYDOME")->second);
	m_renderContext->BindTexture(4, m_RenderTargets.find("EMITTED")->second);
	m_renderContext->BindTexture(5, m_RenderTargets.find("DEBUGDRAW")->second);
	m_renderContext->BindTexture(6, m_RenderTargets.find("LIGHT")->second);
	m_renderContext->BindTexture(7, m_RenderTargets.find("PARTICLE")->second);

	m_renderContext->CopyData(m_ConstantBuffers[ConstantBuffer::Object3D], &world, sizeof(Renderable::WorldMatrix));


	m_renderContext->DrawIndexed(quad->GetIndexCount(0));

	Skye::ObjectHandle nullTextures[8] = { Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null() };
	m_renderContext->BindTextures(8, nullTextures);

	m_renderContext->BindRenderTarget(ObjectHandle::Null());
}

void SkyeGraphics::RenderSplitScreen()
{

	////Binding deferred shaders
	//m_renderContext->BindVertexShader(m_VertexShaders["Graphics3DVS.hlsl"]);
	//m_renderContext->BindPixelShader(m_PixelShaders["Graphics3DPS.hlsl"]);

	////Array of texture handles for deferred pass
	//ObjectHandle targets[]
	//{
	//	m_RenderTargets.find("POSITION")->second,
	//	m_RenderTargets.find("NORMAL")->second,
	//	m_RenderTargets.find("DIFFUSED")->second,
	//	m_RenderTargets.find("SPECULAR")->second,
	//	m_RenderTargets.find("EMITTED")->second,
	//	m_RenderTargets.find("SKYDOME")->second,
	//};

	////Clearing and binding textures as render targets;
	//m_renderContext->ClearRenderTargets(6, targets, float4(0.0f, 0.0f, 0.0f, 0.0f));
	//m_renderContext->ClearRenderTarget(m_RenderTargets.find("LIGHT")->second, float4());
	//m_renderContext->ClearRenderTarget(m_RenderTargets.find("PARTICLE")->second, float4());
	//m_renderContext->BindRenderTargets(6, targets);

	//Skye::ObjectHandle nullTextures[6] = { Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null() };

	//m_renderContext->SetViewport(m_CameraManager.m_Viewports[Viewports::Player1]);
	//SetCameraConstantBuffer(Viewports::Player1);
	//SplitScreenDeferred();
	//
	//m_renderContext->BindRenderTargets(6, nullTextures);

	//
	//PointLightPass();
	//ParticlePass();

	////Binding deferred shaders
	//m_renderContext->BindVertexShader(m_VertexShaders["Graphics3DVS.hlsl"]);
	//m_renderContext->BindPixelShader(m_PixelShaders["Graphics3DPS.hlsl"]);
	//m_renderContext->BindRenderTargets(6, targets);
	//m_renderContext->SetViewport(m_CameraManager.m_Viewports[Viewports::Player2]);
	//SetCameraConstantBuffer(Viewports::Player2);
	//SplitScreenDeferred();
	//

	//m_renderContext->BindRenderTargets(6, nullTextures);
	//
	//PointLightPass();
	//ParticlePass();

	//m_renderContext->BindVertexShader(m_VertexShaders["Graphics3DVS.hlsl"]);
	//m_renderContext->BindPixelShader(m_PixelShaders["Graphics3DPS.hlsl"]);
	//m_renderContext->BindRenderTargets(6, targets);
	//m_renderContext->SetViewport(m_CameraManager.m_Viewports[Viewports::Player3]);
	//SetCameraConstantBuffer(Viewports::Player3);
	//SplitScreenDeferred();
	//
	//m_renderContext->BindRenderTargets(6, nullTextures);

	//PointLightPass();
	//ParticlePass();

	//m_renderContext->BindVertexShader(m_VertexShaders["Graphics3DVS.hlsl"]);
	//m_renderContext->BindPixelShader(m_PixelShaders["Graphics3DPS.hlsl"]);
	//m_renderContext->BindRenderTargets(6, targets);
	//m_renderContext->SetViewport(m_CameraManager.m_Viewports[Viewports::Player4]);
	//SetCameraConstantBuffer(Viewports::Player4);
	//SplitScreenDeferred();

	//m_renderContext->BindRenderTargets(6, nullTextures);

	//PointLightPass();
	//ParticlePass();
	//
	//CompositePass();
	//
	//if (Config::installer)
	//	AwesomiumPass();

}

void SkyeGraphics::RenderSingleScreen()
{
	m_renderContext->ClearRenderTarget(m_RenderTargets.find("LIGHT")->second, float4());

	m_renderContext->ClearRenderTarget(m_RenderTargets.find("PARTICLE")->second, float4());
	m_renderContext->SetViewport(m_CameraManager.m_Viewports[Viewports::FullScreen]);
	
	SetCameraConstantBuffer(Viewports::FullScreen);
	
	DeferredPass();

	PointLightPass();

	ParticlePass();

	CompositePass();

	if (Config::installer)
		AwesomiumPass();
}

void SkyeGraphics::SplitScreenDeferred()
{
	//No culling to draw the inside of the sphere
	//m_renderContext->SetCullMode(CullMode::CULL_MODE_NONE);
  //
  //
	////Getting a sphere model
	//Skye::Model* model = m_ModelManager.GetModel("Sphere");
	//Core::Transform trans;
  //
	////Set the transform for the skydome
	//trans.SetTranslation(float4(0, 0, 0, 1));
	//trans.SetScale(float4(10000, 10000, 10000, 0));
	//float4x4 rotation;
	//rotation.Identity();
	//Renderable::WorldMatrix matrix;
  //
	//matrix.m_WorldMatrix = trans.GetWorldMatrix();
  //
	////Binding vertex and index buffers of the model
	//m_renderContext->BindVertexBuffer(model->GetVertexBuffer(), sizeof(Vertex3D));
  //
	//m_renderContext->BindIndexBuffer(model->GetIndexBuffer());
  //
	//m_renderContext->BindTexture(0, GetTexture(""));
  //
	//m_Constants.isSkydome = 1.0f;
  //
	////Pass the object data and constant buffers to the shader
	//m_renderContext->CopyData(m_ConstantBuffers[ConstantBuffer::Object3D], &matrix, sizeof(Renderable::WorldMatrix));
  //
	//m_renderContext->CopyData(m_ConstantBuffers[ConstantBuffer::Constants], &m_Constants, sizeof(ConstantBuffer::ConstantsItems));
  //
	//m_renderContext->DrawIndexed(model->GetIndexCount());

	//Setting the culling to backface for the rest of the objects
	m_renderContext->SetCullMode(CullMode::CULL_MODE_BACK_FACE);

	RenderPass();
}

void SkyeGraphics::SplitScreenComposite()
{

}


void SkyeGraphics::AwesomiumPass()
{
	m_renderContext->SetDepthTesting(false);
	m_renderContext->SetDepthWriting(false);
	m_renderContext->SetBlendMode(BlendMode::BLEND_MODE_ALPHA);
	if (m_UITexture)
	{
		m_renderContext->BindVertexShader(m_VertexShaders["UserInterfaceVS.hlsl"]);
		m_renderContext->BindPixelShader(m_PixelShaders["UserInterfacePS.hlsl"]);
		
		m_renderContext->BindRenderTarget(m_renderContext->GetBackBuffer());
		m_renderContext->SetViewport(m_CameraManager.m_Viewports[Viewports::FullScreen]);

		Skye::Model* quad = m_ModelManager.GetModel("Quad");

		float4x4 world;
		world.Identity();

		m_renderContext->BindVertexBuffer(quad->m_DXDataList[0].m_VertexBuffer, sizeof(Vertex3D));

		m_renderContext->BindIndexBuffer(quad->m_DXDataList[0].m_IndexBuffer);

		m_renderContext->BindTexture(0, m_UITexture);

		m_renderContext->CopyData(m_ConstantBuffers[ConstantBuffer::Object3D], &world, sizeof(Renderable::WorldMatrix));

		m_renderContext->DrawIndexed(quad->GetIndexCount(0));

		m_renderContext->BindTexture(0, ObjectHandle::Null());

		m_renderContext->BindRenderTarget(ObjectHandle::Null());
	}
	m_renderContext->SetBlendMode(BlendMode::BLEND_MODE_NONE);
	m_renderContext->SetDepthTesting(true);
	m_renderContext->SetDepthWriting(true);
}

void SkyeGraphics::ParticlePass()
{

	//m_renderContext->SetDepthTesting(false);
	//m_renderContext->SetDepthWriting(false);

  //Bind the right shaders for the particles...
	m_renderContext->BindVertexShader(m_VertexShaders["particleSystemVS.hlsl"]);
	m_renderContext->BindPixelShader(m_PixelShaders["particleSystemPS.hlsl"]);

	m_renderContext->BindRenderTarget(m_RenderTargets.find("PARTICLE")->second);
  m_renderContext->BindIndexBuffer(ParticleSystem::GetIndexBufferHandle());

  m_renderContext->SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  m_renderContext->BindNullVertexBuffer();

	for (auto it = Core::ENGINE->Spaces().begin(); it != Core::ENGINE->Spaces().end(); ++it)
	{
		Base::ObjectAllocator* particleSystems = (*it)->GetComponents(eParticleSystem);

		//RENDER PARTICLE SYSTEM
		for (auto ps = particleSystems->begin<ParticleSystem>(); ps != particleSystems->end<ParticleSystem>(); ++ps)
		{
      m_renderContext->SetBlendMode((Skye::BlendMode)ps->m_particleBlendMode);

			//if the system is visible and has 1 particle or more
			if (ps->m_systemIsVisible && ps->m_particleCount > 0)
			{
				//If the system is not looping then check if it's lifetime has run out
				if (!ps->m_systemIsLooping && ps->m_systemLifeTime <= 0)
					continue;
        
        if (!(ps->m_systemIsPaused || GameLogic::GAMELOGIC_SYSTEM->GetPaused()) )
				  ps->UpdateParticles(Core::FRAMERATECONTROLLER->GetDT());
				
        m_renderContext->BindTexture(0, ps->GetTextureHandle());

        //the particles constant buffer is already bound on index 4
				m_renderContext->CopyData(m_ConstantBuffers[ConstantBuffer::Particles],
					&ParticleSystem::m_particleConstBuffData, sizeof(Particle_HLSLBuffer));

   			m_renderContext->DrawIndexed(ps->GetParticleIndexCount());
			}
		}
	}

	m_renderContext->SetDepthTesting(true);
	m_renderContext->SetDepthWriting(true);
}

void SkyeGraphics::LoadingScreen()
{
	//Create vertex layout input for shader
	Skye::ObjectHandle loadingVS, loadingPS, texture;

	Skye::InputLayout vertex3DLayout;

	//3D Vertex
	vertex3DLayout.push_back(Skye::InputData("POSITION", Skye::DataFormat::FLOAT3));
	vertex3DLayout.push_back(Skye::InputData("NORMAL", Skye::DataFormat::FLOAT3));
	vertex3DLayout.push_back(Skye::InputData("TANGENT", Skye::DataFormat::FLOAT3));
	vertex3DLayout.push_back(Skye::InputData("BITANGENT", Skye::DataFormat::FLOAT3));
	vertex3DLayout.push_back(Skye::InputData("TEXTURE", Skye::DataFormat::FLOAT2));
	vertex3DLayout.push_back(Skye::InputData("COLOR", Skye::DataFormat::FLOAT4));

	//Create vertex shader for the loading screen
	m_renderContext->CreateVertexShader(loadingVS, "shaders/LoadingVS.hlsl", vertex3DLayout, false);

	//Create pixel shader for the screen
	m_renderContext->CreatePixelShader(loadingPS, "shaders/LoadingPS.hlsl", false);

	//Create the quad model for rendering the quad onto the screen
	PrimitiveGenerator geoGen;

	Model* quad = new Model();

	geoGen.CreateFullscreenQuad(quad->m_ModelData);

	quad->m_DataList.push_back(quad->m_ModelData);

	quad->GenerateBuffers(m_renderContext.get());

	//Load texture to display
	m_renderContext->CreateTexture(texture, "textures/loading/LoadingScreen.png");

	//Bind the appropriate shaders
	m_renderContext->BindVertexShader(loadingVS);
	m_renderContext->BindPixelShader(loadingPS);

	//Set the back buffer as the render target
	m_renderContext->BindRenderTarget(m_renderContext->GetBackBuffer());

	//Set the fullscreen viewport
	m_renderContext->SetViewport(m_CameraManager.m_Viewports[Viewports::FullScreen]);

	//Get identity matrix
	float4x4 world;
	world.Identity();

	//Bind the quad's vertex and index buffers
	m_renderContext->BindVertexBuffer(quad->m_DXDataList[0].m_VertexBuffer, sizeof(Vertex3D));

	m_renderContext->BindIndexBuffer(quad->m_DXDataList[0].m_IndexBuffer);

	//Bind loading texture as a resource
	m_renderContext->BindTexture(0, texture);

	//Set the constant buffer
	m_renderContext->CopyData(m_ConstantBuffers[ConstantBuffer::Object3D], &world, sizeof(Renderable::WorldMatrix));

	//Render the quad
	m_renderContext->DrawIndexed(quad->GetIndexCount(0));

	//Unbind the resource texture
	m_renderContext->BindTexture(0, Skye::ObjectHandle::Null());

	//Unbind the shaders
	m_renderContext->BindVertexShader(Skye::ObjectHandle::Null());
	m_renderContext->BindPixelShader(Skye::ObjectHandle::Null());

	//Unbind the back buffer as a render target
	m_renderContext->BindRenderTarget(ObjectHandle::Null());

	//Release shaders
	m_renderContext->Release(loadingVS);
	m_renderContext->Release(loadingPS);

	//Release quad
	delete quad;
}

//Quad struct for the light geometry shader
static const float2 g_LightQuadPosition[4] =
{
	float2(-0.5, 0.5),
	float2(0.5, 0.5),
	float2(-0.5, -0.5),
	float2(0.5, -0.5)
};


void SkyeGraphics::CalculateBillboardQuad(PointLight* light)
{
	float4 viewPos = Transpose(m_CameraManager.m_Mat3DBuffer.view) * light->GetTransform()->GetTranslation();

	for (int i = 0; i < 3; ++i)
	{
		PointLightData lightData;
		lightData.lightColor = light->GetLightColor();
		lightData.rangeAttenuation = light->GetRangeAttenuation();
		lightData.position = viewPos + (float4(g_LightQuadPosition[i].x, g_LightQuadPosition[i].y, 0, 0) * lightData.rangeAttenuation.x * 2);
		lightData.center = light->GetTransform()->GetTranslation();
		m_PointLights.push_back(lightData);
	}

	for (int i = 1; i < 4; ++i)
	{
		PointLightData lightData;
		lightData.lightColor = light->GetLightColor();
		lightData.rangeAttenuation = light->GetRangeAttenuation();
		lightData.position = viewPos + (float4(g_LightQuadPosition[i].x, g_LightQuadPosition[i].y, 0, 0) * lightData.rangeAttenuation.x * 2);
		lightData.center = light->GetTransform()->GetTranslation();

		m_PointLights.push_back(lightData);
	}
}

void SkyeGraphics::PointLightPass()
{
	//Populate the list of lights to be drawn
	PopulatePointLights();

	//Bind the proper shaders
	m_renderContext->BindVertexShader(m_VertexShaders["PointLightVS.hlsl"]);
	m_renderContext->BindGeometryShader(m_GeometryShaders["PointLightGS.hlsl"]);
	m_renderContext->BindPixelShader(m_PixelShaders["PointLightPS.hlsl"]);

	//Bind the proper texture resources in order to do light pass
	m_renderContext->BindTexture(0, m_RenderTargets.find("POSITION")->second);
	m_renderContext->BindTexture(1, m_RenderTargets.find("NORMAL")->second);
	m_renderContext->BindTexture(2, m_RenderTargets.find("DIFFUSED")->second);
	m_renderContext->BindTexture(3, m_RenderTargets.find("SKYDOME")->second);

	//Lights are deawn as a point list
	m_renderContext->SetPrimitiveTopology(PrimitiveTopology::PRIMITIVE_TOPOLOGY_POINTLIST);

	//Set the proper cull and blend modes
	m_renderContext->SetCullMode(CullMode::CULL_MODE_NONE);
	m_renderContext->SetBlendMode(BlendMode::BLEND_MODE_ADDITIVE);

	//Remove depth testing and writing
	m_renderContext->SetDepthTesting(false);
	m_renderContext->SetDepthWriting(false);

	//Set render target to light render target
	m_renderContext->BindRenderTarget(m_RenderTargets.find("LIGHT")->second);

	//Reserve the necessary memory for the number of lighs to draw per pass
	m_PointLights.reserve(MAX_LIGHTS);

	unsigned index = 0;

	//While the current index is under the number of lights to draw
	while (index < PointLightList.size())
	{
		m_PointLights.clear();

		//start at the current index and iterate for the number of maximum lights per pass
		for (unsigned i = index; i < index + MAX_LIGHTS; ++i)
		{
			//If we have reached the end of the list (under the max light limit)
			if (PointLightList.size() == i)
			{
				//Early exit
				break;
			}

			//Get the point light data that will be passed to the shader 
			PointLightData pointLight;

			pointLight.position = PointLightList[i]->GetTransform()->GetTranslation();

			float2 rangeAttenuation;

			//Modified range (in case of flicker or death offsets are acting on it)
			rangeAttenuation.x = PointLightList[i]->GetRangeAttenuation().x + PointLightList[i]->GetOffset() + PointLightList[i]->GetDeathOffset().x;

			//Modified attenuation (in case of death offset acting on it)
			rangeAttenuation.y = PointLightList[i]->GetRangeAttenuation().y + PointLightList[i]->GetDeathOffset().y;

			//Set range and attenuation that will be used in the shader
			pointLight.rangeAttenuation = rangeAttenuation;

			pointLight.lightColor = PointLightList[i]->GetLightColor();

			m_PointLights.push_back(pointLight);

		}

		//Copy light data to the buffer for the shader
		m_renderContext->CopyData(m_LightVertexBuffer, &m_PointLights[0], sizeof(PointLightData) * MAX_LIGHTS);

		//Bind buffers
		m_renderContext->BindVertexBuffer(m_LightVertexBuffer, sizeof(PointLightData));

		//Draw lights
		m_renderContext->Draw(m_PointLights.size());

		//Increment the index by the number of maximum lights to draw per pass
		index += MAX_LIGHTS;
	}

	//Unbind render target and geometry shader
	m_renderContext->BindRenderTarget(Skye::ObjectHandle::Null());
	m_renderContext->BindGeometryShader(Skye::ObjectHandle::Null());

	//Turn depth testing back on
	m_renderContext->SetDepthTesting(true);
	m_renderContext->SetDepthWriting(true);

	//Turn off blending
	m_renderContext->SetBlendMode(BlendMode::BLEND_MODE_NONE);

	//Set topology back to triangle list
	m_renderContext->SetPrimitiveTopology(PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Unbind texture resources
	Skye::ObjectHandle nullTextures[4] = { Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null(), Skye::ObjectHandle::Null() };

	m_renderContext->BindTextures(4, nullTextures);
}

void SkyeGraphics::PopulatePointLights()
{
	PointLightList.clear();

	//For all spaces
	for (auto it = Core::ENGINE->Spaces().begin(); it != Core::ENGINE->Spaces().end(); ++it)
	{
		//Get all point light objects in current space
		Base::ObjectAllocator* pointLights = (*it)->GetComponents(ePointLight);

		//Iterate through all lights
		for (auto rt = pointLights->begin<PointLight>(); rt != pointLights->end <PointLight>(); ++rt)
		{
			//if the current light is not null
			if (rt != nullptr)
			{
				//If we are doing debug draw
				if (!Config::installer)
				{
					//Get the position as a float3
					float3 position = float3(rt->GetTransform()->GetTranslation().x, rt->GetTransform()->GetTranslation().y, rt->GetTransform()->GetTranslation().z);

					//Draw sphere in position
					m_DebugDrawer.DrawSphere(position, 1.0f, rt->GetLightColor());
				}

				//Get flicker component in the object
				Flicker* flicker = dynamic_cast<Flicker*>(rt->GetOwner()->GetComponent(eFlicker));

				//If component exists, set the light's offset to the flicker offset
				if (flicker)
					rt->SetOffset(flicker->GetOffset());
					
				//Get death component
				LightDeath* death = dynamic_cast<LightDeath*>(rt->GetOwner()->GetComponent(eLightDeath));

				//If the component exists, set the death offset
				if (death)
					rt->SetDeathOffset(death->GetOffset());

				//Add light to the draw list
				PointLightList.push_back(rt);
			}
		}
	}
}

void SkyeGraphics::InitializeConstantBuffers()
{
	m_renderContext->CreateConstantBuffer(m_ConstantBuffers[ConstantBuffer::Matrix3D], sizeof(ConstantBuffer::Matrix3DBuffer));
	m_renderContext->CreateConstantBuffer(m_ConstantBuffers[ConstantBuffer::Object3D], sizeof(Renderable::WorldMatrix));
	m_renderContext->CreateConstantBuffer(m_ConstantBuffers[ConstantBuffer::Constants], sizeof(ConstantBuffer::ConstantsItems));
	m_renderContext->CreateConstantBuffer(m_ConstantBuffers[ConstantBuffer::Matrix2D], sizeof(ConstantBuffer::Matrix2DBuffer));
	m_renderContext->CreateConstantBuffer(m_ConstantBuffers[ConstantBuffer::Particles], sizeof(Particle_HLSLBuffer));
	
	m_renderContext->CreateVertexBuffer(m_LightVertexBuffer, BufferUsage::USAGE_DYNAMIC, sizeof(PointLightData) * MAX_LIGHTS);
	
	BindConstantBuffers();
}

void SkyeGraphics::BindConstantBuffers()
{
	m_renderContext->BindConstantBuffer(0, m_ConstantBuffers[ConstantBuffer::Matrix3D], Skye::ObjectType::VERTEX_SHADER);
	m_renderContext->BindConstantBuffer(1, m_ConstantBuffers[ConstantBuffer::Object3D], Skye::ObjectType::VERTEX_SHADER);
	m_renderContext->BindConstantBuffer(2, m_ConstantBuffers[ConstantBuffer::Constants], Skye::ObjectType::VERTEX_SHADER);
	m_renderContext->BindConstantBuffer(3, m_ConstantBuffers[ConstantBuffer::Matrix2D], Skye::ObjectType::VERTEX_SHADER);
	m_renderContext->BindConstantBuffer(4, m_ConstantBuffers[ConstantBuffer::Particles], Skye::ObjectType::VERTEX_SHADER);

	m_renderContext->BindConstantBuffer(0, m_ConstantBuffers[ConstantBuffer::Matrix3D], Skye::ObjectType::PIXEL_SHADER);
	m_renderContext->BindConstantBuffer(1, m_ConstantBuffers[ConstantBuffer::Object3D], Skye::ObjectType::PIXEL_SHADER);
	m_renderContext->BindConstantBuffer(2, m_ConstantBuffers[ConstantBuffer::Constants], Skye::ObjectType::PIXEL_SHADER);
	m_renderContext->BindConstantBuffer(3, m_ConstantBuffers[ConstantBuffer::Matrix2D], Skye::ObjectType::PIXEL_SHADER);
	m_renderContext->BindConstantBuffer(4, m_ConstantBuffers[ConstantBuffer::Particles], Skye::ObjectType::PIXEL_SHADER);
}

#define RENDER_TARGET(x) #x ,
static const char* renderTargetNames[] =
{
#include "Graphics/includes/GraphicsSystem/RenderTargets.h"
	nullptr,
};
#undef RENDER_TARGET

void SkyeGraphics::InitializeRenderTargets()
{
	for (int i = 0; renderTargetNames[i] != nullptr; ++i)
	{
		Skye::ObjectHandle rt;
		m_renderContext->CreateRenderTarget(rt, DataFormat::FLOAT4, m_CameraManager.m_Viewport.size);
		m_RenderTargets.insert(std::pair<std::string, Skye::ObjectHandle>(renderTargetNames[i], rt));
	}
	
	m_renderContext->CreateDepthBuffer(m_CameraManager.m_DepthBuffer, m_renderContext->GetBackBuffer());
	m_renderContext->CreateDepthBuffer(m_CameraManager.m_DepthBufferCopy, m_renderContext->GetBackBuffer());
}

void SkyeGraphics::InitializeDefaultAssets()
{
	const unsigned textureSize = 512;
	unsigned char *textureData = new unsigned char[textureSize * textureSize * 4];

	for (unsigned j = 0; j < textureSize; j++)
	{
		for (unsigned i = 0; i < textureSize; i++)
		{
			int c = (j ^ i) % 256;

			*(textureData + (j * textureSize * 4) + (i * 4) + 0) = 255 - c;
			*(textureData + (j * textureSize * 4) + (i * 4) + 1) = (c + 128);
			*(textureData + (j * textureSize * 4) + (i * 4) + 2) = c % 128;
			*(textureData + (j * textureSize * 4) + (i * 4) + 3) = 255;
		}
	}

	Skye::ObjectHandle defaultTexture;
	m_renderContext->CreateTexture(defaultTexture, textureData, Skye::Area(textureSize, textureSize), Skye::DataFormat::RGBA_UNORM);
	m_Textures.insert(std::pair<std::string, Skye::ObjectHandle>("", defaultTexture));

	delete[] textureData;
}

void SkyeGraphics::LoadVertexShaders()
{
	Skye::ObjectHandle graphicsVS, staticVS, compositeVS, graphics2DVS, lineVS, pointLightVS, userInterfaceVS, particleSystemVS;

	Skye::InputLayout vertex3DLayout, vertex2DLayout, lineLayout, lightLayout;

	//3D Vertex
	vertex3DLayout.push_back(Skye::InputData("POSITION", Skye::DataFormat::FLOAT3));
	vertex3DLayout.push_back(Skye::InputData("NORMAL", Skye::DataFormat::FLOAT3));
	vertex3DLayout.push_back(Skye::InputData("TANGENT", Skye::DataFormat::FLOAT3));
	vertex3DLayout.push_back(Skye::InputData("BITANGENT", Skye::DataFormat::FLOAT3));
	vertex3DLayout.push_back(Skye::InputData("TEXTURE", Skye::DataFormat::FLOAT2));
	vertex3DLayout.push_back(Skye::InputData("COLOR", Skye::DataFormat::FLOAT4));

	//2D Vertex
	vertex2DLayout.push_back(Skye::InputData("POSITION", Skye::DataFormat::FLOAT3));
	vertex2DLayout.push_back(Skye::InputData("COLOR", Skye::DataFormat::FLOAT4));
	vertex2DLayout.push_back(Skye::InputData("UV", Skye::DataFormat::FLOAT2));

	//Line Vertex
	lineLayout.push_back(Skye::InputData("POSITION", Skye::DataFormat::FLOAT4));
	lineLayout.push_back(Skye::InputData("COLOR", Skye::DataFormat::FLOAT4));

	//Point light
	lightLayout.push_back(Skye::InputData("COLOR", Skye::DataFormat::FLOAT4));
	lightLayout.push_back(Skye::InputData("POSITION", Skye::DataFormat::FLOAT4));
	lightLayout.push_back(Skye::InputData("CENTER", Skye::DataFormat::FLOAT4));
	lightLayout.push_back(Skye::InputData("RANGE", Skye::DataFormat::FLOAT2));
	lightLayout.push_back(Skye::InputData("PAD", Skye::DataFormat::FLOAT2));

	m_renderContext->CreateVertexShader(graphicsVS, "shaders/Graphics3DVS.hlsl", vertex3DLayout, false);
	m_renderContext->CreateVertexShader(compositeVS, "shaders/CompositeVS.hlsl", vertex3DLayout, false);
	m_renderContext->CreateVertexShader(graphics2DVS, "shaders/Graphics2DVS.hlsl", vertex2DLayout, false);
	m_renderContext->CreateVertexShader(lineVS, "shaders/LineVS.hlsl", lineLayout, false);
	m_renderContext->CreateVertexShader(pointLightVS, "shaders/PointLightVS.hlsl", lightLayout, false);
	m_renderContext->CreateVertexShader(userInterfaceVS, "shaders/UserInterfaceVS.hlsl", vertex3DLayout, false);
	m_renderContext->CreateVertexShader(particleSystemVS, "shaders/particleSystemVS.hlsl", vertex3DLayout, false);


	m_renderContext->CreateVertexShader(staticVS, "Shaders/Static3DVS.hlsl", vertex3DLayout, false);

	m_VertexShaders.insert(std::make_pair("Graphics3DVS.hlsl", graphicsVS));
	m_VertexShaders.insert(std::make_pair("CompositeVS.hlsl", compositeVS));
	m_VertexShaders.insert(std::make_pair("Graphics2DVS.hlsl", graphics2DVS));
	m_VertexShaders.insert(std::make_pair("LineVS.hlsl", lineVS));
	m_VertexShaders.insert(std::make_pair("PointLightVS.hlsl", pointLightVS));
	m_VertexShaders.insert(std::make_pair("UserInterfaceVS.hlsl", userInterfaceVS));
	m_VertexShaders.insert(std::make_pair("particleSystemVS.hlsl", particleSystemVS));


	m_VertexShaders.insert(std::make_pair("Static3DVS.hlsl", staticVS));
}

void SkyeGraphics::LoadGeometryShaders()
{
	Skye::ObjectHandle lightGS;
	m_renderContext->CreateGeometryShader(lightGS, "shaders/PointLightGS.hlsl", false);

	m_GeometryShaders.insert(std::make_pair("PointLightGS.hlsl", lightGS));
}

void SkyeGraphics::LoadPixelShaders()
{
	Skye::ObjectHandle graphicsPS, staticPS, compositePS, compositeDarkPS, graphics2DPS, linePS, pointLightPS, userInterfacePS, particleSystemPS;

	m_renderContext->CreatePixelShader(graphicsPS, "shaders/Graphics3DPS.hlsl", false);
	m_renderContext->CreatePixelShader(compositePS, "shaders/CompositePS.hlsl", false);
	m_renderContext->CreatePixelShader(compositeDarkPS, "shaders/CompositePSDark.hlsl", false);
	m_renderContext->CreatePixelShader(graphics2DPS, "shaders/Graphics2DPS.hlsl", false);
	m_renderContext->CreatePixelShader(linePS, "shaders/LinePS.hlsl", false);
	m_renderContext->CreatePixelShader(pointLightPS, "shaders/PointLightPS.hlsl", false);
	m_renderContext->CreatePixelShader(userInterfacePS, "shaders/UserInterfacePS.hlsl", false);
	m_renderContext->CreatePixelShader(particleSystemPS, "shaders/particleSystemPS.hlsl", false);

	m_renderContext->CreatePixelShader(staticPS, "Shaders/Static3DPS.hlsl", false);

	m_PixelShaders.insert(std::make_pair("Graphics3DPS.hlsl", graphicsPS));
	m_PixelShaders.insert(std::make_pair("CompositePS.hlsl", compositePS));
	m_PixelShaders.insert(std::make_pair("CompositePSDark.hlsl", compositeDarkPS));
	m_PixelShaders.insert(std::make_pair("Graphics2DPS.hlsl", graphics2DPS));
	m_PixelShaders.insert(std::make_pair("LinePS.hlsl", linePS));
	m_PixelShaders.insert(std::make_pair("PointLightPS.hlsl", pointLightPS));
	m_PixelShaders.insert(std::make_pair("UserInterfacePS.hlsl", userInterfacePS));
	m_PixelShaders.insert(std::make_pair("particleSystemPS.hlsl", particleSystemPS));

	m_PixelShaders.insert(std::make_pair("Static3DPS.hlsl", staticPS));
}

void SkyeGraphics::InitializeDefaultFontWrappers(void)
{
  Skye::ObjectHandle fontArial;
  std::string tempFontName("Arial");
  m_renderContext->CreateFontWrapper(fontArial, tempFontName);
}

void SkyeGraphics::DrawString(const std::string text, float fontSize, float xPos, float yPos, unsigned int color)
{
  m_renderContext->DrawString(text, fontSize, xPos, yPos, color);
}

void SkyeGraphics::RenderFontWrapperDrawCalls(void)
{
  m_renderContext->DrawFontWrapperDrawCalls();
}

void SkyeGraphics::ToggleViewports(bool bMultipleViewport)
{
  m_MultipleViewports = bMultipleViewport;
}

bool SkyeGraphics::RenderMultipleViewports(void) const
{
  return m_MultipleViewports;
}