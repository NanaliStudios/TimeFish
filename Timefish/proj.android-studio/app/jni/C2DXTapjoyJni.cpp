#include "C2DXTapjoyJni.h"
#include "TapjoyX.h"

#define  CLASS_NAME "es/Nanali/C2DXTapjoy/C2DXTapjoyBridge"

using namespace cocos2d;

extern "C"
{
    bool isVideoReadyJNI()
    {
        JniMethodInfo methodInfo;

        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "isVideoReady", "()Z"))
        {
            return false;
        }

        jboolean retV = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, "");

        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        
        return retV;
    }
    bool isPlacementReadyJNI(const char *placementName)
    {
        JniMethodInfo methodInfo;

        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "isPlacementReady", "(Ljava/lang/String;)Z"))
        {
            return false;
        }
        jstring stringArg = methodInfo.env->NewStringUTF(placementName);
        jboolean retV = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, stringArg);

        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg);

        return retV;
    }
    void startTapjoySessionJNI()
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "startTapjoySession", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    void endTapjoySessionJNI()
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "endTapjoySession", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    void requestPlacementJNI(const char *placementName)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "requestPlacement", "(Ljava/lang/String;)V"))
        {
            return;
        }
        jstring stringArg = methodInfo.env->NewStringUTF(placementName);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
        
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg);
    }
    void showPlacementJNI(const char *placementName)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "showPlacement", "(Ljava/lang/String;)V"))
        {
            return;
        }
        jstring stringArg = methodInfo.env->NewStringUTF(placementName);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
        
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg);
    }
    void setUserLevelJNI(int lvl)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "setUserLevel", "(I)V"))
        {
            return;
        }
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, lvl);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    void logEventInEconomyJNI(int eventTypeIdx, int param)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "logEventInEconomy", "(II)V"))
        {
            return;
        }
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, eventTypeIdx, param);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    void logEventInUIFlowJNI(const char *eventName, int param)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "logEventInUIFlow", "(Ljava/lang/String;I)V"))
        {
            return;
        }
        jstring stringArg = methodInfo.env->NewStringUTF(eventName);
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg, param);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg);
    }
    void logEventInUIFlowResultJNI(const char *eventName, int resultTypeIdx)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "logEventInUIFlowResult", "(Ljava/lang/String;I)V"))
        {
            return;
        }
        jstring stringArg = methodInfo.env->NewStringUTF(eventName);
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg, resultTypeIdx);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg);
    }
    void logEventInUIFlowOptionJNI(int btnIdx)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "logEventInUIFlowOption", "(I)V"))
        {
            return;
        }
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, btnIdx);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    void logEventInResultFlowResultButtonJNI(int btnIdx, int skinNum)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "logEventInResultFlowResultButton", "(II)V"))
        {
            return;
        }
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, btnIdx, skinNum);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    void logEventInIAPJNI(const char *eventName, int skinIdx)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "logEventInIAP", "(Ljava/lang/String;I)V"))
        {
            return;
        }
        jstring stringArg = methodInfo.env->NewStringUTF(eventName);
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg, skinIdx);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Java_es_Nanali_C2DXTapjoy_C2DXTapjoyBridge_onRequestSuccess(JNIEnv*  env, jobject thiz, jstring placement)
    {
        if (TapjoyXDelegate* delegate = TapjoyX::getInstance()->getDelegate()) {
            delegate->requestDidSucceed();
        }
    }
    void Java_es_Nanali_C2DXTapjoy_C2DXTapjoyBridge_onRequestFailure(JNIEnv*  env, jobject thiz, jstring placement)
    {
        if (TapjoyXDelegate* delegate = TapjoyX::getInstance()->getDelegate()) {
            delegate->requestDidFail();
        }
    }
    void Java_es_Nanali_C2DXTapjoy_C2DXTapjoyBridge_onContentReady(JNIEnv*  env, jobject thiz, jstring placement)
    {
        if (TapjoyXDelegate* delegate = TapjoyX::getInstance()->getDelegate()) {
            delegate->contentIsReady(JniHelper::jstring2string(placement).c_str());
        }
    }
    void Java_es_Nanali_C2DXTapjoy_C2DXTapjoyBridge_onContentShow(JNIEnv*  env, jobject thiz, jstring placement)
    {
        if (TapjoyXDelegate* delegate = TapjoyX::getInstance()->getDelegate()) {
            delegate->contentDidAppear();
        }
    }
    void Java_es_Nanali_C2DXTapjoy_C2DXTapjoyBridge_onContentDismiss(JNIEnv*  env, jobject thiz, jstring placement)
    {
        if (TapjoyXDelegate* delegate = TapjoyX::getInstance()->getDelegate()) {
            delegate->contentDidDisappear(JniHelper::jstring2string(placement).c_str());
        }
    }
}
