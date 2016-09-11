#include "C2DXAdColonyJni.h"
#include "AdColonyX.h"

#define  CLASS_NAME "es/Nanali/C2DXAdColony/C2DXAdColonyBridge"

using namespace cocos2d;

extern "C"
{
    void showVideoJNI()
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "showVideo", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }

    bool isVideoAvailableJNI()
    {
        JniMethodInfo methodInfo;

        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "isVideoAvailable", "()Z"))
        {
            return false;
        }

        jboolean retV = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, "");

        methodInfo.env->DeleteLocalRef(methodInfo.classID);

        return retV;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Java_es_Nanali_C2DXAdColony_C2DXAdColonyBridge_onAdColonyAdStartedInZone(JNIEnv*  env, jobject thiz, jstring zoneID)
    {
        if (AdColonyXDelegate* delegate = AdColonyX::getInstance()->getDelegate()) {
            delegate->onAdColonyAdStartedInZone(JniHelper::jstring2string(zoneID).c_str());
        }
    }
    void Java_es_Nanali_C2DXAdColony_C2DXAdColonyBridge_onAdColonyAdAttemptFinished(JNIEnv*  env, jobject thiz, jboolean shown, jstring zoneID)
    {
        if (AdColonyXDelegate* delegate = AdColonyX::getInstance()->getDelegate()) {
            delegate->onAdColonyAdAttemptFinished(shown, JniHelper::jstring2string(zoneID).c_str());
        }
    }
    void Java_es_Nanali_C2DXAdColony_C2DXAdColonyBridge_onAdColonyAdAvailabilityChange(JNIEnv*  env, jobject thiz, jboolean available, jstring zoneID)
    {
        if (AdColonyXDelegate* delegate = AdColonyX::getInstance()->getDelegate()) {
            delegate->onAdColonyAdAvailabilityChange(available, JniHelper::jstring2string(zoneID).c_str());
        }
    }
}
