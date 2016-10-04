/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: InGameEditor.cpp
 * Author: Jiyun Kang
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

#include "Core\systems\window\Window.h"

#include "InGameEditor.h"
#include "imgui\imgui.h"

#include "Editor\EditorUI\EditorHierarchyWindow.h"
#include "Editor\EditorUI\EditorInspectorWindow.h"
#include "Editor\EditorUI\EditorMainMenuBar.h"
#include "Editor\EditorUI\EditorProjectWindow.h"
#include "Editor\EditorUI\EditorObjEditWindow.h"
#include "Editor\EditorUI\EditorWindow.h"

#include "Editor/InGameEditor/imgui_impl_dx11.h"
#include "Graphics\includes\SkyeDX\DebugDrawer.h"
#include "Core\systems\input\Input.h"
#include "Core/systems/input/KeyboardManager.h"
#include "Graphics\includes\GraphicsSystem\SkyeGraphics.h"
#pragma warning( push )
#pragma warning( disable : 4005 )
#include <d3d11.h>
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800
#pragma warning(pop)
#include <dinput.h>

//#include "imgui_impl_dx11.h"

const unsigned NumberOfVerts = 30000;
const unsigned NumberOfRecs = 1000;
const float GizmoSize = 20.0f;



    //InGameEditor* INGAMEEDITOR = NULL;

    INT64 ticksPerSecond = 0;
    INT64 time = 0;

    //SkyeGraphics*  InGameEditor::m_Graphics = nullptr;
    EditorVertex*  InGameEditor::m_EditorVertexBuffer = new EditorVertex[NumberOfVerts];
    EditorRects*   InGameEditor::m_EditorRectBuffer = new EditorRects[NumberOfRecs];
    unsigned       InGameEditor::m_RectCount = 0;

    bool InGameEditor::m_isEditorActive = false;
    //bool InGameEditor::m_IsGamePaused = INGAMEEDITOR->IsGamePaused();

    //CameraID InGameEditor::m_editorCamera = -1;
    //CameraID InGameEditor::m_gameCamera = -1;
    float4x4 InGameEditor::m_editorCameraMatrix;

    EditorMainMenuBar* InGameEditor::m_MainMenuBar = nullptr;
    std::map<std::string, EditorWindow* > InGameEditor::m_editorWindowList;
    std::vector<std::string> InGameEditor::m_editorNameList;
    std::vector<std::string> InGameEditor::m_textureNameList;
    std::vector<std::string> InGameEditor::m_modelNameList;
    std::vector<std::string> InGameEditor::m_normalTextureNameList;
    std::vector<std::string> InGameEditor::m_emissiveTextureNameList;
    float InGameEditor::m_objCamDistance = 50;
    CommandManager*  InGameEditor::m_commandManager = nullptr;
    bool InGameEditor::m_undo = false;
    Core::GameObject* InGameEditor::m_currentObject;
    Core::GameSpace*  InGameEditor::m_currentSpace;

    bool InGameEditor::m_ReloadedAsset = false;
    bool InGameEditor::m_IsReloaded = false;
    HWND InGameEditor::m_hWnd = nullptr;
    WNDCLASSEX wc;

    static ImVec4 clear_col = ImColor(58, 149, 161);

    InGameEditor::InGameEditor(HWND hWnd) : m_currentObjectName(), m_currentSpaceName()
    {
        //m_Graphics = graphics;
        m_currentObject = nullptr;
        m_currentSpace = nullptr;
        m_hWnd = hWnd;
        //INGAMEEDITOR = this;
        //m_editorCamera = graphics->GetCameraManager().AddCamera();
    }

    InGameEditor::~InGameEditor(void)
    {
        ImGui_ImplDX11_Shutdown();
        //CleanupDeviceD3D();
        //UnregisterClass("ImGui Example", wc.hInstance);
    }
    void InGameEditor::Initialize(void)
    {
        std::vector<Core::GameSpace*> spacelist = Core::ENGINE->Spaces();
        if (spacelist.size() != 0)
        {
            m_currentSpace = spacelist.front();

            if (m_currentSpace && m_currentSpace->GetObjectList().Size() != 0)
                m_currentObject = m_currentSpace->GetObjectList().begin<Core::GameObject>();
        }

        m_MainMenuBar = new EditorMainMenuBar(this);

        m_editorWindowList["Hierarchy"] = new HierarchyWindow(this);
        m_editorNameList.push_back("Hierarchy");

        m_editorWindowList["Inspector"] = new InspectorWindow(this);
        m_editorNameList.push_back("Inspector");

        //m_editorWindowList["ObjEdit"] = new ObjEditWindow(this);
       // m_editorNameList.push_back("ObjEdit");

        //m_editorWindowList["Project"] = new ProjectWindow(this);
       // m_editorNameList.push_back("Project");

        m_MainMenuBar->Initialize();
        m_MainMenuBar->SetMenuBarActive(true);

        auto it = m_editorWindowList.begin();
        while (it != m_editorWindowList.end())
        {
            (*it).second->Initialize();
            //(*it).second->SetWindowActive(m_isEditorActive);
            ++it;
        }

        m_commandManager = new CommandManager();

    }

    void InGameEditor::Update(float dt)
    {
        if (m_IsReloaded)
        {
            //If game is not the paused 
            m_IsReloaded = false;
        }

        ImGui_ImplDX11_NewFrame();

        //Update Gizmo :later

        // Update all level windows
        m_MainMenuBar->Update();

        if (m_isEditorActive)
        {

            //Update camera
            //UpdateEditorCamera();
			CameraControls();

            //m_commandManager->ArrangeCommandStack();
            std::list<UndoableCommand* > test = m_commandManager->GetcommandStack();
            if (m_undo)
            {
                m_commandManager->Undo();
                m_undo = false;
            }


            auto it = m_editorWindowList.begin();
            while (it != m_editorWindowList.end())
            {
                (*it).second->Update();
                ++it;
            }


            static bool isGizmoGrabbed[4] = { false, false, false, false };

            SelectGizmo();

            if (m_currentObject != NULL)
            {
                // Show selected entity's AABB if one is selected
                HighlightObject(m_currentObject);

                // Update gizmo positions
                //UpdateGizmoPosition();

                // Show selected entity's Gizmo
                //DrawGizmo();
            }

            CheckObjectClick();
        }

    }

	void InGameEditor::Render()
	{
		ImGui::Render();
	}

    void InGameEditor::Shutdown(void)
    {
    }

    bool InGameEditor::IsEditorActive(void)
    {
        return m_isEditorActive;
    }

    void InGameEditor::SetEditorActive(bool setting)
    {
        m_isEditorActive = setting;

        auto it = m_editorWindowList.begin();
        while (it != m_editorWindowList.end())
        {
			(*it).second->SetWindowVisible(setting);
            ++it;
        }
    }

   /* SkyeGraphics* InGameEditor::GetGraphicSystem(void)
    {
        return m_Graphics;
    }*/

    //CameraID InGameEditor::GetEditorCameraId(void)
    //{
    //    return m_editorCamera;
    //}

    //float4x4& InGameEditor::GetEditorCameraMatrix(void)
    //{
    //    return m_editorCameraMatrix;
    //}

    Core::GameObject* InGameEditor::GetCurrentObject(void)
    {
        return m_currentObject;
    }
    void InGameEditor::SetCurrentObject(Core::GameObject* object)
    {
        m_currentObject = object;
    }

    Core::GameSpace* InGameEditor::GetCurrentSpace(void)
    {
        return m_currentSpace;
    }
    void InGameEditor::SetCurrentSpace(Core::GameSpace* space)
    {
        m_currentSpace = space;
    }

    /*bool InGameEditor::IsGamePaused(void)
    {
        return m_IsGamePaused;
    }
    void InGameEditor::SetGamePaused(bool setting)
    {
        m_IsGamePaused = setting;
    }*/

    bool InGameEditor::GetIsReloaded(void)
    {
        return m_IsReloaded;
    }
    void InGameEditor::SetIsReloaded(bool setting)
    {
        m_IsReloaded = setting;
    }
    std::vector<std::string> InGameEditor::GetWindowsName(void)
    {
        return m_editorNameList;
    }
    bool InGameEditor::GetSpecificWindowVis(std::string window)
    {
        return m_editorWindowList[window]->IsWindowVisible();
    }
    void InGameEditor::SetSpecificWindowVis(std::string window)
    {
        bool curr = m_editorWindowList[window]->IsWindowVisible();
        m_editorWindowList[window]->SetWindowVisible(!curr);
    }

    void InGameEditor::AddModelNameOnList(std::string name)
    {
        m_modelNameList.push_back(name);
    }
    void InGameEditor::AddTextureNameOnList(std::string name)
    {
        m_textureNameList.push_back(name);
    }
    void InGameEditor::AddNormalTextureNameOnList(std::string name)
    {
        m_normalTextureNameList.push_back(name);
    }

    void InGameEditor::AddEmissiveTextureNameOnList(std::string name)
    {
        m_emissiveTextureNameList.push_back(name);
    }

    std::vector<std::string> InGameEditor::GetModelNameList(void)
    {
        return m_modelNameList;
    }
    std::vector<std::string> InGameEditor::GetTextureNameList(void)
    {
        return m_textureNameList;
    }
    std::vector<std::string> InGameEditor::GetNormalTextureNameList(void)
    {
        return m_normalTextureNameList;
    }
    std::vector<std::string> InGameEditor::GetEmissiveTextureNameList(void)
    {
        return m_emissiveTextureNameList;
    }
    void InGameEditor::SetAssetReload(bool setting)
    {
        m_ReloadedAsset = setting;
    }
    bool InGameEditor::GetAssetReload(void)
    {
        return m_ReloadedAsset;
    }

    void InGameEditor::ClearCurrentObjSpace(void)
    {
        m_currentObject = nullptr;
        m_currentSpace = nullptr;
    }

    void InGameEditor::FocusOnthisObject(Core::GameObject* object)
    {
        Skye::Camera& cam = GRAPHICS->GetCameraManager().GetActiveCamera();

        Core::Transform* trans = dynamic_cast<Core::Transform*>(object->GetComponent(eTransform));

        float4 position = trans->m_position.translation;

		float4x4 camMatrix = cam.GetViewMatrix();
        camMatrix = Transpose(camMatrix);

        cam.SetPosition(float3(position.x + m_objCamDistance, position.y, position.z + m_objCamDistance));

        float4 getcolumn = float4(cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z, 1);
        float4 dir = trans->m_position.translation - getcolumn;

		dir.Normal();
		cam.SetLookAt(dir);
    }

    void InGameEditor::SetFocusDistance(float dis)
    {
        m_objCamDistance = dis;
    }

    void InGameEditor::ShowEditor(void)
    {
        auto it = m_editorWindowList.begin();
        while (it != m_editorWindowList.end())
        {
            if ((*it).second->IsWindowActive())
                (*it).second->SetWindowVisible(true);
            ++it;
        }
    }
    void InGameEditor::HideEditor(void)
    {
        auto it = m_editorWindowList.begin();
        while (it != m_editorWindowList.end())
        {
            (*it).second->SetWindowVisible(false);
            ++it;
        }
    }

    void InGameEditor::UpdateEditorCamera(void)
    {
        /*CameraID camID = GRAPHICS->GetCameraManager().GetActiveCameraID();
        Skye::Camera *cam = GRAPHICS->GetCamera(camID);
        float camRotSpeed = 0.2f;
        static float4 camLook = float4(0, 0, 1, 1);


        float mouseDeltaX = Input::MouseEvent::GetMouseDelta().x;
        float mouseDeltaY = Input::MouseEvent::GetMouseDelta().y;

        if (Input::MouseEvent::IsLeftMouseButtonHeld())
        {

            cam->RotateY(Input::MouseEvent::GetMouseDelta().x / 1000.0f);
            cam->RotateAlongX(Input::MouseEvent::GetMouseDelta().y / 1000.0f);

        }
        if (Input::MouseEvent::IsRightMouseButtonHeld())
        {

            cam->MoveAlongX(Input::MouseEvent::GetMouseDelta().x * 10.0f);
            cam->MoveAlongY(Input::MouseEvent::GetMouseDelta().y * 10.0f);

        }
        if (Input::MouseEvent::IsMiddleMouseButtonPressed())
        {
            cam->ResetAxis();
        }*/
    }

    void InGameEditor::CheckObjectClick(void)
    {
        //Physics::Ray resultRay;
        //float2 mousePos;

        //if (Input::MouseEvent::IsLeftMouseButtonPressed())
        //{
        //    mousePos = Input::MouseEvent::GetMousePosition();
        //    resultRay = FindPickingRay(mousePos.x, mousePos.y);

        //    //Skye::Camera c2(*cam);
        //    //c2.GetViewMatrix().GetInverse();
        //    //c2.SetPosition(float3(0, 0, 0));
        //    //c2.SetLookAt(float4(0, 0, -1, 0));
        //    //m_editorCameraMatrix = (c2.GetViewMatrix());
        //    //m_editorCameraMatrix = c2.GetViewMatrix().GetInverse();
        //    //m_editorCameraMatrix = (cam->GetViewMatrix());
        //    //m_editorCameraMatrix = Transpose(m_editorCameraMatrix);

        //    bool check = false;
        //    bool iscollide = true;
        //    std::vector<Core::GameSpace*> spacelist = Core::ENGINE->Spaces();
        //    for (unsigned sIndex = 0; sIndex < spacelist.size() && iscollide; ++sIndex)
        //    {
        //        unsigned index = 0;
        //        for (auto it = spacelist[sIndex]->GetObjectList().begin<Core::GameObject>(); it != spacelist[sIndex]->GetObjectList().end<Core::GameObject>(); it++)
        //        {
        //            Renderable *renderable = dynamic_cast<Renderable*>((*it).GetComponent(eRenderable));
        //            Core::Transform* trans = dynamic_cast<Core::Transform*>((*it).GetComponent(eTransform));
        //            float4 halfscale = trans->m_position.scale / 2.f;
        //            Physics::Box box;
        //            box.m_HalfExtents = halfscale;

        //            //Test code
        //            //GRAPHICS->GetDebugDrawer().DrawBox(box.m_HalfExtents*2, trans, float4(1.f, 0.f, 0.f, 1.f));
        //            //GRAPHICS->GetDebugDrawer().DrawSphere(float3(resultRay.m_Direction.x, resultRay.m_Direction.y, resultRay.m_Direction.z), 0.1f, float4(1.f, 1.f, 1.f, 1.f));
        //            //GRAPHICS->GetDebugDrawer().DrawSphere(float3(resultRay.m_Start.x, resultRay.m_Start.y, resultRay.m_Start.z), 0.1f, float4(1.f, 1.f, 1.f, 1.f));

        //            //float3 testpoint = float3(resultRay.m_Start.x, resultRay.m_Start.y, resultRay.m_Start.z) + float3(resultRay.m_Direction.x, resultRay.m_Direction.y, resultRay.m_Direction.z);
        //            //GRAPHICS->GetDebugDrawer().DrawSphere(testpoint, 0.01f, float4(1.f, 1.f, 1.f, 1.f));
        //            //GRAPHICS->GetDebugDrawer().DrawLine(float3(resultRay.m_Start.x, resultRay.m_Start.y, resultRay.m_Start.z), testpoint, float4(0.f, 0.f, 0.f, 1.f));

        //            float4 RayStart = Physics::RotatePointIntoBasis(trans->GetRotationMatrix(), trans->GetTranslation(), resultRay.m_Start);
        //            float4 RayDir = Physics::RotateVectorIntoBasis(trans->GetRotationMatrix(), resultRay.m_Direction);

        //            Physics::Ray temp(RayStart, RayDir);
        //            check = Physics::RayIntersectionTest(temp, box);

        //            if (check)
        //            {
        //                m_currentObject = it;
        //                iscollide = false;
        //                break;
        //            }
        //            ++index;
        //        }
        //        index = 0;
        //    }


        //}
        //else if (Input::MouseEvent::IsRightMouseButtonPressed())
        //{
        //}
    }
    //Physics::Ray InGameEditor::FindPickingRay(int screenX, int screenY)
   // {
  //      Physics::Ray resultRay;
  //      float4 ray_direction(0.0f, 0.0f, 0.0f, 0.0f);
  //      float4 ray_start(0.0f, 0.0f, 0.0f, 0.0f);

  //      Skye::Camera cam = GRAPHICS->GetCameraManager().GetActiveCamera();

  //      ///////////////////////////////////////////////////////////////////////////////////////
  //      //float vecX, vecY, vecZ;
  //      //Skye::Camera cam = GRAPHICS->GetCameraManager().GetActiveCamera();
  //      //float m00 = GRAPHICS->GetCameraManager().GetActiveCamera().GetProjectionMatrix().m00;
  //      //float m11 = GRAPHICS->GetCameraManager().GetActiveCamera().GetProjectionMatrix().m11;
  //      //float x = GRAPHICS->GetCameraManager().GetActiveCamera().m_Width;
  //      //float y = GRAPHICS->GetCameraManager().GetActiveCamera().m_Height;
  //      //vecX = (((2.0f * screenX) / x) - 1) / m00;
  //      //vecY = -(((2.0f * screenY) / y) - 1) / m11;
  //      //vecZ = 1.0f;	//View space's Z direction ranges from 0 to 1, so we set 1 since the ray goes "into" the screen

  //      //ray_direction = float4(vecX, vecY, vecZ, 0.0f);

  //      //float4x4 view = GRAPHICS->GetCameraManager().GetActiveCamera().GetViewMatrix();

  //      //resultRay.m_Start     = GRAPHICS->Vector3TransformCoord(ray_start, view);
  //      //resultRay.m_Direction = GRAPHICS->Vector3TransformNormal(ray_direction, view);
  //      //return resultRay;
  //      ///////////////////////////////////////////////////////////////////////////////////////////
  //      Skye::Viewport viewport = GRAPHICS->GetCameraManager().GetViewport();
  //      int width = viewport.size.width;
  //      int height = viewport.size.height;
  //      float aspect = cam.GetAspectRatio();
  //      float nearDist = cam.GetNear();
  //      float4 campos = float4(cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z, 1);
		//float4 camView = float4(cam.GetLookAt().x, cam.GetLookAt().y, cam.GetLookAt().z, 1);
		//float4 camUp = -float4(cam.GetUp().x, cam.GetUp().y, cam.GetUp().z, 1);
  //      float4 right = -camUp.Cross(camView);
  //      camView.Normalize();
  //      right.Normalize();
  //      camUp.Normalize();

  //      float x = (float)screenX;
  //      float y = (float)screenY;

  //      x -= width / 2.0f;
  //      y -= height / 2.0f;

  //      x /= (width / (2.0f*aspect));
  //      y /= (height / 2.0f);

  //      float4 pos = right*x + camUp*y + camView*nearDist;

  //      float4 dir = pos;
  //      dir.Normalize();

  //      resultRay.m_Start = campos;
  //      resultRay.m_Direction = dir;
  //      return resultRay;
  //      /////////////////////////////////////////////////////////////////////////////////////////
  //      //float fov2 = cam->GetFOV() / 2.0f;
  //      //Skye::Viewport viewport = GRAPHICS->GetCameraManager().GetViewport();
  //      //int width = viewport.size.width;
  //      //int height = viewport.size.height;
  //      //float aspect = viewport.size.width / (float)viewport.size.height;
  //      //float nearDist = cam->GetNear();
  //      //float farDist = cam->GetFar();
  //      //
  //      //float4 camView = -GetColumn(camMatrix, 2);
  //      //float4 camUp = GetColumn(camMatrix, 1);
  //      //float4 h = GetColumn(camMatrix, 0);
  //      //float4 v = -camUp;
  //      //v.Normalize();
  //      //h.Normalize();
  //      //
  //      //float x = (float)screenX;
  //      //float y = (float)screenY;
  //      //
  //      //x -= width / 2.0f;
  //      //y -= height / 2.0f;
  //      //
  //      //y /= (height / 2.0f);
  //      //x /= (width / (2.0f*aspect));
  //      //
  //      //float4 pos = camView*nearDist + h*x + v*y;		
  //      //float4 dir = pos.Normal();
  //      //resultRay.m_Start = float4(0,0,0, 0);
  //      //resultRay.m_Direction = dir;

  //      /////////////////////////////////////////////////////////////////////////////////////////
  //      //Skye::Viewport viewport = GRAPHICS->GetCameraManager().GetViewport();
  //      //int width_half = viewport.size.width/2;
  //      //int height_half = viewport.size.height/2;
  //      //float aspect = viewport.size.width / (float)viewport.size.height;
  //      //float cam_near = cam->GetNear();
  //      //float cam_far = cam->GetFar();
  //      //float tandf = std::tanf(cam->GetFOV() * 0.5f);

  //      ///*float dx = tandf * (screenX / width_half - 1.0f) / aspect;
  //      //float dy = tandf * (- screenY / height_half + 1.0f);*/

  //      //float dx = 1.0f - (2.f * screenX) / width_half;
  //      //float dy = 1.0f - (2.f * screenY) / height_half;

  //      //float4 p1 = float4(dx*cam_near, dy*cam_near, cam_near,0);
  //      //float4 p2 = float4(dx*cam_far, dy*cam_far, cam_far, 0);
  //      //

  //      //float4x4 viewProjectInverse = view * project;
  //      ////Inverse viewproject matrix
  //      //viewProjectInverse = viewProjectInverse.ToInverse();

  //      //p1 = project.ToInverse() * p1;
  //      //p1 = view.ToInverse() * p1;
  //      //
  //      ////p1 = viewProjectInverse * p1;
  //      ////p2 = viewProjectInverse * p2;

  //      //p2 = project.ToInverse() * p2;
  //      ////p2 = view.ToInverse() * p2;

  //      ////not just add position, mut cam's world matirx, 
  //      //p1 += float4(cam->GetPosition().x, cam->GetPosition().y, cam->GetPosition().z, 0);
  //      //p2 += float4(cam->GetPosition().x, cam->GetPosition().y, cam->GetPosition().z, 0);

  //      //float4 dir = p2 - p1;
  //      //dir.Normalize();
  //      //resultRay.m_Start =p1;
  //      //resultRay.m_Direction = dir;
  //      //

   // }


    void InGameEditor::DrawGizmo(void)
    {
     
    }
    void InGameEditor::HighlightObject(Core::GameObject* object)
    {
        Core::Transform* trans = dynamic_cast<Core::Transform*>((*object).GetComponent(eTransform));

        if (trans)
        {
            float4 min = *(float4*)&(trans->GetTranslation() - (trans->GetScale() * 0.5f));
            float4 max = *(float4*)&(trans->GetTranslation() + (trans->GetScale() * 0.5f));

            float4 halfscale = trans->m_position.scale / 2.f;
            Physics::Box box;
            box.m_HalfExtents = halfscale;
            //GRAPHICS->GetDebugDrawer().DrawBox(box.GetExtents(), trans, Skye::Color::s_White);
        }
    }
    void InGameEditor::UpdateGizmoPosition(void)
    {
        if (!m_currentObject)
            return;

        float dist = 18.0f;
    }
    void InGameEditor::SelectGizmo(void)
    {
        // Works only if the Left Control Key is being held
        /*if (!Input::KeyboardEvent::IsKeyHeld(Input::KeyboardEvent::IsKeyHeld(Input::VirtualKey::KEY_LEFT)))
            return;

        if (Input::KeyboardEvent::IsKeyPressed(Input::VirtualKey::KEY_Q))
            m_currentGizmoType = Gizmo::NONE;

        else if (Input::KeyboardEvent::IsKeyPressed(Input::VirtualKey::KEY_W))
            m_currentGizmoType = Gizmo::TRANSLATE;

        else if (Input::KeyboardEvent::IsKeyPressed(Input::VirtualKey::KEY_E))
            m_currentGizmoType = Gizmo::ROTATE;

        else if (Input::KeyboardEvent::IsKeyPressed(Input::VirtualKey::KEY_R))
            m_currentGizmoType = Gizmo::SCALE;*/
    }


	void InGameEditor::CameraControls(void)
	{
		static float bg_alpha = 0.65f;

		// Demonstrate the various window flags. Typically you would just use the default.
		ImGuiWindowFlags window_flags = 0;

		if (!ImGui::Begin("Camera Controls", &m_CameraControls, ImVec2(250, 400), bg_alpha, window_flags))
		{
			ImGui::End();
			return;
		}

		if (m_DisplayCameraText)
		{
			ImGui::Text("Camera Controls:");
			ImGui::Text("W - Move forward");
			ImGui::Text("A - Strafe left");
			ImGui::Text("S - Move backwards");
			ImGui::Text("D - Strafe right");
			ImGui::Text("Right-click + Mouse move - Look around");

			ImGui::Text("");
		}

		ImGui::Text("Camera information:");
		float3 position = GRAPHICS->GetCameraManager().GetActiveCamera().GetPosition();
		//float3 lookAt = GRAPHICS->GetCameraManager().GetActiveCamera().GetLookAt();

		ImGui::Text("Camera position: X %.1f, Y %.1f, Z %.1f", position.x, position.y, position.z);
		//ImGui::Text("Camera direction: X %.1f, Y %.1f, Z %.1f", lookAt.x, lookAt.y, lookAt.z);

		if (!Input::InputHandler::GetKeyInput())
		{
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Controls Disabled");
		}
		else
		{
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Controls Enabled");
		}

		if (ImGui::Button("Display Controls"))
		{
			m_DisplayCameraText = !m_DisplayCameraText;
		}

		//ImGui::InputFloat("Movement Speed", &GRAPHICS->GetCameraManager().GetActiveCamera().GetSpeed(), 0.1f, 1.0f);

		ImGui::End();

	}

