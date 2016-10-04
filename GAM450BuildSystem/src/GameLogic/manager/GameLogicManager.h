/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: GameLogicManager.h 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Core/systems/System.h"

namespace GameLogic
{
  class GameLogicManager : public Core::ISystem
  {
  public:
    GameLogicManager();
    ~GameLogicManager();

    virtual void Initialize(void);
    virtual void ReceiveMessage(Core::Message& msg);
    virtual void Update(float dt);
    virtual void RegisterComponents(void);

    virtual std::string GetName() const { return "GameLogic"; }

  private:

  };

  extern GameLogicManager* GAMELOGIC;
}