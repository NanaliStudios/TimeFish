#ifndef __C2DXTAPJOY_H__
#define __C2DXTAPJOY_H__

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

extern "C"
{
    extern bool isVideoReadyJNI();
    extern bool isPlacementReadyJNI(const char *placementName);
    extern void startTapjoySessionJNI();
    extern void endTapjoySessionJNI();
    extern void requestPlacementJNI(const char *placementName);
    extern void showPlacementJNI(const char *placementName);
    extern void setUserLevelJNI(int lvl);
    extern void logEventInEconomyJNI(int eventTypeIdx, int param);
    extern void logEventInUIFlowJNI(const char *eventName, int param = -1);
    extern void logEventInUIFlowResultJNI(const char *eventName, int resultTypeIdx);
    extern void logEventInUIFlowOptionJNI(int btnIdx);
    extern void logEventInResultFlowResultButtonJNI(int btnIdx, int skinNum);
    extern void logEventInIAPJNI(const char *eventName, int skinIdx = -1);
}

#endif // __C2DXTAPJOY_H__
