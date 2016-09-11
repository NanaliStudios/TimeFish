#include "C2DXFlurryJni.h"
#include "FlurryX.h"

#define  CLASS_NAME "es/Nanali/C2DXFlurry/C2DXFlurryBridge"

using namespace cocos2d;

extern "C"
{
    JNIEXPORT void JNICALL Java_es_Nanali_C2DXFlurry_C2DXFlurryBridge_onCancelCallback(JNIEnv*  env, jobject thiz);

    void startSessionJNI()
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "startSession", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    void logEventJNI(const char* eventName)
    {
        JniMethodInfo methodInfo;

		if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "logEvent", "(Ljava/lang/String;)V"))
		{
			return;
		}
		jstring stringArg = methodInfo.env->NewStringUTF(eventName);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);

		methodInfo.env->DeleteLocalRef(methodInfo.classID);
		methodInfo.env->DeleteLocalRef(stringArg);
    }
    
    void logBackgroundEventWithIntJNI(const char* eventName, int playCount)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "logBackgroundEvent", "(Ljava/lang/String;I)V"))
        {
            return;
        }
        jstring stringArg = methodInfo.env->NewStringUTF(eventName);
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg, playCount);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg);
    }
    
    void logPosterSelectionJNI(const char* eventName, int posterType)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "logPosterSelection", "(Ljava/lang/String;I)V"))
        {
            return;
        }
        jstring stringArg = methodInfo.env->NewStringUTF(eventName);
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg, posterType);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg);
    }
    
    void logResultEventJNI(int score, int bestScore, int playCount, int totalCount)
    {
        JniMethodInfo methodInfo;

		if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "logResultEvent", "(IIII)V"))
		{
			return;
		}

        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, score, bestScore, playCount, totalCount);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

