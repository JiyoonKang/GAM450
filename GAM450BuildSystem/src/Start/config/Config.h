/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Config.h 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Core\engine\core\Engine.h"

namespace Config
{
  static int desiredWidth = 1366;
  static int desiredHeight = 768;

  // if installer is true, then we will limit certain capabilities in the editor
  // same if installer is false, we will limit some functionality in the game
  static bool installer = false;
}

class Core::Engine;

Core::Engine* AllocateEngine(int argc, char** argv);

void InitEngine(void);

  
