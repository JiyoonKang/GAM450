/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: main.cpp 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "Core/engine/core/Engine.h"
#include "Start/config/Config.h"

#include <Windows.h>
#include <iostream>

#include <Core/systems/Profiler/SProfiler.h>
// Make sure you have this installed on your computer to use
// https://vld.codeplex.com/releases
//#include <vld.h> 

using namespace Base;
using namespace Core;

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3dcompiler.lib")

namespace boost {
  void throw_exception(std::exception const & e)
  {
    throw e;
  }
}


int main(int argc, char** argv)
{
  Core::Engine* Core = AllocateEngine(argc, argv);
  InitEngine();

  //StartProfiler();
  while (ENGINE->Running())
  {
    ENGINE->MainLoop();
  }

  ENGINE->Shutdown();

  delete Core;

  return 0;
}