/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: SoundType.h 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Audio/soundinstance/SoundInstance.h"

namespace Audio
{
  class SoundType
  {
  public:
    SoundType() {};
    ~SoundType() {};

    virtual void Play(SoundInstance *soundinst) = 0;
    virtual std::string GetName(void) const = 0;

  private:
    SoundType(const SoundType&);
    SoundType& operator=(const SoundType&);
    virtual void ErrorCheck(FMOD_RESULT result) = 0;
  };
}

