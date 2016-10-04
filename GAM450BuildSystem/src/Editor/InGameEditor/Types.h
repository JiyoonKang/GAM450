/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Types.h
 * Author: Jiyun Kang
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/

#pragma once


#include <Windows.h>
#include <vector>
#include "Base/util/math/Float2.h"
#include "Base/util/math/Float3.h"
#include "Base/util/math/Float4.h"


// Structure
//namespace Editor

    struct EditorVertex
    {
        float3 pos;
        float4 color;
        float2 uv;
    };

    struct EditorRects
    {
        RECT rect;
        unsigned vtxOffset;
    };
    struct Names
    {
        std::string object_name;
        std::string member_name;

        Names(void) {};

        Names(std::string obj, std::string mem)
        {
            object_name = obj;
            member_name = mem;
        }
    };


// Enums

    enum Gizmo
    {
        NONE = 0,
        TRANSLATE,
        ROTATE,
        SCALE
    };

