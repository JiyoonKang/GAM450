/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: EditorProjectWindow.h
 * Author: Jiyun Kang
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include <string>

#include "Editor\EditorUI\EditorWindow.h"

//namespace Editor


    class ProjectWindow : public EditorWindow
    {

    public:
        ProjectWindow(void);
        ProjectWindow(InGameEditor* editor);
        ~ProjectWindow(void);

        virtual bool Initialize(void) override { return InitializeProject(); };
        virtual void Update(void) override { ProjectUpdate(); };

        virtual bool IsWindowActive(void) override { return IsProjectActive(); };
        virtual void SetWindowActive(bool setting) override { SetProjectActive(setting); };
        virtual bool IsWindowVisible(void) override { return IsProjectVisible(); };
        virtual void SetWindowVisible(bool setting) override { SetProjectVisible(setting); };

    private:
        //Methods

        static bool InitializeProject(void);
        static void ProjectUpdate(void);
        static bool IsProjectActive(void);
        static void SetProjectActive(bool setting);
        static bool IsProjectVisible(void);
        static void SetProjectVisible(bool setting);



    private:
        //Variables

        static std::string m_name;
        static bool        m_isProjectActive;
        static bool        m_isProjectVisible;

        static InGameEditor*   m_editor;
    };

