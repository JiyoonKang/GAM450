/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Space.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Base/modules/memory/ObjectAllocator.h"
#include "Base/modules/handlemanager/HandleManager.h"
#include "Core/components/base/Component.h"
#include "Core/engine/hooks/Hooks.h"

#include "GameLogic/manager/GameLogicManager.h"

namespace Core
{
  class GameComponent;
  class GameObject;

	class GameSpace
	{
  public:
    GameSpace();
    ~GameSpace();


    void SetPaused(bool paused);
    inline bool Paused() { return m_paused; }

    inline void SetHidden(bool hidden) { m_hidden = hidden; }
    inline bool Hidden() { return m_hidden; }

    inline bool Ready(void) { return m_ready; }
    inline bool Valid(void) { return m_valid; }

    inline const std::string GetName() const { return m_name; }
	inline void SetName(std::string name) { m_name = name; }
    GameSpace* CopyGameSpace(const char* new_name);

    // Marks a space to be destroyed
    void Destroy(void);

    unsigned int GUID() { return m_spaceGUID; }
    HandleManager& GetHandles();

    //////////////////////////////////////////////////////////////////////////
    // COMPONENT FUNCTIONS

    GameComponent* CreateComponent(EComponent type);
    GameComponent* CreateComponent(size_t type);

    void RemoveComponent(GameComponent* comp);
    void RemoveComponent(Handle handle);

    // Gets the object allocator for a type of components
    ObjectAllocator* GetComponents(EComponent type);

    // Gets a specific component from an object
    GameComponent* GetComponent(EComponent type, Handle owner);

    //////////////////////////////////////////////////////////////////////////
    // OBJECT FUNCTIONS

    GameObject* CreateEmptyObject();

    GameObject* CreateObjectFromArchetype(const char* name);

    GameObject* GetGameObject(Handle handle);
    GameObject* GetFirstGameObjectNamed(std::string name);

    // Marks every single object in the space for deletion
    void Clear(void);
   
    //////////////////////////////////////////////////////////////////////////
    // SERIALIZATION ROUTINE

    static void Serialize(File& file, Variable var);

    // Deserialization routine, var must be an empty object
    static void Deserialize(File& file, Variable var);

    //////////////////////////////////////////////////////////////////////////

    inline ObjectAllocator& GetObjectList() { return m_objects; }

    //////////////////////////////////////////////////////////////////////////
    // Public members
    HookManager hooks;

    // Physics Space to simulate this game space's physics
    void* m_pSpace = nullptr;
    
    // The game space is being edited
    bool m_edit;

    // Filename of the space [DO NOT WRITE TO]
    std::string m_fileName;

  private:
    struct SerializerData
    {
      std::vector<std::string>* instanceData;
      bool includeGeneric;
      bool saveAllData;
      bool standalone;
    };

    void RemoveGameObject(GameObject* object);

    // Cleans up a space of any (to be) removed game objects.
    // Called at the end of the GameLogic update
    void Cleanup(void);

    //////////////////////////////////////////////////////////////////////////
    
    // The collection of all game objects in this game space
    ObjectAllocator m_objects;

    // The collection of all components in this game space
    ObjectAllocator m_components[ecountComponents];

    // The handle manager for this game space
    HandleManager m_handles;

    // Name of the space
    std::string m_name;

    // Global Unique ID counter
    unsigned int m_guid = 0;

    unsigned int m_spaceGUID;
    static unsigned int maxGuid;

    bool m_shuttingDown;

    bool m_paused;
    bool m_hidden;

    // If a space is invalid at the end of a frame it is deleted
    bool m_valid;

    bool m_ready = false;

    friend class Engine;
    friend class Factory;
    friend class GameLogic::GameLogicManager;
	};
}