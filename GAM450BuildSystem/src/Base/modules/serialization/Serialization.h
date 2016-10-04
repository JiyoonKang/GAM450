/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Serialization.h 
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
  class Serializer
  {
  public:
    static Serializer* Get(void)
    {
      static Serializer instance;
      return &instance;
    }

    void Serialize(File& file, Variable var);
    void Deserialize(File& file, Variable var);

    void Padding(File& file, unsigned int count);

    const TypeInfo* PeekType(File& file, int stopLevel = 0);

    const Member* PeekMember(File& file, const std::vector<Member>& vec, int startLevel);

    inline int& GetPadLevel(void) { return m_paddingLevel; }
    
    // We use these in case we feel like setting some special data
    // on the serializer itself that we want accessible from anywhere
    inline void* GetUserData(void) const { return m_userData; }
    void SetUserData(void* data);

  private:
    Serializer();
    ~Serializer();

    int m_paddingLevel;
    void *m_userData;

    friend class Factory;
  
  };
}