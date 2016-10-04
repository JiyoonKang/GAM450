/*******************************************************************************
 * All content � 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Object.cpp 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "Object.h"

#include "components/base/Component.h"

#include <algorithm>
#include <functional>
#include <iterator>

using namespace Base;

namespace Core
{

  //////////////////////////////////////////////////////////////////////////
  // HELPERS

  //Binary search a sorted array of Objects
  static GameObject* BinaryChildSearch(GameSpace* space, ChildArray& children, size_t uid)
  {
    size_t begin = 0;
    size_t end = children.size();

    while(begin < end)
    {
      size_t mid = (begin + end) / 2;
      if (space->GetHandles().GetAs<GameObject>(children[mid])->guid < uid)
        begin = mid + 1;
      else
        end = mid;
    }

    if((begin < children.size()) && (space->GetHandles().GetAs<GameObject>(children[begin])->guid == uid))
      return space->GetHandles().GetAs<GameObject>(children[begin]);
    else
      return NULL;
  }

  bool GameObject::ObjectSorter(Handle left, Handle right)
  {
    return space->GetHandles().GetAs<GameObject>(left)->guid > space->GetHandles().GetAs<GameObject>(right)->guid;
  }

  //////////////////////////////////////////////////////////////////////////

  GameObject::GameObject()
    : Generic(eGameObject), archetype(), name(), m_allocated(true)
  {
    for (unsigned int i = 0; i < ecountComponents; ++i)
      m_components[i] = Handle::null;
    fastChildSearch = false;
  }

  GameObject::~GameObject()
  {
    // If the object is not active, time to destroy it

    // Remove components from object first
    for (unsigned int j = 0; j < ecountComponents; ++j)
    {
      EComponent type = (EComponent)j;

      // Check to see if the Object has that type of component
      if (HasComponent(type) && type != eLuaProxy)
      {
        RemoveComponent(type);
      }
    }

    // Then detatch all components
    for (unsigned int j = 0; j < ecountComponents; ++j)
    {
      EComponent type = (EComponent)j;
      if (HasComponent(type) && type != eLuaComponent)
      {
        DetatchComponent(type);
      }
    }

    for (unsigned int j = 0; j < m_luaComponents.size(); ++j)
    {
      Lua::LuaComponent* comp = GetLuaComponent(j);
      comp->Remove(); // Call the remove routine

      // Remove the handle from the space
      space->GetHandles().Remove(comp->self);

      // Free the component and update any handles
      GameComponent* moved =
        (GameComponent*)space->GetComponents(eLuaProxy)->Free(comp);

      if (moved)
        space->GetHandles().Update(moved, moved->self);

    }
    m_luaComponents.clear();
  }

  void GameObject::Initialize()
  {
    hooks.space = space;

    GameComponent* component;

    for (size_t i = 0; i < ecountComponents; ++i)
    {
      if (m_components[i] == Handle::null)
        continue;
      component = space->GetHandles().GetAs<GameComponent>(m_components[i]);
      component->owner = self;
      component->Initialize();
    }

    if (space->GetName().length() > 0)
    for (size_t i = 0; i < m_luaComponents.size(); ++i)
    {
      component = space->GetHandles().GetAs<GameComponent>(m_luaComponents[i]);
      component->owner = self;
      component->Initialize();
    }
  }

  void GameObject::Destroy()
  {
    m_active = false;
  }

  void GameObject::Copy(GameObject& rhs)
  {
    archetype = rhs.archetype;
    name = rhs.name;

    for (unsigned int i = 0; i < ecountComponents; ++i)
      m_components[i] = Handle::null;
    fastChildSearch = false;

    // Check for components in the rhs object
    for (unsigned int i = 0; i < ecountComponents; ++i)
    {
      if (rhs.HasComponent(EComponent(i)) && i != eLuaComponent) // TODO
      {
        // Create the component that is needed and add it to self
        GameComponent* comp = space->CreateComponent(EComponent(i));
        AddComponent(comp);

        // Get the type info relating to this component
        const TypeInfo *typeInfo = FACTORY->GetComponentType(EComponent(i));

        Variable LVar = Variable(typeInfo, (char*)comp);
        Variable RVar = Variable(typeInfo, (char*)rhs.GetComponent(i));
        typeInfo->Copy(LVar.GetData(), RVar.GetData());

        // Iterate through all members in the component that are registered to be serialized
        //for (unsigned int m = 0; m < typeInfo->GetMembers().size(); ++m)
        //{
        //  // Get the actual member
        //  const Member member = typeInfo->GetMembers()[m];
        //  // Copy over the member data from the archetype into the object
        //  Variable LVar = Variable(member.Type(), (char*)comp + member.Offset());
        //  Variable RVar = Variable(member.Type(), (char*)rhs.GetComponent(i) + member.Offset());
        //  member.Type()->Copy(LVar.GetData(), RVar.GetData());
        //}

      }
    } // End component iteration

    //const TypeInfo* typeInfo = GET_TYPE(LuaComponent);

    //for (unsigned int i = 0; i < rhs.m_luaComponents.size(); ++i)
    //{
    //  LuaComponent* comp = (LuaComponent*)space->CreateComponent(eLuaComponent);
    //  AddComponent(comp);

    //  // Iterate through all members in the component that are registered
    //  // to be serialized
    //  for (unsigned int m = 0; m < typeInfo->GetMembers().size(); ++m)
    //  {
    //    const Base::Member member = typeInfo->GetMembers()[m];


    //    // Copy over the member data from the archetype into the object
    //    Base::Variable LVar = Base::Variable(member.Type(), (char*)comp + member.Offset());
    //    Base::Variable RVar = Base::Variable(member.Type(),
    //      (char*)(rhs.GetLuaComponent(i)) + member.Offset());

    //    member.Type()->PlacementDelete(LVar.GetData());
    //    member.Type()->PlacementCopy(LVar.GetData(), RVar.GetData());
    //  }

    //}

  }

  //////////////////////////////////////////////////////////////////////////
  // CHILD / PARENT

  GameObject* GameObject::GetParent()
  {
    return space->GetHandles().GetAs<GameObject>(m_parent);
  }

  GameObject* GameObject::GetChild(size_t uid)
  {
    if (!fastChildSearch)
      return NULL;

    return BinaryChildSearch(space, m_children, uid);
  }

  void GameObject::AddChild(Handle obj)
  {
    m_children.push_back(obj);

    if (!fastChildSearch)
      return;

    // Sort the child array so binary search can be used to find children quickly
    std::sort(m_children.begin(), m_children.end(), std::bind(&GameObject::ObjectSorter, 
      this, std::placeholders::_1, std::placeholders::_2) );
  }

  void GameObject::SetParent(Handle obj)
  {
    // Set the parent
    m_parent = obj;

    // Add the child onto the parent
    space->GetHandles().GetAs<GameObject>(obj)->AddChild(self);
  }

  //////////////////////////////////////////////////////////////////////////
  // COMPONENT FUNCTIONS

  void GameObject::AddComponent(GameComponent* component)
  {
    // Set the component's owner to ourself
    component->owner = self;

    if (component->typeID == eLuaProxy)
      m_luaComponents.push_back(component->self);
    else
      m_components[component->typeID] = component->self;
  }

  void GameObject::DetatchComponent(EComponent type)
  {
    if (type == eLuaProxy)
      return;

    // Get the component
    GameComponent* comp = GetComponent(type);

    if (!comp)
      return;

    if (!comp->m_toDelete)
      RemoveComponent(comp);

    // Remove the handle from the space
    space->GetHandles().Remove(comp->self);

    // Deconstruct the game component
    comp->~GameComponent();

    // Free the component and update any handles
    GameComponent* moved =
      (GameComponent*)space->GetComponents(type)->Free(comp);


    if (moved)
      space->GetHandles().Update(moved, moved->self);

    m_components[type] = Handle::null;
  }

  void GameObject::DetatchLuaComponent(unsigned index)
  {
    // Get a pointer to the actual component
    Lua::LuaComponent* comp = GetLuaComponent(index);

    // Replace the entry in the array with the furthest right
    // entry (back of the vector) or if it is the back, just pop it
    if (index != m_luaComponents.back())
      m_luaComponents[index] = m_luaComponents.back();

    m_luaComponents.pop_back();

    comp->Remove();
    space->GetHandles().Remove(comp->self);

    GameComponent* moved = (GameComponent*)space->GetComponents(eLuaComponent)->Free(comp);
  }

  void GameObject::RemoveComponent(EComponent type)
  {
    // Get the component
    GameComponent* comp = GetComponent(type);
	  if (comp)
	  {
	  	comp->Remove();
	  	comp->m_toDelete = true;
	  }
  }

  void GameObject::RemoveComponent(GameComponent* comp)
  {
    comp->Remove();
    comp->m_toDelete = true;
  }

  bool GameObject::HasComponent(EComponent type) const
  {
    return m_components[type] != Handle::null;
  }

  bool GameObject::HasComponent(size_t type) const
  {
    return m_components[type] != Handle::null;
  }

  int GameObject::HasLuaComponent(const char* name) const
  {
    for (unsigned i = 0; i < m_luaComponents.size(); ++i)
    {
      if (strcmp(
        space->GetHandles().GetAs<Lua::LuaComponent>(m_luaComponents[i])->Name.c_str(),
        name) == 0)
      {
        return i;
      }
    }

    return -1;
  }

  Handle GameObject::GetComponentHandle(const char* type)
  {
    if (GET_ENUM(Component)->IsAnEntry(type))
    {
      EComponent i = (EComponent)GET_ENUM(Component)->GetIndexFromString(type);
      return GetComponentHandle(i);
    }

    return Handle::null;
  }

  Handle GameObject::GetComponentHandle(EComponent type)
  {
    return m_components[type];
  }

  GameComponent* GameObject::GetComponent(const char *type)
  {
    if (GET_ENUM(Component)->IsAnEntry(type))
    {
      EComponent i = (EComponent)GET_ENUM(Component)->GetIndexFromString(type);
      return GetComponent(i);
    }
    return NULL;
  }

  GameComponent* GameObject::GetComponent(unsigned int type)
  {
	  if (m_components[type] == Handle::null)
		  return NULL;
    return space->GetHandles().GetAs<GameComponent>(m_components[type]);
  }

  Lua::LuaComponent* GameObject::GetLuaComponent(unsigned int index)
  {
    if (m_luaComponents[index] == Handle::null)
      return NULL;
    return space->GetHandles().GetAs<Lua::LuaComponent>(m_luaComponents[index]);
  }

  Lua::LuaComponent* GameObject::GetLuaComponent(const char* name)
  {
    int lcIndex = HasLuaComponent(name);
    if (lcIndex != -1)
    {
      return GetLuaComponent(lcIndex);
    }

    return nullptr;
  }

  int GameObject::lua_GetComponent(lua_State *L)
  {
    // If the top of the Lua stack is a string (which it should be!)
    if (lua_isstring(L, -1))
    {
      // Then we grab the string, which should be the string name of the type we want to get
      const char *type = lua_tostring(L, -1);
      GameComponent *comp = GetComponent(type);
      // If the component exists on the object
      if (comp)
      {
        // Then we push it to Lua as this function's return value.
        comp->pushToLua(L);
        return 1;
      }
      // Otherwise check to see if it's a valid Lua component
      else
      {
        Lua::LuaComponent *lComp = GetLuaComponent(type);
        // If it's a valid Lua component
        if (lComp)
        {
          // Push it as our return value for this function.
          luabridge::push(L, lComp);
          return 1;
        }
      }
    }
    // Otherwise we return 0 from the function, which forcibly returns nil from this function.
    return 0;
  }

  GameObject *GameObject::GetChildByName(const char *name)
  {
    auto& hManager = space->GetHandles();

    for (int i = 0; i < m_children.size(); ++i)
    {
      if (hManager.GetAs<GameObject>(m_children[i])->GetName() == name)
      {
        return hManager.GetAs<GameObject>(m_children[i]);
      }
    }

    return NULL;
  }
  

  //////////////////////////////////////////////////////////////////////////
  // SERIALIZATION ROUTINES

  void GameObject::Serialize(File& file, Variable var)
  {
    GameObject *o = &var.GetValue<GameObject>();
    Serializer *s = Serializer::Get();
    const TypeInfo* info = var.GetTypeInfo();
    int& pad = s->GetPadLevel();

    s->Padding(file, pad);
    file.Write("GameObject\n");
    s->Padding(file, pad++ );
    file.Write("{\n");

    // Iterate through the members we need to serialize
    for(unsigned i = 0; i < info->GetMembers().size(); ++i)
    {
      // Get a pointer to the member
      const Base::Member* member = &info->GetMembers().front() + i;

      // Add some padding in
      s->Padding( file, pad );

      // Get a pointer to the member type
      const TypeInfo* memberInfo = member->Type();

      // Write the name of the member
      file.Write("%s ", member->Name());

      // Get a pointer to the location of the member inside the data
      // struct/class itself
      void* offsetData = PTR_ADD(o, member->Offset());

      // Construct a new variable out of that, and then serialize that variable
      memberInfo->Serialize(file, Variable(memberInfo, offsetData));
    }

    Serializer::Get()->SetUserData(o->space);

    // Now we are going to iterate through every component and serialize them
    for ( unsigned int i = 0; i < ecountComponents; ++i)
    {
      if (o->HasComponent(EComponent(i)) && i != eLuaComponent)
      {
        s->Padding( file, pad );

        GameComponent* gc = o->space->GetHandles().GetAs<GameComponent>(o->m_components[i]);
        Variable v(gc->typeinfo, gc);
        v.Serialize(file);
      }
    }

    // Serialize lua components
    info = GET_TYPE(Lua::LuaComponent);
    for (unsigned int i = 0; i < o->m_luaComponents.size(); ++i)
    {
      Lua::LuaComponent* LC =
        o->space->GetHandles().GetAs<Lua::LuaComponent>(o->m_luaComponents[i]);

      s->Padding(file, pad);
      Variable v(info, LC);
      v.Serialize(file);

      if (i != o->m_luaComponents.size() - 1)
        file.Write("\n");
    }

    Serializer::Get()->SetUserData(nullptr);

    // We are all done so close the bracket
    s->Padding(file, --pad);
    file.Write("}\n");
  }

  void GameObject::Deserialize(File& file, Variable var)
  {
    Serializer* s = Serializer::Get();
    GameObject* obj = &var.GetValue<GameObject>();
    fpos_t lastcomp;

    // Peak into the file and see if we can recognize a data type
    const TypeInfo* info = Serializer::Get()->PeekType(file);

    // Make sure it is indeed an object
    ErrorIf2(var.GetTypeInfo( ) != info, "GameObject deserialization",
      "Invalid type found");

    // Our peek function was nice enough to figure out
    // the starting level for us
    int startLevel = Serializer::Get()->GetPadLevel();


    for (unsigned int i = 0; i < info->GetMembers().size(); ++i)
    {
      const Member* mem = s->PeekMember(file, info->GetMembers(), startLevel);

      // If we found a member then we can turn it into a variable type and then
      // deserialize that variable into to data we can use
      if (mem)
      {
        // Create a Variable out of the member we found,
        // we need to offset the start position of the variable by the start
        // of our current variable by the offset from the member and
        // the start of the current variable
        Variable member( mem->Type(), PTR_ADD(var.GetData(), mem->Offset()) );

        // Deserialize the Variable
        member.Deserialize(file);
      }
    }
    
    // Now that we have read in any additional members from the GameObject
    // We can move onto reading in the components
    fgetpos(file.fp, &lastcomp);

    for(;;)
    {
      GameComponent* comp = FACTORY->DeserializeComponent(file, obj->space);

      if (!comp)
        break;

      obj->AddComponent(comp);
      fgetpos(file.fp, &lastcomp);
    }

    // We read in all the components, initialize the object
    obj->Initialize();

    // Sometimes we are reading an object in from the middle of a level file
    // so we need to return the file pointer back to the last } we encountered
    if (file.Validate())
    {
      fsetpos(file.fp, &lastcomp);
      file.GetLine("}");
    }
  }

  //////////////////////////////////////////////////////////////////////////
  // LUA FUNCTIONS

  void GameObject::LuaGetComponent(size_t type)
  {
    //TODO(JUDY)
  }

  bool GameObject::LuaHasComponent(size_t type)
  {
    if (!HasComponent(EComponent(type)))
      return false;
    else
      return true;
  }

};