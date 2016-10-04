/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: precompiled.h 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
//#include "config/Config.h"

#ifdef _DEBUG
#define DBG_NEW new //( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else  // _DEBUG
#define DBG_NEW new
#endif

extern "C"
{
#include "lua52/include/lua.h"
#include "lua52/include/lua.hpp"
#include "lua52/include/lauxlib.h"
#include "lua52/include/luaconf.h"
#include "lua52/include/lualib.h"
#include "lua52/include/lfs.h"
}

#include <assert.h>

// Utilities
#include "Base/modules/introspection/Qualifiers.h" // Qualifier removal
#include "Base/util/Macros.h" // Macro Helpers
// Debug macros and helpers
#include "Base/util/Utilities.h"


//#include "modules/serializer/file/File.h" // File handler
#include "Base/modules/introspection/variable/Variable.h" // Variable delegate
#include "Base/modules/introspection/enum/Enum.h" // Enumeration Serialization/Introspection

#include "Base/modules/introspection/types/TypeInfo.h" // Type introspection

#include "Base/modules/serialization/Serialization.h" // Serialization
#include "Base/modules/serialization/PODSerialization.h" // Plain old Data serialization routines

#include "Base/modules/functionbinding/FuncSignature.h" // Function Signatures (For Binding)
#include "Base/modules/functionbinding/Function.h" // Function binding

#include "Base/modules/introspection/manager/IntrospectionManager.h" // Introspection

//#include "engine/lua/transfer/LuaTransfer.h" // Lua<->C++ Transfer
//#include "engine/lua/interface/LuaInterface.h" // Lua interface

#include "Base/modules/handle/Handle.h" // Handle type

#include "Base/modules/memory/ObjectAllocator.h" // Object Memory manager
#include "types/generic/Generic.h" // Generic Object/Component type

#include "engine/hooks/Hooks.h" // Hook System

#include "components/base/Component.h" // Component type
#include "components/base/ComponentCreator.h" // Component Factory

#include "types/object/Object.h" // Object type
#include "types/archetype/Archetype.h" // Object Archetype (Virtual type)


#include "types/message/Message.h" // Messaging system


#include "types/space/Space.h" // Game Space type

#include "systems/System.h" // Generic System/Interface type


#include "engine/factory/Factory.h" // Object Factory
#include "engine/core/Engine.h" // Core Engine
#include "engine/frameratecontroller/FrameRateController.h" // Framerate Controller


//// type and function binding
//#include "config/defaulttypes.h"
//#include "config/luadefaultbinds.h"

#include "Base/util/file/file.h"
#include "Base/modules/introspection/variable/variable.h"

#include "Base/modules/handlemanager/HandleManager.h"