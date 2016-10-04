/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: IntrospectionManager.cpp 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "IntrospectionManager.h"

#include <vector>
#include <string>

namespace Base
{

  IntrospectionManager::IntrospectionManager() {}

  IntrospectionManager::~IntrospectionManager() {}

  const std::unordered_map<std::string, TypeInfo*>& IntrospectionManager::GetTypeMap() const
  {
    return m_typeMap;
  }

  const TypeInfo* IntrospectionManager::GetType(const char* typeName) const
  {
    std::string typeNameStr = typeName;

    if (m_typeMap.find(typeNameStr) != m_typeMap.end())
    {
      return m_typeMap.at(typeNameStr);
    }

    return nullptr;
  }

  const TypeInfo* IntrospectionManager::GetType(std::string& typeName) const
  {
    if (m_typeMap.find(typeName) != m_typeMap.end())
    {
      return m_typeMap.at(typeName);
    }

    return nullptr;
  }

}

