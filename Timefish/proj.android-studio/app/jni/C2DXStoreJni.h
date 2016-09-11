#ifndef __C2DXSTORE_H__
#define __C2DXSTORE_H__

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

extern "C"
{
    extern void beginPurchaseFreeshSkinJNI(const char* skinName);
//    extern void restorePurchasesItemsJNI();
    extern void beginPurchasePackageJNI(const char* packageID);
    extern void beginPurchaseRemoveAdsJNI(const char* productName);
    extern void showRestoreAllPopupJNI(const char* title, const char* msg, const char* btn);
}

#endif // __C2DXSTORE_H__
