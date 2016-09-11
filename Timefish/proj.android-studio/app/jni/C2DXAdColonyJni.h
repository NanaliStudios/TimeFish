#ifndef __C2DXADCOLONY_H__
#define __C2DXADCOLONY_H__

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

extern "C"
{
    extern void showVideoJNI();
    extern bool isVideoAvailableJNI();
}

#endif // __C2DXADCOLONY_H__
