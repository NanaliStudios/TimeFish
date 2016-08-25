//
//  AdColonyX_ios.mm
//  Timefish
//
//  Created by midchow on 2015. 1. 23..
//
//

#import <AdColony/AdColony.h>
#include "AdColonyX.h"

#define ADCOLONY_APP_ID @"app0cf6ad3949624f219d"
#define ZONE_ID @"vzfded579cb1774a55bf"

// AdColonyDelegateBridge
@interface AdColonyDelegateBridge : NSObject<AdColonyDelegate,AdColonyAdDelegate>
@end
static AdColonyDelegateBridge* s_pADCDelegateBridge = nil;
static AdColonyX* s_pAdColonyX = NULL;

AdColonyX* AdColonyX::getInstance()
{
    if (s_pAdColonyX == NULL)
    {
        s_pAdColonyX = new AdColonyX();
        s_pADCDelegateBridge = [[AdColonyDelegateBridge alloc] init];
        
        //
        [AdColony configureWithAppID:ADCOLONY_APP_ID zoneIDs:@[ZONE_ID] delegate:s_pADCDelegateBridge logging:NO];
    }
    return s_pAdColonyX;
}

void AdColonyX::startSession()
{
    // do nothing here
}

void AdColonyX::showVideo()
{
    [AdColony playVideoAdForZone:ZONE_ID withDelegate:s_pADCDelegateBridge];
}

bool AdColonyX::isVideoAvailable()
{
    ADCOLONY_ZONE_STATUS status = [AdColony zoneStatusForZone:ZONE_ID];

    //
    // return true, only when zone is active!!!
    //
    return status == ADCOLONY_ZONE_STATUS_ACTIVE;
}

#pragma mark -
#pragma mark AdColonyAdDelegate
@implementation AdColonyDelegateBridge

- ( void ) onAdColonyAdStartedInZone:( NSString * )zoneID
{
    if (AdColonyXDelegate* delegate = AdColonyX::getInstance()->getDelegate()) {
        delegate->onAdColonyAdStartedInZone([zoneID UTF8String]);
    }
}

- ( void ) onAdColonyAdAttemptFinished:(BOOL)shown inZone:( NSString * )zoneID
{
    if (AdColonyXDelegate* delegate = AdColonyX::getInstance()->getDelegate()) {
        delegate->onAdColonyAdAttemptFinished(shown, [zoneID UTF8String]);
    }
}

- ( void ) onAdColonyAdAvailabilityChange:(BOOL)available inZone:(NSString*) zoneID
{
    if (AdColonyXDelegate* delegate = AdColonyX::getInstance()->getDelegate()) {
        delegate->onAdColonyAdAvailabilityChange(available, [zoneID UTF8String]);
    }
}


@end