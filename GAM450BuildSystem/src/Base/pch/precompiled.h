/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: precompiled.h 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/

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

#include "util/Macros.h"
#include "util/Utilities.h"

#include "util/math/Float2.h"
#include "util/math/Float3.h"
#include "util/math/Float4.h"
#include "util/math/Float4x4.h"
#include "util/math/VectorMath.h"
#include "util/math/VectorLua.h"

#include "modules/memory/ObjectAllocator.h"
#include "modules/handlemanager/HandleManager.h"

#include "modules/introspection/types/TypeInfo.h"
#include "modules/introspection/variable/Variable.h"
#include "Base/modules/introspection/enum/Enum.h"
#include "modules/functionbinding/FuncSignature.h"
#include "modules/functionbinding/Function.h"
#include "modules/introspection/manager/IntrospectionManager.h"

#include "modules/introspection/Qualifiers.h"


#include "modules/serialization/PODSerialization.h"
#include "modules/serialization/Serialization.h"

//#include "modules/lua/interface/LuaInterface.h"

//#include "modules/lua/transfer/LuaTransfer.h"

//#include "util/eventhandler/EventHandler.h"
//#include "util/messager/Messenger.h"