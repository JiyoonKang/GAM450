/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: EditorInspectorWindow.h
 * Author: Jiyun Kang
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include <string>
#include <map>

#include "Editor\EditorUI\EditorWindow.h"
#include "Base\modules\introspection\types\TypeInfo.h"
#include "Base\modules\introspection\variable\Variable.h"

//namespace Editor

    class InspecModifier
    {
    public:

        InspecModifier(void);
        ~InspecModifier(void);

        InspecModifier(Core::GameObject* Object, InGameEditor* editor);

        static bool ChangeIntData(Base::Variable var, const Base::Member* member);
        static bool ChangeFloatData(Base::Variable var, const Base::Member* member);
        static bool ChangeBoolData(Base::Variable var, const Base::Member* member);
        static bool ChangeStringData(Base::Variable var, const Base::Member* member);
        static bool ChangeFloat3Data(Base::Variable var, const Base::Member* member);
        static bool ChangeFloat4Data(Base::Variable var, const Base::Member* member);
        static bool ChangeTransform(Base::Variable var, const Base::Member* member);
        static bool ChangeBox(Base::Variable var, const Base::Member* member);
        static bool ChangeSphere(Base::Variable var, const Base::Member* member);
        static bool ChangeCapsule(Base::Variable var, const Base::Member* member);

        static Core::GameObject* mObject;

    private:
        static InGameEditor*   m_editor;
    };

    class InspectorWindow : public EditorWindow
    {

    public:
        InspectorWindow(void);
        InspectorWindow(InGameEditor* editor);
        ~InspectorWindow(void);

        virtual bool Initialize(void) override { return InitializeInspector(); };
        virtual void Update(void) override { InspectorUpdate(); };

        virtual bool IsWindowActive(void) override { return IsInspectorActive(); };
        virtual void SetWindowActive(bool setting) override { SetInspectorActive(setting); };
        virtual bool IsWindowVisible(void) override { return IsInspectorVisible(); };
        virtual void SetWindowVisible(bool setting) override { SetInspectorVisible(setting); };

        static bool AddComponent(void);
        static bool ChangeName(void);
        static bool ChangeArchtype(void);
        static bool Inspect(Core::GameObject* object);

        static void CreateModelButton(std::string& result);
        static void CreateTextureButton(std::string& result);
        static void CreateNormalTextureButton(std::string& result);
        static void CreateEmissiveTextureButton(std::string& result);


        static char m_TextBuffer[256];

    private:
        //Methods

        static bool InitializeInspector(void);
        static void InspectorUpdate(void);
        static bool IsInspectorActive(void);
        static void SetInspectorActive(bool setting);
        static bool IsInspectorVisible(void);
        static void SetInspectorVisible(bool setting);

        static bool InspecRec(const EComponent typeID, Base::Variable var, Core::GameObject* object = nullptr);
        static bool InspecPod(const EComponent typeID, Base::Variable var, const Base::Member* member, Core::GameObject* object = nullptr);
        static bool IsBaseCase(const Base::TypeInfo* info);
        static void SaveToArchytype(Core::GameObject* object);


    private:
        //Variables
        static std::string m_name;
        static bool        m_isInspectorActive;
        static bool        m_isInspectorVisible;
        static InGameEditor*   m_editor;

        static std::unordered_map<std::string, size_t> m_orderedComponent;
        static std::vector<std::string> m_orderedComponentStr;

        static InspecModifier* m_modifier;

        //Keep doing...
    };

