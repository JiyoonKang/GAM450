/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Variable.cpp 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "Variable.h"

namespace Base
{
  Variable::Variable()
    : m_data(NULL), m_typeInfo(NULL) {}

  Variable::Variable(const TypeInfo *typeInfo, void *data)
    : m_data(data), m_typeInfo(typeInfo) {}

  Variable::Variable(const TypeInfo* typeInfo, const void* data)
    : m_data(const_cast<void*>(data)), m_typeInfo(typeInfo) {}

  Variable::Variable(const Variable& rhs)
  {
    m_data = rhs.m_data;
    m_typeInfo = rhs.m_typeInfo;
  }

  Variable& Variable::operator=(const Variable& rhs)
  {
    if (this != &rhs)
    {
      m_data = rhs.m_data;
      m_typeInfo = rhs.m_typeInfo;
    }
    return *this;
  }

  void Variable::PlacementNew(void)
  {
    m_typeInfo->PlacementNew(m_data);
  }

  void Variable::PlacementDelete(void)
  {
    m_typeInfo->PlacementDelete(m_data);
  }

  void Variable::Delete(void)
  {
    m_typeInfo->PlacementDelete(m_data);
    m_typeInfo->Delete(m_data);
  }

  void Variable::Serialize(File& file) const
  {
    m_typeInfo->Serialize(file, *this);
  }

  void Variable::Deserialize(File& file)
  {
    m_typeInfo->Deserialize(file, *this);
  }

  /*void Variable::ToLua(lua_State* L) const
  {
    m_typeInfo->ToLua(L, *this);
  }

  void Variable::FromLua(lua_State* L, int index)
  {
    m_typeInfo->FromLua(L, index, this);
  }*/
}
