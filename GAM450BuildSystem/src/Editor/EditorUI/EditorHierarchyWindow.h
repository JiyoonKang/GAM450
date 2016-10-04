/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: EditorHierarchyWindow.h
 * Author: Jiyun Kang
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include <string>

#include "Editor\EditorUI\EditorWindow.h"
#include "Core\types\space\Space.h"

//namespace Editor


    class HierarchyWindow : public EditorWindow
    {

    public:
        HierarchyWindow(void);
        HierarchyWindow(InGameEditor* editor);
        ~HierarchyWindow(void);

        virtual bool Initialize(void) override { return InitializeHierarchy(); };
        virtual void Update(void) override { HierarchyUpdate(); };

        virtual bool IsWindowActive(void) override { return IsHierarchyActive(); };
        virtual void SetWindowActive(bool setting) override { SetHierarchyActive(setting); };
        virtual bool IsWindowVisible(void) override { return IsHierarchyVisible(); };
        virtual void SetWindowVisible(bool setting) override { SetHierarchyVisible(setting); };

    private:
        //Methods

        static bool InitializeHierarchy(void);
        static void HierarchyUpdate(void);
        static bool IsHierarchyActive(void);
        static void SetHierarchyActive(bool setting);
        static bool IsHierarchyVisible(void);
        static void SetHierarchyVisible(bool setting);

        static void MakeObjectButton(Core::GameObject* objects, int index);
        static void CreateEmptySpace(void);
        static void LoadSpaceFromFile(void);
        static void CreateEmptyObject(void);
        static void LoadObjectFromFile(void);
        static void SaveToNewArchetype(void);
        static void SaveToExistArchetype(void);
        static void CopyAndPasteCurrentObject(void);

    private:
        //Variables

        static std::string m_name;
        static bool        m_isHierarchyActive;
        static bool        m_isHierarchyVisible;
        static InGameEditor*   m_editor;

    };

