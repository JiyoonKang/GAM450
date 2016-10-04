/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: TypeInfo.cpp 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "TypeInfo.h"
#include "Base/modules/serialization/Serialization.h"

namespace Base
{
  TypeInfo::TypeInfo()
    : m_serialize(nullptr), m_deserialize(nullptr), m_metatable(nullptr) {}

  void TypeInfo::Init(const char* name, unsigned int size)
  {
    std::string clean = "";
    std::string theName = name;

    for (unsigned i = 0; i < theName.length(); ++i)
      if (theName[i] != ' ')
        clean += theName[i];

    m_name = clean;
    m_size = size;
  }

  void TypeInfo::AddMember(const TypeInfo* typeInfo, const char* name, unsigned int offset, bool autoLua)
  {
    Member mem;
    mem.m_name = name;
    mem.m_offset = offset;
    mem.m_typeInfo = typeInfo;

    mem.m_autoLua = autoLua;
    m_members.push_back(mem);
  }

  const Member* TypeInfo::GetMember(const char* memberName) const
  {
    for(unsigned i = 0; i < m_members.size(); ++i)
    {
      if (strcmp(m_members[i].m_name, memberName) == 0)
        return &m_members[i];
    }

    return nullptr;
  }

  void TypeInfo::SetSerializer(SerializeCB cb)
  {
    m_serialize = cb;
  }

  void TypeInfo::SetDeserializer(DeserializeCB cb)
  {
    m_deserialize = cb;
  }

  void TypeInfo::Serialize(File& file, Variable var) const
  {
    if (m_serialize)
      m_serialize(file, var);
    else
      Serializer::Get()->Serialize(file, var);
  }

  void TypeInfo::Deserialize(File& file, Variable var) const
  {
    if (m_deserialize)
      m_deserialize(file, var);
    else
      Serializer::Get()->Deserialize(file, var);
  }


  //void TypeInfo::ToLua(lua_State* L, Variable var) const
  //{
  //  if (m_toLua)
  //    m_toLua(L, var);
  //  else
  //    Core::Lua::GenericToLua(L, var); // Generic lua pass function
  //}

  //void TypeInfo::FromLua(lua_State* L, int index, Variable* var) const
  //{
  //  if (m_fromLua)
  //    m_fromLua(L, index, var);
  //  else
  //    Core::Lua::GenericFromLua(L, index, var);
  //}

  //void TypeInfo::SetFromLua(FromLuaCB cb)
  //{
  //  m_fromLua = cb;
  //}

  //void TypeInfo::SetToLua(ToLuaCB cb)
  //{
  //  m_toLua = cb;
  //}

  const std::vector<Member>& TypeInfo::GetMembers() const
  {
    return m_members;
  }
}