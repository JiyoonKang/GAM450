/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: AudioFile.h 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Audio/soundtypes/SoundType.h"

namespace Audio
{
  // LOW LEVEL API
  class AudioFile : public SoundType
  {
  public:
    AudioFile(FMOD::System *system, const std::string &name, FMOD::ChannelGroup *musicChannel, bool stream);
    void Play(SoundInstance *instance);
    std::string GetName(void) const;

    FMOD::Channel* GetChannel(void);
  private:
    void ErrorCheck(FMOD_RESULT result);

    FMOD::Sound *m_sound;
    std::string m_name;
    FMOD::Channel *m_channel;

    static FMOD::System *m_system;
    static FMOD::ChannelGroup *m_musicChannelGroup;
    static FMOD::ChannelGroup *m_sfxChannelGroup;
  };
}