#ifndef __C2DXUNITYADS_H__
#define __C2DXUNITYADS_H__

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

extern "C"
{
    extern void showUnityVideoJNI();
    extern bool isUnityVideoAvailableJNI();
}

#endif // __C2DXUNITYADS_H__
