/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: EditorInspectorWindow.cpp
 * Author: Jiyun Kang
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "Editor\EditorUI\EditorInspectorWindow.h"
#include "Editor\InGameEditor\InspecCommand.h"

#include "Base\util\fileSystem\FileSystem.h"
#include "Base\util\math\Float3.h"
#include "Base\modules\introspection\enum\Enum.h"
#include "imgui\imgui.h"

#include "Physics\util\Geometry.h"
#include "Core\components\transform\CTransform.h"

//Temp for merge 
#define MAX_PARTICLE_COUNT 100
//namespace Editor

    ////////////////////////////////////////////////////////////////////////////////////////////
    // Inspector Window Class
    ////////////////////////////////////////////////////////////////////////////////////////////

    bool InspectorWindow::m_isInspectorActive = true;
	bool InspectorWindow::m_isInspectorVisible = false;
    char InspectorWindow::m_TextBuffer[256];
    std::string InspectorWindow::m_name;
    InGameEditor* InspectorWindow::m_editor = NULL;
    std::unordered_map<std::string, size_t> InspectorWindow::m_orderedComponent;
    std::vector<std::string> InspectorWindow::m_orderedComponentStr;
    InspecModifier* InspectorWindow::m_modifier = nullptr;

    InspectorWindow::InspectorWindow(void)
    {
        m_name = "Inspector";
        m_modifier = new InspecModifier(nullptr, m_editor);
    }
    InspectorWindow::InspectorWindow(InGameEditor* editor)
    {
        m_editor = editor;
    }
    InspectorWindow::~InspectorWindow(void)
    {

    }
    bool InspectorWindow::InitializeInspector(void)
    {
        //Get existing components and order by name, 
        //to make it easier to choose from the combo box
        Base::Enum *e = GET_ENUM(Component);
        m_orderedComponentStr = e->m_literals;

        for (unsigned index = 0; index < m_orderedComponentStr.size(); ++index)
            m_orderedComponent[m_orderedComponentStr[index]] = index;

        std::sort(m_orderedComponentStr.begin(), m_orderedComponentStr.end());
        return true;
    }
    void InspectorWindow::InspectorUpdate(void)
    {
        if (!m_isInspectorActive || !m_isInspectorVisible)
            return;

        Core::GameObject* object = m_editor->GetCurrentObject();

        ImGui::SetNextWindowPos(ImVec2(948, 35), ImGuiSetCond_Once);
        ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiSetCond_Once);

        if (ImGui::Begin("Inspector"))
        {
            if (Core::ENGINE->Spaces().size() != 0 && object)
            {
                ImGui::Text("Game Object[");
                ImGui::SameLine();
                std::string name = object->GetName().c_str();
                if (name == "")
                    ImGui::Text("Unnamed");
                else
                    ImGui::Text(object->GetName().c_str());
                ImGui::SameLine();
                ImGui::Text("]");


                if (ImGui::CollapsingHeader(object->GetName().c_str(), (const char*)0, true, true))
                {
                    if (ImGui::CollapsingHeader("Factory", (const char*)0, true, true))
                    {
                        ChangeName();
                        ImGui::SameLine();
                        AddComponent();
                        ImGui::SameLine();
                        ChangeArchtype();
                    }

                    if (ImGui::CollapsingHeader("Components", (const char*)0, true, true))
                    {
                        Inspect(object);
                    }
                }
            }
            ImGui::End();
        }

        ImGui::SetNextWindowPos(ImVec2(1048, 440), ImGuiSetCond_Once);
        ImGui::SetNextWindowSize(ImVec2(300, 280), ImGuiSetCond_Once);

        if (ImGui::Begin("History"))
        {
            std::list<UndoableCommand* > commandlist = m_editor->m_commandManager->GetcommandStack();
            std::list<UndoableCommand* > ::iterator it;

            if (ImGui::Button("Undo"))
            {
                m_editor->m_undo = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Clear History"))
            {
                m_editor->m_commandManager->ClearCommandList();
            }
            if (ImGui::CollapsingHeader("History", (const char*)0, true, true))
            {
                ImGui::BeginChild("Sub1", ImVec2(300, 300), false, ImGuiWindowFlags_HorizontalScrollbar);
                
                for (it = commandlist.begin(); it != commandlist.end(); ++it)
                {
                    std::string messg = (*it)->name.object_name + ": " + (*it)->name.member_name + " is changed.";
                    ImGui::Text(messg.c_str());
                }
                ImGui::EndChild();
        
            }
            ImGui::End();
        }
    }
    bool InspectorWindow::IsInspectorActive(void)
    {
        return m_isInspectorActive;
    }
    void InspectorWindow::SetInspectorActive(bool setting)
    {
        m_isInspectorActive = setting;
    }
    bool InspectorWindow::IsInspectorVisible(void)
    {
        return m_isInspectorVisible;
    }
    void InspectorWindow::SetInspectorVisible(bool setting)
    {
        m_isInspectorVisible = setting;
    }

    bool InspectorWindow::AddComponent(void)
    {
        bool isChanged = false;
        static bool addComponenet = false;

        if (ImGui::Button(" Add Component "))
            addComponenet = true;

        if (!addComponenet)
            return false;

        if (addComponenet)
        {
            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);

            if (ImGui::Begin(" Add Component ", &addComponenet, ImVec2(0, 0)))
            {
                static int index = 0;
                ImGui::Combo("Components", &index, StringItemsGetter, &m_orderedComponentStr, m_orderedComponentStr.size());

                if (ImGui::Button(" Add "))
                {
                    Core::GameComponent* component = nullptr;

                    component = m_editor->GetCurrentSpace()->CreateComponent(m_orderedComponent[m_orderedComponentStr[index]]);
                    m_editor->GetCurrentObject()->AddComponent(component);
                    component->Initialize();
                    addComponenet = false;
                    isChanged = true;
                }

                ImGui::SameLine();
                if (ImGui::Button(" Cancel "))
                {
                    addComponenet = false;
                    isChanged = false;
                }
                ImGui::End();
            }
        }
        return isChanged;
    }

    bool InspectorWindow::ChangeName(void)
    {
        bool isChanged = false;
        static bool changeName = false;

        if (ImGui::Button(" Change Name "))
            changeName = true;

        if (!changeName)
            return false;

        if (changeName)
        {
            static char newname[128] = "";

            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);
            if (ImGui::Begin(" Change Name ", &changeName, ImVec2(0, 0)))
            {
                ImGui::Text(" Enter object's new name: ");
                ImGui::SameLine();
                ImGui::InputText("", newname, IM_ARRAYSIZE(newname));

                if (ImGui::Button(" Change "))
                {
                    m_editor->GetCurrentObject()->SetName(newname);
                    changeName = false;
                    isChanged = true;
                }

                ImGui::SameLine();
                if (ImGui::Button(" Cancel "))
                {
                    changeName = false;
                    isChanged = false;
                }
                ImGui::End();
            }
        }
        return isChanged;
    }

    bool InspectorWindow::ChangeArchtype(void)
    {
        bool isChanged = false;
        static bool changeArch = false;

        if (ImGui::Button(" Change Archtype "))
            changeArch = true;

        if (!changeArch)
            return false;

        if (changeArch)
        {
            static char newname[128] = "";

            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);
            if (ImGui::Begin(" Change Archtype ", &changeArch, ImVec2(0, 0)))
            {
                std::vector<std::string> objectList = Base::FileSystem::GetFilesNameFromDirectory(std::string("content\\data\\archetypes"));
                for (unsigned i = 0; i < objectList.size(); ++i)
                    objectList[i] = Base::FileSystem::RemoveFileExtensionPath(objectList[i]);
                static int currIndex = -1;

                ImGui::Combo("Objects", &currIndex, StringItemsGetter, &objectList, objectList.size());

                if (ImGui::Button(" Load "))
                {
                    //Load object by using its name
                    Core::Transform* transform = dynamic_cast<Core::Transform*>(m_editor->GetCurrentObject()->GetComponent<Core::Transform>(eTransform));

                    Core::GameObject* object = Core::ENGINE->ObjectFactory.LoadObjectFromArchetype(m_editor->GetCurrentSpace(), objectList[currIndex].c_str());
                    object->SetName(m_editor->GetCurrentObject()->GetName());
                   
                    m_editor->GetCurrentObject()->Destroy();
                    m_editor->SetCurrentObject(object);
                    
                    Core::GameComponent* gameComponent = object->GetComponent(eTransform);
                    Base::Variable compVar = gameComponent;
                    void *compData = compVar.GetData();
                    Core::Transform* posInfo = reinterpret_cast<Core::Transform*>(compData);
                    *posInfo = *transform;

                    changeArch = false;
                }

                ImGui::SameLine();
                if (ImGui::Button(" Cancel "))
                {
                    changeArch = false;
                }
                ImGui::End();
            }
        }
        return isChanged;
    }
    bool InspectorWindow::Inspect(Core::GameObject* object)
    {
        bool isChanged = false;
        bool isCurrChanged = false;
        unsigned emptycount = 0;
        bool isComponentsEmpty = true;

        Core::GameComponent* gameComponent = NULL;

        for (unsigned it = 0; it < ecountComponents; ++it)
        {
            gameComponent = object->GetComponent(it);

            if (gameComponent == NULL)
            {
                ++emptycount;
                if (emptycount == ecountComponents)
                    isComponentsEmpty = false;
                continue;
            }

            std::string name = gameComponent->typeinfo->Name();//test
            if (ImGui::TreeNode(std::string(name).c_str()))
            {
                // Get reflection information
                const Base::TypeInfo* compInfo = gameComponent->typeinfo;
                Base::Variable compVar = gameComponent;
                void *compData = compVar.GetData();
                //Todo need all memeber;
                std::vector<Base::Member> compMembers = compInfo->GetMembers();

                if (compMembers.empty())
                {
                    ImGui::Text("This Component is empty");
                    ImGui::TreePop();
                    break;
                }

                // For every member in the current component
                for (unsigned j = 0; j < compMembers.size(); ++j)
                {

                    // Get reflection information
                    const Base::Member* mem = &compMembers[j];
                    const Base::TypeInfo* memInfo = mem->Type();
                    void* offsetData = PTR_ADD(compData, mem->Offset());

                    // Base Case
                    if (IsBaseCase(memInfo))
                        isCurrChanged = InspecPod(gameComponent->typeID, Base::Variable(memInfo, offsetData), mem, object);
                    // Recursive Case
                    else
                    {
                        // Start a tree node with the name of the member
                        if (ImGui::TreeNode(mem->Name()))
                        {
                            // Kickstart a recursive call to get members of members
                            isCurrChanged = InspecRec(gameComponent->typeID, Base::Variable(memInfo, offsetData), object);

                            // Pop member name tree node
                            ImGui::TreePop();
                        }
                    }

                    if (isCurrChanged)
                        isChanged = true;

                }

                std::string remCompText = "Remove " + name + " Component";
                if (ImGui::Button(remCompText.c_str()))
                {
                    object->DetatchComponent(gameComponent->typeID);
                    //--it;
                    // Without this we accidentally skip a component
                    if (isCurrChanged)
                        isChanged = true;
                }

                ImGui::TreePop();
            }

        }

        return isChanged;
    }
    bool InspectorWindow::InspecRec(const EComponent typeID, Base::Variable var, Core::GameObject* object)
    {
        bool isChanged = false;
        bool isCurrChanged = false;

        // Get reflection information
        const Base::TypeInfo* info = var.GetTypeInfo();
        std::vector<Base::Member> members = info->GetMembers();
        void* data = var.GetData();

        // For every member in the type
        for (unsigned i = 0; i < members.size(); ++i)
        {
            // Get reflection information
            const Base::Member* mem = &members[i];
            const Base::TypeInfo* memInfo = mem->Type();
            void* offsetData = PTR_ADD(data, mem->Offset());

            // Base Case
            if (IsBaseCase(memInfo))
                isCurrChanged = InspecPod(typeID, Base::Variable(memInfo, offsetData), mem, object);

            // Recursive Case
            else
            {
                // Start a tree node with the name of the member
                if (ImGui::TreeNode(mem->Name()))
                {
                    // Kickstart a recursive call to get members of members
                    isCurrChanged = InspecRec(typeID, Base::Variable(memInfo, offsetData), object);

                    // Pop member name tree node
                    ImGui::TreePop();
                }
            }

            if (isCurrChanged)
                isChanged = true;
        }

        return isChanged;
    }
    bool InspectorWindow::InspecPod(const EComponent typeID, Base::Variable var, const Base::Member* member, Core::GameObject* object)
    {
        bool isChanged = false;

        m_modifier->mObject = object;

        // Get reflection information
        const Base::TypeInfo* info = var.GetTypeInfo();
        //void* data = var.GetData();


        if (info == GET_STR_TYPE("float"))
        {
            isChanged = m_modifier->ChangeFloatData(var, member);
        }
        else if (info == GET_STR_TYPE("int"))
        {
            isChanged = m_modifier->ChangeIntData(var, member);
        }
        else if (info == GET_STR_TYPE("unsigned"))
        {
            isChanged = m_modifier->ChangeIntData(var, member);
        }
        else if (info == GET_STR_TYPE("bool"))
        {
            isChanged = m_modifier->ChangeBoolData(var, member);
        }
        else if (info == GET_STR_TYPE("std::string"))
        {
            isChanged = m_modifier->ChangeStringData(var, member);
        }
        else if (info == GET_STR_TYPE("float3"))
        {
            isChanged = m_modifier->ChangeFloat3Data(var, member);
        }
        else if (info == GET_STR_TYPE("float4"))
        {
            isChanged = m_modifier->ChangeFloat4Data(var, member);
        }
        else if (info == GET_STR_TYPE("Position"))
        {
            isChanged = m_modifier->ChangeTransform(var, member);
        }
        else if (info == GET_STR_TYPE("Box"))
        {
            isChanged = m_modifier->ChangeBox(var, member);
        }
        else if (info == GET_STR_TYPE("Sphere"))
        {
            isChanged = m_modifier->ChangeSphere(var, member);
        }
        else if (info == GET_STR_TYPE("Capsule"))
        {
            isChanged = m_modifier->ChangeCapsule(var, member);
        }
        // Used to determine if a certain POD case hasn't been dealt with
        else
        {
            ImGui::Text(member->Name());
            ImGui::SameLine();
            ImGui::Text(info->Name());
        }

        return isChanged;
    }
    bool InspectorWindow::IsBaseCase(const Base::TypeInfo* info)
    {
        // Todo:: check !! another type??
        if (info->IsPOD() || info == GET_STR_TYPE("float3") || info == GET_STR_TYPE("float4") || info == GET_STR_TYPE("Model") || info == GET_STR_TYPE("Texture") || info == GET_STR_TYPE("Position")
            || info == GET_STR_TYPE("Box") || info == GET_STR_TYPE("Sphere") || info == GET_STR_TYPE("Capsule") || info == GET_STR_TYPE("std::string"))
            return true;
        else
            return false;
    }
    void InspectorWindow::SaveToArchytype(Core::GameObject* object)
    {

    }
    void InspectorWindow::CreateModelButton(std::string& result)
    {
        static bool modelbutton = false;

        if (ImGui::SmallButton(" Load Model"))
            modelbutton = true;

        if (!modelbutton)
            return;

        if (modelbutton)
        {
            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);

            if (ImGui::Begin(" Model Load ", &modelbutton, ImVec2(0, 0)))
            {
                std::vector<std::string> resourselist;
                resourselist = m_editor->GetModelNameList();
                static int currIndex = -1;
                int index = -1;

                ImGui::Combo("Model", &currIndex, StringItemsGetter, &resourselist, resourselist.size());

                if (ImGui::Button(" Load "))
                {
                    result = resourselist[currIndex];
                    modelbutton = false;
                }
                ImGui::SameLine();
                if (ImGui::Button(" Cancel "))
                {
                    modelbutton = false;
                }
                ImGui::End();
            }
        }
    }
    void InspectorWindow::CreateTextureButton(std::string& result)
    {
        static bool texturebutton = false;

        if (ImGui::SmallButton(" Load Texture "))
            texturebutton = true;

        if (!texturebutton)
            return;

        if (texturebutton)
        {
            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);

            if (ImGui::Begin(" Texture Load ", &texturebutton, ImVec2(0, 0)))
            {
                std::vector<std::string> resourselist;
                resourselist = m_editor->GetTextureNameList();
                static int currIndex = -1;
                int index = -1;

                ImGui::Combo("Texture", &currIndex, StringItemsGetter, &resourselist, resourselist.size());

                if (ImGui::Button(" Load "))
                {
                    result = resourselist[currIndex];
                    texturebutton = false;
                }
                ImGui::SameLine();
                if (ImGui::Button(" Cancel "))
                {
                    texturebutton = false;
                }
                ImGui::End();
            }
        }
    }
    void InspectorWindow::CreateNormalTextureButton(std::string& result)
    {
        static bool normalTexturebutton = false;

        if (ImGui::SmallButton(" Load Normal "))
            normalTexturebutton = true;

        if (!normalTexturebutton)
            return;

        if (normalTexturebutton)
        {
            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);

            if (ImGui::Begin(" Normal Texture Load ", &normalTexturebutton, ImVec2(0, 0)))
            {
                std::vector<std::string> resourselist;
                resourselist = m_editor->GetNormalTextureNameList();
                static int currIndex = -1;
                int index = -1;

                ImGui::Combo("Normal Texture", &currIndex, StringItemsGetter, &resourselist, resourselist.size());

                if (ImGui::Button(" Load "))
                {
                    result = resourselist[currIndex];
                    normalTexturebutton = false;
                }
                ImGui::SameLine();
                if (ImGui::Button(" Cancel "))
                {
                    normalTexturebutton = false;
                }
                ImGui::End();
            }
        }
    }
    void InspectorWindow::CreateEmissiveTextureButton(std::string& result)
    {
        static bool paintTexturebutton = false;

        if (ImGui::SmallButton(" Load Emissive "))
            paintTexturebutton = true;

        if (!paintTexturebutton)
            return;

        if (paintTexturebutton)
        {
            ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiSetCond_Always);

            if (ImGui::Begin(" Emissive Texture Load ", &paintTexturebutton, ImVec2(0, 0)))
            {
                std::vector<std::string> resourselist;
                resourselist = m_editor->GetEmissiveTextureNameList();
                static int currIndex = -1;
                int index = -1;

                ImGui::Combo("Emissive Texture", &currIndex, StringItemsGetter, &resourselist, resourselist.size());

                if (ImGui::Button(" Load "))
                {
                    result = resourselist[currIndex];
                    paintTexturebutton = false;
                }
                ImGui::SameLine();
                if (ImGui::Button(" Cancel "))
                {
                    paintTexturebutton = false;
                }
                ImGui::End();
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    // InspecModifier Class
    ////////////////////////////////////////////////////////////////////////////////////////////
    Core::GameObject* InspecModifier::mObject = nullptr;
    InGameEditor*  InspecModifier::m_editor = nullptr;

    InspecModifier::InspecModifier(void)
    {
    }

    InspecModifier::~InspecModifier(void)
    {
    }

    InspecModifier::InspecModifier(Core::GameObject* Object, InGameEditor* editor)
    {
        mObject = Object;
        m_editor = editor;
    }

    bool InspecModifier::ChangeIntData(Base::Variable var, const Base::Member* member)
    {
        bool isChanged = false;
        std::string memName = member->Name();
        // Get reflection information
        void* data = var.GetData();
        int* vec = reinterpret_cast<int*>(data);
        int pre = static_cast<int>(*vec);

        if (memName == "m_particleBlendMode")
        {
            if (ImGui::DragInt(member->Name(), vec, 1, 0, 3))
            {
                isChanged = true;
            }
        }
        else if (memName == "m_particleCount")
        {
            if (ImGui::DragInt(member->Name(), vec, 1, 0, MAX_PARTICLE_COUNT))
            {
                isChanged = true;
            }
        }
        else
        {
            if (ImGui::InputInt(member->Name(), vec))
            {
                IntCommand* cmd = new IntCommand(pre, var);
                m_editor->m_commandManager->GetcommandStack().push_back(cmd);
                m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
                isChanged = true;
            }
        }

        return isChanged;
    }
    bool InspecModifier::ChangeFloatData(Base::Variable var, const Base::Member* member)
    {
        bool isChanged = false;

        // Get reflection information
        void* data = var.GetData();
        float* vec = reinterpret_cast<float*>(data);
        float pre = static_cast<float>(*vec);

        if (ImGui::InputFloat(member->Name(), vec, 0.1f, 0.5f))
        {
            FloatCommand* cmd = new FloatCommand(pre, var);
            m_editor->m_commandManager->GetcommandStack().push_back(cmd);
            m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
            isChanged = true;
        }

        return isChanged;
    }
    bool InspecModifier::ChangeBoolData(Base::Variable var, const Base::Member* member)
    {
        bool isChanged = false;

        // Get reflection information
        void* data = var.GetData();
        bool* vec = reinterpret_cast<bool*>(data);        
        bool pre = static_cast<bool>(*vec);

        if (ImGui::Checkbox(member->Name(), vec))
        {
            BoolCommand* cmd = new BoolCommand(pre, var);
            m_editor->m_commandManager->GetcommandStack().push_back(cmd);
            m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
            return isChanged;
        }
    }
    bool InspecModifier::ChangeStringData(Base::Variable var, const Base::Member* member)
    {
        bool isChanged = false;

        // Get reflection information
        void* data = var.GetData();
        std::string* stringdata = reinterpret_cast<std::string*>(data);
        std::string pre = static_cast<std::string>(*stringdata);
           
        std::strcpy(InspectorWindow::m_TextBuffer, stringdata->c_str());
        std::string result = InspectorWindow::m_TextBuffer;
        std::string memName = member->Name();
        ImGui::Text(member->Name());
        ImGui::SameLine();

        if (memName == "m_ModelName")
        {
            InspectorWindow::CreateModelButton(result);
            std::strcpy(InspectorWindow::m_TextBuffer, result.c_str());
            if (result != "")
            {
                std::strcpy(InspectorWindow::m_TextBuffer, result.c_str());
            }

            if (result != pre)
            {
                StringCommand* cmd = new StringCommand(pre, var);
                m_editor->m_commandManager->GetcommandStack().push_back(cmd);
                m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
            }
        }
        if (memName == "m_TextureName")
        {
            InspectorWindow::CreateTextureButton(result);
            std::strcpy(InspectorWindow::m_TextBuffer, result.c_str());
            if (result != "")
            {
                std::strcpy(InspectorWindow::m_TextBuffer, result.c_str());
            }
            if (result != pre)
            {
                StringCommand* cmd = new StringCommand(pre, var);
                m_editor->m_commandManager->GetcommandStack().push_back(cmd);
                m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
            }
        }
        if (memName == "m_NormalTexture")
        {
            InspectorWindow::CreateNormalTextureButton(result);
            std::strcpy(InspectorWindow::m_TextBuffer, result.c_str());
            if (result != "")
            {
                std::strcpy(InspectorWindow::m_TextBuffer, result.c_str());
            }
            if (result != pre)
            {
                StringCommand* cmd = new StringCommand(pre, var);
                m_editor->m_commandManager->GetcommandStack().push_back(cmd);
                m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
            }
        }
        if (memName == "m_EmissiveTexture")
        {
            InspectorWindow::CreateEmissiveTextureButton(result);
            std::strcpy(InspectorWindow::m_TextBuffer, result.c_str());
            if (result != "")
            {
                std::strcpy(InspectorWindow::m_TextBuffer, result.c_str());
            }
            if (result != pre)
            {
                StringCommand* cmd = new StringCommand(pre, var);
                m_editor->m_commandManager->GetcommandStack().push_back(cmd);
                m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
            }
        }
        *reinterpret_cast<std::string*>(data) = InspectorWindow::m_TextBuffer;
        if (ImGui::InputText("", InspectorWindow::m_TextBuffer, 256, ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll |
            ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
        {
            //Didn't Add command on here, i am not sure but there is no this kind of components
            *reinterpret_cast<std::string*>(data) = InspectorWindow::m_TextBuffer;
            isChanged = true;
        }

        return isChanged;
    }
    bool InspecModifier::ChangeFloat3Data(Base::Variable var, const Base::Member* member)
    {
        bool isChanged = false;

        // Get reflection information
        void* data = var.GetData();
        float* vec = reinterpret_cast<float*>(data);
        float pre[3];
        pre[0] = vec[0];
        pre[1] = vec[1];
        pre[2] = vec[2];

        if (ImGui::InputFloat3(member->Name(), vec, 2))
        {
            if (abs(pre[0] - vec[0]) > EPSILON ||
                abs(pre[1] - vec[1]) > EPSILON ||
                abs(pre[2] - vec[2]) > EPSILON)
            {
                Float3Command* cmd = new Float3Command(pre, var);
                m_editor->m_commandManager->GetcommandStack().push_back(cmd);
                m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
                isChanged = true;
            }
        }
        return isChanged;
    }
    bool InspecModifier::ChangeFloat4Data(Base::Variable var, const Base::Member* member)
    {
        bool isChanged = false;

        // Get reflection information
        void* data = var.GetData();

        std::string memName = member->Name();

        float* vec = reinterpret_cast<float*>(data);
        float4 pre;
        pre.x = vec[0]; pre.y = vec[1]; pre.z = vec[2]; pre.w = vec[3];

        if (memName == "m_Color" || memName == "m_particleTint" || memName == "m_particleEndTint")
        {
            if (ImGui::ColorEdit4(member->Name(), vec))
            {
				if (abs(pre.x - vec[0]) > EPSILON ||
					abs(pre.y - vec[1]) > EPSILON ||
					abs(pre.z - vec[2]) > EPSILON ||
					abs(pre.w - vec[3]) > EPSILON)
				{
					Float4Command* cmd = new Float4Command(pre, var);
					m_editor->m_commandManager->GetcommandStack().push_back(cmd);
					m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
					isChanged = true;
				}
            }
        }
        else
        {
            if (ImGui::InputFloat4(member->Name(), vec))
            {
                if (abs(pre.x - vec[0]) > EPSILON ||
                    abs(pre.y - vec[1]) > EPSILON ||
                    abs(pre.z - vec[2]) > EPSILON ||
                    abs(pre.w - vec[3]) > EPSILON)
				{
					Float4Command* cmd = new Float4Command(pre, var);
					m_editor->m_commandManager->GetcommandStack().push_back(cmd);
					m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
					isChanged = true;
				}
            }
        }

        return isChanged;
    }
    bool InspecModifier::ChangeTransform(Base::Variable var, const Base::Member* member)
    {
        bool isChanged = false;

        // Get reflection information
        void* data = var.GetData();
       
        Core::Position* posInfo = reinterpret_cast<Core::Position*>(data);

        Core::Position pre = static_cast<Core::Position>(*posInfo);

        if (ImGui::InputFloat4("Translation", posInfo->translation, 2))
        {
            if (abs(pre.translation.x - posInfo->translation.x) > EPSILON ||
                abs(pre.translation.y - posInfo->translation.y) > EPSILON ||
                abs(pre.translation.z - posInfo->translation.z) > EPSILON ||
                abs(pre.translation.w - posInfo->translation.w) > EPSILON)
            {
                TransformCommand* cmd = new TransformCommand(pre, var, mObject);
                m_editor->m_commandManager->GetcommandStack().push_back(cmd);
                m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
                isChanged = true;
            }
        }
        if(ImGui::InputFloat4("Scale", posInfo->scale, 2))
        {
            if (abs(pre.scale.x - posInfo->scale.x) > EPSILON ||
                abs(pre.scale.y - posInfo->scale.y) > EPSILON ||
                abs(pre.scale.z - posInfo->scale.z) > EPSILON ||
                abs(pre.scale.w - posInfo->scale.w) > EPSILON)
            {
                TransformCommand* cmd = new TransformCommand(pre, var, mObject);
                m_editor->m_commandManager->GetcommandStack().push_back(cmd);
                m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
                isChanged = true;
            }
        }

        if(ImGui::InputFloat("RotateX", &posInfo->rotation.x, 2))
        {
            TransformCommand* cmd = new TransformCommand(pre, var, mObject);
            m_editor->m_commandManager->GetcommandStack().push_back(cmd);
            m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
            isChanged = true;
        }
        if (ImGui::InputFloat("RotateY", &posInfo->rotation.y, 2))
        {
            TransformCommand* cmd = new TransformCommand(pre, var, mObject);
            m_editor->m_commandManager->GetcommandStack().push_back(cmd);
            m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
            isChanged = true;
        }
        if (ImGui::InputFloat("RotateZ", &posInfo->rotation.z, 2))
        {
            TransformCommand* cmd = new TransformCommand(pre, var, mObject);
            m_editor->m_commandManager->GetcommandStack().push_back(cmd);
            m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
            isChanged = true;
        }
        if (ImGui::InputFloat("RotateW", &posInfo->rotation.w, 2))
        {
            TransformCommand* cmd = new TransformCommand(pre, var, mObject);
            m_editor->m_commandManager->GetcommandStack().push_back(cmd);
            m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
            isChanged = true;
        }

        Core::Transform* transform = dynamic_cast<Core::Transform*>(mObject->GetComponent<Core::Transform>(eTransform));
        transform->SetTranslation(posInfo->translation);
        transform->SetRotation(posInfo->rotation);
        transform->SetScale(posInfo->scale);

        return isChanged;
    }
    bool InspecModifier::ChangeBox(Base::Variable var, const Base::Member* member)
    {
        bool isChanged = false;

        // Get reflection information
        void* data = var.GetData();

        Physics::Box* box = reinterpret_cast<Physics::Box*>(data);
        Physics::Box pre = static_cast<Physics::Box>(*box);

        if (ImGui::InputFloat4(member->Name(), box->m_HalfExtents, 2))
        {
            if (abs(pre.m_HalfExtents.x - box->m_HalfExtents.x) > EPSILON ||
                abs(pre.m_HalfExtents.y - box->m_HalfExtents.y) > EPSILON ||
                abs(pre.m_HalfExtents.z - box->m_HalfExtents.z) > EPSILON ||
                abs(pre.m_HalfExtents.w - box->m_HalfExtents.w) > EPSILON)
            {
                BoxCommand* cmd = new BoxCommand(pre, var);
                m_editor->m_commandManager->GetcommandStack().push_back(cmd);
                m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
                isChanged = true;
            }
        }

        return isChanged;
    }
    bool InspecModifier::ChangeSphere(Base::Variable var, const Base::Member* member)
    {
        bool isChanged = false;

        // Get reflection information
        void* data = var.GetData();

        Physics::Sphere* sphere = reinterpret_cast<Physics::Sphere*>(data);
        Physics::Sphere pre = static_cast<Physics::Sphere>(*sphere);
        if (ImGui::InputFloat("m_fRadius", &sphere->m_fRadius, 2))
        {
            SphereCommand* cmd = new SphereCommand(pre, var);
            m_editor->m_commandManager->GetcommandStack().push_back(cmd);
            m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
            isChanged = true;
            isChanged = true;
        }

        return isChanged;
    }
    bool InspecModifier::ChangeCapsule(Base::Variable var, const Base::Member* member)
    {
        bool isChanged = false;

        // Get reflection information
        void* data = var.GetData();

        Physics::Capsule* capsule = reinterpret_cast<Physics::Capsule*>(data);
        Physics::Capsule pre = static_cast<Physics::Capsule>(*capsule);

        if (ImGui::InputFloat4("m_TopPoint", capsule->m_TopPoint, 2))
        {
            if (abs(pre.m_TopPoint.x - capsule->m_TopPoint.x) > EPSILON ||
                abs(pre.m_TopPoint.y - capsule->m_TopPoint.y) > EPSILON ||
                abs(pre.m_TopPoint.z - capsule->m_TopPoint.z) > EPSILON ||
                abs(pre.m_TopPoint.w - capsule->m_TopPoint.w) > EPSILON)
            {
                CapsuleCommand* cmd = new CapsuleCommand(pre, var);
                m_editor->m_commandManager->GetcommandStack().push_back(cmd);
                m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
                isChanged = true;
            }
        }
        if (ImGui::InputFloat4("m_BottomPoint", capsule->m_BottomPoint, 2))
        {
            if (abs(pre.m_BottomPoint.x - capsule->m_BottomPoint.x) > EPSILON ||
                abs(pre.m_BottomPoint.y - capsule->m_BottomPoint.y) > EPSILON ||
                abs(pre.m_BottomPoint.z - capsule->m_BottomPoint.z) > EPSILON ||
                abs(pre.m_BottomPoint.w - capsule->m_BottomPoint.w) > EPSILON)
            {
                CapsuleCommand* cmd = new CapsuleCommand(pre, var);
                m_editor->m_commandManager->GetcommandStack().push_back(cmd);
                m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
                isChanged = true;
            }
        }
        if (ImGui::InputFloat("m_fRadius", &capsule->m_fRadius, 2))
        {
            CapsuleCommand* cmd = new CapsuleCommand(pre, var);
            m_editor->m_commandManager->GetcommandStack().push_back(cmd);
            m_editor->m_commandManager->SetNames(m_editor->GetCurrentObject()->name, member->Name());
            isChanged = true;
        }

        return isChanged;
    }

