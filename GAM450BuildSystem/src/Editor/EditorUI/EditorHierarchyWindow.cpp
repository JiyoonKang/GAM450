/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: EditorHierarchyWindow.cpp
 * Author: Jiyun Kang
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

#include <string>
#include <cstdio>
#include "Editor\EditorUI\EditorHierarchyWindow.h"
#include "Base\util\fileSystem\FileSystem.h"

#include "imgui\imgui.h"

//namespace Editor


    bool HierarchyWindow::m_isHierarchyActive = true;
	bool HierarchyWindow::m_isHierarchyVisible = false;
    InGameEditor* HierarchyWindow::m_editor = NULL;
    std::string HierarchyWindow::m_name;
    HierarchyWindow::HierarchyWindow(void)
    {
        m_name = "Hierarchy";
    }

    HierarchyWindow::HierarchyWindow(InGameEditor* editor)
    {
        m_editor = editor;
    }

    HierarchyWindow::~HierarchyWindow(void)
    {
    }
    bool HierarchyWindow::InitializeHierarchy(void)
    {
        return true;
    }
    void HierarchyWindow::HierarchyUpdate(void)
    {
        bool spacenameWindow = false;
        if (!m_isHierarchyActive || !m_isHierarchyVisible)
            return;

        ImGui::SetNextWindowPos(ImVec2(5, 35), ImGuiSetCond_Once);
        ImGui::SetNextWindowSize(ImVec2(250, 350), ImGuiSetCond_Once);
        if (ImGui::Begin("Hierarchy"))
        {
            std::string currentLevel = Core::ENGINE->GetCurrentLevel();
            //Current level tag
            if (ImGui::CollapsingHeader(currentLevel.c_str(), (const char*)0, true, true))
            {
                std::vector<Core::GameSpace*> spacelist = Core::ENGINE->Spaces();
                for (unsigned sIndex = 0; sIndex < spacelist.size(); ++sIndex)
                {
                    //Each space's tag
                    if (ImGui::TreeNode(spacelist[sIndex]->GetName().c_str()))
                    {
                        ImGui::PushID(sIndex);
                        m_editor->SetCurrentSpace(spacelist[sIndex]);

                        ImGui::SameLine();
                        if (ImGui::SmallButton("x"))
                        {
                            spacelist[sIndex]->Destroy();
                            m_editor->SetCurrentSpace(NULL);
                        }

                        //Get ObjectList from current space
                        unsigned index = 0;
                        for (auto it = spacelist[sIndex]->GetObjectList().begin<Core::GameObject>(); it != spacelist[sIndex]->GetObjectList().end<Core::GameObject>(); it++)
                        {
                            MakeObjectButton(it, index);
                            ++index;
                        }
                        index = 0;

                        ImGui::PopID();
                        ImGui::TreePop();
                    }
                }
            }
            ImGui::End();
        }

        ImGui::SetNextWindowPos(ImVec2(5, 400), ImGuiSetCond_Once);
        ImGui::SetNextWindowSize(ImVec2(250, 300), ImGuiSetCond_Once);
        if (ImGui::Begin("Factory"))
        {
            if (ImGui::CollapsingHeader(" Space Factory ", (const char*)0, true, true))
            {
                CreateEmptySpace();
                LoadSpaceFromFile();
            }
            if (ImGui::CollapsingHeader(" Object Factory ", (const char*)0, true, true))
            {
                CreateEmptyObject();
                LoadObjectFromFile();
                SaveToNewArchetype();
                SaveToExistArchetype();
                CopyAndPasteCurrentObject();
                ImGui::Text(" Focus Distance ");
                ImGui::InputFloat(" ", &m_editor->m_objCamDistance, 1.f, 2.0f);
            }

            ImGui::Separator();

            ImGui::End();
        }
    }

    bool HierarchyWindow::IsHierarchyActive(void)
    {
        return m_isHierarchyActive;
    }
    void HierarchyWindow::SetHierarchyActive(bool setting)
    {
        m_isHierarchyActive = setting;
    }
    bool HierarchyWindow::IsHierarchyVisible(void)
    {
        return m_isHierarchyVisible;
    }
    void HierarchyWindow::SetHierarchyVisible(bool setting)
    {
        m_isHierarchyVisible = setting;
    }
    void HierarchyWindow::MakeObjectButton(Core::GameObject* objects, int index)
    {
        ImGui::PushID(index);

        if (ImGui::Button(objects->GetName().c_str()))
        {
            m_editor->SetCurrentObject(objects);
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("Focus"))
        {
            m_editor->FocusOnthisObject(objects);
        }

        ImGui::SameLine();
        if (ImGui::SmallButton("x"))
        {
            objects->Destroy();
            m_editor->SetCurrentObject(NULL);
        }


        ImGui::PopID();
    }

    void HierarchyWindow::CreateEmptySpace(void)
    {
        static bool spacenameWindow = false;

        if (ImGui::Button(" Create Space "))
            spacenameWindow = true;

        if (!spacenameWindow)
            return;

        if (spacenameWindow)
        {
            static char emptyspacename[128] = "";

            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);
            if (ImGui::Begin(" Create Space ", &spacenameWindow, ImVec2(0, 0)))
            {
                ImGui::Text(" Enter Space name: ");
                ImGui::SameLine();
                ImGui::InputText("", emptyspacename, IM_ARRAYSIZE(emptyspacename));

                if (ImGui::Button(" Create "))
                {
                    Core::GameSpace* space = Core::ENGINE->CreateSpace(emptyspacename);
                    space->SetName(emptyspacename);
                    m_editor->SetCurrentSpace(space);
                    spacenameWindow = false;
                }

                ImGui::SameLine();
                if (ImGui::Button(" Cancel "))
                {
                    spacenameWindow = false;
                }
                ImGui::End();
            }
        }
    }
    void HierarchyWindow::LoadSpaceFromFile(void)
    {
        static bool spaceLoadWindow = false;

        if (ImGui::Button(" Load Space   "))
            spaceLoadWindow = true;

        if (!spaceLoadWindow)
            return;

        if (spaceLoadWindow)
        {
            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);
            if (ImGui::Begin(" Space Load ", &spaceLoadWindow, ImVec2(0, 0)))
            {
                std::vector<std::string> spaceList = Base::FileSystem::GetFilesNameFromDirectory(std::string("content\\data\\spaces"));
                for (unsigned i = 0; i < spaceList.size(); ++i)
                    spaceList[i] = Base::FileSystem::RemoveFileExtensionPath(spaceList[i]);
                static int currIndex = -1;
                int index = -1;

                ImGui::Combo(" Spaces ", &currIndex, StringItemsGetter, &spaceList, spaceList.size());

                if (ImGui::Button(" Load "))
                {
                    Core::GameSpace* space = Core::ENGINE->ObjectFactory.LoadSpace(spaceList[currIndex].c_str());
                    m_editor->SetCurrentSpace(space);
                    spaceLoadWindow = false;
                }

                ImGui::SameLine();
                if (ImGui::Button(" Cancel "))
                {
                    spaceLoadWindow = false;
                }
                ImGui::End();
            }
        }
    }

    void HierarchyWindow::CreateEmptyObject(void)
    {
        static bool objectWindow = false;

        if (ImGui::Button(" Create Object "))
            objectWindow = true;

        if (!objectWindow)
            return;

        if (objectWindow)
        {
            static char empthobjectname[128] = "";

            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);
            if (ImGui::Begin(" Create Empty Object ", &objectWindow, ImVec2(0, 0)))
            {
                if (Core::ENGINE->Spaces().size() == 0)
                {
                    ImGui::Text("There is no space on level!!");
                    ImGui::Text("Create Space First");
                    if (ImGui::Button(" Cancel "))
                    {
                        objectWindow = false;
                    }
                }
                else
                {
                    ImGui::Text(" Enter Object name: ");
                    ImGui::SameLine();
                    ImGui::InputText("", empthobjectname, IM_ARRAYSIZE(empthobjectname));
                    if (ImGui::Button(" Create "))
                    {
                        //Load object by using its name
                        Core::GameObject* emptyObject = m_editor->GetCurrentSpace()->CreateEmptyObject();
                        emptyObject->SetName(empthobjectname);
                        m_editor->SetCurrentObject(emptyObject);

                        Core::GameComponent* component = nullptr;

                        component = m_editor->GetCurrentSpace()->CreateComponent(eTransform);
                        m_editor->GetCurrentObject()->AddComponent(component);
                        component->Initialize();

                        objectWindow = false;
                    }

                    ImGui::SameLine();
                    if (ImGui::Button(" Cancel "))
                    {
                        objectWindow = false;
                    }
                }
                ImGui::End();
            }
        }
    }
    void HierarchyWindow::LoadObjectFromFile(void)
    {
        static bool objectfileWindow = false;

        if (ImGui::Button(" Load Object   "))
            objectfileWindow = true;

        if (!objectfileWindow)
            return;

        if (objectfileWindow)
        {
            static char fileobjectname[128] = "";

            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);
            if (ImGui::Begin(" Load Object ", &objectfileWindow, ImVec2(0, 0)))
            {
                if (Core::ENGINE->Spaces().size() == 0)
                {
                    ImGui::Text("There is no space on level!!");
                    ImGui::Text("Create Space First");
                    if (ImGui::Button(" Cancel "))
                    {
                        objectfileWindow = false;
                    }
                }
                else
                {
                    std::vector<std::string> objectList = Base::FileSystem::GetFilesNameFromDirectory(std::string("content\\data\\archetypes"));
                    for (unsigned i = 0; i < objectList.size(); ++i)
                        objectList[i] = Base::FileSystem::RemoveFileExtensionPath(objectList[i]);
                    static int currIndex = -1;
                    //const char* outpara;

                    ImGui::Text(" Enter Object name: ");
                    ImGui::SameLine();
                    ImGui::InputText("", fileobjectname, IM_ARRAYSIZE(fileobjectname));


                    ImGui::Combo("Objects", &currIndex, StringItemsGetter, &objectList, objectList.size());

                    if (ImGui::Button(" Load "))
                    {
                        //Load object by using its name
                        Core::GameObject* object = Core::ENGINE->ObjectFactory.LoadObjectFromArchetype(m_editor->GetCurrentSpace(), objectList[currIndex].c_str());
                        object->SetName(fileobjectname);
                        m_editor->SetCurrentObject(object);
                        objectfileWindow = false;
                    }

                    ImGui::SameLine();
                    if (ImGui::Button(" Cancel "))
                    {
                        objectfileWindow = false;
                    }
                }
                ImGui::End();
            }
        }
    }

    void HierarchyWindow::SaveToNewArchetype(void)
    {
        static bool newarchetypeWindow = false;

        if (ImGui::Button(" Save object to new archetype   "))
            newarchetypeWindow = true;

        if (!newarchetypeWindow)
            return;

        if (newarchetypeWindow)
        {
            static char archyname[128] = "";

            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);
            if (ImGui::Begin(" Save obj to new archetype ", &newarchetypeWindow, ImVec2(0, 0)))
            {
                ImGui::Text(" Enter Archetype name: ");
                ImGui::SameLine();
                ImGui::InputText("", archyname, IM_ARRAYSIZE(archyname));

                if (ImGui::Button(" Save "))
                {
                    Core::ENGINE->ObjectFactory.SaveObjectToArchetype(m_editor->GetCurrentObject(), archyname);
                    newarchetypeWindow = false;
                }

                ImGui::SameLine();
                if (ImGui::Button(" Cancel "))
                {
                    newarchetypeWindow = false;
                }
                ImGui::End();
            }
        }
    }

    void HierarchyWindow::SaveToExistArchetype(void)
    {
        static bool existarchetypeWindow = false;

        if (ImGui::Button(" Save object to exist archetype "))
            existarchetypeWindow = true;

        if (!existarchetypeWindow)
            return;

        if (existarchetypeWindow)
        {
            static char archyname[128] = "";

            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);
            if (ImGui::Begin(" Save obj to exist archetype ", &existarchetypeWindow, ImVec2(0, 0)))
            {
                ImGui::Text(" Choose Archetype name ");

                std::vector<std::string> objectList = Base::FileSystem::GetFilesNameFromDirectory(std::string("content\\data\\archetypes"));
                for (unsigned i = 0; i < objectList.size(); ++i)
                    objectList[i] = Base::FileSystem::RemoveFileExtensionPath(objectList[i]);
                static int currIndex = -1;

                ImGui::Combo("Objects", &currIndex, StringItemsGetter, &objectList, objectList.size());

                if (ImGui::Button(" Save "))
                {
                    Core::ENGINE->ObjectFactory.SaveObjectToArchetype(m_editor->GetCurrentObject(), objectList[currIndex].c_str());
                    existarchetypeWindow = false;
                }

                ImGui::SameLine();
                if (ImGui::Button(" Cancel "))
                {
                    existarchetypeWindow = false;
                }
                ImGui::End();
            }
        }
    }

    void HierarchyWindow::CopyAndPasteCurrentObject(void)
    {
        if (ImGui::Button(" Copy and Paste current Object  "))
        {
            if (Core::ENGINE->Spaces().size() == 0)
            {
                return;
            }
            else
            {
                Core::GameObject* currObject = m_editor->GetCurrentObject();
                Core::GameObject* newObject = m_editor->GetCurrentSpace()->CreateEmptyObject();
                std::string objName;
                if (currObject)
                {
                    newObject->Copy(*currObject);
                    objName = currObject->GetName() + "_copy";
                    newObject->SetName(objName);
                }
            }

        }
    }
