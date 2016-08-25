//
//  SoundManager.cpp
//  SpaceCleaner
//
//  Created by midchow on 2015. 1. 6..
//
//

#include "SoundManager.h"

USING_NS_CC;
using namespace cocos2d::experimental;

// singleton stuff
static SoundManager *_sharedSoundManager = nullptr;

SoundManager* SoundManager::getInstance()
{
    if (!_sharedSoundManager)
    {
        _sharedSoundManager = new (std::nothrow) SoundManager();
        _sharedSoundManager->init();
    }
    
    return _sharedSoundManager;
}

bool SoundManager::init(void)
{
    checkAudioInUse();

    backgroundMusicID = AudioEngine::INVALID_AUDIO_ID;
    ambienceSoundID = AudioEngine::INVALID_AUDIO_ID;
    muted = false;

    return true;
}

//
// checkAudioInUse is called
// when the App is launched or the App is entered to foreground.
//
void SoundManager::checkAudioInUse()
{
    audioInUse = SocialX::getInstance()->getAudioInUse();
}

void SoundManager::checkAudioInUseAndSetBackgroundMusic()
{
    checkAudioInUse();

    //
    if (audioInUse) {
        stopBackgroundMusic();
        stopAmbienceSound();
    }
    else {
        if (!muted && backgroundMusicID != AudioEngine::INVALID_AUDIO_ID) {
            setSoundVolume(backgroundMusicID, 1);
        }
        
        if (!muted && ambienceSoundID == AudioEngine::INVALID_AUDIO_ID) {
            startAmbienceSound();
        }
    }
}

void SoundManager::setMuteFromFile(bool flag)
{
    muted = flag;
    
    //
    UserInfo::getInstance()->setSoundOption(muted);
}

void SoundManager::setMute(bool flag)
{
    muted = flag;

    //
    UserInfo::getInstance()->setSoundOption(muted);
    
    if (muted) {
        stopAmbienceSound();
    }
    else {
        startAmbienceSound();
    }
}

void SoundManager::preloadSounds()
{
    for (int i=SoundGameStart; i<SoundEffectMax; i++) {
        playSoundEffect((SoundEffect)i, 0, false);
    }
}

void SoundManager::startAmbienceSound()
{
    if (!audioInUse) {
        if (ambienceSoundID == AudioEngine::INVALID_AUDIO_ID) {
            ambienceSoundID = playSoundEffect(SoundAmbience, 0.8, true);
        }
    }
}

void SoundManager::startBackgroundMusic(int skinIdx, float v)
{
    if (!audioInUse) {
        if (backgroundMusicID == AudioEngine::INVALID_AUDIO_ID) {
            backgroundMusicID = playBackgroundMusic(skinIdx);
        }
        else {
            setSoundVolume(backgroundMusicID, v);
        }
    }
}

void SoundManager::stopBackgroundMusic()
{
    if (backgroundMusicID != AudioEngine::INVALID_AUDIO_ID) {
        stopSoundEffect(backgroundMusicID);
        backgroundMusicID = AudioEngine::INVALID_AUDIO_ID;
    }
}

void SoundManager::stopAmbienceSound()
{
    if (ambienceSoundID != AudioEngine::INVALID_AUDIO_ID) {
        stopSoundEffect(ambienceSoundID);
        ambienceSoundID = AudioEngine::INVALID_AUDIO_ID;
    }
}

void SoundManager::pauseAmbienceSound()
{
    if (ambienceSoundID != AudioEngine::INVALID_AUDIO_ID) {
        pauseSoundEffect(ambienceSoundID);
    }
}

void SoundManager::stopAll()
{
    AudioEngine::stopAll();
}

int SoundManager::playSoundEffect(SoundEffect type, bool loop)
{
    SoundInfo sInfo = getSoundFileName(type);

    //
    // for sound modulation
    //
    float volume = rand_0_1()*0.1 + 0.9;

    volume = (muted)? 0:volume;

    int soundID = AudioEngine::play2d(sInfo.filePath, loop, volume);
    if (AudioEngine::getState(soundID) ==  AudioEngine::AudioState::ERROR) {
        soundID = -1;
    }

    return soundID;
}

int SoundManager::playBackgroundMusic(int skinIdx)
{
    //
    // for sound modulation
    //
    float volume = (muted)? 0:1;
    
    int soundID = AudioEngine::play2d(getBackgroundMusicFileName(skinIdx), true, volume);
    if (AudioEngine::getState(soundID) ==  AudioEngine::AudioState::ERROR) {
        soundID = -1;
    }
    
    return soundID;
}

int SoundManager::playFreeshJumpSoundEffect(int skinIdx, bool loop)
{
    //
    // for sound modulation
    //
    float volume = rand_0_1()*0.1 + 0.9;
    
    volume = (muted)? 0:volume;
    
    int soundID = AudioEngine::play2d(getFreeshJumpSoundFileName(skinIdx), loop, volume);
    if (AudioEngine::getState(soundID) ==  AudioEngine::AudioState::ERROR) {
        soundID = -1;
    }
    
    return soundID;
}

int SoundManager::playFreeshBoostSoundEffect(int skinIdx, bool loop)
{
    //
    // for sound modulation
    //
    float volume = rand_0_1()*0.1 + 0.9;
    
    volume = (muted)? 0:volume;
    
    int soundID = AudioEngine::play2d(getFreeshBoostSoundFileName(skinIdx), loop, volume);
    if (AudioEngine::getState(soundID) ==  AudioEngine::AudioState::ERROR) {
        soundID = -1;
    }
    
    return soundID;
}

int SoundManager::playFreeshVoiceSoundEffect(int skinIdx, bool loop)
{
    //
    // for sound modulation
    //
    float volume = rand_0_1()*0.1 + 0.9;
    
    volume = (muted)? 0:volume;

    int soundID = AudioEngine::play2d(getFreeshVoiceSoundFileName(skinIdx), loop, volume);
    if (AudioEngine::getState(soundID) ==  AudioEngine::AudioState::ERROR) {
        soundID = -1;
    }
    
    return soundID;
}

int SoundManager::playSoundEffectWithCallback(SoundEffect type, bool loop, const std::function<void(int, const std::string &)>& callback)
{
    SoundInfo sInfo = getSoundFileName(type);

    float volume = (muted)? 0:sInfo.volume;

    int soundID = AudioEngine::play2d(sInfo.filePath, loop, volume);
    AudioEngine::setFinishCallback(soundID, callback);

    return soundID;
}

int SoundManager::playSoundEffect(SoundEffect type, float volume, bool loop)
{
    volume = (muted)? 0:volume;

    SoundInfo sInfo = getSoundFileName(type);
    
    return AudioEngine::play2d(sInfo.filePath, loop, volume);
}

int SoundManager::playTimonSoundEffect(int skinIdx, float volume, bool loop)
{
    volume = (muted)? 0:volume;

    int idx = timonBGSoundIdx[skinIdx];
    if (idx == -1) {
        return AudioEngine::INVALID_AUDIO_ID;
    }

    return AudioEngine::play2d(getTimonSoundFileName(skinIdx).c_str(), loop, volume);
}

void SoundManager::stopSoundEffect(int effectId)
{
    AudioEngine::stop(effectId);
}

void SoundManager::stopAllSoundEffect()
{
    AudioEngine::stopAll();
    backgroundMusicID = AudioEngine::INVALID_AUDIO_ID;
    ambienceSoundID = AudioEngine::INVALID_AUDIO_ID;
}

void SoundManager::pauseSoundEffect(int effectId)
{
    AudioEngine::pause(effectId);
}

void SoundManager::pauseAllSoundEffect()
{
    AudioEngine::pauseAll();
}

void SoundManager::resumeSoundEffect(int effectId)
{
    AudioEngine::resume(effectId);
}

void SoundManager::resumeAllSoundEffect()
{
    if (!muted) {
        AudioEngine::resumeAll();
    }
}

void SoundManager::setSoundVolume(int effectId, float volume)
{
    if (volume < 0.0f || volume > 1.0f) {
        return;
    }

    volume = (muted)? 0:volume;

    AudioEngine::setVolume(effectId, volume);
}

//void SoundManager::setSoundPitch(int effectId, float pitch)
//{
//    AudioEngine::setPitch(effectId, pitch);
//}

