/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Serialization.cpp 
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
#include "Serialization.h"

namespace Base
{
  Serializer::Serializer() {};

  Serializer::~Serializer() {};

  void Serializer::Serialize(File& file, Variable var)
  {
    // Grab the TypeInfo data from the variable
    const TypeInfo* info = var.GetTypeInfo();
    // Get a pointer to the data from the variable
    void *data = var.GetData();

    //assert(strlen(info->Name()) > 0);

    // Write to the file the name of the type
    file.Write("%s\n", info->Name());

    // Increase padding
    Padding(file, m_paddingLevel++);

    // Make a bracket and newline
    file.Write("{\n");

    // Iterate through every single member of the variable
    for(unsigned i = 0; i < info->m_members.size(); ++i)
    {
      // Get a pointer to the member
      const Member* member = &info->m_members.front() + i;
      // Add some padding in
      Padding( file, m_paddingLevel );

      // Get a pointer to the member type
      const TypeInfo* memberInfo = member->Type();
      // Write the name of the member
      file.Write("%s ", member->Name());

      // Get a pointer to the location of the member inside the data
      // struct/class/whatever itself
      void* offsetData = PTR_ADD(data, member->Offset());

      // Construct a new variable out of that, and then serialize that variable
      memberInfo->Serialize(file, Variable(memberInfo, offsetData));
    }

    // Decrease padding level
    Padding(file, --m_paddingLevel);
    // Write a bracket
    file.Write("}\n");
  }

  void Serializer::Deserialize(File& file, Variable var)
  {
    // Peak into the file and see if we can recognize a data type
    const TypeInfo* info = PeekType(file);

    if (!info)
    {
      assert( false ); // Invalid data read from file
      return;
    }
    else
    {
      // Make sure that both our types align
      // If we ever peeked a type that was not what we're expecting then assert
      assert(var.GetTypeInfo() == info);
    }

    // Our peek function was nice enough to figure out the starting level for us
    int startLevel = m_paddingLevel;

    // Iterate through everything that is inside the current bracket set
    do
    {
      // Peek and find a member
      const Member* mem = PeekMember(file, info->m_members, startLevel);

      // If we found a member then we can turn it into a variable type and then
      // deserialize that variable into to data we can use
      if (mem)
      {
        // Create a Variable out of the member we found, we need to offest the
        // start position of the variable by the start of our current variable
        // by the offset from the member and the start of the current variable
        Variable member( mem->Type(), PTR_ADD(var.GetData(), mem->Offset()) );

        // Now that we have the Variable, lets deserialize it
        member.Deserialize(file);
      }
    } while(m_paddingLevel > startLevel); // Once everything inside the current
    // bracket set has been exhuasted, out padding level will be the same as the
    // starting level, so that means we have finished this deserialization
    // and can move onto the next one
  }

  void Serializer::Padding(File& file, unsigned count)
  {
    for(unsigned i = 0; i < count; ++i)
      file.Write("  ");
  }

  const TypeInfo *Serializer::PeekType(File& file, int stopLevel)
  {
    // Create a buffer of characters so we have something to read into
    char temp[256];

    for(;;)
    {
      // Read in a line from the file
      file.Read("%s", temp);

      // If the first character is a bracket then
      // we should increase the padding level
      if(strcmp(temp, "{") == 0)
      {
        ++m_paddingLevel;
        continue;
      }

      // we hit the close bracket, decrease padding level
      // And if we hit the stoplevel, return
      else if(strcmp( temp, "}" ) == 0)
      {
        if(--m_paddingLevel == stopLevel)
          return NULL;
        continue;
      }

      // We didn't hit a bracket
      break;
    }

    // Make sure our filepointer hasn't gone out of bounds
    if (file.Validate())
      // Return the type of the variable we hit
      return GET_STR_TYPE(temp);
    else
      return NULL;
  }

  void Serializer::SetUserData(void *data)
  {
    m_userData = data;
  }

  const Member* Serializer::PeekMember(File& file, const std::vector<Member>& vec, int startLevel)
  {
    // Temp buffer
    char temp[256];

    for(;;)
    {
      // Read in a line from the file
      file.Read("%s", temp);

      // We hit a { so our padding level should go up
      if (strcmp(temp, "{") == 0)
      {
        ++m_paddingLevel;
        continue;
      } // We hit a } so padding level goes down
      else if (strcmp(temp, "}") == 0)
      {
        // Check if we should exit
        if (--m_paddingLevel == startLevel)
          return NULL;
        continue;
      }

      break;
    }

    for (unsigned i = 0; i < vec.size( ); ++i)
    {
      // Get the member from the vector
      const Member* mem = &vec.front() + i;

      // Compare their names, if it's a match return the member
      if (strcmp(mem->Name(), temp) == 0)
        return mem;
    }

    return NULL;
  }
}