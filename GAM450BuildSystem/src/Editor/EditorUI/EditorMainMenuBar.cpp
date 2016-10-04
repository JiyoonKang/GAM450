/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: EditorMainMenuBar.cpp
 * Author: Jiyun Kang
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "Editor\EditorUI\EditorMainMenuBar.h"
#include "Editor\InGameEditor\EditorInclude.h"
#include "Core\engine\factory\Factory.h"
#include "Base\util\fileSystem\FileSystem.h"
#include "graphics\includes\GraphicsSystem\SkyeGraphics.h"

//namespace Editor

    bool          EditorMainMenuBar::m_isActive = true;
    bool          EditorMainMenuBar::m_isVisible = true;

    std::string   EditorMainMenuBar::m_currentLevelName;
    std::string   EditorMainMenuBar::m_newLevelName;
    bool          EditorMainMenuBar::m_levelChange = false;
    bool          EditorMainMenuBar::m_levelDelete = false;
    bool          EditorMainMenuBar::m_levelChagneComplete = false;
    bool          EditorMainMenuBar::loadCheck = false;

    InGameEditor* EditorMainMenuBar::m_editor = NULL;

    static bool newlevelwindow = false;
    static bool savelevelwindow = false;
    static bool saveaslevelwindow = false;

    EditorMainMenuBar::EditorMainMenuBar(void)
    {
    }

    EditorMainMenuBar::EditorMainMenuBar(InGameEditor* editor)
    {
        m_editor = editor;
    }

    EditorMainMenuBar::~EditorMainMenuBar(void)
    {
    }

    bool EditorMainMenuBar::Initialize(void)
    {
        m_currentLevelName = Core::ENGINE->GetCurrentLevel();

        if (Core::ENGINE->Spaces().size() != 0)
        {
            m_editor->SetCurrentSpace(Core::ENGINE->Spaces().front());
            if (m_editor->GetCurrentSpace()->GetObjectList().Size() != 0)
            {
                Core::GameObject* object = m_editor->GetCurrentSpace()->GetObjectList().begin<Core::GameObject>();
                m_editor->SetCurrentObject(object);
                //m_editor->FocusOnthisObject(object);
            }
        }
        return true;
    }

    void EditorMainMenuBar::Update(void)
    {

        // If this window is not active and also not visible, 
        // don't need to update 
        if (!m_isActive || !m_isVisible)
            return;

        if (ImGui::BeginMainMenuBar())
        {

            if (ImGui::BeginMenu("Editor"))
            {
                UpdateEditorTab();
                ImGui::EndMenu();
            }

            if (m_editor->IsEditorActive())
            {
                if (ImGui::BeginMenu("File"))
                {
                    UpdateFileTab();
                    ImGui::EndMenu();
                }

                CreateNewLevel();
                SaveWindow();
                SaveAsWindow();

                if (ImGui::BeginMenu("View"))
                {
                    UpdateViewTab();
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Edit"))
                {
                    UpdateEditTab();
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Setting"))
                {
                    UpdateSettingTab();
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Help"))
                {
                    UpdateHelpTab();
                    ImGui::EndMenu();
                }
            }
            ImGui::EndMainMenuBar();
        }
    }

    void EditorMainMenuBar::UpdateEditorTab(void)
    {
        if (ImGui::MenuItem("Editor ON", NULL, m_editor->IsEditorActive()))
        {
            m_editor->SetEditorActive(true);
            //m_editor->SetGameCameraId(GRAPHICS->GetCameraManager().GetActiveCameraID());
            //GRAPHICS->GetCameraManager().SetActiveCamera(m_editor->GetEditorCameraId());
        }

        if (ImGui::MenuItem("Editor OFF", NULL, !m_editor->IsEditorActive()))
        {
            m_editor->SetEditorActive(false);
            //GRAPHICS->GetCameraManager().SetActiveCamera(m_editor->GetGameCameraId());
        }

    }
    void EditorMainMenuBar::UpdateFileTab(void)
    {
        static bool openWindow = false;

        m_currentLevelName = Core::ENGINE->GetCurrentLevel();

        if (ImGui::MenuItem("New Level"))
        {
            m_editor->m_commandManager->ClearCommandList();
            newlevelwindow = true;
        }

        if (ImGui::BeginMenu("Open Level"))
        {
            std::vector<std::string> levelList = Base::FileSystem::GetFilesNameFromDirectory(std::string("content\\data\\levels"));

            for (unsigned i = 0; i < levelList.size(); ++i)
            {
                levelList[i] = Base::FileSystem::RemoveFileExtensionPath(levelList[i]);
                if (ImGui::MenuItem(levelList[i].c_str()))
                {
                    m_editor->m_commandManager->ClearCommandList();
                    Core::ENGINE->ObjectFactory.LoadLevel(levelList[i].c_str(), nullptr);
                    m_editor->SetIsReloaded(true);
                    m_editor->ClearCurrentObjSpace();

                    loadCheck = true;
                    if (Core::ENGINE->Spaces().size() != 0)
                    {
                        m_editor->SetCurrentSpace(Core::ENGINE->Spaces().front());
                        if (m_editor->GetCurrentSpace()->GetObjectList().Size() != 0)
                        {
                            Core::GameObject* object = m_editor->GetCurrentSpace()->GetObjectList().begin<Core::GameObject>();
                            m_editor->SetCurrentObject(object);
                        }
                        loadCheck = false;
                    }

					GRAPHICS->GetCameraManager().GetActiveCamera().ResetBasis();
					//GRAPHICS->GetCameraManager().GetActiveCamera().ResetAxis();
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Save Level"))
        {
            savelevelwindow = true;
            m_editor->m_commandManager->ClearCommandList();
        }
           
        if (ImGui::MenuItem("Save As.."))
            saveaslevelwindow = true;


        /*if (ImGui::MenuItem("Quit"))
        {
        }*/
    }

    void EditorMainMenuBar::UpdateEditTab(void)
    {
        if (ImGui::MenuItem("Undo"))
        {
            m_editor->m_undo = true;
        }

    }
    void EditorMainMenuBar::UpdateSettingTab(void)
    {
        if (ImGui::BeginMenu("Windows Visible"))
        {
            std::vector<std::string> windows = m_editor->GetWindowsName();

            for (unsigned i = 0; i < windows.size(); ++i)
            {
                bool check = m_editor->GetSpecificWindowVis(windows[i]);
                if (ImGui::MenuItem(windows[i].c_str(), NULL, check))
                {
                    m_editor->SetSpecificWindowVis(windows[i]);
                }
            }
            ImGui::EndMenu();
        }


        if (ImGui::MenuItem("Debug Draw", NULL, GRAPHICS->IsDebugDrawOn()))
        {
            GRAPHICS->ToggleDebugDraw();
        }

    }
    void EditorMainMenuBar::UpdateHelpTab(void)
    {

    }

    void EditorMainMenuBar::UpdateViewTab(void)
    {
        if (ImGui::BeginMenu("Viewport Setting"))
        {
            if (ImGui::MenuItem("One Viewport", NULL, !GRAPHICS->RenderMultipleViewports()))
                GRAPHICS->ToggleViewports(false);
            if (ImGui::MenuItem("Four Viewports", NULL, GRAPHICS->RenderMultipleViewports()))
                GRAPHICS->ToggleViewports(true);
            ImGui::EndMenu();
        }
    }

    bool EditorMainMenuBar::IsMenuBarActive(void)
    {
        return m_isActive;
    }
    void EditorMainMenuBar::SetMenuBarActive(bool setting)
    {
        m_isActive = setting;
    }
    bool EditorMainMenuBar::IsMenuBarVisible(void)
    {
        return m_isVisible;
    }
    void EditorMainMenuBar::SetMenuBarVisible(bool setting)
    {
        m_isVisible = setting;
    }
    void EditorMainMenuBar::SaveWindow(void)
    {
        if (!savelevelwindow)
            return;

        if (savelevelwindow)
        {
            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);
            if (ImGui::Begin("Confirm Save Level", &savelevelwindow, ImVec2(0, 0)))
            {
                if (ImGui::Button(" Save "))
                {
                    Core::ENGINE->ObjectFactory.SaveLevel(m_currentLevelName.c_str());
                    savelevelwindow = false;
                }
                ImGui::SameLine();
                if (ImGui::Button(" Cancel "))
                {
                    savelevelwindow = false;
                }

                ImGui::End();
            }
        }
    }
    void EditorMainMenuBar::SaveAsWindow(void)
    {
        if (!saveaslevelwindow)
            return;

        if (saveaslevelwindow)
        {
            static char saveaslevel[128] = "";
            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);
            if (ImGui::Begin("Level Name", &saveaslevelwindow, ImVec2(0, 0)))
            {
                ImGui::Text("Enter Level name:");
                ImGui::SameLine();
                ImGui::InputText("", saveaslevel, IM_ARRAYSIZE(saveaslevel));

                if (ImGui::Button(" Save "))
                {
                    m_currentLevelName = saveaslevel;
                    savelevelwindow = true;
                    saveaslevelwindow = false;
                }
                ImGui::SameLine();
                if (ImGui::Button(" Cancel "))
                {
                    saveaslevelwindow = false;
                }

                ImGui::End();
            }
        }
    }

    void EditorMainMenuBar::CreateNewLevel(void)
    {
        if (!newlevelwindow)
            return;

        if (newlevelwindow)
        {
            static char newlevel[128] = "";
            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);
            if (ImGui::Begin("Level Name", &newlevelwindow, ImVec2(0, 0)))
            {
                ImGui::Text("Enter Level name:");
                ImGui::SameLine();
                ImGui::InputText("", newlevel, IM_ARRAYSIZE(newlevel));

                if (ImGui::Button(" Create "))
                {
                    Core::ENGINE->ObjectFactory.CreateLevel(newlevel);
                    m_editor->ClearCurrentObjSpace();

					GRAPHICS->GetCameraManager().GetActiveCamera().ResetBasis();
                    //GRAPHICS->GetCameraManager().GetActiveCamera().ResetAxis();
                    newlevelwindow = false;
                }
                ImGui::SameLine();
                if (ImGui::Button(" Cancel "))
                {
                    newlevelwindow = false;
                }

                ImGui::End();
            }
        }
    }
