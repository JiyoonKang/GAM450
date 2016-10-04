/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: EditorProjectWindow.cpp
 * Author: Jiyun Kang
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

#include <string>
#include <cstdio>
#include "Editor\EditorUI\EditorProjectWindow.h"

#include "imgui\imgui.h"

//namespace Editor


    bool ProjectWindow::m_isProjectActive = true;
	bool ProjectWindow::m_isProjectVisible = false;
    InGameEditor* ProjectWindow::m_editor = NULL;
    std::string ProjectWindow::m_name;

    ProjectWindow::ProjectWindow(void)
    {
        m_name = "ObjEdit";
    }

    ProjectWindow::ProjectWindow(InGameEditor* editor)
    {
        m_editor = editor;
    }

    ProjectWindow::~ProjectWindow(void)
    {
    }
    bool ProjectWindow::InitializeProject(void)
    {
        return true;
    }
    void ProjectWindow::ProjectUpdate(void)
    {

        if (!m_isProjectActive || !m_isProjectVisible)
            return;

        ImGui::SetNextWindowPos(ImVec2(625, 50), ImGuiSetCond_Once);
        ImGui::SetNextWindowSize(ImVec2(180, 500), ImGuiSetCond_Once);
        if (ImGui::Begin("Project"))
        {

            //Show all Asset folder's containing files 
            if (ImGui::Button(" Reload Assets "))
            {
                m_editor->SetAssetReload(true);
            }

            if (ImGui::CollapsingHeader("Assets"))
            {
            }


            ImGui::End();
        }
    }

    bool ProjectWindow::IsProjectActive(void)
    {
        return m_isProjectActive;
    }
    void ProjectWindow::SetProjectActive(bool setting)
    {
        m_isProjectActive = setting;
    }
    bool ProjectWindow::IsProjectVisible(void)
    {
        return m_isProjectVisible;
    }
    void ProjectWindow::SetProjectVisible(bool setting)
    {
        m_isProjectVisible = setting;
    }

