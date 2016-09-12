#include "C2DXSocialJni.h"
#include "SocialX.h"

#define  CLASS_NAME "es/Nanali/C2DXSocial/C2DXSocialBridge"

using namespace cocos2d;

extern "C"
{
    void openActionSheetWithMessageJNI(const char* message, const char* path)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "openActionSheetWithMessage", "(Ljava/lang/String;Ljava/lang/String;)V"))
        {
            return;
        }
        jstring stringArg1 = methodInfo.env->NewStringUTF(message);
        jstring stringArg2 = methodInfo.env->NewStringUTF(path);
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg1, stringArg2);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg1);
        methodInfo.env->DeleteLocalRef(stringArg2);
    }

    float getCurrentTimeSinceLastBootInSecJNI()
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getCurrentTimeSinceLastBootInSec", "()F"))
        {
            return false;
        }
        
        jfloat retV = methodInfo.env->CallStaticFloatMethod(methodInfo.classID, methodInfo.methodID, "");

        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        
        return retV;
    }

    void rateAppJNI()
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "rateApp", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    void openFacebookPageJNI()
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "openFacebookPage", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }

    void openHomepageJNI()
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "openHomepage", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }

    void openYoutubePageJNI()
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "openYoutubePage", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    void openArtworkPageJNI()
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "openArtworkPage", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    void showExitPopupJNI()
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "showExitPopup", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    std::string getDeviceUUIDJNI()
    {
        JniMethodInfo methodInfo;
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "getDeviceUUID", "()Ljava/lang/String;"))
        {
            return NULL;
        }
        jstring jStr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, "");
        
        //
        const char *buff = methodInfo.env->GetStringUTFChars(jStr, 0);
        std::string ret(buff);
        methodInfo.env->ReleaseStringUTFChars(jStr, buff);
        
        //
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(jStr);
        
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////

    JNIEXPORT void JNICALL Java_es_Nanali_C2DXSocial_C2DXSocialBridge_onCancelCallback(JNIEnv*  env, jobject thiz)
    {
        if (SocialXDelegate* delegate = SocialX::getInstance()->getDelegate()) {
            delegate->onCancelCallback();
        }
    }
}

