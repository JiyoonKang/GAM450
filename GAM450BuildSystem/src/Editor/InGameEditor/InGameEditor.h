/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: InGameEditor.h
 * Author: Jiyun Kang
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include <string>
#include <Windows.h>
//CoreSystem
#include "Core/systems/System.h"
//Editor
#include"Editor\InGameEditor\EditorInclude.h"
#include "Editor\EditorUI\EditorWindow.h"
#include "Editor\InGameEditor\Types.h"
#include "Editor\InGameEditor\InspecCommand.h"

//#include "graphics\includes\GraphicsSystem\SkyeGraphics.h"

#include "Core\types\object\Object.h"
#include "Core\types\space\Space.h"
#include "Base\util\math\Float4x4.h"
#include "Physics\util\Geometry.h"


    class EditorWindow;
    class EditorMainMenuBar;


    class InGameEditor
    {
    public:
		InGameEditor();
        InGameEditor(HWND hWnd);
        ~InGameEditor(void);

        // Shut down 
		virtual void Initialize(void);
        virtual void Shutdown(void);
        // Update Editor's state
        virtual void Update(float dt);

		void Render(void);
        // Initialize InGameEditor
        //static int Initialize(void);
        // Get Editor's active type
        static bool IsEditorActive(void);
        // Set Editor's active 
        static void SetEditorActive(bool setting);

        // Get current selected object
        Core::GameObject* GetCurrentObject(void);
        // Set current object
        void SetCurrentObject(Core::GameObject* object);

        Core::GameSpace* GetCurrentSpace(void);
        // Set current object
        void SetCurrentSpace(Core::GameSpace* space);

        /*static bool IsGamePaused(void);
        static void SetGamePaused(bool setting);*/

        static bool GetIsReloaded(void);
        static void SetIsReloaded(bool setting);

        static bool GetDebugDraw(void);
        static void SetDebugDraw(bool setting);

        static std::vector<std::string> GetWindowsName(void);
        static bool GetSpecificWindowVis(std::string window);
        static void SetSpecificWindowVis(std::string window);

        // Getter and Setter for Gaphics and inspector window
        static void AddModelNameOnList(std::string name);
        static void AddTextureNameOnList(std::string name);
        static void AddNormalTextureNameOnList(std::string name);
        static void AddEmissiveTextureNameOnList(std::string name);
        static std::vector<std::string> GetModelNameList(void);
        static std::vector<std::string> GetTextureNameList(void);
        static std::vector<std::string> GetNormalTextureNameList(void);
        static std::vector<std::string> GetEmissiveTextureNameList(void);

        static void SetAssetReload(bool setting);
        static bool GetAssetReload(void);

        void ClearCurrentObjSpace(void);
        void FocusOnthisObject(Core::GameObject* object);
        void SetFocusDistance(float dis);

        static float            m_objCamDistance;
        static CommandManager*  m_commandManager;
        static bool             m_undo;
    private:

        //////////////////Editor//////////////////
        static void ShowEditor(void);
        static void HideEditor(void);
        static void UpdateEditorCamera(void);

        void CheckObjectClick(void);
        //Physics::Ray FindPickingRay(int screenX, int screenY);
        void TransfromRay(Physics::Ray& resultRay, float4x4 invertViewMatrix);


        //Gizmo functions
        static void DrawGizmo(void);
        static void HighlightObject(Core::GameObject* object);
        static void UpdateGizmoPosition(void);
        static void SelectGizmo(void);


		void CameraControls(void);
		bool m_CameraControls;
		bool m_DisplayCameraText;


        //////////////////For Rendering//////////////////
        //static SkyeGraphics*  m_Graphics;              //Graphics system
        static EditorVertex*  m_EditorVertexBuffer;    //Vertex for draw Editor
        static EditorRects*   m_EditorRectBuffer;      //Rect for clip Editor
        static unsigned       m_RectCount;

        static bool m_isEditorActive;
        //static bool m_IsGamePaused;

        //Gizmo variable
        /*   ...   */

        //Camera Variable
        //static CameraID m_editorCamera;
        //static CameraID m_gameCamera;
        static float4x4 m_editorCameraMatrix;

        //////////////////EditorUIVariable//////////////////

        /*   MainMenuBar    */
        static EditorMainMenuBar* m_MainMenuBar;

        static std::map<std::string, EditorWindow* > m_editorWindowList;
        static std::vector<std::string> m_editorNameList;
        static std::vector<std::string> m_textureNameList;
        static std::vector<std::string> m_modelNameList;
        static std::vector<std::string> m_normalTextureNameList;
        static std::vector<std::string> m_emissiveTextureNameList;

        static Core::GameObject* m_currentObject;
        std::string              m_currentObjectName;
        static Core::GameSpace*  m_currentSpace;
        std::string              m_currentSpaceName;

        static bool             m_ReloadedAsset;
        static bool             m_IsReloaded;
        static HWND             m_hWnd;
    };



