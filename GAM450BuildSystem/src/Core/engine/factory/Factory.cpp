/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Factory.cpp 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "components/transform/CTransform.h"

#include <time.h>
#include <direct.h>
#include <fstream>

using namespace Base;

namespace Core
{
  Factory *FACTORY = NULL;

  const std::string Factory::SpaceFileExtension = ".space";
  const std::string Factory::LevelFileExtension = ".level";
  const std::string Factory::ArchetypeFileExtension = ".arche";
  const std::string Factory::ArchetypePrefix = "ach_";

  const std::string Factory::ArchetypeFilePath = "content\\data\\archetypes\\";
  const std::string Factory::LevelFilePath = "content\\data\\levels\\";
  const std::string Factory::SpaceFilePath = "content\\data\\spaces\\";

  Factory::Factory()
  {
    for (unsigned int i = 0; i < ecountComponents; ++i)
    {
      m_componentCreators[i] = nullptr;
      m_componentTypes[i] = nullptr;
    }
    FACTORY = this;
  }

  Factory::~Factory()
  {
    FACTORY = nullptr;
  }

  //////////////////////////////////////////////////////////////////////////
  // COMPONENT FUNCTIONS

  void Factory::RegisterComponentCreator(ComponentCreator& creator, const TypeInfo* type)
  {
    m_componentCreators[creator.typeID] = &creator;
    m_componentTypes[creator.typeID] = type;
	componentsNameList.push_back(creator.typeID);
  }

  const TypeInfo* Factory::GetComponentType(EComponent type)
  {
    return m_componentTypes[type];
  }

  DISABLE_WARNING(4996)
  const Member* Factory::GetComponentMember(const char *instring, Variable& out)
  {
    char data[256];

    // Create a copy of the component name
    strcpy(data, instring);
    //std::string componentName = strtok(data, "\n");
    std::string componentName = strtok(data, ":");
    //char* temp = strtok(data, ":");

    // Get a pointer to the typeinfo for the component
    const TypeInfo* componentType = GET_STR_TYPE(componentName.c_str());

    // Turn out into the game object
    GameObject* o = out.GetValue<GameObject*>(); // assume out holds game object ptr

    // literalMap does not take contain namespace so remove it
    //temp = strtok(NULL, ":");

    // Turn out into a variable for the component
    out = Variable(componentType, o->GetComponent(componentName.c_str()));
    //out = Variable(componentType, o->GetComponent(temp));

    // Get the member of the component we are looking for
    const Member* member = out.GetTypeInfo()->GetMember(strtok(NULL, ":"));

    // Finally get the actual variable which is the member we are looking for
    out = Variable(member->Type(), PTR_ADD(out.GetData(), member->Offset()));

    // Now that we have done that, we enter our recursive loop to strip off
    // any remaining :'s from the instring
    return GetComponentMemberRecursive(member, out);
  }

  const Member* Factory::GetComponentMemberRecursive(const Member *member, Variable &val)
  {    
    // Check to see if there is anything left on the token list
    const char *memberNamePtr = strtok( NULL, ":" );

    // Nothing left, we found the member we want
    if(!memberNamePtr)
      return member;

    // Set the member name we are looking for
    std::string memberName = memberNamePtr;

    // Find the member of the the member
    const Member *deepMember = member->Type()->GetMember(memberName.c_str());

    // Get the type of the member
    const TypeInfo *deepMemberType = deepMember->Type();

    // memberName should == deepMemberType->Name()

    // Get a variable of the member we just got
    val = Variable(deepMemberType, PTR_ADD(val.GetData(), deepMember->Offset()));

    // And recursion
    return GetComponentMemberRecursive(deepMember, val);
  }
  END_DISABLE()

    GameComponent* Factory::DeserializeComponent(File& file, GameSpace* space)
  {
    // Get the generic serializer
      Base::Serializer* s = Serializer::Get();

    // Peek into the file to see if we can see a type
    const TypeInfo* info = s->PeekType(file);

    // No type, exit
    if(!info)
      return nullptr;

    // Variables we are going to use
    GameComponent* c;
    Variable var;

    // Get the component enum to check
    Enum* e = GET_ENUM(Component);

    // Check to see if the type we read is a component
    if (e->IsAnEntry(info->Name()))
    {
      // Get the type of component we need to make
      EComponent type = (EComponent)e->GetIndexFromString(info->Name());
      // Create a blank component in the space
      c = space->CreateComponent(type);

      // Make a variable from the component
      var = Variable(info, c);
    }
    else
      return NULL;

    // Set our start level
    int startLevel = s->GetPadLevel();

    if (var.GetTypeInfo()->m_deserialize == nullptr)
    {
      // Iterate through all member variables until our padding is back to the start
      do
      {
        // Peek to see the next possible member
        const Member* mem = s->PeekMember(file,   info->GetMembers(), startLevel);

      
        if (mem) // We have a member
        {
          // Create a variable from the member
          Variable member(mem->Type(), PTR_ADD(var.GetData(), mem->Offset()));
          // Deserialize the variable
          member.Deserialize(file);
        }
      } while(s->GetPadLevel() > startLevel);
    }
    else
    {
      var.Deserialize(file);
    }

    return c;
  }

  //////////////////////////////////////////////////////////////////////////
  // ARCHETYPE AND OBJECT FUNCTIONS

  void Factory::SaveObjectToArchetype(GameObject* obj, const char* name)
  {
    if (obj->archetype.length() == 0)
      obj->archetype = name;

    ArchetypeMap[obj->archetype].CopyObject(obj);

    SaveArchetypeToFile(ArchetypeMap[obj->archetype]);
  }

  void Factory::SaveArchetypeToFile(const Archetype& archetype)
  {
    File file; // File to load from
    std::string filepath;

    filepath = ArchetypeFilePath + archetype.archetype + ArchetypeFileExtension;

    file.Open(filepath.c_str(), FileAccess::Write);

    ErrorIf2(!file.Validate(), "Factory", "Invalid file!");

    Variable var = archetype;
    var.Serialize(file);

    file.Close();
  }

  void Factory::SaveArchetypeToFile(std::string name)
  {
    const Archetype& archetype = GetArchetype(name);

    if (&archetype != &Archetype::null)
      SaveArchetypeToFile(archetype);
  }

  GameObject* Factory::LuaLoadObjectFromArchetype(GameSpace* space, const char* name)
  {
    return FACTORY->LoadObjectFromArchetype(space, name);
  }

  GameObject* Factory::LoadObjectFromArchetype(GameSpace* space, const char* name)
  {
    std::string archetype = name;

    // Quickly grab the archetype from our map if it exists
    const Archetype& aType = GetArchetype(archetype);

    // Check to see if the archetype is valid
    if (&aType != &Archetype::null)
    {
      // Make the object!
      GameObject* obj = aType.CreateObject(space);
      return obj;
    }

    File file; // File to load from
    std::string filePath = ArchetypeFilePath + std::string(name) + ArchetypeFileExtension;

    if (!File::FileExists(filePath.c_str()))
      return nullptr;

    file.Open(filePath.c_str(), FileAccess::Read);

    ErrorIf2(!file.Validate(), "Factory", "Invalid file!");

    // Create an empty object
    GameObject* obj = space->CreateEmptyObject();
    Variable var = *obj; // Set the object as a variable

    // Deserialize the file into the object
    GET_TYPE(GameObject)->Deserialize(file, var);

	// Reset the Translation to 0,0,0
	if (obj->GetComponent<Transform>(eTransform))
		obj->GetComponent<Transform>(eTransform)->SetTranslation(float4(0, 0, 0, 0));

    obj->archetype = archetype;

    // The archetype was not found, so we will save the object into our map
    if (ArchetypeMap.find(archetype) == ArchetypeMap.end())
      ArchetypeMap[archetype].CopyObject(obj);

    file.Close();

    return obj;
  }

  GameObject* Factory::LoadObjectFromArchetypeFP(GameSpace* space, const char* filep)
  {
	  std::string archetype = filep;
    archetype = archetype.substr(archetype.find_last_of('\\') + 1, archetype.find_last_of('.') - archetype.find_last_of('\\') - 1);

	  // Quickly grab the archetype from our map if it exists
	  const Archetype& aType = GetArchetype(archetype);

	  // Check to see if the archetype is valid
	  if (&aType != &Archetype::null)
	  {
		  // Make the object!
		  GameObject* obj = aType.CreateObject(space);
		  return obj;
	  }

	  File file; // File to load from
	  std::string filePath = filep;

	  if (!File::FileExists(filePath.c_str()))
		  return nullptr;

	  file.Open(filePath.c_str(), FileAccess::Read);

	  ErrorIf2(!file.Validate(), "Factory", "Invalid file!");

	  // Create an empty object
	  GameObject* obj = space->CreateEmptyObject();
	  Variable var = *obj; // Set the object as a variable

	  // Deserialize the file into the object
	  GET_TYPE(GameObject)->Deserialize(file, var);

	  // Reset the Translation to 0,0,0
    obj->GetComponent<Transform>(eTransform)->SetTranslation(float4(0, 0, 0, 0));

	  obj->archetype = archetype;

	  // The archetype was not found, so we will save the object into our map
	  if (ArchetypeMap.find(archetype) == ArchetypeMap.end())
		  ArchetypeMap[archetype].CopyObject(obj);

	  file.Close();

	  return obj;
  }

  GameObject* Factory::LoadObjectFromArchetype(GameSpace* space, const Archetype& archetype)
  {
    return archetype.CreateObject(space);
  }


  void Factory::UpdateArchetypeObjects(GameSpace* space, const char* name)
  {
    // Grab the archetype of the object
    const Archetype& atype = GetArchetype(name);

    if (&atype == &Archetype::null) // The hackiest == operator on earth
      return;

    GameObject* it;
    for (unsigned int i = 0; i < space->m_objects.Size(); ++i)
    {
      it = (GameObject*)space->m_objects[i];

      if (it->archetype != atype.archetype)
        continue;

      // Update the values
      atype.UpdateObject(it);

    }
  }

  const Archetype& Factory::GetArchetype(std::string name)
  {
    if (ArchetypeMap.find(name) != ArchetypeMap.end())
      return ArchetypeMap[name];

    return Archetype::null;
  }

  bool Factory::LoadArchetypeFromFile(const char* name)
  {
    File file; // File to load from
    std::string filepath = name;
    std::string archetype = name;

    filepath = ArchetypeFilePath + filepath + ArchetypeFileExtension;

    // Quickly grab the archetype from our map if it exists
    const Archetype& aType = GetArchetype(archetype);

    // Check to see if the archetype is valid
    if (&aType != &Archetype::null)
    {
      // Make the object!
      return true;
    }

    if (!File::FileExists(filepath.c_str()))
    {
      return false;
    }

    file.Open(filepath.c_str(), FileAccess::Read);

    ErrorIf2(!file.Validate(), "Factory", "Invalid file!");

    Archetype buffer;
    Variable var = buffer;
    Archetype::Deserialize(file, var);
    buffer.archetype = archetype;

    ArchetypeMap[buffer.archetype] = buffer;

    return true;
  }

  //////////////////////////////////////////////////////////////////////////

  // Backups a file into the backup folder
  static void StoreBackup(const char* filepath)
  {
    if (File::FileExists(filepath))
    {
      time_t t = time(0);
      struct tm * now = localtime(&t);

      std::string backUpFile = "backup\\";
      backUpFile += filepath;
      backUpFile += "." + std::to_string(now->tm_mon) + "_" + std::to_string(now->tm_mday) + "_" +
        std::to_string(now->tm_hour) + "_" + std::to_string(now->tm_min) + "_" + std::to_string(now->tm_sec) + ".backup";

      
      std::string backUpPath;
      
      int lastBackslash = backUpFile.find_last_of('\\');
      if (lastBackslash != std::string::npos)
        backUpPath = backUpFile.substr(0, lastBackslash);

      _mkdir(backUpPath.c_str());
      //boost::filesystem::create_directories(backUpPath);

      std::ifstream src;
      src.open(filepath);
      std::ofstream dest;
      dest.open(backUpFile.c_str());
      dest << src.rdbuf();
    }
  }

  //////////////////////////////////////////////////////////////////////////
  // SPACE

  void Factory::SaveSpaceToFile(GameSpace* space, const char* name, std::vector<std::string>* objInstanceData, bool includeGeneric, bool allData)
  {
    File file; // File to save the space to
    std::string filepath = SpaceFilePath + name + SpaceFileExtension;
    GameSpace::SerializerData extraData;

    extraData.instanceData = objInstanceData;
    extraData.includeGeneric = includeGeneric;
    extraData.saveAllData = allData;
    extraData.standalone = false;


    //StoreBackup(filepath.c_str()); comment this out for now
    file.Open(filepath.c_str(), FileAccess::Write); // Open the file

    Serializer::Get()->SetUserData(&extraData);

    Variable(*space).Serialize(file);

    Serializer::Get()->SetUserData(NULL);

    file.Close();
  }

  void Factory::SaveSpaceToFile(GameSpace* space, const char* name, bool standalone)
  {
    File file; // File to save the space to
    std::string filepath = SpaceFilePath + name + SpaceFileExtension;
    GameSpace::SerializerData extraData;
    
    extraData.instanceData = NULL;
    extraData.includeGeneric = false;
    extraData.saveAllData = false;
    extraData.standalone = standalone;

    //StoreBackup(filepath.c_str()); comment this out for now

    file.Open(filepath.c_str(), FileAccess::Write); // Open the file

    Serializer::Get()->SetUserData(&extraData);

    Variable(*space).Serialize(file);

    Serializer::Get()->SetUserData(NULL);

    file.Close();

    space->m_fileName = name;
  }

  void Factory::SaveSpaceToFilePath(GameSpace* space, const char* path)
  {
    File file; // File to save the space to
    std::string filepath = path;
    GameSpace::SerializerData extraData;

    extraData.instanceData = NULL;
    extraData.includeGeneric = false;
    extraData.saveAllData = false;
    extraData.standalone = true;

    //StoreBackup(filepath.c_str()); comment this out for now

    file.Open(filepath.c_str(), FileAccess::Write); // Open the file

    Serializer::Get()->SetUserData(&extraData);

    Variable(*space).Serialize(file);

    Serializer::Get()->SetUserData(NULL);

    file.Close();
  }


  GameSpace* Factory::LoadSpaceFilePath(const char* filepath)
  {
    File file; // File to save the space to

    file.Open(filepath, FileAccess::Read); // Open the file

    ErrorIf2(!file.Validate(), "Factory - Load Level", "Invalid level: %s", filepath);

    std::string spaceName = file.GetLine('\n');

    if (ENGINE->GetSpace(spaceName.c_str()) != nullptr)
    {
      file.Close();
      return nullptr;
    }

    GameSpace* space = ENGINE->CreateSpace(spaceName.c_str());

    Variable(*space).Deserialize(file);

    file.Close();

    std::string fileName(filepath);
    int lastSlash = fileName.find_last_of('\\');
    if (lastSlash != std::string::npos)
      fileName = fileName.substr(lastSlash - 1, fileName.length() - lastSlash);

    space->m_fileName = fileName;

    return space;
  }

  GameSpace* Factory::LoadSpace(const char* name)
  {
    std::string filepath = SpaceFilePath + std::string(name) + SpaceFileExtension;
    return LoadSpaceFilePath(filepath.c_str());
  }



  //////////////////////////////////////////////////////////////////////////
  // LEVEL
  /*void Factory::CreateLevelFilePath(const char* filePath)
  {
	  File file;

	  std::string levelPath = filePath;
	  if (levelPath.find_last_of('.') != std::string::npos)
	  {
		  std::string extension = levelPath.substr(levelPath.find_last_of('.'), levelPath.length() - levelPath.find_last_of('.'));
		  if (extension != ".level")
			  levelPath += LevelFileExtension;
	  }
	  else
		  levelPath += LevelFileExtension;

	  file.Open(levelPath.c_str(), FileAccess::Write);
	  ErrorIf2(!file.Validate(), "Level Factory", "Failed to save a level? %s", levelPath.c_str());

	  ENGINE->Spaces().clear();

	  ENGINE->SetCurrentLevel(filePath);

	  file.Close();
  }*/
  void Factory::CreateLevel(const char* name)
  {
	  File file;

	  std::string levelPath = name;
	  if (levelPath.find_last_of('.') != std::string::npos)
	  {
		  std::string extension = levelPath.substr(levelPath.find_last_of('.'), levelPath.length() - levelPath.find_last_of('.'));
		  if (extension != ".level")
			  levelPath += LevelFileExtension;
	  }
	  else
		  levelPath += LevelFileExtension;

	  levelPath = LevelFilePath + levelPath;

	  file.Open(levelPath.c_str(), FileAccess::Write);
	  ErrorIf2(!file.Validate(), "Level Factory", "Failed to save a level? %s", levelPath.c_str());

	  ENGINE->Spaces().clear();

	  ENGINE->SetCurrentLevel(name);

	  file.Close();
  }

  void Factory::SaveLevel(const char* filePath)
  {
    File file;

    std::string levelPath = filePath;
    if (levelPath.find_last_of('.') != std::string::npos)
    {
      std::string extension = levelPath.substr(levelPath.find_last_of('.'), levelPath.length() - levelPath.find_last_of('.'));
      if (extension == ".level")
      {

      }
      else
      {
        levelPath += LevelFileExtension;
      }
    }
    else
    {
      levelPath += LevelFileExtension;
    }

	levelPath = LevelFilePath + levelPath;

    file.Open(levelPath.c_str(), FileAccess::Write);

    ErrorIf2(!file.Validate(), "Level Factory", "Failed to save a level? %s", levelPath.c_str());

    for (size_t i=0; i < ENGINE->Spaces().size(); ++i)
    {
      GameSpace* sp = ENGINE->Spaces()[i];
      file.Write("%s\n", sp->GetName().c_str());
      SaveSpaceToFile(sp, sp->GetName().c_str(), true);
    }

    file.Close();
  }

  void Factory::LoadLevel(const char* name, void(*cb)(GameSpace* space))
  {
    File file;

    std::string filePath = LevelFilePath + std::string(name) + LevelFileExtension;

    file.Open(filePath.c_str(), FileAccess::Read);

    ErrorIf2(!file.Validate(), "Level Factory", "Failed to load the level.");

    // Clear spaces

    // Creating a new level and a new space in the editor will automatically put  
    // the space in the space map. If you make a second level and reload the first level 
    // you made, the first level space still exists in the space map.
    Core::ENGINE->m_spaceMap.clear();
    ENGINE->ClearSpaces();
    ENGINE->CleanUp();

    std::string spaceName;

    while (file.Validate())
    {
      spaceName = file.GetLine('\n');
      if (spaceName.length() > 0)
      {
        spaceName = SpaceFilePath + spaceName + SpaceFileExtension;
        GameSpace* sp;
        sp = LoadSpaceFilePath(spaceName.c_str());
        if (cb)
          cb(sp);
      }
    }

    file.Close();

    ENGINE->m_currentLevel = name;
  }

  void Factory::LoadLevelFilePath(const char* filePath, const char* name, void(*cb)(GameSpace* space))
  {
    File file;

    file.Open(filePath, FileAccess::Read);

    ErrorIf2(!file.Validate(), "Level Factory", "Failed to load the level.");

    // Clear spaces
    ENGINE->ClearSpaces();
    ENGINE->CleanUp();

    std::string spaceName;

    while (file.Validate())
    {
      spaceName = file.GetLine('\n');
      if (spaceName.length() > 0)
      {
        spaceName = SpaceFilePath + spaceName + SpaceFileExtension;
        GameSpace* sp;
        sp = LoadSpaceFilePath(spaceName.c_str());
        if (cb)
          cb(sp);
      }
    }

    file.Close();

    ENGINE->m_currentLevel = name;

  }
}