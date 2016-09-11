#ifndef __C2DXFLURRY_H__
#define __C2DXFLURRY_H__

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

extern "C"
{
    extern void startSessionJNI();
    extern void logEventJNI(const char* eventName);
    extern void logBackgroundEventWithIntJNI(const char* eventName, int playCount);
    extern void logPosterSelectionJNI(const char* eventName, int posterType);
    extern void logResultEventJNI(int score, int bestScore, int playCount, int totalCount);
//    extern void rateAppJNI();
//    extern void openFacebookPageJNI();
//    extern void openHomepageJNI();
//    extern void openYoutubePageJNI();
//    extern void openPinterestJNI();
//    extern void showExitPopupJNI();
}

#endif // __C2DXFLURRY_H__
