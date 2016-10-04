#include "pch/precompiled.h"
#include "../Core/pch/precompiled.h"

#include "LuaEngine.h"
#include "LuaComponent.h"
#include "../deps/lua52/include/lj_obj.h"

namespace Lua
{
    LuaComponent::LuaComponent(const char *name) 
      : FileInfo(std::string(name) + ".lua"), Name(name), RegRef(0) 
    {
      typeID = eLuaComponent;
    }

    LuaComponent::~LuaComponent() { }

    void LuaComponent::Initialize()
    {
        loadLuaScript(gLua->L());
    }

    void LuaComponent::Remove()
    {
        // If we have a valid reference to a function environment
        if (RegRef)
        {
            // Shutdown the script and destroy the reference to the environment
            // So that the garbage collector can take the environment.
            shutdownLuaScript(gLua->L());
            luaL_unref(gLua->L(), LUA_REGISTRYINDEX, RegRef);
        }
    }

    const char *LuaComponent::getName() const { return Name.c_str(); }
    void LuaComponent::setName(const char *newName) { Name = std::string(newName); this->SetFileInfoName(Name + ".lua"); }

    void LuaComponent::loadLuaScript(lua_State *L)
    {
        // If an environment is already referenced, destroy the reference,
        // so that it can be garbage collected
        if (RegRef != 0)
            luaL_unref(L, LUA_REGISTRYINDEX, RegRef);

        // Load the lua script contents into memory
        bool result = report_errors(L, luaL_loadfile(L, (gLua->GetPath() + GetFileInfoName()).c_str() ));

        if (!result)
        {
            // Create a new function environment to hold all of the functions from the loadfile
            lua_createtable(L, 0, 1);
            
            // Construct the environment's metatable
            lua_createtable(L, 0, 1);
            lua_getglobal(L, "_G");
            lua_setfield(L, -2, "__index");
            lua_setmetatable(L, -2);
            lua_pushvalue(L, -1);

            // Sets the this reference for the script environment
            // (when the script code is run it can reference the
            // LuaComponent in C++ that hold its information)
            luabridge::push(L, this);
            luabridge::rawsetfield(L, -3, "this");

            // Create a reference to the function environment, so
            // it doesn't get taken by the garbage collector
            RegRef = luaL_ref(L, LUA_REGISTRYINDEX);

            // Set the environment for the script and execute it
            lua_setfenv(L, -2);

            report_errors(L, lua_pcall(L, 0, 0, 0));

            // Run the script's initialization function
            callLuaFunction(L, "Initialize");
        }
    }

    void LuaComponent::callLuaFunction(lua_State *L, const char *function, int args, int returns)
    {
        // Retrieve the function environment from the registry
        lua_rawgeti(L, LUA_REGISTRYINDEX, RegRef);

        // In case the script wasn't loaded properly
        if (lua_isnil(L, -1))
        {
            std::cout << "Error: " << GetFileInfoName() << " instance has not been loaded"
                << function << " cannot be called!" << std::endl;

            return;
        }

        // Get the function to be called from the function environment
        lua_getfield(L, -1, function);

        // In case the function wasn't found
        if (lua_isnil(L, -1))
        {
            std::cout << "Error: " << GetFileInfoName() << " does not contain a function named "
                << ", call terminated!" << std::endl;
            
            return;
        }

        // Make copy references of the function arguments
        // and put them in the right spot for the protected call!
        if (args != 0)
        {
            for (int i = 0; i < args; ++i)
            {
                lua_pushvalue(L, -2 - args);
            }
        }

        // Do a protected call of the function with the arguments
        // and return values.
        report_errors(L, lua_pcall(L, args, returns, 0));

        // TODO(Austin): Check to see if the following pops remove return
        // values from the stack. They may need to popped off, put into a
        // table (if there are multiple) and then returned in a LuaRef from
        // the function.

        // Pop whatever is on the stack
        lua_pop(L, 1);

        // If there were arguments pop the remnants off 
        // of the stack
        if (args != 0)
            lua_pop(L, args);
    }

    void LuaComponent::callLuaFunction(lua_State *L, luabridge::LuaRef function, int args, int returns)
    {
      // In case the script wasn't loaded properly
      if (function == luabridge::Nil())
      {
        std::cout << "Error: " << GetFileInfoName() << " instance has not been loaded"
          << function << " cannot be called!" << std::endl;

        return;
      }

      // Get the function to be called from the function environment
      function.push(gLua->L());

      // In case the function wasn't found
      if (lua_isnil(L, -1))
      {
        std::cout << "Error: " << GetFileInfoName() << " does not contain a function named "
          << ", call terminated!" << std::endl;

        return;
      }

      // Make copy references of the function arguments
      // and put them in the right spot for the protected call!
      if (args != 0)
      {
        for (int i = 0; i < args; ++i)
        {
          lua_pushvalue(L, -1 - args);
        }
      }

      // Do a protected call of the function with the arguments
      // and return values.
      report_errors(L, lua_pcall(L, args, returns, 0));

      // If there were arguments pop the remnants off 
      // of the stack
      if (args != 0)
        lua_pop(L, args);
    }

    void LuaComponent::shutdownLuaScript(lua_State *L)
    {
        callLuaFunction(L, "Shutdown");
    }

    void LuaComponent::setProp(const char *key, luabridge::LuaRef val)
    {
        // Find the script's global variable in the
        // map of variables
        auto it = Vars.find(key);

        // If it's not there, then add it with the new value
        if (it == Vars.end())
            Vars.insert(std::make_pair(key, val));

        // Otherwise just assign what's already there to val
        else
            it->second = val;
    }

    luabridge::LuaRef LuaComponent::getProp(const char *key) const
    {
        // Grab the function environment to look through it
        lua_rawgeti(gLua->L(), LUA_REGISTRYINDEX, RegRef);

        // Make sure you grabbed a table
        if (lua_istable(gLua->L(), -1))
        {
            // Get the referenced variable from the function environment
            lua_getfield(gLua->L(), -1, key);

            // If it's in the function environment
            if (!lua_isnil(gLua->L(), -1))
            {
                // Grab the value
                luabridge::LuaRef v = luabridge::LuaRef::fromStack(gLua->L(), -1);
                // Clean up the stack
                lua_pop(gLua->L(), -2);
                // And return the function that was pulled from the environment table
                return v;
            }
        }

        // If it wasn't in the function environment, check the variable list
        auto it = Vars.find(key);
        // If it's not in the variable list
        if (it == Vars.end())
        {
            // Return nil back to whoever wanted the non-existant variable/function
            return luabridge::LuaRef(gLua->L());
        }

        // If you found the variable in the map, return it back to whoever wanted it
        return it->second;
    }

    // Returns true if there was an error
    bool LuaComponent::report_errors(lua_State *L, int state)
    {
#ifndef DEPLOYMENT
      // Check to see if there is an error string on the stack
      // Grab it if there is one.
      if (gLua->L()->base != gLua->L()->top)
      {
        if (lua_isstring(gLua->L(), -1))
        {
          std::cout << "Error: " << GetFileInfoName() << " <-----> " <<
            lua_tostring(gLua->L(), -1) << std::endl;

          lua_pop(L, 1);
        }
      }

        // Check the state against known Lua Error Codes
        switch (state)
        {
            case 0:
            {
                return false;
            } break;
            case LUA_ERRRUN:
            {
                return true;
            } break;
            case LUA_ERRSYNTAX:
            {
                std::cerr << "Error: " << GetFileInfoName() << 
                    " <-----> Syntax error while being compiled" << std::endl;
                return true;
            } break;
            case LUA_ERRMEM:
            {
                // Generally a fatal error, since it means other Lua calls
                // are unlikely to work either
                std::cout << "Error: " << GetFileInfoName() << 
                    " <-----> Memory allocation error while loading script" << std::endl;
                return true;
            } break;
            case LUA_ERRERR:
            {
                return true;
            } break;
            case LUA_ERRFILE:
            {
                std::cout << "Error: " << GetFileInfoName() << 
                    " <-----> Cannot find/open Lua script" << std::endl;
                return true;
            } break;

            default:
            {
                std::cout << "Unknown Error encountered in: " <<
                    GetFileInfoName() << std::endl;
                return true;
            } break;
        }
#endif
        return false;
    }

    void LuaComponent::Serialize(File& file, Variable var)
    {
      LuaComponent *lComp = var.GetValue<LuaComponent *>();

      // Get a pointer to the serializer
      Serializer* s = Serializer::Get();

      // Store a pointer to the typeinfo for later use
      const TypeInfo* info = var.GetTypeInfo();

      // Variable for members (translation and stuff)
      Variable mem;

      // Write to the file the name of the type
      file.Write("%s\n", info->Name());

      // Increase padding
      s->Padding(file, s->GetPadLevel()++);

      // Make a bracket and newline
      file.Write("{\n");

      // Name of the LuaComponent
      s->Padding(file, s->GetPadLevel());
      file.Write("%s ", "Name");
      mem = Variable(lComp->Name);
      mem.Serialize(file);

      // Decrease padding level
      s->Padding(file, --s->GetPadLevel());

      // Write a bracket
      file.Write("}\n");
    }

    void LuaComponent::Deserialize(File& file, Variable var)
    {
      LuaComponent *lComp = var.GetValue<LuaComponent *>();
      Serializer* s = Serializer::Get();

      // Peek function can figure out the starting level
      int startLevel = s->GetPadLevel();

      std::string line = file.GetLine('{');
      char buf[64];

      // Read past Name
      file.Read("%s", &buf);

      std::string name;

      GET_TYPE(std::string)->Deserialize(file, name);

      lComp->setName(name.c_str());

      file.GetLine('\n');
      file.GetLine('\n');

      gLua->AddInitToList(lComp);
    }

    //COMP_LUA_DEP(LuaComponent);
}

