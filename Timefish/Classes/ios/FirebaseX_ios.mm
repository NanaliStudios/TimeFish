//
//  FirebaseX_ios.mm
//  Timefish
//
//  Created by Midchow on 2016. 12. 5..
//
//

#include "FirebaseX.h"
#import <GoogleMobileAds/GoogleMobileAds.h>

#define AdMobTest 0

//
#define BannerAdUnitID @"ca-app-pub-6269735295695961/4926074937"
#define InterstitialAdUnitID @"ca-app-pub-6269735295695961/3449341732"

//
// FirebaseDelegateBridge
//
@interface FirebaseDelegateBridge : NSObject <GADInterstitialDelegate>
{
    UIViewController* m_RootViewController;
    
    GADBannerView *bannerView;
    GADInterstitial *interstitial;
};
- (void) initBanner;
- (void) showBanner;
- (void) hideBanner;
- (void) initInterstitial;
- (BOOL) isReady;
- (void) showInterstitial;
@end

static FirebaseDelegateBridge* s_pFBDelegateBridge = nil;
static FirebaseX* s_pFirebaseX = NULL;

FirebaseX* FirebaseX::getInstance()
{
    if (s_pFirebaseX == NULL)
    {
        s_pFirebaseX = new FirebaseX();
        s_pFBDelegateBridge = [[FirebaseDelegateBridge alloc] init];
    }
    return s_pFirebaseX;
}

void FirebaseX::startSession()
{
    if (isPossibleOSVersion()) {
        [s_pFBDelegateBridge initBanner];
        [s_pFBDelegateBridge initInterstitial];
    }
}

void FirebaseX::showBanner()
{
    if (isPossibleOSVersion()) {
        [s_pFBDelegateBridge showBanner];
    }
}

void FirebaseX::hideBanner()
{
    if (isPossibleOSVersion()) {
        [s_pFBDelegateBridge hideBanner];
    }
}

bool FirebaseX::isPossibleOSVersion()
{
    return true;

    //
    // NOTE:iOS 7에서 Firebase AdMob에 문제가 있는듯 하여
    // iOS 7에서는 사용하지 않도록 iOS버전 체크해주는 함수
    //
    NSString *versionString = [[UIDevice currentDevice] systemVersion];
    
    // Use NSNumberFormatter instead of 'intValue' to detect invalid characters.
    NSNumberFormatter *formatter = [[NSNumberFormatter alloc] init];
    [formatter setNumberStyle:NSNumberFormatterDecimalStyle];
    
    NSArray *components = [versionString componentsSeparatedByString:@"."];
    
    NSNumber *number = [formatter numberFromString:[components objectAtIndex:0]];
    int major = [number intValue];
    
    CCLOG("FirebaseX::isPossibleOSVersion: %d", major);
    if (major < 8) {
        return false;
    }
    
    return true;
}

bool FirebaseX::isInterstitialReady()
{
    if (isPossibleOSVersion()) {
        return [s_pFBDelegateBridge isReady];
    }
    
    return false;
}

void FirebaseX::showInterstitial()
{
    if (isPossibleOSVersion()) {
        [s_pFBDelegateBridge showInterstitial];
    }
}

@implementation FirebaseDelegateBridge
- (void) initBanner
{
    m_RootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
    
    // Create a banner ad and add it to the view hierarchy.
    bannerView = [[GADBannerView alloc] initWithAdSize:kGADAdSizeSmartBannerPortrait];
    bannerView.hidden = YES;
    bannerView.adUnitID = BannerAdUnitID;
    
    //
    // NOTE: 스크린 하단에 붙이기 위해 origin을 다시 잡아준다.
    //
    CGPoint _origin = CGPointMake(0, m_RootViewController.view.frame.size.height - bannerView.frame.size.height);
    CGRect frame = bannerView.frame;
    frame.origin = _origin;
    [bannerView setFrame:frame];
    
    //
    bannerView.rootViewController = m_RootViewController;
    [m_RootViewController.view addSubview:bannerView];
}

- (void) showBanner
{
    bannerView.hidden = NO;
    
    GADRequest *request = [GADRequest request];
#if AdMobTest == 1
    request.testDevices = @[kGADSimulatorID];
#endif
    [bannerView loadRequest:request];
}

- (void) hideBanner
{
    bannerView.hidden = YES;
}

///////
- (void) initInterstitial
{
    interstitial = [self createAndLoadInterstitial];
}

- (BOOL) isReady
{
    return [interstitial isReady];
}

- (void) showInterstitial
{
    [interstitial presentFromRootViewController:m_RootViewController];
}

- (GADInterstitial *)createAndLoadInterstitial
{
    GADInterstitial *_interstitial = [[GADInterstitial alloc] initWithAdUnitID:InterstitialAdUnitID];
    _interstitial.delegate = self;
    
    GADRequest *request = [GADRequest request];
#if AdMobTest == 1
    request.testDevices = @[kGADSimulatorID];
#endif
    [_interstitial loadRequest:request];
    
    return _interstitial;
}

- (void)interstitialDidDismissScreen:(GADInterstitial *)ad
{
    interstitial = [self createAndLoadInterstitial];
    
    if (FirebaseXDelegate* delegate = FirebaseX::getInstance()->getDelegate()) {
        delegate->admobInterstitialClosed();
    }
}

- (void)interstitialDidFailToPresentScreen:(GADInterstitial *)ad
{
    if (FirebaseXDelegate* delegate = FirebaseX::getInstance()->getDelegate()) {
        delegate->interstitialDidFailToPresentScreen();
    }
}

- (void)interstitialWillLeaveApplication:(GADInterstitial *)ad
{
    if (FirebaseXDelegate* delegate = FirebaseX::getInstance()->getDelegate()) {
        delegate->interstitialWillLeaveApplication();
    }
}
@end
