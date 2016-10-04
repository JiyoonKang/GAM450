/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: FmodEvent.cpp 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

namespace Audio
{
  FmodEvent::FmodEvent(FMOD::Studio::System *system, std::string& path)
  {
    // get event description
    FMOD_RESULT res = system->getEvent(path.c_str(), &m_eventDescription);

    // Getting the name
    std::size_t pos = path.find("/");
    std::size_t endPos = path.length() - pos;
    m_name = path.substr((pos + 1), endPos);
  }

  bool FmodEvent::IsOneShot(void)
  {
    bool oneShot;
    m_eventDescription->isOneshot(&oneShot);
    return oneShot;
  }

  void FmodEvent::ErrorCheck(FMOD_RESULT result)
  {
    if (result != FMOD_OK)
    {
      printf("ERROR! IN SOUND SYSTEM");
    }
  }

  void FmodEvent::InitializeEvent(SoundInstance *soundinst)
  {
    ErrorCheck(m_eventDescription->createInstance(&soundinst->instance.m_eventInstance));

    // set sound and pitch
    ErrorCheck(soundinst->instance.m_eventInstance->setPitch(soundinst->pitch));
    ErrorCheck(soundinst->instance.m_eventInstance->setVolume(soundinst->volume));
  }

  void FmodEvent::Play(SoundInstance *soundinst)
  {
    ErrorCheck(m_eventDescription->createInstance(&soundinst->instance.m_eventInstance));

    // set sound and pitch
    ErrorCheck(soundinst->instance.m_eventInstance->setPitch(soundinst->pitch));
    ErrorCheck(soundinst->instance.m_eventInstance->setVolume(soundinst->volume));

    ErrorCheck(soundinst->instance.m_eventInstance->start());

    if (!IsOneShot())
      soundinst->playing = true;

  }

  std::string FmodEvent::GetName(void) const
  {
    return m_name;
  }
}


