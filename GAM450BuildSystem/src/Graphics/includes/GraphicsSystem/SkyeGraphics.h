/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: SkyeGraphics.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  SkyeGraphics.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <memory>

#include "Graphics/includes/GraphicsSystem/Renderable.h"
#include "Graphics/includes/GraphicsSystem/Paintable.h"
#include "Graphics/includes/GraphicsSystem/ModelManager.h"

#include <Core\systems\System.h>
#include "Graphics/includes/SkyeDX/Camera.h"

#include "Graphics/includes/SkyeDX/ObjectHandle.h"
#include "Graphics/includes/SkyeDX/Viewport.h"
#include "Graphics/includes/SkyeDX/DebugDrawer.h"
#include "Graphics/includes/GraphicsSystem/ImguiGraphics.h"
#include "Editor/InGameEditor/InGameEditor.h"

#define WIN32_LEAN_AND_MEAN	
#include <windows.h>


class PointLight;
typedef std::shared_ptr<Skye::Renderer> RenderContextPtr;
typedef unsigned int CameraID;
typedef std::vector<Skye::Camera> CameraList;
typedef std::vector<Skye::Viewport> ViewportList;
typedef std::unordered_map<std::string, Skye::ObjectHandle> Dictionary;

namespace Viewports
{
	enum
	{
		/*Player1 = 0,
		Player2 = 1,
		Player3 = 2,
		Player4 = 3,*/
		FullScreen = 0,
	};
}

namespace DeferredPipeline
{
#define RENDER_TARGET(X) X,
	enum
	{
#include "Graphics/includes/GraphicsSystem/RenderTargets.h"
		MaxRenderTargets,
	};
#undef RENDER_TARGET
}

namespace ConstantBuffer
{
	enum
	{
		Matrix3D,
		Object3D,
		Constants,
		Matrix2D,
		PointLights,
		Particles,
		NumItems
	};

	struct Matrix2DBuffer
	{
		float4x4 projection;
		float2 screenSize;
	};

	struct Matrix3DBuffer
	{
		float4x4 view;
		float4x4 viewInverse;
		float4x4 projection;
		float4x4 projectionInverse;
		float4x4 viewProjection;
	};

	struct ConstantsItems
	{
		float3 eyePosition;
		float isSkydome;
		float4 emissiveColor;
		float offset;
		float4 apexColor;
		float4 centerColor;
	};

	struct EffectBuffer
	{
		float2 viewPlanes;
	};
}

struct PointLightData
{
	float4 lightColor;
	float4 position;
	float4 center;
	float2 rangeAttenuation;
	float2 pad;
};

class CameraManager
{
	public:
		CameraManager(void);
		~CameraManager(void);
	
		CameraID AddCamera(void);
		void AddViewport(int x, int y, Skye::Area size);
	
		/**********************************/
		/* GETTERS
		/**********************************/
		CameraID GetActiveCameraID(void);
		Skye::Camera& GetActiveCamera(void);
		const Skye::Viewport& GetViewport(void) const;
		const float2& GetOrthographicProjectionSize(void) const;
		
	
		/**********************************/
		/* SETTERS
		/**********************************/
		void SetActiveCamera(CameraID cam);
		void SetActiveViewport(unsigned ID);
	
	private:
		Skye::Viewport m_Viewport;
		ViewportList m_Viewports;
		CameraList m_Cameras;
		CameraID m_ActiveCamera;

		float2 m_OrthographicProjectionSize;

		float m_DestX, m_DestY;

		Skye::ObjectHandle m_DepthBuffer, m_DepthBufferCopy;

		ConstantBuffer::Matrix3DBuffer m_Mat3DBuffer;
		ConstantBuffer::Matrix2DBuffer m_Mat2DBuffer;

		friend class SkyeGraphics;
};

class SkyeGraphics : public Core::ISystem
{
	public:

		typedef std::vector<Base::Handle> RenderableList;

		/**********************************/
		/* CONSTRUCTORS/DESTRUCTORS
		/**********************************/
		SkyeGraphics(const HWND hwnd);
		~SkyeGraphics(void);

		/**********************************/
		/* SYSTEM METHODS
		/**********************************/
		virtual void Initialize(void) override;
		virtual void Shutdown(void) override;
		void SkyeGraphics::OnResize();
		virtual std::string GetName() const override { return "SkyeGraphics"; }

		virtual void Update(float dt) override;

		virtual void ReceiveMessage(Core::Message& msg) override;


		/**********************************/
		/* ASSET LOADING
		/**********************************/
		const HWND& GetWindowHandle(void) const;
		void LoadAssets(void);
		void LoadModelPrimitives(void);
		void LoadTextures(void);
		void LoadDiffuseTextures(void);
		void LoadNormalTextures(void);
		void LoadEmissiveTextures(void);

		/**********************************/
		/* GETTERS
		/**********************************/
		Skye::Camera& GetCamera(void);
		Skye::Camera* GetCamera(unsigned ID);
		float2 GetScreenSize(void);
		Skye::Renderer* GetRenderContext(void);

		/**********************************/
		/* SETTERS
		/**********************************/
		void SetUITexture(Skye::ObjectHandle& texture);

		/**********************************/
		/* TEXTURES
		/**********************************/
		void LoadTexture(const std::string& fileName, const std::string& folder);
		void LoadTextureFromMemory(const std::string& fileName, const void* data, unsigned dataSize);
		void LoadTextureFromMemory(const std::string& fileName, const void* data, Skye::Area dataSize, Skye::DataFormat format);
		const Skye::ObjectHandle& GetTexture(const std::string& textureName);
		const Skye::Area& GetTextureSize(const std::string& textureName);
		void FreeTexture(const std::string& textureName);
		void FreeAllTextures(void);

		/**********************************/
		/* MODELS
		/**********************************/
		void LoadModelFromFile(const std::string& fileName, Skye::Model& model);
		void LoadModels(void);
		void LoadModel(const std::string& fileName);
		Skye::Model* GetModel(const std::string& modelName);
		void FreeModel(const std::string& modelName);
		void FreeAllModels(void);

		/**********************************/
		/* CAMERA
		/**********************************/
		CameraManager& GetCameraManager(void);
		void SetOrthographicProjectionSize(const float2& size);
		void SetFOV(float fov);
		float GetFOV(void);
		void SetViewPlanes(float nearPlane, float farPlane);
		float GetNearPlane(void);
		float GetFarPlane(void);

		/**********************************/
		/* SCREEN
		/**********************************/
		void SetFullScreen(bool fullScreen);
		bool GetFullScreen(void);
		void SetVSync(bool vSync);
		bool GetVSync(void);

		void SetFillMode(const Skye::FillMode fillMode);
		void DrawEditor(bool isEditorActive);

		/**********************************/
		/* COMPONENTS
		/**********************************/
		static void RegisterRenderable(Base::Handle const &handle);
		static void UnregisterRenderable(Base::Handle const &handle);

		/**********************************/
		/* DEBUG DRAWER
		/**********************************/
		Skye::DebugDrawer& GetDebugDrawer(void);
		void ToggleDebugDraw(void);
		bool IsDebugDrawOn(void);

		/**********************************/
		/* TEXT DRAWER
		/**********************************/
		void DrawString(const std::string text, float fontSize, float xPos, float yPos, unsigned int color);
		
		// if bMultipleViewports is true, it will display the game in 4 viewports,
		// false will disply in 1 viewport, mainly used for editing game content
		void ToggleViewports(bool bMultipleViewports);
		bool RenderMultipleViewports(void) const;

	private:
		void ClearBuffers(void);
		void UpdateCameraManager(void);
		void UpdateCamera(unsigned ID);
		void SetCameraConstantBuffer(unsigned ID);
		void ResetScreen(void);
		
		/**********************************/
		/* RENDERING
		/**********************************/
		void RenderEditor(void);
		void RenderDebug(void);
		void Render3DObjects(void);
		void RenderPass(void);
		void RenderHelper(RenderableList& renderableList);
		void DeferredPass(void);
		void CompositePass(void);
		void RenderSplitScreen(void);
		void RenderSingleScreen(void);
		void SplitScreenDeferred(void);
		void SplitScreenComposite(void);
        void RenderFontWrapperDrawCalls(void);
		void AwesomiumPass(void);
		void ParticlePass(void);
		void LoadingScreen(void);
    
		void PointLightPass(void);

		/**********************************/
		/* POINT LIGHTS
		/**********************************/
		void PopulatePointLights(void);
		void CalculateBillboardQuad(PointLight* light);

		/**********************************/
		/* INITIALIZATION
		/**********************************/
		void InitializeConstantBuffers(void);
		void InitializeRenderTargets(void);
		void InitializeDefaultAssets(void);
		void InitializeDefaultFontWrappers(void);
		void BindConstantBuffers(void);

		/**********************************/
		/* SHADERS
		/**********************************/
		void LoadVertexShaders(void);
		void LoadGeometryShaders(void);
		void LoadPixelShaders(void);

	private:
		HWND m_hwnd;
		RenderContextPtr m_renderContext;
		Skye::ModelManager m_ModelManager;

		CameraManager m_CameraManager;
		static RenderableList m_RenderableObjects;

		Skye::DebugDrawer m_DebugDrawer;

		Dictionary m_Textures;
		Dictionary m_RenderTargets;
		Dictionary m_VertexShaders;
		Dictionary m_GeometryShaders;
		Dictionary m_PixelShaders;

		std::vector<PointLightData> m_PointLights;
		
		Skye::ObjectHandle m_ConstantBuffers[ConstantBuffer::NumItems];

		ConstantBuffer::EffectBuffer m_EffectConstants;

		ConstantBuffer::ConstantsItems m_Constants;

		Skye::ObjectHandle m_LightVertexBuffer;

		Skye::ObjectHandle m_UITexture;

		//Skye::ObjectHandle m_CompositeRenderTarget;

		float m_FovY;
		float m_FullscreenAspectRatio;

		float2 m_OrthographicProjectionSize;
		float4x4 m_OrthographicProjection;

		InGameEditor* m_Imgui;
		bool m_DrawImgui;
		bool m_DrawAwesomium;
		bool m_DrawDebug;

		// determines if we are drawing with one or 4 viewports
		bool m_MultipleViewports;

		friend class InGameEditor;
};

extern SkyeGraphics* GRAPHICS;
