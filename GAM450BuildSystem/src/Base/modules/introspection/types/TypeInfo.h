/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: TypeInfo.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Base/modules/functionbinding/Function.h"
#include "Base/util/Macros.h"

#include <vector>

namespace Base
{
  class TypeInfo;

  class Member
  {
  public:
    // The type of the member
    inline const TypeInfo* Type(void) const { return m_typeInfo; }
    // Memory offset of the member in the class
    inline unsigned Offset(void) const { return m_offset; }
    // Name of the member
    inline const char* Name(void) const { return m_name; }
    // Whether or not to auto-create a lua setter/getter
    inline bool AutoLua(void) const { return m_autoLua; }

  private:
    const char *m_name;
    unsigned m_offset;
    const TypeInfo *m_typeInfo;
    // Automatic lua setter/getter
    bool m_autoLua;

    friend class TypeInfo;
  };

  // Serialization
  typedef void (*SerializeCB)(File&, Variable);
  typedef void (*DeserializeCB)(File&, Variable);

  // Lua
  //typedef void (*ToLuaCB)(lua_State*, Variable&);
  //typedef void (*FromLuaCB)(lua_State*, int, Variable*);

  //////////////////////////////////////////////////////////////////////////

  class TypeInfo
  {
  public:
    TypeInfo();
    // Initialization routine
    void Init(const char *name, unsigned size);
    // Adds a member to the type
    void AddMember(const TypeInfo *typeInfo, const char *name, unsigned offset, bool autoLua = true);
    // Gets a member from the type
    const Member *GetMember(const char *memberName) const;

    // Sets a custom serializer callback
    void SetSerializer(SerializeCB cb);
    // Sets a custom deserializer callback
    void SetDeserializer(DeserializeCB cb);

    // Serializes a variable of this type into a file
    void Serialize(File& file, Variable var) const;
    // Deserializes a file into a variable of this type
    void Deserialize(File& file, Variable var) const;

    // Gets a list of all members in the type
    const std::vector<Member>& GetMembers() const;

    // Lua Functions
    // The (name of) lua meta table for this class
    inline const char* LuaMetaTable(void) const { return m_metatable; }
    // Sends a variable of this type to lua
    //void ToLua(lua_State* L, Variable var) const;
    // Gets a variable of this type from lua, from the given stack index
    //void FromLua(lua_State* L, int index, Variable* var) const;
    
    // Sets a ToLua call back function
    //void SetToLua(ToLuaCB cb);
    // Sets a FromLua callback function
    //void SetFromLua(FromLuaCB cb);


    // Gets the size (in bytes) of this type
    inline unsigned Size(void) const { return m_size; }
    // Gets the name of this type
    inline const char *Name(void) const { return m_name.c_str(); }
    // returns TRUE if this type is PLAIN-OLD-DATA otherwise false
    inline bool IsPOD( void ) const { return m_isPOD; };

    // Checks to see if this type has a custom lua callback
    //bool HasToLuaCB(void) const { return m_toLua != nullptr; };

    // Makes a new <whatever this type is>
    void *(*New)( void );
    // Copies the type from once destination to another
    void (*Copy)( void *data, const void *src );
    // Deletes the given void* as if it is this type
    void (*Delete)( void *data );
    // Creates a new <whatever this type is> using a copy constructor from the given source
    void (*NewCopy)( void **dest, const void *src );
    // Does a placement New operator for this type on the given data location
    void (*PlacementNew)( void *data );
    // Does a placement Delete operator for this type on the given data location
    void (*PlacementDelete)( void *data );
    // DOes a placement New copy-constructor for this type on the given data location, using the given source
    void (*PlacementCopy)( void *data, const void *src );

    // **

    // Pointers to the serialization callback functions
    SerializeCB m_serialize;
    DeserializeCB m_deserialize;
  private:
    // Actual size of the type
    unsigned m_size;
    // Actual name of the type
    std::string m_name;

    // List of the members
    std::vector<Member> m_members;

    // Lua metatable name
    const char* m_metatable;

 

    // Pointer to the lua callback functions
    //ToLuaCB m_toLua;
    //FromLuaCB m_fromLua;

    // Check if it's a POD
    bool m_isPOD;

    friend class Serializer;
    friend class IntrospectionManager;
    friend class Factory;
  };

  //////////////////////////////////////////////////////////////////////////

  // Reinterprets a void* into a given type
  template <typename T>
  inline T* Cast(void* data)
  {
    return reinterpret_cast<T *>(data);
  }

  // Reinterprets a void* into a given type (Const)
  template <typename T>
  inline const T* Cast(const void* data)
  {
    return reinterpret_cast<const T*>(data);
  }

  // Constructs a new <type> and uses a placement new operator
  template <typename T>
  inline void* New(void)
  {
    T *data = (T*)malloc( sizeof(T) );
    new (data) T();
    return data;
  }

  // Constructs a new <type> (For POD)
  template <typename T>
  inline void* PODNew(void)
  {
    return malloc(sizeof(T));
  }

  // Placement new on <type>
  template <typename T>
  inline void PlacementNew(void* data)
  {
    new (data) T();
  }

  DISABLE_WARNING(4100)
  // There is no such thing as a placement new on POD
  template <typename T>
  inline void PODPlacementNew(void* data) {}
  END_DISABLE()

  // Copies data from src->dest as if it is the type T
  // Using the assignment operator
  template <typename T>
  inline void Copy(void* dest, const void* src)
  {
    *(Cast<T>(dest)) = *(Cast<T>(src));
  }

  // Plain old data copy from src->dest (memcpy)
  template <typename T>
  inline void PODCopy(void* dest, const void* src)
  {
    std::memcpy(dest, src, sizeof(T));
  }

  // Placement new, with a copy constructor using src
  template <typename T>
  inline void PlacementCopy(void* data, const void* src)
  {
    new (data) T(*(Cast<T>(src)));
  }

  // POD placement copy is just memcpy
  template <typename T>
  inline void PODPlacementCopy(void* data, const void* src)
  {
    std::memcpy(data, src, sizeof(T));
  }

  // Creates a new <type> into dest, using src for a copy constructor
  template <typename T>
  inline void NewCopy(void** dest, const void* src)
  {
    T* newData = (T*)malloc( sizeof(T));
    new (newData) T(*Cast<T>(src));
    *dest = newData;
  }

  // POD Copy constructor just allocates and uses memcpy
  template <typename T>
  inline void PODNewCopy(void** dest, const void* src)
  {
    *dest = malloc(sizeof(T));
    std::memcpy(*dest, src, sizeof(T));
  }

  // Deletes a data as if it were <type>
  template <typename T>
  inline void Delete(void* data)
  {
    Cast<T>(data)->~T();
    free(data);
  }

  // POD delete is just (free)
  template <typename T>
  inline void PODDelete(void* data)
  {
    free(data);
  }

  // Equality Comparison
  template <typename T>
  inline bool MetaCompare(const T* a, const T* b)
  {
    return true;
  }

  // Placement delete (destructor) on data
  template <typename T>
  inline void PlacementDelete(void* data)
  {
    Cast<T>( data )->~T( );
  }

  DISABLE_WARNING(4100)
  template <typename T>
  inline void PODPlacementDelete(void* data) {}
  END_DISABLE()

}