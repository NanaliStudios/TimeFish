//
//  UnityAdsX_android.cpp
//  Timefish
//
//  Created by midchow on 2015. 8. 19..
//
//

#include "UnityAdsX.h"
#include "C2DXUnityAdsJni.h"

static UnityAdsX* s_pUnityAdsX = nullptr;

UnityAdsX* UnityAdsX::getInstance()
{
    if (s_pUnityAdsX == nullptr)
    {
        s_pUnityAdsX = new UnityAdsX();
    }
    return s_pUnityAdsX;
}

void UnityAdsX::startSession()
{
    // do nothing here
}

void UnityAdsX::showVideo()
{
    showUnityVideoJNI();
}

bool UnityAdsX::isVideoAvailable()
{
    return isUnityVideoAvailableJNI();
}
