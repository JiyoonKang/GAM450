/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Variable.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Base/util/file/File.h"


struct lua_State;

namespace Base
{
  class TypeInfo;
  class IntrospectionManager;

  class Variable
  {
  public:
    Variable();
    // Creates a variable, from a Type, and data pointer
    Variable(const TypeInfo* typeInfo, void* data);
    // Creates a variable from a Type, and const data pointer
    Variable(const TypeInfo* typeInfo, const void* data);
    // Creates a variable from another variable
    Variable(const Variable& rhs);
    // Creates a variable from a reference of a type
    template <typename T>
    Variable(const T& rhs);
    // Creates a variable from a const-pointer to a type
    template <typename T>
    Variable(const T* rhs);
    // Creates a variable from a pointer to a type
    template <typename T>
    Variable(T* rhs);
    Variable& operator=(const Variable& rhs);

    inline void* GetData(void) const { return m_data; }
    inline const TypeInfo* GetTypeInfo(void) const { return m_typeInfo; }
    inline bool IsValid(void) const { return m_typeInfo != NULL && m_data != NULL; }

    template <typename T>
    void SetData(T data);

    template <typename T>
    T& GetValue(void);

    template <typename T>
    const T& GetValue(void) const;

    /* void ToLua(lua_State* L) const;
     void FromLua(lua_State* L, int index);*/

    void PlacementNew(void);
    void PlacementDelete(void);
    void Delete(void);

    void Serialize(File& file) const;
    void Deserialize(File& file);

  private:
    void* m_data;
    const TypeInfo *m_typeInfo;
  };
}
  //////////////////////////////////////////////////////////////////////////

  #include "Base/modules/introspection/manager/IntrospectionManager.h"

 namespace Base
 {
  template <typename T>
  void Variable::SetData(T data)
  {
    *((T*)m_data) = data; 
  }

  template <typename T>
  Variable::Variable(const T& rhs) : m_data((T*)&rhs), m_typeInfo(GET_TYPE(T)) {}

  template <typename T>
  Variable::Variable(const T* rhs) : m_data((T*)rhs), m_typeInfo(GET_TYPE(T*)) {}

  template <typename T>
  Variable::Variable(T* rhs) : m_data((T*)rhs), m_typeInfo(GET_TYPE(T*)) {}

  // Cast helper exists to properly handle pointer types
  template <typename T>
  struct CastHelper
  {
    static T& Cast(void *& data) { return *(T*&)(data); }
  };

  template <typename T>
  struct CastHelper<T* >
  {
    static T* & Cast(void *& data) { return (T*&)data; }
  };

  template <typename T>
  T& Variable::GetValue(void)
  {
    return CastHelper<T>::Cast(m_data);
  }

  template <typename T>
  const T& Variable::GetValue(void) const
  {
    return CastHelper<T>::Cast(m_data);
  }
}
