//
//  UnityAdsX.m
//  Timefish
//
//  Created by Midchow on 2016. 9. 13..
//
//

#include "UnityAdsX.h"

static UnityAdsX* s_pUnityAdsX = NULL;

UnityAdsX* UnityAdsX::getInstance()
{
    if (s_pUnityAdsX == NULL)
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
    // do nothing here
}

bool UnityAdsX::isVideoAvailable()
{
    // not available
    return false;
}