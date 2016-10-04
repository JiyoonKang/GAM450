/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: EditorMainMenuBar.h
 * Author: Jiyun Kang
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include <map>

#include"Core\engine\factory\Factory.h"
#include"Editor\InGameEditor\Types.h"
#include "Editor\InGameEditor\InGameEditor.h"

//namespace Editor

    //class EditorMenu;

    class EditorMainMenuBar
    {
    public:
        EditorMainMenuBar(void);
        EditorMainMenuBar(InGameEditor* editor);
        ~EditorMainMenuBar(void);

        static bool Initialize(void);
        static void Update(void);
        static bool IsMenuBarActive(void);
        static void SetMenuBarActive(bool setting);
        static bool IsMenuBarVisible(void);
        static void SetMenuBarVisible(bool setting);

    private:

        static void UpdateEditorTab(void);
        static void UpdateFileTab(void);
        static void UpdateEditTab(void);
        static void UpdateSettingTab(void);
        static void UpdateHelpTab(void);
        static void UpdateViewTab(void);

        static void SaveWindow(void);
        static void SaveAsWindow(void);
        static void CreateNewLevel(void);

        static bool m_isActive;
        static bool m_isVisible;
        //Level Variable
        static std::string m_currentLevelName;
        static std::string m_newLevelName;
        static bool        m_levelChange;
        static bool        m_levelDelete;
        static bool        m_levelChagneComplete;

        static InGameEditor*   m_editor;

        static bool loadCheck;

    };

