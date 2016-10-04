#pragma once

#include "GameLogic/Level.h"

namespace GameLogic
{
  class MainMenu : public GameLevel
  {
  public:
    MainMenu();
    ~MainMenu();

    virtual void Initialize(void) override;

    virtual void Update(float dt) override;

    virtual void Deinitialize(void) override;
  };
}