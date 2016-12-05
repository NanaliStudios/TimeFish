#ifndef __C2DXFIREBASE_H__
#define __C2DXFIREBASE_H__

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

extern "C"
{
    //
    extern void initBannerJNI();
    extern void initInterstitialJNI();
    extern void showBannerJNI();
    extern void hideBannerJNI();
    extern void checkInterstitialReadyJNI();
    extern void showInterstitialFBJNI();
}

#endif // __C2DXFIREBASE_H__
