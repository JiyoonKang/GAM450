/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: SoundEmitter.h 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

// INCLUDES
#include <string>

// MY INCLUDES
#include "Audio/manager/AudioManager.h"
#include "Audio/soundinstance/SoundInstance.h"
#include "Core/components/base/Component.h"
#include "Base/util/file/File.h"
#include "Base/modules/introspection/variable/Variable.h"

namespace Audio
{
  class SoundEmitter : public Core::GameComponent
  {
  public:
    SoundEmitter() 
    {
    // Get pointer to the sound system
      m_audioManager = Audio::AUDIOMANAGER;
    }

    ~SoundEmitter()
    {
      //delete m_audioManager;    
     // m_currentSong.~basic_string();
    }

    inline void Play(const std::string &name)
    {
      m_audioManager->PlayEvent(name, &m_instance);
    }

    inline void Stop()
    {
      m_audioManager->StopEvent(&m_instance, false);
    }

    inline void Pause()
    {
      m_audioManager->PauseEvent(&m_instance);
    }

    inline void Resume()
    {
      m_audioManager->ResumeEvent(&m_instance);
    }

    inline void SetVolume(float volume)
    {
      m_volume = volume;
      m_audioManager->SetVolume(volume, &m_instance);
    }
    inline void SetPitch(float pitch)
    {
      m_pitch = pitch;
      m_audioManager->SetPitch(pitch, &m_instance);
    }
    inline void Mute()
    {
      m_volume = 0.0f;
      m_audioManager->MuteEvent(&m_instance);
    }
    inline void Unmute()
    {
      m_audioManager->UnmuteEvent(&m_instance);
    }

    void Initialize()
    {
    }

    // Volume Change
    // Play Delayed
    // Play Scheduled

    std::string m_currentSong;
    float m_volume;
    float m_pitch;
    bool m_isPlaying;
    bool m_isLoop;

  private:
    AudioManager *m_audioManager;
    SoundInstance m_instance;
    
    COMP_LUA_USABLE;
  };
}