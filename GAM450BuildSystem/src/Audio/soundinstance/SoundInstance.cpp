/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: SoundInstance.cpp 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

namespace Audio
{
  SoundInstance::SoundInstance() : volume(1.0f), pitch(1.0f), playing(false), isMute(false)
  {
    int b = 10;
  }

  SoundInstance::SoundInstance(float vol, float pi) : volume(vol), pitch(pi)
  {
    int b = 10;
  }

  void SoundInstance::SetVolume(float vol)
  {
    volume = vol;//Clamp(volume, 0.0f, 1.0f);
  }

  void SoundInstance::SetPitch(float pi)
  {
    pitch = pi;//Clamp(pitch, 0.0f, 1.0f);
  }
}