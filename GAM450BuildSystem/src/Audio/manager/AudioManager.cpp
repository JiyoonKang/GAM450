/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: AudioManager.cpp 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "util/debug/DebugAssist.h"

//#include "boost/filesystem/operations.hpp"
//#include "boost/filesystem/path.hpp"
#include "Core/engine/factory/Factory.h"
#include <sstream>

namespace Audio
{
  AudioManager *AUDIOMANAGER = NULL;

  AudioManager::AudioManager()
    : m_GUIDs("GUIDs.txt"), 
    m_masterChannel(nullptr), 
    m_musicChannel(nullptr), 
    m_sfxChannel(nullptr),
    m_menuSFXChannel(nullptr), 
    m_musicBus(nullptr), 
    m_sfxBus(nullptr), 
    m_menuSfxBus(nullptr),
    m_eventInstance(nullptr),
    m_masterVolume(1.0f), 
    m_masterPitch(1.0f), 
    m_currentEventVolume(0.0f),
    m_isEventMute(false),
    m_isMusicMute(false),
    m_isAllMute(false)
  {
    AUDIOMANAGER = this;
  }

  AudioManager::~AudioManager()
  {
  /*  m_soundSystem->release();
    m_lowlevelsound_System->release();
    m_currentSound.~basic_string();
    AUDIOMANAGER = NULL;*/
  }

  void AudioManager::Initialize(void)
  {
    CreateFMODSystem();

    //// Test to see current path
    //boost::filesystem::path p = boost::filesystem::current_path();

    std::ifstream infile;
    infile.open(AudioUtils::SourcePath(m_GUIDs, AudioUtils::TYPE_GUID).c_str());
    if (!infile.is_open())
    {
      throw std::invalid_argument("GUIDs.txt file cannot be open!");
    }

    // parse through the banks, and events
    ParseBanks(m_soundSystem, infile, m_banksVec);
    ParseEvents(m_soundSystem, infile, m_soundMap);

    CreateMasterChannel();
    //CreateChannelGroups();

    // Load Buses
    LoadBuses(m_soundSystem);
    //GetChannelGroups();

    PrintOut("Audio Initialize");

  }

  void AudioManager::Update(float dt)
  {
    ErrorCheck(m_soundSystem->update());
  }
  void AudioManager::Shutdown(void)
  {
    UnloadAllSoundBank();
    //ErrorCheck(m_soundSystem->release()); // Crashes when closing for some reason

    for (auto it = m_soundMap.begin(); it != m_soundMap.end(); ++it)
    {
      delete it->second;
      it->second = 0;
    }

  }

  void AudioManager::RegisterComponents()
  {
    REGISTER_COMPONENT(SoundEmitter, Audio::SoundEmitter);
  }

  void AudioManager::GetDrivers(void)
  {
    int numDrivers;
    ErrorCheck(m_lowlevelsound_System->getNumDrivers(&numDrivers));

    if (numDrivers == 0)
    {
      ErrorCheck(m_lowlevelsound_System->setOutput(FMOD_OUTPUTTYPE_NOSOUND));
    }
  }

  void AudioManager::CheckVersion(void)
  {
    unsigned int version;
    ErrorCheck(m_lowlevelsound_System->getVersion(&version));

    // check version
    if (version < FMOD_VERSION)
    {
      printf("Error! You are using an old version of FMOD %08x. This program requires %08x\n",
        version, FMOD_VERSION);
      return;
    }
  }

  void AudioManager::CreateFMODSystem(void)
  {
    // create studio system
    ErrorCheck(FMOD::Studio::System::create(&m_soundSystem));

    // get low level pointer
    ErrorCheck(m_soundSystem->getLowLevelSystem(&m_lowlevelsound_System));

    // initialize fmod studio system
    ErrorCheck(m_soundSystem->initialize(256, FMOD_STUDIO_INIT_NORMAL,
      FMOD_INIT_NORMAL, 0));

    // get version
    CheckVersion();

    // check for sound drivers
    GetDrivers();

  }

  void AudioManager::UnloadAllSoundBank(void)
  {
    ErrorCheck(m_soundSystem->unloadAll());
  }

  void AudioManager::CreateChannelGroups(void)
  {
    ErrorCheck(m_lowlevelsound_System->createChannelGroup("Music", &m_musicChannel));
    ErrorCheck(m_lowlevelsound_System->createChannelGroup("SFX", &m_sfxChannel));
    ErrorCheck(m_lowlevelsound_System->createChannelGroup("MenuSFX", &m_menuSFXChannel));

  }

  void AudioManager::GetChannelGroups(void)
  {
    ErrorCheck(m_musicBus->getChannelGroup(&m_musicChannel));
    ErrorCheck(m_sfxBus->getChannelGroup(&m_sfxChannel));
    ErrorCheck(m_menuSfxBus->getChannelGroup(&m_menuSFXChannel));
  }

  void AudioManager::CreateMasterChannel(void)
  {
    bool active;

    ErrorCheck(m_lowlevelsound_System->createDSPByType(FMOD_DSP_TYPE_FFT, &m_lowPassEffect));
    ErrorCheck(m_lowPassEffect->getActive(&active));
    ErrorCheck(m_lowlevelsound_System->getMasterChannelGroup(&m_masterChannel));

    if (!active)
    {
      ErrorCheck(m_masterChannel->addDSP(0, m_lowPassEffect));
      ErrorCheck(m_lowPassEffect->setActive(true));
    }
  }

  //////////////////////////////////////////////////////////////////////////
  // PARAMETER FUNCTIONS
  void AudioManager::SetParameter(SoundInstance *soundinst, const char* paramName, float param)
  {
    if (soundinst->instance.m_eventInstance)
    {
      ErrorCheck(soundinst->instance.m_eventInstance->setParameterValue(paramName, param));
    }
  }

  float AudioManager::GetParamValue(SoundInstance* soundinst, const char* paramName) const
  {
    FMOD::Studio::ParameterInstance* pi;
    float paramValue;

    // Sets pi to point at the parameter instance (of the event instance)
    soundinst->instance.m_eventInstance->getParameter(paramName, &pi);
    // Gets the value of the parameter (stores inside of paramValue)
    pi->getValue(&paramValue);

    return paramValue;
  }

  ParamDataList AudioManager::GetParamaterList(SoundInstance* soundinst) const
  {
    int params;

    ParamDataList paramList;

    soundinst->instance.m_eventInstance->getParameterCount(&params);
    FMOD::Studio::ParameterInstance* pi;
    FMOD_STUDIO_PARAMETER_DESCRIPTION pd;
    ParameterData pdata;

    for (int i = 0; i < params; ++i)
    {
      soundinst->instance.m_eventInstance->getParameterByIndex(i, &pi);
      pi->getDescription(&pd);

      pi->getValue(&pdata.currentValue);

      pdata.minValue = pd.minimum;
      pdata.maxValue = pd.maximum;
      pdata.paramName = pd.name;

      paramList.push_back(pdata);
    }

    return paramList;
  }

  //////////////////////////////////////////////////////////////////////////
  // CUE FUNCTIONS

  int AudioManager::GetCueCount(SoundInstance *soundinst)
  {
    int count;
    ErrorCheck(soundinst->instance.m_eventInstance->getCueCount(&count));;
    return count;
  }

  FMOD::Studio::CueInstance *AudioManager::GetCueByName(SoundInstance *soundinst, const char *name)
  {
    FMOD::Studio::CueInstance *cue;
    ErrorCheck(soundinst->instance.m_eventInstance->getCue(name, &cue));
    return cue;

  }
  FMOD::Studio::CueInstance *AudioManager::GetCueByIndex(SoundInstance *soundinst, int index)
  {
    FMOD::Studio::CueInstance *cue;
    ErrorCheck(soundinst->instance.m_eventInstance->getCueByIndex(index, &cue));
    return cue;
  }

  //////////////////////////////////////////////////////////////////////////

  bool AudioManager::Is3D(SoundInstance *soundinst)
  {
    FMOD::Studio::EventDescription *description = nullptr;
    bool is3D;
    ErrorCheck(soundinst->instance.m_eventInstance->getDescription(&description));
    ErrorCheck(description->is3D(&is3D));
    return is3D;
  }
  void AudioManager::Set3DAttributes(SoundInstance *soundinst, FMOD_3D_ATTRIBUTES *attributes)
  {
    ErrorCheck(soundinst->instance.m_eventInstance->set3DAttributes(attributes));
  }
  FMOD_3D_ATTRIBUTES AudioManager::Get3DAttributes(SoundInstance *soundinst)
  {
    FMOD_3D_ATTRIBUTES attributes;
    ErrorCheck(soundinst->instance.m_eventInstance->get3DAttributes(&attributes));
    return attributes;
  }

  //FMOD3DAttributes AudioManager::Get3DAttributes(SoundInstance *soundinst)
  //{
  //    FMOD_3D_ATTRIBUTES attributes;
  //    FMOD3DAttributes result;
  //    ErrorCheck(soundinst->instance.eventInstance_->get3DAttributes(&attributes));
  //    
  //    result.forward.set(attributes.forward.x, attributes.forward.y, attributes.forward.z);
  //    result.position.set(attributes.position.x, attributes.position.y, attributes.position.z);
  //    result.up.set(attributes.up.x, attributes.up.y, attributes.up.z);
  //    result.velocity.set(attributes.velocity.x, attributes.velocity.y, attributes.velocity.z);
  //
  //    return result;
  //}

  FMOD_STUDIO_PLAYBACK_STATE *AudioManager::GetPlayBackState(SoundInstance *soundinst)
  {
    FMOD_STUDIO_PLAYBACK_STATE *state = nullptr;
    ErrorCheck(soundinst->instance.m_eventInstance->getPlaybackState(state));

    return state;
  }


  int AudioManager::GetTimeLinePosition(SoundInstance *soundinst)
  {
    int position;
    ErrorCheck(soundinst->instance.m_eventInstance->getTimelinePosition(&position));
    return position;
  }

  void AudioManager::InitializeEvent(const std::string &name, SoundInstance *soundinst)
  {
    // check if name exist
    SoundMap::const_iterator it = m_soundMap.find(name);
    if (it == m_soundMap.end())
    {
      printf("Error! Sound not found");
    }
    else
    {
      //FmodEvent* h;
      //h->
      m_soundMap[name]->InitializeEvent(soundinst);
    }
  }

  void AudioManager::SetTimeLinePosition(SoundInstance *soundinst, int position)
  {
    ErrorCheck(soundinst->instance.m_eventInstance->setTimelinePosition(position));
  }

  //////////////////////////////////////////////////////////////////////////
  // EVENT FUNCTIONS
  void AudioManager::PlayEvent(const std::string &name, SoundInstance *soundinst)
  {
    // check if name exist
    SoundMap::const_iterator it = m_soundMap.find(name);
    if (it == m_soundMap.end())
    {
      printf("Error! Sound not found");
    }
    else
    {
      m_soundMap[name]->Play(soundinst);
      soundinst->name = name;

      //if (!m_soundMap[name]->IsOneShot())
      //  m_currentSound = name;
      /*else
        m_currentSound.clear();*/

    }
  }

  void AudioManager::StopEvent(SoundInstance *soundinst, bool fadeout)
  {
    if (soundinst->playing)
    {
      if (fadeout)
        ErrorCheck(soundinst->instance.m_eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));
      else
        ErrorCheck(soundinst->instance.m_eventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE));

      ErrorCheck(soundinst->instance.m_eventInstance->release());
     // m_currentSound.clear();
    }
  }

  bool AudioManager::IsPaused(SoundInstance *soundinst)
  {
    bool pause;
    ErrorCheck(soundinst->instance.m_eventInstance->getPaused(&pause));
    return pause;
  }

  void AudioManager::PauseEvent(SoundInstance *soundinst)
  {
    ErrorCheck(soundinst->instance.m_eventInstance->setPaused(true));
  }

  void AudioManager::ResumeEvent(SoundInstance *soundinst)
  {
    ErrorCheck(soundinst->instance.m_eventInstance->setPaused(false));
  }

  void AudioManager::MuteEvent(SoundInstance *soundinst)
  {
    if (!soundinst->isMute)
    {
      // get the current volume and save it
      m_currentEventVolume = GetVolume(soundinst);

      ErrorCheck(soundinst->instance.m_eventInstance->setVolume(0.0f));

      soundinst->isMute = true;
    }
  }

  void AudioManager::UnmuteEvent(SoundInstance *soundinst)
  {
    if (soundinst->isMute)
    {
      ErrorCheck(soundinst->instance.m_eventInstance->setVolume(m_currentEventVolume));
      soundinst->isMute = false;
    }
  }

  void AudioManager::SetVolume(float volume, SoundInstance *soundinst)
  {
    soundinst->volume = volume;
    ErrorCheck(soundinst->instance.m_eventInstance->setVolume(volume));

    soundinst->isMute = (soundinst->volume < 0.0f) ? true : false;
  }

  void AudioManager::SetPitch(float pitch, SoundInstance *soundinst)
  {
    soundinst->pitch = pitch;
    ErrorCheck(soundinst->instance.m_eventInstance->setPitch(pitch));
  }

  float AudioManager::GetVolume(SoundInstance *soundinst)
  {
    float volume;
    ErrorCheck(soundinst->instance.m_eventInstance->getVolume(&volume));
    return volume;
  }

  float AudioManager::GetPitch(SoundInstance *soundinst)
  {
    float pitch;
    ErrorCheck(soundinst->instance.m_eventInstance->getPitch(&pitch));
    return pitch;
  }

  //////////////////////////////////////////////////////////////////////////
  // MASTER CHANNEL FUNCTIONS
  void AudioManager::StopAll(void)
  {
    int result = m_masterChannel->stop();
    for (auto & iter : m_soundMap)
    {
      iter.second->m_eventDescription->releaseAllInstances();
    }
  }

  void AudioManager::PauseAll(void)
  {
    m_masterChannel->setPaused(true);
  }

  void AudioManager::ResumeAll(void)
  {
    m_masterChannel->setPaused(false);
  }

  void AudioManager::MuteAll(void)
  {
    if (!m_isEventMute)
    {
      // get the current volume
      ErrorCheck(m_masterChannel->getVolume(&m_currentEventVolume));

      ErrorCheck(m_masterChannel->setVolume(0.0f));

      m_isEventMute = true;
    }
  }

  void AudioManager::UnmuteAll(void)
  {
    if (m_isEventMute)
    {
      ErrorCheck(m_masterChannel->setVolume(m_currentEventVolume));
      m_isEventMute = false;
    }
  }
  void AudioManager::SetMasterVolume(float volume)
  {
    m_masterVolume = volume; // Clamp(volume, 0.0f, 1.0f);
    m_masterChannel->setVolume(m_masterVolume);
  }

  void AudioManager::SetMasterPitch(float pitch)
  {
    m_masterPitch = pitch;//Clamp(pitch, 0.0f, 1.0f);
    m_masterChannel->setPitch(m_masterPitch);
  }

  float AudioManager::GetMasterVolume(void)
  {
    float masterVolume;
    ErrorCheck(m_masterChannel->getVolume(&masterVolume));
    return masterVolume;
  }

  float AudioManager::GetMasterPitch(void)
  {
    float masterPitch;
    ErrorCheck(m_masterChannel->getPitch(&masterPitch));
    return masterPitch;
  }
  void AudioManager::SetAllMute(bool state)
  {
    m_isAllMute = state;
  }
  bool AudioManager::GetAllMute(void)
  {
    return m_isAllMute;
  }
  //////////////////////////////////////////////////////////////////////////
  // MUSIC BUS
  void AudioManager::StopMusic(void)
  {
    ErrorCheck(m_musicBus->stopAllEvents(FMOD_STUDIO_STOP_IMMEDIATE));
  }
  void AudioManager::PauseMusic(void)
  {
    ErrorCheck(m_musicBus->setPaused(true));
  }
  void AudioManager::ResumeMusic(void)
  {
    ErrorCheck(m_musicBus->setPaused(false));
  }
  bool AudioManager::IsMusicMute(void)
  {
    bool isMute;
    ErrorCheck(m_musicBus->getMute(&isMute));
    return isMute;
  }
  void AudioManager::MuteMusic(void)
  {
    ErrorCheck(m_musicBus->setMute(true));
  }
  void AudioManager::UnmuteMusic(void)
  {
    ErrorCheck(m_musicBus->setMute(false));
  }
  void AudioManager::SetMusicVolume(float volume)
  {
    ErrorCheck(m_musicBus->setFaderLevel(volume));
  }
  float AudioManager::GetMusicVolume(void)
  {
    float volume;
    ErrorCheck(m_musicBus->getFaderLevel(&volume));
    return volume;
  }
  //////////////////////////////////////////////////////////////////////////
  // SFX BUS
  void AudioManager::StopSFX(void)
  {
    ErrorCheck(m_sfxBus->stopAllEvents(FMOD_STUDIO_STOP_IMMEDIATE));
  }
  void AudioManager::PauseSFX(void)
  {
    ErrorCheck(m_sfxBus->setPaused(true));
  }
  void AudioManager::ResumeSFX(void)
  {
    ErrorCheck(m_sfxBus->setPaused(false));
  }
  bool AudioManager::IsMusicSFX(void)
  {
    bool isMute;
    ErrorCheck(m_sfxBus->getMute(&isMute));
    return isMute;
  }
  void AudioManager::MuteSFX(void)
  {
    ErrorCheck(m_sfxBus->setMute(true));
  }
  void AudioManager::UnmuteSFX(void)
  {
    ErrorCheck(m_sfxBus->setMute(false));
  }
  void AudioManager::SetSFXVolume(float volume)
  {
    ErrorCheck(m_sfxBus->setFaderLevel(volume));
  }
  float AudioManager::GetSFXVolume(void)
  {
    float volume;
    ErrorCheck(m_sfxBus->getFaderLevel(&volume));
    return volume;
  }
  //////////////////////////////////////////////////////////////////////////
  // MENU SFX BUS
  void AudioManager::StopMenuSFX(void)
  {
    ErrorCheck(m_menuSfxBus->stopAllEvents(FMOD_STUDIO_STOP_IMMEDIATE));
  }
  void AudioManager::PauseMenuSFX(void)
  {
    ErrorCheck(m_menuSfxBus->setPaused(true));
  }
  void AudioManager::ResumeMenuSFX(void)
  {
    ErrorCheck(m_menuSfxBus->setPaused(false));
  }
  bool AudioManager::IsMusicMenuSFX(void)
  {
    bool isMute;
    ErrorCheck(m_menuSfxBus->getMute(&isMute));
    return isMute;
  }
  void AudioManager::MuteMenuSFX(void)
  {
    ErrorCheck(m_menuSfxBus->setMute(true));
  }
  void AudioManager::UnmuteMenuSFX(void)
  {
    ErrorCheck(m_menuSfxBus->setMute(false));
  }
  void AudioManager::SetMenuSFXVolume(float volume)
  {
    ErrorCheck(m_menuSfxBus->setFaderLevel(volume));
  }
  float AudioManager::GetMenuSFXVolume(void)
  {
    float volume;
    ErrorCheck(m_menuSfxBus->getFaderLevel(&volume));
    return volume;
  }

  //////////////////////////////////////////////////////////////////////////
  void AudioManager::ErrorCheck(FMOD_RESULT result)
  {
    if (result != FMOD_OK)
    {
      printf("ERROR! IN SOUND SYSTEM");
    }
  }

  void AudioManager::ParseEvents(FMOD::Studio::System* system, std::ifstream &file, SoundMap &soundMap)
  {
    std::string strbuffer;

    // beginning of the file
    file.clear();
    file.seekg(0, file.beg);

    while (std::getline(file, strbuffer))
    {
      std::size_t pos = strbuffer.find("event:/");

      if (pos != std::string::npos)
      {
        std::size_t endPos = strbuffer.length() - pos;

        LoadEvent(system, strbuffer.substr(pos, endPos), soundMap);
      }
    }
  }

  void AudioManager::ParseBanks(FMOD::Studio::System* system, std::ifstream &file, BankVector &banksVec)
  {
    std::string strbuffer;

    file.clear();
    file.seekg(0, file.beg);

    while (std::getline(file, strbuffer))
    {
      std::size_t pos = strbuffer.find("bank:/");

      if (pos != std::string::npos)
      {
        std::size_t prefix = std::string("bank:/").length();
        std::size_t startPos = pos + prefix;
        std::size_t endPos = strbuffer.length() - startPos;

        LoadBank(system, strbuffer.substr(startPos, endPos).append(".bank"), banksVec);
      }
    }

    LoadBank(system, strbuffer = "Master Bank.strings.bank", banksVec);
  }

  void AudioManager::ParseFiles(FMOD::System *lowlevelSystem, SoundMap &soundMap, FMOD::ChannelGroup *musicChannel)
  {

  }

  void AudioManager::LoadEvent(FMOD::Studio::System* system, std::string eventName, SoundMap &soundMap)
  {
    // create a new event
    FmodEvent *newSound = new FmodEvent(system, eventName);

    // getting rid of the event:/
    std::size_t pos = eventName.find("/");
    std::size_t endPos = eventName.length() - pos;
    std::string fullname = eventName.substr((pos + 1), endPos);

    soundMap[fullname] = newSound;
    return;
  }

  void AudioManager::LoadBank(FMOD::Studio::System* system, std::string &bankName, BankVector &banksVec)
  {
    FMOD::Studio::Bank *newBank = NULL;
    std::string pathName = AudioUtils::SourcePath(bankName, AudioUtils::TYPE_AUDIO);

    //ErrorCheck(system->loadBankFile(pathName.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &newBank));
    FMOD_RESULT result = system->loadBankFile(pathName.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &newBank);
    ErrorCheck(newBank->loadSampleData());

    banksVec.push_back(newBank);
    return;
  }

  void AudioManager::LoadBuses(FMOD::Studio::System* system)
  {
    ErrorCheck(system->getBus("bus:/Music", &m_musicBus));
    ErrorCheck(system->getBus("bus:/SFX", &m_sfxBus));
    ErrorCheck(system->getBus("bus:/MenuSFX", &m_menuSfxBus));
  }

  void AudioManager::ReceiveMessage(Core::Message& msg)
  {
    SoundInstance menuClick;
    switch (msg.MessageId)
    {
    case Core::Message::UIMessage:
      Core::MessageUI& m = dynamic_cast<Core::MessageUI&>(msg);

      //Split the string into tokens=================================
      std::string buf; // Have a buffer string
      std::stringstream ss(m.msg); // Insert the string into a stream
      std::vector<std::string> args; // Create vector to hold our words
      while (ss >> buf)
        args.push_back(buf);

      if (args.size() > 1 && strcmp("Audio", args[0].c_str()) == 0)
      {

        //////////////////////////////////////////////////////////////////////////
        // Controls the Master Sounds

        // Skye.DispatchMsg("Audio PauseAll")
        if (strcmp("PauseAll", args[1].c_str()) == 0) 
        {
          PauseAll();
        }

        // Skye.DispatchMsg("Audio PauseAll")
        if (strcmp("ResumeAll", args[1].c_str()) == 0)
        {
          ResumeAll();
        }

        // Skye.DispatchMsg("Audio MuteAll")
        if (strcmp("MuteAll", args[1].c_str()) == 0)
        {
          MuteAll();
        }

        // Skye.DispatchMsg("Audio UnmuteAll")
        if (strcmp("UnmuteAll", args[1].c_str()) == 0)
        {
          UnmuteAll();
        }

        // Change volume for all sounds
        // Skye.DispatchMsg("Audio SetVolumeAll volNum")
        // [2]= Volume number (must be 0.0f ~ 1.0f)
        if (args.size() > 2 && strcmp("SetVolumeAll", args[1].c_str()) == 0)
        {
          float volNum = (float)atof(args[2].c_str());  // change this later
          //char* pEnd;
          //float volNum = (std::strtof(args[2].c_str(). &pEnd);

          SetMasterVolume(volNum);
        }

        //////////////////////////////////////////////////////////////////////////
        // Controls the Music Sounds

        // Skye.DispatchMsg("Audio PauseMusicAll")
        if (strcmp("PauseMusicAll", args[1].c_str()) == 0)
        {
          PauseMusic();
        }

        // Skye.DispatchMsg("Audio ResumeMusicAll")
        if (strcmp("ResumeMusicAll", args[1].c_str()) == 0)
        {
          ResumeMusic();
          ResumeAll();
        }

        // Skye.DispatchMsg("Audio MuteMusicAll")
        if (strcmp("MuteMusicAll", args[1].c_str()) == 0)
        {
          MuteMusic();
        }

        // Skye.DispatchMsg("Audio UnmuteMusicAll")
        if (strcmp("UnmuteMusicAll", args[1].c_str()) == 0)
        {
          UnmuteMusic();
        }

        // Change volume for all sounds
        // Skye.DispatchMsg("Audio SetVolumeMusicAll volNum")
        // [2]= Volume number (must be 0.0f ~ 1.0f)
        if (args.size() > 2 && strcmp("SetVolumeMusicAll", args[1].c_str()) == 0)
        {
          float volNum = (float)atof(args[2].c_str());  // change this later
          //char* pEnd;
          //float volNum = (std::strtof(args[2].c_str(). &pEnd);

          SetMusicVolume(volNum);
        }
        //////////////////////////////////////////////////////////////////////////

        // Play Menu Click
        // Skye.DispatchMsg("Audio PlayMenuSFX MenuClick")
        if (args.size() > 2 && strcmp("PlayMenuSFX", args[1].c_str()) == 0)
        {      
          PlayEvent(args[2], &menuClick);
        }

        //////////////////////////////////////////////////////////////////////////
        // Controls A Specific Sound

        // Play a specific sound
        // Skye.DispatchMsg("Audio PlaySound SoundName ID")
        // [2]= Sound Name   [3]= Unique ID (for instance)
        if (args.size() > 2 && strcmp("PlaySound", args[1].c_str()) == 0)
        {
          SoundInstance* javaInstance = new SoundInstance();
          PlayEvent(args[2], javaInstance);
          m_javaScriptSounds[atoi(args[3].c_str())] = javaInstance;
        }

        // Stop a specific sound
        // Skye.DispatchMsg("Audio StopSound ID")
        // [2]= Unique ID (for instance)
        if (args.size() > 2 && strcmp("StopSound", args[1].c_str()) == 0)
        {
          int uid = atoi(args[2].c_str());
          if (m_javaScriptSounds.count(uid) == 0)
            return;

          SoundInstance* javaInstance = m_javaScriptSounds[uid];
          StopEvent(javaInstance, false);
          delete javaInstance;
        }

        // Pause a specific sound
        // Skye.DispatchMsg("Audio PauseSound ID")
        // [2]= Unique ID (for instance)
        if (args.size() > 2 && strcmp("PauseSound", args[1].c_str()) == 0)
        {
          int uid = atoi(args[2].c_str());
          if (m_javaScriptSounds.count(uid) == 0)
            return;

          SoundInstance* javaInstance = m_javaScriptSounds[uid];
          PauseEvent(javaInstance);
          delete javaInstance;
        }

        // Resume a specific sound
        // Skye.DispatchMsg("Audio ResumeSound ID")
        // [2]= Unique ID (for instance)
        if (strcmp("ResumeSound", args[1].c_str()) == 0)
        {
          int uid = atoi(args[2].c_str());
          if (m_javaScriptSounds.count(uid) == 0)
            return;

          SoundInstance* javaInstance = m_javaScriptSounds[uid];
          ResumeEvent(javaInstance);
          delete javaInstance;
        }

        // Mute a specific sound
        // Skye.DispatchMsg("Audio MuteSound ID")
        // [2]= Unique ID (for instance)
        if (strcmp("MuteSound", args[1].c_str()) == 0)
        {
          int uid = atoi(args[2].c_str());
          if (m_javaScriptSounds.count(uid) == 0)
            return;

          SoundInstance* javaInstance = m_javaScriptSounds[uid];
          MuteEvent(javaInstance);
          delete javaInstance;
        }

        // Unmute a specific sound
        // Skye.DispatchMsg("Audio UnmuteSound ID")
        // [2]= Unique ID (for instance)
        if (strcmp("UnmuteSound", args[1].c_str()) == 0)
        {
          int uid = atoi(args[2].c_str());
          if (m_javaScriptSounds.count(uid) == 0)
            return;

          SoundInstance* javaInstance = m_javaScriptSounds[uid];
          UnmuteEvent(javaInstance);
          delete javaInstance;
        }

        // Change volume of a specific sound
        // Skye.DispatchMsg("Audio SetVolume ID volNum")
        // [2]= Unique ID (for instance)
        // [3]= Volume number (must be 0.0f ~ 1.0f)
        if (args.size() > 2 && strcmp("SetVolume", args[1].c_str()) == 0)
        {
          int uid = atoi(args[2].c_str());
          float volNum = (float)atof(args[3].c_str()); // change this later

          if (m_javaScriptSounds.count(uid) == 0)
            return;

          SoundInstance* javaInstance = m_javaScriptSounds[uid];
          SetVolume(volNum, javaInstance);
          delete javaInstance;      
        }       
      }

      break;
    }
  }


  //////////////////////////////////////////////////////////////////////////


  namespace AudioUtils
  {
    std::string SourcePath(const std::string filename, SourceType type)
    {
      switch (type)
      {
      case TYPE_AUDIO: return "content\\audio\\SoundBanks\\" + filename;
      case TYPE_GUID: return "content\\audio\\SoundBanks\\" + filename;
      }
      return "";
    }
  }

}