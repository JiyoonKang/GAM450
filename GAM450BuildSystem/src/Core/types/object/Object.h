/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Object.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Core/components/base/Component.h"
#include "Core/engine/hooks/Hooks.h"
#include "Lua/components/LuaProxy.h"
#include "Lua/components/LuaComponent.h"
#include "Core/types/space/Space.h"


#include <vector>
#include <memory>

namespace Core
{
  class GameObject;

  typedef Handle* ComponentArray;
  typedef std::vector<Handle> ChildArray;

  class GameObject : public Generic
  {
    public:
      GameObject();

      ~GameObject();

      // Initializes the object, engine use only
      virtual void Initialize();

      // Copies one object into this object
      void Copy(GameObject& rhs);

      // Marks the object to be destroyed at the end of the frame
      void Destroy(); 

      // Gets the name of the object
      inline std::string GetName() const { return name; }

	  // Sets the name of the object
	  inline void SetName(std::string objectname) { name = objectname; }

      // Gets the archetype of the object
      inline const std::string& GetArchetype() const { return archetype; }

      //////////////////////////////////////////////////////////////////////////
      // CHILD / PARENT

      // Gets a vector of handles which are the objects children
      ChildArray& GetChildren() { return m_children; }

      GameObject *GetChildByName(const char *name);

      // Gets the parent of this object
      GameObject* GetParent();

      // Gets a specific child based on a GUID
      GameObject* GetChild(size_t guid);
      
      // Adds a child to the object
      void AddChild(Handle obj);

      // Sets the objects parent
      void SetParent(Handle obj);

      //////////////////////////////////////////////////////////////////////////
      // COMPONENT FUNCTIONS

      /* ----------- Adding / Removing ---------- */

      // Adds a component to an object
      void AddComponent(GameComponent* component);

      // Runs the remove function on a component attached to the object
      // This is mainly for internal usage.
      void RemoveComponent(EComponent type);

      // Runs the remove function on a component attached to the object
      // This is mainly for internal usage.
      void RemoveComponent(GameComponent* comp);

      // Properly detaches a component from the object
      void DetatchComponent(EComponent type);

      // Properly detaches a Lua Component from the object
      void DetatchLuaComponent(unsigned index);

      /* ----------- Safety Checking ---------- */

      // Checks to see if the object owns a specific component type
      bool HasComponent(EComponent type) const;

      // Checks to see if the object owns a specific component type
      bool HasComponent(size_t type) const;

      // Checks to see if the object has a specific Lua component
      // WARNING: Very slow.
      // Returns -1 if the component is not attached
      int HasLuaComponent(const char* name) const;

      /* ----------- Getters ---------- */

      // Gets a component properly typed
      template <typename T>
      T *GetComponent(EComponent type);

      GameComponent* GetComponent(unsigned int type);

      GameComponent* GetComponent(const char* type);

      // Gets a Lua component via index from the Lua Component vector
      Lua::LuaComponent *GetLuaComponent(unsigned int index);

      // Gets a Lua component by name from the object
      // WARNING: Very Slow
      Lua::LuaComponent* GetLuaComponent(const char* name);

      /* ----------- Handle Getters ---------- */
      Handle GetComponentHandle(EComponent type);

      Handle GetComponentHandle(const char* type);

      //////////////////////////////////////////////////////////////////////////
      // SERIALIZATION ROUTINES

      // Serialization routine
      static void Serialize(File& file, Variable var);

      // Deserialization routine, var must be an empty object
      static void Deserialize(File& file, Variable var);

      //////////////////////////////////////////////////////////////////////////
      // LUA FUNCTIONS

      void LuaGetComponent(size_t type);

      bool LuaHasComponent(size_t type);

      int lua_GetComponent(lua_State *L);

      //////////////////////////////////////////////////////////////////////////
      // PUBLIC MEMBERS

      // The objects Globally Unique ID
      size_t guid;

      // The name of the object
      std::string name;

      // Name of the archetype
      std::string archetype;

      // Hook manager object
      HookManager hooks;

      // If true, GetChild is enabled and the children list will be sorted
      bool fastChildSearch;

    private:
      // Internally used to sort the children objects
      bool ObjectSorter(Handle left, Handle right);

      bool m_allocated = false;

      // Active flag, if the object is inactive it will be removed
      bool m_active;

      // Vector of the components belonging to this object
      Handle m_components[ecountComponents];

      // Vector of all lua components
      std::vector<Handle> m_luaComponents;

      // Vector of the children belonging to this object
      std::vector<Handle> m_children;
      typedef std::vector<Handle>::iterator ChildrenIt;

      // The parent of the object;
      Handle m_parent;

      friend class Factory;
      friend class GameSpace;
      friend class Archetype;
  };

  //////////////////////////////////////////////////////////////////////////

  template<typename T>
  T* GameObject::GetComponent(EComponent type)
  {
    return space->GetHandles().GetAs<T>(m_components[type]);
  }
};


#define LUA_BIND_GAMEOBJECT(luastate) \
  luabridge::getGlobalNamespace(luastate)\
    .beginClass<Core::GameObject>("GameObject")\
      .addCFunction("GetComponent", &Core::GameObject::lua_GetComponent)\
      .addProperty("Name", &Core::GameObject::GetName, &Core::GameObject::SetName)\
      .addProperty("Parent", &Core::GameObject::GetParent, &Core::GameObject::SetParent)\
      .addProperty("Archetype", &Core::GameObject::GetArchetype)\
      .addFunction("Destroy", &Core::GameObject::Destroy)\
      .addFunction("GetChildByName", &Core::GameObject::GetChildByName)\
    .endClass()

// This lets you do Object->Has(Transform)
#define Has(type) GetComponent<type>(e##type)
