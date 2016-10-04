/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: ComponentCreator.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Base/util/Utilities.h"
#include "Core/types/space/Space.h"
using namespace Base;

namespace Core
{
  class GameComponent;

  struct ComponentCreator
  {
    // Creator Constructor
    ComponentCreator(EComponent type, unsigned int size) : typeID(type), m_size(size) {};

    // Type of component this constructor creates
    EComponent typeID;

    // Creation function, abstract
    virtual GameComponent* Create(GameSpace* space) = 0;

    // Simply creates a new component
    virtual GameComponent* Allocate() = 0;
	  
	  // Removal function, abstract
	  virtual void Remove(GameSpace* space, GameComponent* component) = 0;

	  // The size in bytes of the component
	  unsigned int m_size;
  };

  // template version of the Creator, used to have a copy
  // of the creator for every single type of component
  template<typename T>
  struct ComponentCreatorType : public ComponentCreator
  {
    ComponentCreatorType(EComponent type) : ComponentCreator(type, sizeof(T)) {}

    // Override the create function with the type we want to make
    virtual GameComponent* Create(GameSpace* space)
    {
      // Get a pointer to the ObjectAllocator for this type of component
		  ObjectAllocator* componentList = space->GetComponents(typeID);

      // Allocate a new component from the allocator
      T* component = (T*)componentList->Allocate();

      // Zero it out
      memset(component, NULL, sizeof(T));

      // Initialize the component
      new (component) T;

      // Setup the internals in the component
      component->self = space->GetHandles().Insert(component); // Push a handle into the HandleManager
      component->space = space;                                // Reference back to the space
      component->typeID = typeID;                              // Set the components type
      component->typeinfo = GET_TYPE(T);

      // Sync the handles
      space->GetHandles().SyncHandles<T>(*componentList);
      
      return component;
    }

    virtual GameComponent* Allocate()
    {
      // Allocate a new component
      T* comp = new T();

      // Set the type
      comp->typeID = typeID; // Set the components type
      comp->typeinfo = GET_TYPE(T);

      // Send back
      return comp;
    }

    virtual void Remove( GameSpace* space, GameComponent* component )
    {
      ObjectAllocator *components = space->GetComponents( typeID );

      // Call the destructor on the component
      component->~GameComponent( );

      GameComponent* moved = (GameComponent*)components->Free( component );
      // Resync handles
      space->GetHandles().Update( moved, moved->self );
    }
  };
};
