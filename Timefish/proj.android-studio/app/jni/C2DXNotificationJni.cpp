#include "C2DXNotificationJni.h"
#include "NotificationX.h"

//#define  CLASS_NAME "es/Nanali/C2DXSocial/C2DXSocialBridge"
#define  CLASS_NAME "org/cocos2dx/cpp/AppActivity"

using namespace cocos2d;

extern "C"
{
    void cancelAllNotificationsJNI()
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "cancelAllNotifications", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    void registerLocalNotificationJNI(const char* message, int seconds, int tag)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "registerLocalNotification", "(Ljava/lang/String;II)V"))
        {
            return;
        }
        jstring stringArg = methodInfo.env->NewStringUTF(message);

        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg, seconds, tag);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg);
    }
}
