/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Enum.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Base/util/Macros.h"
#include "Base/util/file/File.h"
#include "Base/modules/introspection/variable/Variable.h"

#include <vector>
#include <unordered_map>
#include <string>

namespace Base
{
  #undef E_NAME
  #undef E_ENTRY
  #undef E_END

  #define E_NAME( NAME ) \
    enum E##NAME {

  #define E_ENTRY( ENTRY ) \
    e##ENTRY,

  #define E_END( NAME ) \
  };

#include "Start/config/EnumData.h"

  void RegisterEnums();

  struct Enum
  {
    Enum(const char *name);
    Enum(const char *name, const char **literals);
    Enum(const char *name, const std::vector<std::string>& literals);

    std::string m_name;
    std::vector<std::string> m_literals;
    std::unordered_map<std::string, unsigned int> m_literalMap;

    bool IsAnEntry(const char *entryName);
    unsigned GetIndexFromString(const char *entryName);

  };

  // Declare Enum objects
  #undef  E_NAME
  #define E_NAME( NAME ) \
    extern "C" Enum Enum##NAME; \
    __pragma( comment ( linker, STRINGIZE( /INCLUDE:_##Enum##NAME ) ) )

  #undef  E_ENTRY
  #define E_ENTRY( ENTRY )

  #undef  E_END
  #define E_END( NAME)

  //#include "config/EnumData.h"
#include "Start/config/EnumData.h"

  template <typename EnumTYpe>
  void SerializeEnum(File& file, Variable var);

  #undef  E_NAME
  #define E_NAME(NAME)\
  template<>\
  void SerializeEnum<E##NAME>(File& file, Variable var);

  //#include "config/EnumData.h"
#include "Start/config/EnumData.h"

  template <typename EnumTYpe>
  void DeserializeEnum(File& file, Variable var);

  #undef  E_NAME
  #define E_NAME(NAME)\
  template<>\
  void DeserializeEnum<E##NAME>(File& file, Variable var);

 // #include "config/EnumData.h"
#include "Start/config/EnumData.h"
}
