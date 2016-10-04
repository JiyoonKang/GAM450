/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Archetype.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Lua/components/LuaProxy.h"

namespace Core
{
  class Archetype
  {
  public:
    ~Archetype();
    Archetype();
    Archetype& operator=(const Archetype& rhs);

    //////////////////////////////////////////////////////////////////////////
    // COMPONENT FUNCTIONS

    template<typename T>
    T* GetComponent(EComponent type) const;

    const GameComponent* GetComponent(unsigned int type) const;

    bool HasComponent(EComponent type) const;

    //////////////////////////////////////////////////////////////////////////
    // OBJECT FUNCTIONS

    GameObject* CreateObject(GameSpace* space) const;

    void UpdateObject(GameObject* obj) const;

    //////////////////////////////////////////////////////////////////////////
    // SERIALIZATION ROUTINE

    static void Serialize(File& file, Variable var);
 
    // Deserialization routine, var must be an empty archetype (buffer)
    static void Deserialize(File& file, Variable var);

    void SerializeDifferences(GameObject* obj, File& file) const;

    //////////////////////////////////////////////////////////////////////////
    // Public Variables

    std::string name;
    std::string archetype;

    static const Archetype null;

  private:
    void CopyObject(GameObject*);

    GameComponent* GetComponent(unsigned int type);

    GameComponent* m_components[ecountComponents];

    std::vector<Lua::LuaProxy*> m_luaComponents;
    
    friend class Factory;
    friend class GameSpace;
  };

  template<typename T>
  T* Archetype::GetComponent(EComponent type) const
  {
    return static_cast<T*>(m_components[type]);
  }
}

