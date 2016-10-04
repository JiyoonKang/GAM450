/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: PODSerialization.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/*****************************************************************
Filename:
Project:
Author(s):

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

namespace Base
{
  template <typename T>
  void SerializeArray(File& file, Variable var)
  {
    // Write the name of the variable type
    file.Write("%s ", var.GetTypeInfo()->Name());

    // Get a reference to the data from the variable
    std::vector<T> &arr = var.GetValue<std::vector<T>>();

    // First off we write how big the array is
    file.Write("count = %d\n", arr.size());

    // Get the padding level and increase it
    int& level = Serializer::Get()->GetPadLevel();
    Serializer::Get()->Padding(file, level++); // pad
    file.Write("{\n"); // newline babe

    // Iterate through every entry inside the array
    for (unsigned i = 0; i < arr.size(); ++i)
    {
      // Insert padding
      Serializer::Get()->Padding(file, level);
      // Serialize the actual array entry
      GET_TYPE_OF(arr[i])->Serialize(file, Variable(arr[i]));
    }

    // Now that we are done, decrease padding
    Serializer::Get()->Padding(file, --level);

    // Close Bracket and newline
    file.Write("}\n");
  }

  template <typename T>
  void DeserializeArray(File& file, Variable var)
  {
    // Get the variable that we are filling
    std::vector<T> &arr = var.GetValue<std::vector<T>>();

    // Get the padding level
    int& level = Serializer::Get()->GetPadLevel();

    // Run a quick check on the types
    if (var.GetTypeInfo() != Serializer::Get()->PeekType(file, Serializer::Get()->GetPadLevel()))
      assert(false); // Type mismatch between type from file and actual member type.

    unsigned int count; // Get the number of elements inside the array
    file.Read("%*[^=]= %d", &count); // Read the amount in

    // Iterate through every single element
    for (unsigned int i = 0; i < count; ++i)
    {
      // Make sure that our array is big enough
      assert(i <= arr.size());

      // Insert an empty value into the back
      if (i == arr.size())
        arr.emplace_back();

      // Grab the value we just setup
      Variable temp(arr[i]);

      // Deserialize the variable
      temp.Deserialize(file);

      // if  pad level every is 0, exit
      if  (!level)
        break;
    }
  }

  template <typename T>
  void SerializePtr(File& file, Variable var)
  {
    // Write the name of the type
    file.Write("%s ", var.GetTypeInfo()->Name());

    // Write the actual value of the pointer
    // note: dangerous
    file.Write("%x\n", (unsigned)var.GetValue<T>());
  }

  template <typename T>
  void DeserializePtr(File& file, Variable var)
  {
    // Do a quick check on the types
    if (var.GetTypeInfo() != Serializer::Get()->PeekType(file, Serializer::Get()->GetPadLevel()))
      assert(false); // Type mismatch between type from file and actual member type.

    unsigned int temp; // The pointer

    file.Read("%x", &temp); // Read in the value from the file

    // Set the value on the variable to the pointer casted correctly
    var.GetValue<T>() = (T)temp;
  }

  template <typename T>
  void SerializePOD(File& file, Variable var);
  template <>
  void SerializePOD<int>(File& file, Variable var);
  template <>
  void SerializePOD<unsigned int>(File& file, Variable var);
  template <>
  void SerializePOD<float>(File& file, Variable var);
  template <>
  void SerializePOD<double>(File& file, Variable var);
  template <>
  void SerializePOD<bool>(File& file, Variable var);
  template <>
  void SerializePOD<char>(File& file, Variable var);
  template <>
  void SerializePOD<char *>(File& file, Variable var);

  template <typename T>
  void DeserializePOD(File& file, Variable var);
  template <>
  void DeserializePOD<int>(File& file, Variable var);
  template <>
  void DeserializePOD<unsigned int>(File& file, Variable var);
  template <>
  void DeserializePOD<float>(File& file, Variable var);
  template <>
  void DeserializePOD<bool>(File& file, Variable var);
  template <>
  void DeserializePOD<char>(File& file, Variable var);

  void SerializeString(File& file, Variable var);
  void DeserializeString(File& file, Variable var);
};