/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: FmodEvent.h 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Audio/soundtypes/SoundType.h"

namespace Audio
{
  // HIGH LEVEL API
  class FmodEvent : public SoundType
  {
  public:
    FmodEvent(FMOD::Studio::System *system, std::string& path);
    ~FmodEvent() {};

    // Don't call this if you're just playing a sound
    void InitializeEvent(SoundInstance *soundinstance); 
    void Play(SoundInstance *soundinst);
    std::string GetName(void) const;
    bool IsOneShot(void);

    FmodEvent(SoundInstance*);
    FmodEvent& operator=(SoundInstance*);
    void ErrorCheck(FMOD_RESULT result);

    FMOD::Studio::EventDescription *m_eventDescription;
    std::string m_name;
  };
}
