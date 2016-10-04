/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: GameLogicManager.cpp 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "GameLogicManager.h"

#include "Core/engine/core/Engine.h"
#include "Core/engine/factory/Factory.h"
#include "Core/types/space/Space.h"
#include "Core/types/object/Object.h"
#include "Core/types/archetype/Archetype.h"

// Components
#include "Core/components/transform/CTransform.h"
#include "Audio/components/emitter/SoundEmitter.h"

#include "Audio/soundinstance/SoundInstance.h"

#include "Lua/components/LuaProxy.h"

#include <vector>
#include "Core/systems/input/Input.h"
#include "Base/util/debug/DebugAssist.h"
#include "GameLogic\Timer.h"

// Components - Graphics
#include<Graphics/includes/GraphicsSystem/Renderable.h>
#include<Graphics/includes/GraphicsSystem/Paintable.h>
#include<Graphics/includes/GraphicsSystem/ParticleSystem.h>

namespace GameLogic
{
  GameLogicManager* GAMELOGIC = NULL;

  GameLogicManager::GameLogicManager()
  {
    GAMELOGIC = this;
  }

  GameLogicManager::~GameLogicManager() 
  {
    GAMELOGIC = nullptr;
  }

  void GameLogicManager::Initialize(void)
  {
   PrintOut("GameLogic Initialize");
  }

  void GameLogicManager::ReceiveMessage(Core::Message& msg)
  {

  }

  void GameLogicManager::Update(float dt)
  {
    // Loop through the game spaces, and update
    Core::GameSpace* space;
    std::vector<Core::GameSpace*> removeList;

    for (auto it = Core::ENGINE->m_spaces.begin(); it != Core::ENGINE->m_spaces.end(); ++it)
    {
      space = *it;

      if (!space->m_ready)
        continue;

      if (!space->Paused())
        space->hooks.Call("LogicUpdate", dt);

      space->hooks.Call("FrameUpdate", dt); 
    }

    // TODO@@ Call Lua updates

    // Clean up game spaces
    for (auto it = Core::ENGINE->m_spaces.begin(); it != Core::ENGINE->m_spaces.end(); ++it)
    {
      space = *it;

      if (!space->m_ready)
        continue;

      space->Cleanup();
    }

#if(0)
    //EXAMPLE()
    /* ============ ============  Component only iteration sample code ============ ============ */
    for (auto it = Core::ENGINE->m_spaces.begin(); it != Core::ENGINE->m_spaces.end(); ++it)
    {
      /* Get the game space */
      Core::GameSpace* sp = *it;

      /* Get the object allocator containing our transform components */
      Base::ObjectAllocator* TransformList = sp->GetComponents(eTransform);

      /* Iterate through the object allocator.
      We have to template it so the iterator (tr) is the right type
      and when we dereference it we get a Transform component. */
      /*    Type of tr = Core::Transform*    */
      for (auto tr = TransformList->begin<Core::Transform>();
        tr != TransformList->end<Core::Transform>(); ++tr)
      {
        // Bad way
        Core::GameObject* obj = sp->GetGameObject(tr->owner);
        obj->GetComponent<Core::Transform>(eTransform);
        // Good way
        Core::Transform* tr2 = sp->GetHandles().GetAs<Core::Transform>(tr->self); // this actually doesn't do anything


        /* Now we do some code using our transform component */
        float3 MyTranslation = tr->GetTranslation();
        float3 SomeVector = float3(1, 0, 0);
        float3 crossVector = MyTranslation.Cross(SomeVector);
      }

    }
    /* ============ ============  End of sample code ============ ============ */

#endif
  }

  void GameLogicManager::RegisterComponents(void)
  {
    REGISTER_COMPONENT(Transform, Core::Transform);
    REGISTER_COMPONENT(Paintable, Paintable);
    REGISTER_COMPONENT(ParticleSystem, ParticleSystem);
    REGISTER_COMPONENT(Renderable, Renderable);
    REGISTER_COMPONENT(LuaProxy, Lua::LuaProxy);
  }
}