/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: EditorObjEditWindow.h
 * Author: Jiyun Kang
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include <string>

#include "Editor\EditorUI\EditorWindow.h"

//namespace Editor


    class ObjEditWindow : public EditorWindow
    {

    public:
        ObjEditWindow(void);
        ObjEditWindow(InGameEditor* editor);
        ~ObjEditWindow(void);

        virtual bool Initialize(void) override { return InitializeObjEdit(); };
        virtual void Update(void) override { ObjEditUpdate(); };

        virtual bool IsWindowActive(void) override { return IsObjEditActive(); };
        virtual void SetWindowActive(bool setting) override { SetObjEditActive(setting); };
        virtual bool IsWindowVisible(void) override { return IsObjEditVisible(); };
        virtual void SetWindowVisible(bool setting) override { SetObjEditVisible(setting); };

    private:
        //Methods

        static bool InitializeObjEdit(void);
        static void ObjEditUpdate(void);
        static bool IsObjEditActive(void);
        static void SetObjEditActive(bool setting);
        static bool IsObjEditVisible(void);
        static void SetObjEditVisible(bool setting);

        //static void Show

    private:
        //Variables

        static std::string m_name;
        static bool        m_isObjEditActive;
        static bool        m_isObjEditVisible;

        static InGameEditor*   m_editor;
    };

