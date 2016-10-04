/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: SoundInstance.h 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

// INCLUDES
#include "fmod.hpp"
#include "fmod_studio.hpp"
#include <string>

namespace Audio
{
  union InstanceType
  {
    FMOD::Studio::EventInstance *m_eventInstance; // Studio API
    FMOD::Sound *m_soundInstance;                 // Low Level API
  };

  struct SoundInstance
  {
    SoundInstance();
    SoundInstance(float volume, float pitch);

    void SetVolume(float volume);
    void SetPitch(float pitch);

    bool playing;
    bool isMute;
    float volume;
    float pitch;
    std::string name;
    InstanceType instance;

    /* union
    {
    FMOD::Studio::EventInstance *eventInstance_;
    FMOD::Sound *soundInstance_;
    };*/

  };
}