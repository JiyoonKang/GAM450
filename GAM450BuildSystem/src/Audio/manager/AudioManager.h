/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: AudioManager.h 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

// INCLUDES
#include "fmod.hpp"
#include "fmod_studio.hpp"
#include <unordered_map>
#include <vector>

// MY INCLUDES
#include "Audio/soundtypes/FmodEvent.h"
#include "Audio/soundtypes/AudioFile.h"
#include "Audio/soundinstance/SoundInstance.h"
#include "Core/systems/System.h"
#include <map>

namespace Audio
{

  struct SoundInstance;

  // Contains the parameter data in an event
  struct ParameterData
  {
    std::string paramName;
    float currentValue;
    float maxValue;
    float minValue;
  };

  struct FMOD3DAttributes
  {
    //MathLib::Vector3D position;
    //MathLib::Vector3D velocity;
    //MathLib::Vector3D forward;
    //MathLib::Vector3D up;
  };

  typedef std::unordered_map<std::string, FmodEvent*> SoundMap;
  typedef std::vector<FMOD::Studio::Bank *> BankVector;
  typedef std::vector<ParameterData> ParamDataList;

 
  class AudioManager : public Core::ISystem
  {
  public:
    AudioManager();
    ~AudioManager();

    // Systems
    void Initialize(void) override;
    void Update(float dt) override;
    void Shutdown(void) override;
    virtual void ReceiveMessage(Core::Message& msg);
    void RegisterComponents(void) override;

    virtual std::string GetName() const { return "Audio"; };

    void InitializeEvent(const std::string &name, SoundInstance *soundinst);

    // INDIVIDUALS
    void PlayEvent(const std::string &name, SoundInstance *soundinst);
    void StopEvent(SoundInstance *soundinst, bool fadeout = false);
    bool IsPaused(SoundInstance *soundinst);
    void PauseEvent(SoundInstance *soundinst);
    void ResumeEvent(SoundInstance *soundinst);
    void MuteEvent(SoundInstance *soundinst);
    void UnmuteEvent(SoundInstance *soundinst);
    void SetVolume(float volume, SoundInstance *soundinst);
    void SetPitch(float pitch, SoundInstance *soundinst);
    float GetVolume(SoundInstance *soundinst);
    float GetPitch(SoundInstance *soundinst);

    // MASTER CHANNEL
    void StopAll(void);
    void PauseAll(void);
    void ResumeAll(void);
    void MuteAll(void);
    void UnmuteAll(void);
    void SetMasterVolume(float volume);
    void SetMasterPitch(float pitch);
    float GetMasterVolume(void);
    float GetMasterPitch(void);
    void SetAllMute(bool state);
    bool GetAllMute(void);

    // MUSIC BUS
    void StopMusic(void);
    void PauseMusic(void);
    void ResumeMusic(void);
    bool IsMusicMute(void);
    void MuteMusic(void);
    void UnmuteMusic(void);
    void SetMusicVolume(float volume);
    float GetMusicVolume(void);

    // SFX BUS
    void StopSFX(void);
    void PauseSFX(void);
    void ResumeSFX(void);
    bool IsMusicSFX(void);
    void MuteSFX(void);
    void UnmuteSFX(void);
    void SetSFXVolume(float volume);
    float GetSFXVolume(void);

    // MENU SFX BUS
    void StopMenuSFX(void);
    void PauseMenuSFX(void);
    void ResumeMenuSFX(void);
    bool IsMusicMenuSFX(void);
    void MuteMenuSFX(void);
    void UnmuteMenuSFX(void);
    void SetMenuSFXVolume(float volume);
    float GetMenuSFXVolume(void);

    // PARAMETERS FUNCTIONS
    ParamDataList GetParamaterList(SoundInstance* soundinst) const;

    /* Returns how many parameters are in an event */
    float GetParamValue(SoundInstance* soundinst, const char* paramName) const;

    /* Set the parameter value */
    void SetParameter(SoundInstance *soundinst, const char* paramName, float param);

    // 3D FUNCTIONS
    bool Is3D(SoundInstance *soundinst);
    void Set3DAttributes(SoundInstance *soundinst, FMOD_3D_ATTRIBUTES *attributes);
    FMOD_3D_ATTRIBUTES Get3DAttributes(SoundInstance *soundinst);
    //FMOD3DAttributes Get3DAttributes(SoundInstance *instance);

    // CUE FUNCTIONS
    int GetCueCount(SoundInstance *soundinst);
    FMOD::Studio::CueInstance *GetCueByName(SoundInstance *soundinst, const char *name);
    FMOD::Studio::CueInstance *GetCueByIndex(SoundInstance *soundinst, int index);

    /*
    Gets the play back state
    FMOD_STUDIO_PLAYBACK_PLAYING,
    FMOD_STUDIO_PLAYBACK_SUSTAINING,
    FMOD_STUDIO_PLAYBACK_STOPPED,
    FMOD_STUDIO_PLAYBACK_STARTING,
    FMOD_STUDIO_PLAYBACK_STOPPING
    */
    FMOD_STUDIO_PLAYBACK_STATE *GetPlayBackState(SoundInstance *soundinst);

    // Get the time line position in milliseconds
    int GetTimeLinePosition(SoundInstance *soundinst);

    // Set the time line position in milliseconds
    void SetTimeLinePosition(SoundInstance *soundinst, int position);

    void ErrorCheck(FMOD_RESULT result);
    void CreateFMODSystem(void);
    void CreateChannelGroups(void);
    void GetChannelGroups(void);
    void CreateMasterChannel(void);
    void GetDrivers(void);
    void CheckVersion(void);
    void UnloadAllSoundBank(void);

    void ParseEvents(FMOD::Studio::System* system, std::ifstream &file, SoundMap &soundMap);
    void ParseBanks(FMOD::Studio::System* system, std::ifstream &file, BankVector &banksVec);
    void ParseFiles(FMOD::System *lowlevelSystem, SoundMap &soundMap, FMOD::ChannelGroup *musicChannel);
    void LoadEvent(FMOD::Studio::System* system, std::string eventName, SoundMap &soundMap);
    void LoadBank(FMOD::Studio::System* system, std::string &bankName, BankVector &banksVec);

    void LoadBuses(FMOD::Studio::System* system);

   

    FMOD::Studio::System* m_soundSystem;
    FMOD::System* m_lowlevelsound_System;
    FMOD::DSP* m_lowPassEffect;
    const std::string m_GUIDs;

    // containers
    SoundMap m_soundMap;
    BankVector m_banksVec;

    // contain the sounds played from the menu script
    std::map<int, SoundInstance*> m_javaScriptSounds;

    // channel groups
    FMOD::ChannelGroup* m_masterChannel;
    FMOD::ChannelGroup* m_musicChannel;
    FMOD::ChannelGroup* m_sfxChannel;
    FMOD::ChannelGroup* m_menuSFXChannel;

    // buses
    FMOD::Studio::Bus* m_musicBus;
    FMOD::Studio::Bus* m_sfxBus;
    FMOD::Studio::Bus* m_menuSfxBus;

    // instances
    FMOD::Studio::EventInstance* m_eventInstance;
    std::string m_currentSound;

    float m_masterVolume; // 0.0 ~ 1.0
    float m_masterPitch;  // 0.0 ~ 1.0
    float m_currentEventVolume;
    bool m_isEventMute;
    bool m_isMusicMute;
    bool m_isAllMute;

  };

  extern AudioManager* AUDIOMANAGER;

  namespace AudioUtils
  {
    enum SourceType
    {
      TYPE_AUDIO,
      TYPE_GUID
    };

    std::string SourcePath(const std::string filename, SourceType type);
  }

}