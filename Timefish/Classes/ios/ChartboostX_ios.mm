//
//  ChartboostX_ios.mm
//  Timefish
//
//  Created by midchow on 2015. 1. 21..
//
//

#import <Chartboost/Chartboost.h>
#include "ChartboostX.h"

#define CHARTBOOST_APP_ID @"54bf35ef0d60251109ddd17a"
#define CHARTBOOST_APP_SIGNATURE @"596ba2735c7111922948fd1f127962e494b7d564"

// ChartboostDelegateBridge
@interface ChartboostDelegateBridge : NSObject<ChartboostDelegate>
@end

static ChartboostDelegateBridge* s_pCBDelegateBridge = nil;
static ChartboostX* s_pChartboostX = NULL;

ChartboostX* ChartboostX::getInstance()
{
    if (s_pChartboostX == NULL)
    {
        s_pChartboostX = new ChartboostX();
        s_pCBDelegateBridge = [[ChartboostDelegateBridge alloc] init];

        //
        // Begin a user session. Must not be dependent on user actions or any prior network requests.
        //
        [Chartboost startWithAppId:CHARTBOOST_APP_ID appSignature:CHARTBOOST_APP_SIGNATURE delegate:s_pCBDelegateBridge];
        [Chartboost setShouldRequestInterstitialsInFirstSession:YES];

    }
    return s_pChartboostX;
}

void ChartboostX::startSession()
{
    // do nothing here
}

void ChartboostX::purgeChartboostX()
{
    CC_SAFE_DELETE(s_pChartboostX);
    [s_pCBDelegateBridge dealloc];
}

void ChartboostX::cacheInterstitial(const char* location)
{
    CBLocation _loc = [NSString stringWithFormat:@"%s", location];
    [Chartboost cacheInterstitial:_loc];
}

void ChartboostX::showInterstitial(const char* location)
{
    CBLocation _loc = [NSString stringWithFormat:@"%s", location];
    [Chartboost showInterstitial:_loc];
}

bool ChartboostX::hasCachedInterstitial(const char* location)
{
    CBLocation _loc = [NSString stringWithFormat:@"%s", location];
    return [Chartboost hasInterstitial:_loc];
}

@implementation ChartboostDelegateBridge
- (BOOL)shouldRequestInterstitial:(CBLocation)location
{
    if (ChartboostXDelegate* delegate = ChartboostX::getInstance()->getDelegate()) {
        return delegate->shouldRequestInterstitial([location UTF8String]);
    }

    return YES;
}

- (BOOL)shouldDisplayInterstitial:(CBLocation)location
{
    if (ChartboostXDelegate* delegate = ChartboostX::getInstance()->getDelegate()) {
        return delegate->shouldDisplayInterstitial([location UTF8String]);
    }
    
    return YES;
}

- (void)didCacheInterstitial:(CBLocation)location
{
    if (ChartboostXDelegate* delegate = ChartboostX::getInstance()->getDelegate()) {
        delegate->didCacheInterstitial([location UTF8String]);
    }
}

- (void)didFailToLoadInterstitial:(CBLocation)location
                        withError:(CBLoadError)error;
{
    if (ChartboostXDelegate* delegate = ChartboostX::getInstance()->getDelegate()) {
        delegate->didFailToLoadInterstitial([location UTF8String], (int)error);
    }
}

- (void)didDismissInterstitial:(CBLocation)location
{
    if (ChartboostXDelegate* delegate = ChartboostX::getInstance()->getDelegate()) {
        delegate->didDismissInterstitial([location UTF8String]);
    }
}

- (void)didCloseInterstitial:(CBLocation)location
{
    if (ChartboostXDelegate* delegate = ChartboostX::getInstance()->getDelegate()) {
        delegate->didCloseInterstitial([location UTF8String]);
    }
}

- (void)didClickInterstitial:(CBLocation)location
{
    if (ChartboostXDelegate* delegate = ChartboostX::getInstance()->getDelegate()) {
        delegate->didClickInterstitial([location UTF8String]);
    }
}

@end
