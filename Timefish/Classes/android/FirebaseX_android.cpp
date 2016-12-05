//
//  FirebaseX_ios.mm
//  Timefish
//
//  Created by Midchow on 2016. 12. 5..
//
//

#include "FirebaseX.h"
#include "C2DXFirebaseJni.h"

static FirebaseX* s_pFirebaseX = NULL;

FirebaseX* FirebaseX::getInstance()
{
    if (s_pFirebaseX == NULL)
    {
        s_pFirebaseX = new FirebaseX();
    }
    return s_pFirebaseX;
}

void FirebaseX::startSession()
{
    if (isPossibleOSVersion()) {
        initBannerJNI();
        initInterstitialJNI();
    }
}

void FirebaseX::showBanner()
{
    if (isPossibleOSVersion()) {
        showBannerJNI();
    }
}

void FirebaseX::hideBanner()
{
    if (isPossibleOSVersion()) {
        hideBannerJNI();
    }
}

bool FirebaseX::isPossibleOSVersion()
{
    return true;
}

void FirebaseX::checkInterstitialReady()
{
    if (isPossibleOSVersion()) {
        checkInterstitialReadyJNI();
    }
}

void FirebaseX::showInterstitial()
{
    if (isPossibleOSVersion()) {
        showInterstitialFBJNI();
    }
}

