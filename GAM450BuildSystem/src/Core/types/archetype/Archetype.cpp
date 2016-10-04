/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Archetype.cpp 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "Base/util/Macros.h"
#include "types/object/Object.h"
#include "Base/modules/introspection/types/TypeInfo.h"

#include "components/transform/CTransform.h"
#include "Lua/components/LuaProxy.h"

namespace Core
{
  const Archetype Archetype::null;

  Archetype::Archetype(void)
  {
    for (unsigned int i = 0; i < ecountComponents; ++i)
      m_components[i] = nullptr;

    name = "";
    archetype = "";
  }

  Archetype::~Archetype(void)
  {
    // Delete any components which we had allocated
    for (unsigned int i = 0; i < ecountComponents; ++i)
    {
      if (m_components[i] != nullptr)
      {
        //m_components[i]->Remove();
        delete m_components[i];
      }
    }

   /* for (unsigned int i = 0; i < m_luaComponents.size(); ++i)
    {
      if (m_luaComponents[i] != nullptr)
      {
        m_luaComponents[i]->Remove();
        delete m_luaComponents[i];
      }
    }

    m_luaComponents.clear();*/

    name.~basic_string();
    archetype.~basic_string();
  }


  Archetype& Archetype::operator=(const Archetype& rhs)
  {
    name = rhs.name;
    archetype = rhs.archetype;

    for (unsigned int i = 0; i < ecountComponents; ++i)
    if (rhs.HasComponent(EComponent(i)) && i != eLuaProxy)
    {
      m_components[i] = FACTORY->m_componentCreators[i]->Allocate();
      *(m_components[i]) = *(rhs.m_components[i]);
    }

    /*for (unsigned int i = 0; i < rhs.m_luaComponents.size(); ++i)
    {
      m_luaComponents.push_back((LuaComponent*)FACTORY->m_componentCreators[eLuaComponent]->Allocate());
      *(m_luaComponents[i]) = *(rhs.m_luaComponents[i]);
    }*/

    return *this;
  }

  //////////////////////////////////////////////////////////////////////////
  // COMPONENT FUNCTIONS

  const GameComponent* Archetype::GetComponent(unsigned int type) const
  {
    return m_components[type];
  }

  GameComponent* Archetype::GetComponent(unsigned int type)
  {
    return m_components[type];
  }

  bool Archetype::HasComponent(EComponent type) const
  {
    return m_components[type] != nullptr; //|| (type == eLuaProxy && m_luaComponents.size() > 0);
  }

  //////////////////////////////////////////////////////////////////////////
  // OBJECT FUNCTIONS

  GameObject* Archetype::CreateObject(GameSpace* space) const
  {
    GameObject* obj = space->CreateEmptyObject();

    // Set the name and archetype
    obj->name = name;
    obj->archetype = archetype;

    for (unsigned int i = 0; i < ecountComponents; ++i)
    {
      if (HasComponent(EComponent(i)) && i != eLuaComponent)
      {
        // Create the component that is needed
        GameComponent* comp = space->CreateComponent(EComponent(i));

        // Add the component to the object
        obj->AddComponent(comp);

        // Get the type info relating to this component
        const TypeInfo *typeInfo = FACTORY->GetComponentType(EComponent(i));
        typeInfo->Copy(comp, m_components[i]);

        // Iterate through all members in the component that are registered to be serialized
        //for (unsigned int m = 0; m < typeInfo->GetMembers().size(); ++m)
        //{
        //  // Get the actual member
        //  const Member member = typeInfo->GetMembers()[m];
        //  // Copy over the member data from the archetype into the object
        //  Variable LVar = Variable(member.Type(), (char*)comp + member.Offset());
        //  Variable RVar = Variable(member.Type(), (char*)m_components[i] + member.Offset());
        //  member.Type()->Copy(LVar.GetData(), RVar.GetData());
        //}

      }
    } // End component iteration

    //const TypeInfo* typeInfo = GET_TYPE(Lua::LuaProxy);

    //for (unsigned int i = 0; i < m_luaComponents.size(); ++i)
    //{
    //  Lua::LuaProxy* comp = (Lua::LuaProxy*)space->CreateComponent(LuaProxy);
    //  obj->AddComponent(comp);

    //  // Iterate through all members in the component that are registered to be serialized
    //  for (unsigned int m = 0; m < typeInfo->GetMembers().size(); ++m)
    //  {
    //    // Get the actual member
    //    const Member member = typeInfo->GetMembers()[m];
    //    // Copy over the member data from the archetype into the object
    //    Variable LVar = Variable(member.Type(), (char*)comp + member.Offset());
    //    Variable RVar = Variable(member.Type(), (char*)m_luaComponents[i] + member.Offset());

    //    member.Type()->PlacementDelete(LVar.GetData());
    //    member.Type()->PlacementCopy(LVar.GetData(), RVar.GetData());
    //  }

    //}

    // We added all components, now initialize the object
    obj->Initialize();

    return obj;
  }

  void Archetype::CopyObject(GameObject* obj)
  {
    for (unsigned int i = 0; i < ecountComponents; ++i)
    {
      // Check the object has any components
      if (obj->HasComponent(EComponent(i)) && i != eLuaComponent)
      {
        // We do not have that component so lets create it quickly
        if (m_components[i] == nullptr)
          m_components[i] = FACTORY->m_componentCreators[i]->Allocate();

        // Get the type info relating to this component
        const TypeInfo *typeInfo = FACTORY->GetComponentType(EComponent(i));

        Variable LVar = Variable(typeInfo, (char*)m_components[i]);
        Variable RVar = Variable(typeInfo, (char*)obj->GetComponent(i));

        typeInfo->Copy(LVar.GetData(), RVar.GetData());

        //// Iterate through all members in the component that are registered to be serialized
        //for (unsigned int m = 0; m < typeInfo->GetMembers().size(); ++m)
        //{
        //  // Get the actual member
        //  const Member member = typeInfo->GetMembers()[m];

        //  // Copy over the member data from the object into the archetype
        //  Variable LVar = Variable(member.Type(), (char*)m_components[i] + member.Offset());
        //  Variable RVar = Variable(member.Type(), (char*)obj->GetComponent(i) + member.Offset());

        //  member.Type()->Copy(LVar.GetData(), RVar.GetData());
        //}

        // Copy over everything from the object's component into the archetype. This could be dangerous
        //memcpy(m_components[i], obj->GetComponent(i), GET_STR_TYPE((GET_ENUM(Component)->m_literals[i]))->Size());
      }
      else
      {
        // The Object does not have the component, lets check to see if for some reason we do
        if (m_components[i] != nullptr)
        {
          //m_components[i]->Remove();

          // Delete the component
          delete m_components[i];
        }

        // Make sure that it is a nullptr
        m_components[i] = nullptr;
      }
    } // End component iteration

    //const TypeInfo *typeInfo = GET_TYPE(Lua::LuaProxy);

    //if (m_luaComponents.size() > 0)
    //{
    //  for (unsigned int i = 0; i < m_luaComponents.size(); ++i)
    //  {
    //    m_luaComponents[i]->Remove();
    //    delete m_luaComponents[i];
    //  }

    //  m_luaComponents.clear();
    //}

    //for (unsigned int i = 0; i < obj->m_luaComponents.size(); ++i)
    //{
    //  Lua::LuaProxy* comp = (Lua::LuaProxy*)(FACTORY->m_componentCreators[eLuaProxy]->Allocate());
    //  m_luaComponents.push_back(comp);

    //  Lua::LuaProxy* objLC = obj->space->GetHandles().GetAs<Lua::LuaProxy>(obj->m_luaComponents[i]);

    //  if (obj->space->GetName().length() > 0)
    //    objLC->QueryLoadCommand();

    //  // Iterate through all members in the component that are registered to be serialized
    //  for (unsigned int m = 0; m < typeInfo->GetMembers().size(); ++m)
    //  {
    //    // Get the actual member
    //    const Member member = typeInfo->GetMembers()[m];

    //    // Copy over the member data from the object into the archetype
    //    Variable LVar = Variable(member.Type(), (char*)comp + member.Offset());
    //    Variable RVar = Variable(member.Type(), (char*)objLC + member.Offset());

    //    member.Type()->PlacementDelete(LVar.GetData());
    //    member.Type()->PlacementCopy(LVar.GetData(), RVar.GetData());
    //  }

    //} // End component iteration


    // Reset Transform component to zero
    if (m_components[eTransform])
      ((Transform*)m_components[eTransform])->SetTranslation(float4(0, 0, 0, 0));


    // Copy over the name and archetype of the object
    name = obj->name;
    archetype = obj->archetype;
  }

  void Archetype::UpdateObject(GameObject* obj) const
  {
    // Force override the archetype
    obj->archetype = archetype;

    GameSpace* space = obj->space;

    for (unsigned int i = 0; i < ecountComponents; ++i)
    {
      if (i == eTransform || i == eLuaComponent)
        continue;

      if (HasComponent(EComponent(i)))
      {
        GameComponent* comp;

        // If the object doesn't have the component then add it
        if (!obj->HasComponent(EComponent(i)))
        {
          comp = space->CreateComponent(EComponent(i));

          // Add the component to the object
          obj->AddComponent(comp);

          //comp->Initialize();
        }
        else // Otherwise grab it
          comp = obj->GetComponent(i);

        // Copy over the data from the archetype to the component
        const TypeInfo *typeInfo = FACTORY->GetComponentType(EComponent(i));
        typeInfo->Copy(comp, m_components[i]);

      }
    } // End component iteration

    //const TypeInfo* typeInfo = GET_TYPE(Lua::LuaProxy);

    //for (unsigned int i = 0; i < m_luaComponents.size(); ++i)
    //{
    //  Lua::LuaProxy* comp;

    //  if (!obj->HasLuaComponent(m_luaComponents[i]->name.c_str()))
    //  {
    //    comp = (Lua::LuaProxy*)space->CreateComponent(eLuaProxy);
    //    obj->AddComponent(comp);
    //  }
    //  else
    //  {
    //    comp = obj->GetLuaComponent(m_luaComponents[i]->name.c_str());
    //  }

    //  // Iterate through all members in the component that are registered to be serialized
    //  for (unsigned int m = 0; m < typeInfo->GetMembers().size(); ++m)
    //  {
    //    // Get the actual member
    //    const Member member = typeInfo->GetMembers()[m];
    //    // Copy over the member data from the archetype into the object
    //    Variable LVar = Variable(member.Type(), (char*)comp + member.Offset());
    //    Variable RVar = Variable(member.Type(), (char*)m_luaComponents[i] + member.Offset());

    //    member.Type()->PlacementDelete(LVar.GetData());
    //    member.Type()->PlacementCopy(LVar.GetData(), RVar.GetData());
    //  }

    //  //comp->Initialize(); // Initalize (or reinitialize) the lua component

    //}


    obj->Initialize();
  }

  //////////////////////////////////////////////////////////////////////////
  // SERIALIZATION ROUTINE

  void Archetype::SerializeDifferences(GameObject* obj, File& file) const
  {
    std::string instance;

    for (unsigned int j = 0; j < ecountComponents; ++j)
    {
      // Check to ensure both objects have the component, safety
      if (!obj->HasComponent(EComponent(j)) || !HasComponent(EComponent(j)))
        continue;

      GameComponent* lComp = obj->GetComponent(j);
      const GameComponent* rComp = GetComponent(j);

      // The components are the same, continue on
      if (*lComp == *rComp)
        continue;

      const TypeInfo* CType = GET_STR_TYPE(GET_ENUM(Component)->m_literals[j].c_str());

      // Prepare the string instance with the component name
      instance = CType->Name();
      instance += ":";

      // Iterate through each member of the component
      for (unsigned int m = 0; m < CType->GetMembers().size(); ++m)
      {
        // Get the member we are currently inspecting
        const Member member = CType->GetMembers()[m];

        // Compare the memory in the archetype and the object, if they are the same continue to the next member
        if (!memcmp((char*)GetComponent(j) + member.Offset(), (char*)obj->GetComponent(j) + member.Offset(), member.Type()->Size()))
          continue;

        // The data was different so we are going to print the different member now
        file.Write("  %s%s ", instance.c_str(), member.Name()); // Write the component and member

        // Create a variable from the member
        Variable var(member.Type(), (char*)obj->GetComponent(j) + member.Offset());

        // Write to the file
        var.Serialize(file);
      }

    } // End component loop
  }

  void Archetype::Serialize(File& file, Variable var)
  {
    Archetype* o = (Archetype*)var.GetData();
    const TypeInfo* info = GET_TYPE(Archetype);

    Serializer *s = Serializer::Get( );
    int& pad = s->GetPadLevel( );

    s->Padding( file, pad );
    file.Write( "GameObject\n" );
    s->Padding( file, pad++ );
    file.Write( "{\n" );

    // Iterate through the members we need to serialize
    for(unsigned i = 0; i < info->GetMembers().size(); ++i)
    {
      // Get a pointer to the member
      const Member* member = &info->GetMembers().front() + i;
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

    //s->Padding( file, pad );

    // Iterate through every component and serialize them
    for(unsigned i = 0; i < ecountComponents; ++i)
    {
      // Find the component type
      EComponent type = (EComponent)i;
      if (o->HasComponent(type) && type != eLuaComponent)
      {
        s->Padding( file, pad );

        // If the object has the specified component, then get the type info
        const TypeInfo *typeInfo = FACTORY->GetComponentType(type);

        // Create a variable from the component
        const Variable v( typeInfo, o->GetComponent(type));

        // Serialize the component into the file
        v.Serialize(file);
      }
    }

    //const TypeInfo *typeInfo = GET_TYPE(Lua::LuaProxy);

    //for (unsigned int i = 0; i < o->m_luaComponents.size(); ++i)
    //{
    //  // Create a variable from the component
    //  const Variable v( typeInfo, o->m_luaComponents[i] );

    //  s->Padding(file, pad);

    //  // Serialize the component into the file
    //  v.Serialize(file);

    //  // If we aren't quite yet at the end of all the components, make a new line
    //  // And pad it for any future components
    //  if(i != ecountComponents - 1)
    //  {
    //    file.Write( "\n" );
    //    
    //  }
    //}

    // We are all done so close the bracket
    s->Padding( file, --pad );
    file.Write( "}\n" );
  }

  void Archetype::Deserialize(File& file, Variable var)
  {
    Archetype& arch = var.GetValue<Archetype>();

    // Create a virtual game space
    GameSpace* space = DBG_NEW GameSpace();

    // Create an empty object in the virtual game space
    GameObject* obj = space->CreateEmptyObject();

    // Set the object as a variable
    var = *obj; 

    // Deserialize the file into the object
    GET_TYPE(GameObject)->Deserialize(file, var);

    // Copy the object we read
    arch.CopyObject(obj);

    delete space;
  }
}