//
//  UnityAdsX.m
//  Timefish
//
//  Created by Midchow on 2016. 9. 13..
//
//

#include "UnityAdsX.h"

#import "UnityAds/UnityAds.h"

#define UnityAdsGameID  @"1220569"
#define integrationID   @"rewardedVideo"

@interface UnityAdsDelegateBridge : NSObject<UnityAdsDelegate>
@end

static UnityAdsDelegateBridge* s_pUADelegateBridge = nil;
static UnityAdsX* s_pUnityAdsX = NULL;

UnityAdsX* UnityAdsX::getInstance()
{
    if (s_pUnityAdsX == NULL)
    {
        s_pUnityAdsX = new UnityAdsX();
        s_pUADelegateBridge = [[UnityAdsDelegateBridge alloc] init];

        //
        [UnityAds initialize:UnityAdsGameID delegate:s_pUADelegateBridge];
    }
    return s_pUnityAdsX;
}

void UnityAdsX::startSession()
{
    // do nothing here
}

void UnityAdsX::showVideo()
{
    UIViewController* rootViewController;
    rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
    [UnityAds show:rootViewController placementId:integrationID];
}

bool UnityAdsX::isVideoAvailable()
{
    bool isAvailable = [UnityAds isReady:integrationID];
    CCLOG("isVideoAvailable: %d", isAvailable);
    return isAvailable;
}

#pragma mark -
#pragma mark UnityAdsDelegateBridge
@implementation UnityAdsDelegateBridge
- (void)unityAdsReady:(NSString *)placementId{
    CCLOG("unityAdsReady: %s", [placementId UTF8String]);
}

- (void)unityAdsDidError:(UnityAdsError)error withMessage:(NSString *)message
{
}

- (void)unityAdsDidStart:(NSString *)placementId
{
    if (UnityAdsXDelegate* delegate = UnityAdsX::getInstance()->getDelegate()) {
        delegate->onUnityAdsStart();
    }
}

- (void)unityAdsDidFinish:(NSString *)placementId withFinishState:(UnityAdsFinishState)state
{
    bool shown = false;
    if (state == kUnityAdsFinishStateCompleted) {
        shown = true;
    }
    
    //
    if (UnityAdsXDelegate* delegate = UnityAdsX::getInstance()->getDelegate()) {
        delegate->onUnityAdsFinish(shown);
    }
}
@end