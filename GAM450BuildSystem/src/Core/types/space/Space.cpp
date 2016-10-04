/*******************************************************************************
* All content © 2015 DigiPen (USA) Corporation, all rights reserved.
* Reproduction or disclosure of this file or its contents without the prior
* written consent of DigiPen Institute of Technology is prohibited.
*
* File: Space.cpp
* Author: Judy Cheng, (Credit Zachary Nawar)
* Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
******************************************************************************/
#include "pch/precompiled.h"
#include "types/space/Space.h"
#include "types/object/Object.h"
#include "engine/core/Engine.h"

#include <iostream>

using namespace Base;

namespace Core
{
	unsigned int GameSpace::maxGuid = 0;

	GameSpace::GameSpace()
		: m_objects(sizeof(GameObject), 1000), m_shuttingDown(false), m_paused(false),
		m_hidden(false), m_valid(true), m_spaceGUID(GameSpace::maxGuid++),
		m_pSpace(nullptr), m_edit(false)
	{
		for (unsigned i = 0; i < ecountComponents; ++i)
		{
			if (FACTORY->m_componentCreators[i])
				m_components[i].Initialize(FACTORY->m_componentCreators[i]->m_size, 1000);
		}

		hooks.space = this;
	}

	GameSpace::~GameSpace()
	{
		m_objects.~ObjectAllocator();

		for (unsigned i = 0; i < ecountComponents; ++i)
		{
			if (FACTORY->m_componentCreators[i])
				m_components[i].Clear();
		}

		m_pSpace = nullptr;
	}

	void GameSpace::SetPaused(bool paused)
	{
		m_paused = paused;
	}

	GameSpace* GameSpace::CopyGameSpace(const char* new_name)
	{
		// Make the new space
		//GameSpace* space = ENGINE->CreateSpace(new_name);

		//for (GameObject* it = m_objects.begin<GameObject>(); it != m_objects.end<GameObject>(); ++it)
		//{
		//  // Create a new game object and copy over all data
		//  GameObject* obj = space->CreateEmptyObject();

		//  GET_TYPE(GameObject)->Copy(obj, it);

		//  obj->Initialize();
		//}

		FACTORY->SaveSpaceToFile(this, "temp_space", true);

		GameSpace* space = FACTORY->LoadSpace("temp_space");

		return space;
	}


	void GameSpace::Destroy()
	{
		if (m_valid)
			ENGINE->m_spaceRemoveList.push_back(this);
		m_valid = false;
	}

	//////////////////////////////////////////////////////////////////////////
	// COMPONENT FUNCTIONS

	GameComponent* GameSpace::CreateComponent(EComponent type)
	{
		return FACTORY->m_componentCreators[type]->Create(this);
	}

	GameComponent* GameSpace::CreateComponent(size_t type)
	{
		return FACTORY->m_componentCreators[type]->Create(this);
	}

	ObjectAllocator* GameSpace::GetComponents(EComponent type)
	{
		return &m_components[type];
	}

	GameComponent* GameSpace::GetComponent(EComponent type, Handle owner)
	{
		return GetHandles().GetAs<GameObject>(owner)->GetComponent(type);
	}

	void GameSpace::RemoveComponent(GameComponent* comp)
	{
		FACTORY->m_componentCreators[comp->typeID]->Remove(this, comp);
	}

	void GameSpace::RemoveComponent(Handle handle)
	{
		GameComponent* comp = GetHandles().GetAs<GameComponent>(handle);
		RemoveComponent(comp);
	}

	//////////////////////////////////////////////////////////////////////////
	// OBJECT FUNCTIONS

	GameObject* GameSpace::CreateEmptyObject()
	{
		// Allocate space for a new object
		GameObject* object = (GameObject*)m_objects.Allocate();

		// Re-construct the game object
		new (object)GameObject();

		// Register the object inside of the handle manager
		object->self = GetHandles().Insert(object);
		object->space = this;
		object->guid = m_guid++;
		object->m_active = true;

		// Check to see if we need to sync the GameObject handles
		m_handles.SyncHandles<GameObject>(m_objects);

		return object;
	}

	GameObject* GameSpace::CreateObjectFromArchetype(const char* name)
	{
		return FACTORY->LoadObjectFromArchetype(this, name);
	}

	GameObject* GameSpace::GetGameObject(Handle handle)
	{
		return m_handles.GetAs<GameObject>(handle);
	}

  GameObject* GameSpace::GetFirstGameObjectNamed(std::string name)
  {
    ObjectAllocator& objects = this->GetObjectList();

    for (unsigned int i = 0; i < objects.Size(); ++i)
    {
      GameObject* obj = reinterpret_cast<GameObject*>(objects[i]);
      if (obj->GetName() == name)
      {
        return obj;
      }
    }

    return nullptr;
  }

	void GameSpace::RemoveGameObject(GameObject* object)
	{
		// Removes all components
		if (object && object->m_allocated)
		{
			object->~GameObject();

			object->m_allocated = false;

			// Remove the object from the handle manager
			GetHandles().Remove(object->self);

			// Remove the object from the object list
			GameObject* moved = (GameObject*)m_objects.Free(object);

			// When we freed the object the right most element
			// was moved to take the old objects place so now
			// we have to update the handle manager and tell it
			// to point at the right place in memory
			if (moved)
				GetHandles().Update(moved, moved->self);
		}
	}

	void GameSpace::Clear()
	{
		for (unsigned int i = 0; i < m_objects.Size(); ++i)
		{
			GameObject* obj = (GameObject*)m_objects[i];
			if (obj)
			{
				obj->m_active = false;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	HandleManager& GameSpace::GetHandles()
	{
		return m_handles;
	}

	void GameSpace::Cleanup()
	{
		std::vector<Handle> removeList;

		for (unsigned int i = 0; i < m_objects.Size(); ++i)
		{
			GameObject* obj = (GameObject*)m_objects[i];

			if (!obj->m_active || m_shuttingDown)
			{
				removeList.push_back(obj->self);
			} // End object not active loop
		}

		for (unsigned int i = 0; i < removeList.size(); ++i)
		{
			RemoveGameObject(this->GetGameObject(removeList[i]));
		}

		removeList.clear();
	}

	//////////////////////////////////////////////////////////////////////////
	// SERIALAZATION ROUTINE

	void GameSpace::Serialize(File& file, Variable var)
	{
		SerializerData* sd = (SerializerData*)Serializer::Get()->GetUserData();

		std::vector<std::string>* objInstanceData = sd->instanceData;
		bool includeGeneric = sd->includeGeneric;
		bool allData = sd->saveAllData;
		bool standalone = sd->standalone;

		GameSpace* space = (GameSpace*)var.GetData();

		file.Write("%s\n", space->GetName().c_str());

		for (auto it = space->m_objects.begin<GameObject>(); it != space->m_objects.end<GameObject>(); ++it)
		{

			if (it->archetype.length() == 0 && includeGeneric || standalone)
			{
				// Can't find an archetype, so we are going to
				// serialize the object into the level

				// Set the variable var to the game object pointer
				Variable var = *it;

				it->Serialize(file, var);
				continue;
			}

			// Write the name of the archetype
			file.Write("ach_%s\n", it->archetype.c_str());

			// Check to see if we have the object's archetypes cached in our map
			const Archetype& objectArchetype = FACTORY->GetArchetype(it->archetype);

			if (&objectArchetype == &Archetype::null || objectArchetype.name != it->name)
			{
				// Get the name member of the object
				const Base::Member* nameMember = GET_TYPE(GameObject)->GetMember("name");

				// Pad and write the name of the member
				file.Write("  %s ", nameMember->Name());

				// Create a variable and write to the file
				Variable name(nameMember->Type(), (char*)it + nameMember->Offset());
				name.GetTypeInfo()->Serialize(file, name);
			}

			// All data saving routines
			if (allData)
			{

				// Our object archetype is valid
				if (&objectArchetype != &Archetype::null)
				{
					// We have the archetype so we will only serialize the differences
					objectArchetype.SerializeDifferences(it, file);
					continue;
				}
			}

			// We are provided instance data to save
			if (objInstanceData)
			{
				for (unsigned j = 0; j < objInstanceData->size(); ++j)
				{
					// Set the variable var to the game object pointer
					Variable var = &(*it);

					// Pull the instance from the vector
					// We have to use the operator[] bit to avoid some un-necessary casting
					const char *instance = objInstanceData->operator[](j).c_str();

					// Now lets pull out the member
					const Base::Member *member = FACTORY->GetComponentMember(instance, var);

					ErrorIf2(!member, "Space serialization", "Member is invalid!");

					// Variable var now is the member variable we wanted
					// Write the name of the variable
					file.Write("  %s ", instance);
					// Serialize the variable
					var.Serialize(file);
				}
			}

		} // end for

	}

	void GameSpace::Deserialize(File& file, Variable var)
	{
		GameSpace* space = (GameSpace*)var.GetData();

		// Pointer to use
		GameObject* obj = nullptr;

		const TypeInfo* typeinfo;

		for (;;)
		{
			// Read in a line
			std::string line = file.GetLine(" \n:");

			// Check to see if we read the entire file
			if (!file.Validate())
				break;

			// Skip white spaces
			if (line.empty())
				continue;

			typeinfo = GET_STR_TYPE(line.c_str());

			if (typeinfo && strcmp(typeinfo->Name(), "GameObject") == 0)
			{
				file.SeekByOffset(-int(line.length()) - 2);

				// Create an empty object
				obj = space->CreateEmptyObject();

				// Set the object as a variable
				Variable var = *obj;

				// Deserialize the file into the object
				GET_TYPE(GameObject)->Deserialize(file, var);

				if (obj->archetype.length() > 0)
					if (&FACTORY->GetArchetype(obj->archetype) == &Archetype::null)
						FACTORY->ArchetypeMap[obj->archetype].CopyObject(obj);

				continue;
			}

		}

	}


}