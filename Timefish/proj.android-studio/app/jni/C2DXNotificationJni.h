#ifndef __C2DXNOTIFICATION_H__
#define __C2DXNOTIFICATION_H__

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

extern "C"
{
    extern void cancelAllNotificationsJNI();
    extern void registerLocalNotificationJNI(const char* message, int seconds, int tag);
}

#endif // __C2DXNOTIFICATION_H__
