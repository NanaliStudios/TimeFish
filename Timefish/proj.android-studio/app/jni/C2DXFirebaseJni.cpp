#include "C2DXFirebaseJni.h"
#include "FirebaseX.h"

#define  CLASS_NAME "es/Nanali/C2DXFirebase/C2DXFirebaseBridge"

using namespace cocos2d;

extern "C"
{
    void initBannerJNI()
    {
        JniMethodInfo methodInfo;
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "initBanner", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    void initInterstitialJNI()
    {
        JniMethodInfo methodInfo;
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "initInterstitial", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    void showBannerJNI()
    {
        JniMethodInfo methodInfo;
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "showBanner", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    void hideBannerJNI()
    {
        JniMethodInfo methodInfo;
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "hideBanner", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    void checkInterstitialReadyJNI()
    {
        JniMethodInfo methodInfo;
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "checkInterstitialReady", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    void showInterstitialFBJNI()
    {
        JniMethodInfo methodInfo;
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "showInterstitial", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////
    void Java_es_Nanali_C2DXFirebase_C2DXFirebaseBridge_admobInterstitialReady(JNIEnv*  env, jobject thiz, jboolean success)
    {
        if (FirebaseXDelegate* delegate = FirebaseX::getInstance()->getDelegate()) {
            delegate->admobInterstitialReady(success);
        }
    }
    void Java_es_Nanali_C2DXFirebase_C2DXFirebaseBridge_admobInterstitialClosed(JNIEnv*  env, jobject thiz)
    {
        if (FirebaseXDelegate* delegate = FirebaseX::getInstance()->getDelegate()) {
            delegate->admobInterstitialClosed();
        }
    }
    void Java_es_Nanali_C2DXFirebase_C2DXFirebaseBridge_onAdmobInterstitialFailed(JNIEnv*  env, jobject thiz)
    {
        if (FirebaseXDelegate* delegate = FirebaseX::getInstance()->getDelegate()) {
            delegate->interstitialDidFailToPresentScreen();
        }
    }
    void Java_es_Nanali_C2DXFirebase_C2DXFirebaseBridge_onAdmobInterstitialLeft(JNIEnv*  env, jobject thiz)
    {
        if (FirebaseXDelegate* delegate = FirebaseX::getInstance()->getDelegate()) {
            delegate->interstitialWillLeaveApplication();
        }
    }
}

