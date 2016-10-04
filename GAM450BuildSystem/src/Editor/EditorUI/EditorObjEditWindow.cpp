/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: EditorObjEditWindow.cpp
 * Author: Jiyun Kang
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

#include <string>
#include <cstdio>
#include "Editor\EditorUI\EditorObjEditWindow.h"

#include "imgui\imgui.h"

//namespace Editor


    bool ObjEditWindow::m_isObjEditActive = true;
	bool ObjEditWindow::m_isObjEditVisible = false;
    InGameEditor* ObjEditWindow::m_editor = NULL;
    std::string ObjEditWindow::m_name;

    ObjEditWindow::ObjEditWindow(void)
    {
        m_name = "ObjEdit";
    }

    ObjEditWindow::ObjEditWindow(InGameEditor* editor)
    {
        m_editor = editor;
    }

    ObjEditWindow::~ObjEditWindow(void)
    {
    }
    bool ObjEditWindow::InitializeObjEdit(void)
    {
        return true;
    }
    void ObjEditWindow::ObjEditUpdate(void)
    {

    }

    bool ObjEditWindow::IsObjEditActive(void)
    {
        return m_isObjEditActive;
    }
    void ObjEditWindow::SetObjEditActive(bool setting)
    {
        m_isObjEditActive = setting;
    }
    bool ObjEditWindow::IsObjEditVisible(void)
    {
        return m_isObjEditVisible;
    }
    void ObjEditWindow::SetObjEditVisible(bool setting)
    {
        m_isObjEditVisible = setting;
    }

