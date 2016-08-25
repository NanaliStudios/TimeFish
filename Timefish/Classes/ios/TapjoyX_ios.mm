//
//  TapjoyX_ios.m
//  Timefish
//
//  Created by midchow on 2015. 8. 5..
//
//

#import <Tapjoy/Tapjoy.h>
#import <Tapjoy/TJPlacement.h>

#include "TapjoyX.h"

@interface TapjoyDelegateBridge: UIResponder <UIApplicationDelegate, TJPlacementDelegate>
- (void) startTapjoy;
@end

static TapjoyDelegateBridge* s_tjDelegateBridge = nil;
static TapjoyX* s_pTapjoyX = NULL;

TapjoyX* TapjoyX::getInstance()
{
    if (s_pTapjoyX == NULL)
    {
        s_pTapjoyX = new TapjoyX();
        s_tjDelegateBridge = [[TapjoyDelegateBridge alloc] init];
    }
    return s_pTapjoyX;
}

void TapjoyX::startSession()
{
    [Tapjoy startSession];
}

void TapjoyX::endSession()
{
    [Tapjoy endSession];
}

void TapjoyX::setUserLevel(int lvl)
{
    [Tapjoy setUserLevel:lvl];
}

void TapjoyX::requestPlacement(const char *placementName)
{
    TJPlacement *p = [TJPlacement placementWithName:[NSString stringWithUTF8String:placementName] delegate:s_tjDelegateBridge];
    [p requestContent];
}

void TapjoyX::showPlacement(const char *placementName)
{
    TJPlacement *p = [TJPlacement placementWithName:[NSString stringWithUTF8String:placementName] delegate:s_tjDelegateBridge];
    if(p.isContentReady) {
        UIViewController *rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
        [p showContentWithViewController: rootViewController];
    }
    else {
        //handle situation where there is no content to show, or it has not yet downloaded.
    }
}

bool TapjoyX::isPlacementReady(const char *placementName)
{
    TJPlacement *p = [TJPlacement placementWithName:[NSString stringWithUTF8String:placementName] delegate:s_tjDelegateBridge];
    return p.isContentReady;
}

bool TapjoyX::isVideoReady()
{
    TJPlacement *p = [TJPlacement placementWithName:@"VideoRewardPlacement" delegate:s_tjDelegateBridge];
    return p.isContentReady;
}

void TapjoyX::logEventInEconomy(int eventTypeIdx, int param)
{
    const char *eventName[] = {
        "VideoReward",
        "FreeGift",
        "EarnInGame",
        "TrialReward",
        "RemoveAdsReward",
        "UseGold",
        "PackageReward",
        "GoldenFish",
    };
    [Tapjoy trackEvent:[NSString stringWithFormat:@"%s", eventName[eventTypeIdx]]
              category:[NSString stringWithFormat:@"Economy"]
            parameter1:[NSString stringWithFormat:@"Tier%d", param + 1]
            parameter2:nil];
}

void TapjoyX::logEventInUIFlow(const char *eventName, int param)
{
    NSString *parameter = nil;
    if (param > -1) {
        parameter = [NSString stringWithFormat:@"Tier%d", param];
    }
    [Tapjoy trackEvent:[NSString stringWithFormat:@"%s", eventName]
              category:[NSString stringWithFormat:@"UIFlow"]
            parameter1:parameter
            parameter2:nil];
}
void TapjoyX::logEventInUIFlowResult(const char *eventName, int resultTypeIdx)
{
    const char *resultTypes[] = {
        "ResultTypeGift1",
        "ResultTypeGift2",
        "ResultTypeDraw1",
        "ResultTypeDraw2",
        "ResultTypeVideo1",
        "ResultTypeVideo2",
        "ResultTypeBuy1",
        "ResultTypeBuy2",
        "ResultTypeRemoveAD1",
        "ResultTypeRemoveAD2",
        "ResultTypeTrial",
        "ResultTypeRate",
        "ResultTypeFacebook",
        "ResultTypeShare",
        "ResultTypeArtwork",
        "ResultTypeBuyPackage",
        "ResultTypeGoldenFish",
    };
    [Tapjoy trackEvent:[NSString stringWithFormat:@"%s", eventName]
              category:[NSString stringWithFormat:@"UIFlow"]
            parameter1:[NSString stringWithFormat:@"%s", resultTypes[resultTypeIdx]]
            parameter2:nil];
}

void TapjoyX::logEventInUIFlowOption(int btnIdx)
{
    if (btnIdx > 5) {
        return;
    }

    const char *eventNames[] = {
        "Sound",
        "Notification",
        "Restore",
        "Tutorial",
        "Facebook",
        "Credit",
    };
    [Tapjoy trackEvent:[NSString stringWithFormat:@"%s", eventNames[btnIdx]]
              category:[NSString stringWithFormat:@"UIFlow"]
            parameter1:nil
            parameter2:nil];
}
void TapjoyX::logEventInResultFlowResultButton(int btnIdx, int skinNum)
{
    const char *eventNames[] = {
        "GiftButton",
        "DrawButton",
        "VideoButton",
        "BuySkinButton",
        "RemoveAdsButton",
        "TrialButton",
        "RateButton",
        "LikeButton",
        "VideoShareButton",
        "ArtButton",
        "PackageButton"
    };
    NSString *parameter = nil;
    if (skinNum > -1) {
        parameter = [NSString stringWithFormat:@"%d", skinNum];
    }
    [Tapjoy trackEvent:[NSString stringWithFormat:@"%s", eventNames[btnIdx]]
              category:[NSString stringWithFormat:@"ResultFlow"]
            parameter1:parameter
            parameter2:nil];
}

void TapjoyX::logEventInIAP(const char *eventName, int skinIdx)
{
    NSString *parameter = nil;
    if (skinIdx > -1) {
        parameter = [NSString stringWithFormat:@"%d", skinIdx + 1];
    }
    [Tapjoy trackEvent:[NSString stringWithFormat:@"%s", eventName]
              category:[NSString stringWithFormat:@"IAP"]
            parameter1:parameter
            parameter2:nil];
}

//void TapjoyX::placeMentSetting()
//{
//    TJPlacement *p = [TJPlacement placementWithName:@"AppOpen" delegate:self ];
//}

@implementation TapjoyDelegateBridge

- (void) startTapjoy
{
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(tjcConnectSuccess:)
                                                 name:TJC_CONNECT_SUCCESS
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(tjcConnectFail:)
                                                 name:TJC_CONNECT_FAILED
                                               object:nil];
}

-(void)tjcConnectSuccess:(NSNotification*)notifyObj
{
    NSLog(@"Tapjoy connect Succeeded");
}
-(void)tjcConnectFail:(NSNotification*)notifyObj
{
    NSLog(@"Tapjoy connect Failed");
}




/**
 * Callback issued by TJ to publisher to state that placement request is successful
 * @param TJPlacement that was successful
 * @return n/a
 */
- (void)requestDidSucceed:(TJPlacement*)placement
{
    if (TapjoyXDelegate* delegate = TapjoyX::getInstance()->getDelegate()) {
        delegate->requestDidSucceed();
    }
}

/**
 * Called when an error occurs while sending the placement
 * @param placement The TJPlacement that was sent
 * @error error code
 * @return n/a
 */
- (void)requestDidFail:(TJPlacement*)placement error:(NSError*)error
{
    if (TapjoyXDelegate* delegate = TapjoyX::getInstance()->getDelegate()) {
        delegate->requestDidFail();
    }
}

/**
 * Called when content for an placement is successfully cached
 * @param placement The TJPlacement that was sent
 */
- (void)contentIsReady:(TJPlacement*)placement
{
    if (TapjoyXDelegate* delegate = TapjoyX::getInstance()->getDelegate()) {
        delegate->contentIsReady([placement.placementName UTF8String]);
    }
}

/**
 * Called when placement content did appear
 * @param placement The TJPlacement that was sent
 * @return n/a
 */
- (void)contentDidAppear:(TJPlacement*)placement
{
    if (TapjoyXDelegate* delegate = TapjoyX::getInstance()->getDelegate()) {
        delegate->contentDidAppear();
    }
}

/**
 * Called when placement content did disappear
 * @param placement The TJPlacement that was sent
 * @return n/a
 */
- (void)contentDidDisappear:(TJPlacement*)placement
{
    if (TapjoyXDelegate* delegate = TapjoyX::getInstance()->getDelegate()) {
        delegate->contentDidDisappear([placement.placementName UTF8String]);
    }
}

@end

