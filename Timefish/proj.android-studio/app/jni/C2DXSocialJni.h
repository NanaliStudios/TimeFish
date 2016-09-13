#ifndef __C2DXSOCIAL_H__
#define __C2DXSOCIAL_H__

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

extern "C"
{
    extern void openActionSheetWithMessageJNI(const char* message, const char* path);
    extern float getCurrentTimeSinceLastBootInSecJNI();
    extern void rateAppJNI();
    extern void openFacebookPageJNI();
    extern void openHomepageJNI();
    extern void openYoutubePageJNI();
    extern void openArtworkPageJNI();
    extern void showExitPopupJNI();
    extern std::string getDeviceUUIDJNI();
    extern void submitScoreJNI(int score);
}

#endif // __C2DXSOCIAL_H__
