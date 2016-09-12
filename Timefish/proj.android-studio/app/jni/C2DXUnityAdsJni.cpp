#include "C2DXUnityAdsJni.h"
#include "UnityAdsX.h"

#define  CLASS_NAME "es/Nanali/C2DXUnityAds/C2DXUnityAdsBridge"

using namespace cocos2d;

extern "C"
{
    void showUnityVideoJNI()
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "showVideo", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }

    bool isUnityVideoAvailableJNI()
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
    void Java_es_Nanali_C2DXUnityAds_C2DXUnityAdsBridge_onUnityAdsStart(JNIEnv*  env, jobject thiz)
    {
        if (UnityAdsXDelegate* delegate = UnityAdsX::getInstance()->getDelegate()) {
            delegate->onUnityAdsStart();
        }
    }
    void Java_es_Nanali_C2DXUnityAds_C2DXUnityAdsBridge_onUnityAdsFinish(JNIEnv*  env, jobject thiz, jboolean shown)
    {
        if (UnityAdsXDelegate* delegate = UnityAdsX::getInstance()->getDelegate()) {
            delegate->onUnityAdsFinish(shown);
        }
    }
}
