/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: EditorWindow.h
 * Author: Jiyun Kang
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Base/modules/introspection/enum/Enum.h"

#include "Editor\InGameEditor\InGameEditor.h"
#include "Editor\InGameEditor\Types.h"
#include "Core\engine\core\Engine.h"

#include "Core\types\object\Object.h"
#include "Core\types\space\Space.h"

#include "Base\util\Macros.h"

#include "imgui\imgui.h"

#include "Editor\InGameEditor\InspecCommand.h"
 
//namespace Editor

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

    class EditorWindow
    {
    public:
        EditorWindow(void);
        ~EditorWindow(void);

        virtual bool Initialize(void) = 0;
        virtual void Update(void) = 0;

        virtual bool IsWindowActive(void) = 0;
        virtual void SetWindowActive(bool setting) = 0;
        virtual bool IsWindowVisible(void) = 0;
        virtual void SetWindowVisible(bool setting) = 0;

    private:


    };

