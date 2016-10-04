/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Factory.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Base/modules/introspection/enum/Enum.h"
#include "Base/modules/introspection/types/TypeInfo.h"
#include "Core/components/base/ComponentCreator.h"
#include "Core/types/archetype/Archetype.h"

#include "Base/util/Macros.h"
#include "Base/modules/introspection/manager/IntrospectionManager.h"
#include "Base/modules/introspection/Qualifiers.h"

namespace Core
{
  class GameObject;
  class GameSpace;

  

#define REGISTER_COMPONENT(NAME, TYPE) \
  static Core::ComponentCreatorType<TYPE> NAME##Creator( e##NAME ); \
  Core::FACTORY->RegisterComponentCreator( NAME##Creator, GET_TYPE(TYPE) )
  
  class Factory
  {
  public:
    Factory();
    ~Factory();

    //////////////////////////////////////////////////////////////////////////
    // COMPONENT FUNCTIONS

    void RegisterComponentCreator(ComponentCreator& creator, const TypeInfo* type);

    const TypeInfo* GetComponentType(EComponent type);
	std::vector<EComponent> GetComponentList(void){ return componentsNameList; }
    //////////////////////////////////////////////////////////////////////////
    // ARCHETYPE AND OBJECT FUNCTIONS

    void SaveObjectToArchetype(GameObject* obj, const char* name);

    // Creates an object of from an archetype. [Loads the archetype if required]
    GameObject* LoadObjectFromArchetype(GameSpace* space, const char* name);

    GameObject* LoadObjectFromArchetypeFP(GameSpace* space, const char* name);

    // Creates an object from a virtual archetype.
    GameObject* LoadObjectFromArchetype(GameSpace* space, const Archetype& archetype);

    void UpdateArchetypeObjects(GameSpace* space, const char* name);

    // Preloads an archetype of a specified name
    bool LoadArchetypeFromFile(const char* name);

    void SaveArchetypeToFile(std::string);

    void SaveArchetypeToFile(const Archetype& archetype);

    static GameObject* LuaLoadObjectFromArchetype(GameSpace* space, const char* name);

    const Archetype& GetArchetype(std::string name);

    //////////////////////////////////////////////////////////////////////////
    // SPACE FUNCTIONS

    static void SaveSpaceToFile(GameSpace* space, const char* name, std::vector<std::string>* objInstanceData = NULL, 
      bool includeGeneric = false, bool allData = false);

    static void SaveSpaceToFile(GameSpace* space, const char* name, bool standalone);

    // Saves a space to an explicit filepath. This should be used with caution.
    static void SaveSpaceToFilePath(GameSpace* space, const char* filepath);

    static GameSpace* LoadSpaceFilePath(const char* filepath);

    static GameSpace* LoadSpace(const char* name);
    //////////////////////////////////////////////////////////////////////////
    // LEVEL FUNCTIONS

	//static void CreateLevelFilePath(const char* filePath);

	static void CreateLevel(const char* name);

    // Saves all of the spaces into a level file
    static void SaveLevel(const char* filePath);

    // Loads a level file, which loads multiple spaces
    static void LoadLevel(const char* name, void(*cb)(GameSpace* space));

    static void LoadLevelFilePath(const char* filePath, const char* name, void(*cb)(GameSpace* space));
  
    //////////////////////////////////////////////////////////////////////////
    // Path Extensions

    static const std::string ArchetypeFileExtension;
    static const std::string ArchetypePrefix;
    static const std::string SpaceFileExtension;
    static const std::string LevelFileExtension;

    static const std::string SpaceFilePath;
    static const std::string LevelFilePath;
    static const std::string ArchetypeFilePath;

	std::vector<EComponent> componentsNameList;

  protected:
    ComponentCreator* m_componentCreators[ecountComponents];
    const TypeInfo* m_componentTypes[ecountComponents];

    std::unordered_map<std::string, Archetype> ArchetypeMap;

  private:
    // Deserializes a component from a file
    GameComponent* DeserializeComponent(File& file, GameSpace* space);

    const Member* GetComponentMember(const char* instring, Variable& out);
    const Member* GetComponentMemberRecursive(const Member *member, Variable &val);

    friend class GameObject;
    friend class GameSpace;
    friend class Archetype;
  };

  extern Factory* FACTORY;
}