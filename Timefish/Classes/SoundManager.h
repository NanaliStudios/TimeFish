#ifndef __SpaceCleaner__SoundManager__
#define __SpaceCleaner__SoundManager__

#include "cocos2d.h"

#include "audio/include/AudioEngine.h"
//#include "audio/include/SimpleAudioEngine.h"
#include "Global.h"

#include "SocialX.h"

#include "UserInfo.h"

class SoundManager
{
public:
    static SoundManager* getInstance();
    
    bool init();

    void checkAudioInUse();
    void checkAudioInUseAndSetBackgroundMusic();
    
    //
    void startAmbienceSound();

    //
    void setMuteFromFile(bool flag);
    void setMute(bool flag);
    bool getMuted() {
        return muted;
    }
    
    void preloadSounds();
    void startBackgroundMusic(int skinIdx, float v);
    int playBackgroundMusic(int skinIdx);
    void stopBackgroundMusic();
    void stopAmbienceSound();
    void pauseAmbienceSound();

    int playSoundEffect(SoundEffect type, bool loop);
    int playFreeshJumpSoundEffect(int skinIdx, bool loop);
    int playFreeshVoiceSoundEffect(int skinIdx, bool loop);
    int playFreeshBoostSoundEffect(int skinIdx, bool loop);

    int playTimonSoundEffect(int skinIdx, float volume, bool loop);
    int playSoundEffect(SoundEffect type, float volume, bool loop);

    int playSoundEffectWithCallback(SoundEffect type, bool loop, const std::function<void(int, const std::string &)>& callback);
    void stopSoundEffect(int effectId);
    void setSoundVolume(int effectId, float volume);
//    void setSoundPitch(int effectId, float pitch);
    void stopAllSoundEffect();
    void pauseAllSoundEffect();
    void pauseSoundEffect(int effectId);
    void resumeSoundEffect(int effectId);
    void resumeAllSoundEffect();
    void stopAll();

private:
    int backgroundMusicID;
    int ambienceSoundID;
    
    bool muted;

    //
    // NOTE:
    // if audio is used by other music app,
    // we're not gonna play background music.
    //
    bool audioInUse;
};


#endif /* defined(__SpaceCleaner__SoundManager__) */
