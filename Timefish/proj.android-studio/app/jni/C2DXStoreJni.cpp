#include "C2DXStoreJni.h"
#include "StoreKitX.h"

#define  CLASS_NAME "org/cocos2dx/cpp/AppActivity"

using namespace cocos2d;

extern "C"
{
    void beginPurchaseFreeshSkinJNI(const char* skinName)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "beginPurchaseFreeshSkin", "(Ljava/lang/String;)V"))
        {
            return;
        }
        
        jstring stringArg = methodInfo.env->NewStringUTF(skinName);
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg);
    }
    
    void beginPurchasePackageJNI(const char* packageID)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "beginPurchasePackage", "(Ljava/lang/String;)V"))
        {
            return;
        }

        jstring stringArg = methodInfo.env->NewStringUTF(packageID);
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg);
    }

    void beginPurchaseRemoveAdsJNI(const char* productName)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "beginPurchaseRemoveAds", "(Ljava/lang/String;)V"))
        {
            return;
        }
        
        jstring stringArg = methodInfo.env->NewStringUTF(productName);

        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg);
    }

//    void restorePurchasesItemsJNI()
//    {
//        JniMethodInfo methodInfo;
//        
//        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "restorePurchasesItems", "()V"))
//        {
//            return;
//        }
//
//        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, "");
//        methodInfo.env->DeleteLocalRef(methodInfo.classID);
//    }

    void showRestoreAllPopupJNI(const char* title, const char* msg, const char* btn)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "showRestoreAllPopup", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
        {
            return;
        }

        jstring stringArg1 = methodInfo.env->NewStringUTF(title);
        jstring stringArg2 = methodInfo.env->NewStringUTF(msg);
        jstring stringArg3 = methodInfo.env->NewStringUTF(btn);

        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg1, stringArg2, stringArg3);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg1);
        methodInfo.env->DeleteLocalRef(stringArg2);
        methodInfo.env->DeleteLocalRef(stringArg3);
    }

    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////

    void Java_org_cocos2dx_cpp_AppActivity_onRequestProduct(JNIEnv*  env, jobject thiz, jstring productId, jstring price, jstring currency, jboolean purchased)
    {
        StoreKitX::getInstance()->onRequestProduct(JniHelper::jstring2string(productId).c_str(),
                                                   JniHelper::jstring2string(price).c_str(),
                                                   JniHelper::jstring2string(currency).c_str(),
                                                   purchased);
    }
    void Java_org_cocos2dx_cpp_AppActivity_onFinishPurchase(JNIEnv*  env, jobject thiz, jstring transactionID, jstring productID, jboolean restored, jboolean success)
    {
        StoreKitX::getInstance()->onFinishPurchase(JniHelper::jstring2string(transactionID).c_str(),
                                                   JniHelper::jstring2string(productID).c_str(),
                                                   restored,
                                                   success);
    }
    void Java_org_cocos2dx_cpp_AppActivity_onFinishRestore(JNIEnv*  env, jobject thiz, jboolean success)
    {
        if (StoreKitXDelegate* delegate = StoreKitX::getInstance()->getDelegate()) {
            delegate->finishRestore(success);
        }
    }
//    void Java_org_cocos2dx_cpp_AppActivity_onFinishRestoreAll(JNIEnv*  env, jobject thiz, jboolean success)
//    {
//        if (StoreKitXDelegate* delegate = StoreKitX::getInstance()->getDelegate()) {
//            delegate->finishRestoreAll(success);
//        }
//    }
}

