/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: PODSerialization.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/*****************************************************************
Filename:
Project:
Author(s):

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "PODSerialization.h"

namespace Base
{
  template <>
  void SerializePOD<int>(File& file, Variable var)
  {
    file.Write("%s ", var.GetTypeInfo()->Name());
    file.Write("%d\n", var.GetValue<int>());
  }

  template <>
  void SerializePOD<unsigned int>(File& file, Variable var)
  {
    file.Write("%s ", var.GetTypeInfo()->Name());
    file.Write("%u\n", var.GetValue<unsigned>());
  }

  template <>
  void SerializePOD<float>(File& file, Variable var)
  {
    file.Write("%s ", var.GetTypeInfo()->Name());
    file.Write("%f\n", var.GetValue<float>());
  }

  template <>
  void SerializePOD<double>(File& file, Variable var)
  {
    file.Write("%s ", var.GetTypeInfo()->Name());
    file.Write("%lf\n", var.GetValue<double>());
  }

  template <>
  void SerializePOD<bool>(File& file, Variable var)
  {
    file.Write("%s ", var.GetTypeInfo()->Name());
    if(var.GetValue<bool>())
      file.Write("true\n");
    else
      file.Write("false\n");
  }

  template <>
  void SerializePOD<char>(File& file, Variable var)
  {
    file.Write("%s ", var.GetTypeInfo()->Name());
    file.Write("%c\n", var.GetValue<char>());
  }

  template <>
  void SerializePOD<char *>(File& file, Variable var)
  {
    file.Write("%s ", var.GetTypeInfo()->Name());
    file.Write("\"%s\"\n", *(char **)var.GetData());
  }

  template <>
  void DeserializePOD<int>(File& file, Variable var)
  {
    if(var.GetTypeInfo() != Serializer::Get()->PeekType(file, Serializer::Get()->GetPadLevel()))
      assert(false); // Type mismatch between type from file and actual member type.
    file.Read("%d", var.GetData());
  }

  template <>
  void DeserializePOD<unsigned int>(File& file, Variable var)
  {
    if(var.GetTypeInfo() != Serializer::Get()->PeekType(file, Serializer::Get()->GetPadLevel()))
      assert(false); // Type mismatch between type from file and actual member type.
    file.Read("%u", var.GetData());
  }

  template <>
  void DeserializePOD<float>(File& file, Variable var)
  {
    if(var.GetTypeInfo() != Serializer::Get()->PeekType(file, Serializer::Get()->GetPadLevel()))
      assert(false); // Type mismatch between type from file and actual member type.
    file.Read("%f", var.GetData());
  }

  template <>
  void DeserializePOD<double>(File& file, Variable var)
  {
    if(var.GetTypeInfo() != Serializer::Get()->PeekType(file, Serializer::Get()->GetPadLevel()))
      assert(false); // Type mismatch between type from file and actual member type.
    file.Read("%lf", var.GetData());
  }

  template <>
  void DeserializePOD<bool>(File& file, Variable var)
  {
    if(var.GetTypeInfo() != Serializer::Get()->PeekType(file, Serializer::Get()->GetPadLevel()))
      assert(false); // Type mismatch between type from file and actual member type.
    char temp[256];
    file.Read("%s", temp);
    if(strcmp(temp, "true") == 0)
      var.GetValue<bool>() = true;
    else
      var.GetValue<bool>() = false;
  }

  template <>
  void DeserializePOD<char>(File& file, Variable var)
  {
    if(var.GetTypeInfo() != Serializer::Get()->PeekType(file, Serializer::Get()->GetPadLevel()))
      assert(false); // Type mismatch between type from file and actual member type.
    file.Read("%c", var.GetData());
  }

  template <>
  void DeserializePOD<char*>(File& file, Variable var)
  {
    if (var.GetTypeInfo() != Serializer::Get()->PeekType(file, Serializer::Get()->GetPadLevel()))
      assert(false); // Type mismatch between type from file and actual member type.
    file.Read("%s", var.GetData());
  }

  void SerializeString(File& file, Variable var)
  {
    file.Write("%s \"%s\"\n", var.GetTypeInfo()->Name(), var.GetValue<std::string>().c_str());
  }

  void DeserializeString(File& file, Variable var)
  {
    char temp[256] = {};

    file.Read("%*[^\"]\"");
    file.Read("%[^\"]\"", temp);

    // Empty string catch
    if (temp[0] == NULL)
    {
      // Move over one more character so we aren't at the \"
      file.SeekByOffset(1);
    }

    var.GetValue<std::string>() = temp;
  }
}