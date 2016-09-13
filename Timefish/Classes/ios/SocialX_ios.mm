//
//  SocialX_ios.mm
//  Timefish
//
//  Created by midchow on 2015. 1. 23..
//
//

#import <Social/Social.h>
#import <AVFoundation/AVAudioSession.h>

#include "SocialX.h"

#import <StoreKit/SKStoreProductViewController.h>

//#include <mach/mach.h>
//#include <mach/mach_time.h>

#define ReviewLink        @"https://itunes.apple.com/app/id960150767"
#define AppStoreLink        @"http://goo.gl/6EIst4"
#define FacebookLink        @"https://www.facebook.com/freeshtimon"
#define FacebookDeepLink    @"fb://profile/628031447322532"
#define HomepageLink        @"https://www.facebook.com/NanaliStudios"
#define HomepageDeepLink    @"fb://profile/271071806318884"
#define YoutubeLink         @"https://youtu.be/8z2Y0z0zXRU"
#define YoutubeDeepLink     @"vnd.youtube:8z2Y0z0zXRU"
#define BehanceLink       @"https://www.behance.net/gallery/28576369/Timefish"

#import <Foundation/Foundation.h>
#include <sys/types.h>
#include <sys/sysctl.h>

@interface UIDeviceHardware : NSObject
{
    
}
+ (NSString *) platform;
+ (NSString *) platformString;
@end

@implementation UIDeviceHardware


+ (NSString *) platform{
    
    size_t size;
    
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    
    char *machine = (char *)malloc(size);
    
    sysctlbyname("hw.machine", machine, &size, NULL, 0);
    
    NSString *platform = [NSString stringWithUTF8String:machine];
    
    free(machine);
    
    return platform;
    
}


+ (NSString *) platformString{
    
    NSString *platform = [self platform];
    
//    if ([hardware isEqualToString:@"iPhone1,1"])    return IPHONE_2G;
//    if ([hardware isEqualToString:@"iPhone1,2"])    return IPHONE_3G;
//    if ([hardware isEqualToString:@"iPhone2,1"])    return IPHONE_3GS;
//    if ([hardware isEqualToString:@"iPhone3,1"])    return IPHONE_4;
//    if ([hardware isEqualToString:@"iPhone3,2"])    return IPHONE_4;
//    if ([hardware isEqualToString:@"iPhone3,3"])    return IPHONE_4_CDMA;
//    //
//    //
//    //
//    if ([hardware isEqualToString:@"iPhone4,1"])    return IPHONE_4S;
//    if ([hardware isEqualToString:@"iPhone5,1"])    return IPHONE_5;
//    if ([hardware isEqualToString:@"iPhone5,2"])    return IPHONE_5_CDMA_GSM;
//    if ([hardware isEqualToString:@"iPhone5,3"])    return IPHONE_5C;
//    if ([hardware isEqualToString:@"iPhone5,4"])    return IPHONE_5C_GLOBAL;
//    if ([hardware isEqualToString:@"iPhone6,1"])    return IPHONE_5S;
//    if ([hardware isEqualToString:@"iPhone6,2"])    return IPHONE_5S_GLOBAL;
//    ////////////////////////////////////////////////////////////////////////
//    ////////////////////////////////////////////////////////////////////////
//    ////////////////////////////////////////////////////////////////////////
//    if ([hardware isEqualToString:@"iPod1,1"])      return IPOD_TOUCH_1G;
//    if ([hardware isEqualToString:@"iPod2,1"])      return IPOD_TOUCH_2G;
//    if ([hardware isEqualToString:@"iPod3,1"])      return IPOD_TOUCH_3G;
//    //
//    //
//    //
//    if ([hardware isEqualToString:@"iPod4,1"])      return IPOD_TOUCH_4G;
//    if ([hardware isEqualToString:@"iPod5,1"])      return IPOD_TOUCH_5G;
//    ////////////////////////////////////////////////////////////////////////
//    ////////////////////////////////////////////////////////////////////////
//    ////////////////////////////////////////////////////////////////////////
//    if ([hardware isEqualToString:@"iPad1,1"])      return IPAD;
//    if ([hardware isEqualToString:@"iPad1,2"])      return IPAD_3G;
//    if ([hardware isEqualToString:@"iPad2,1"])      return IPAD_2_WIFI;
//    if ([hardware isEqualToString:@"iPad2,2"])      return IPAD_2;
//    if ([hardware isEqualToString:@"iPad2,3"])      return IPAD_2_CDMA;
//    if ([hardware isEqualToString:@"iPad2,4"])      return IPAD_2;
//    //
//    //
//    //
//    if ([hardware isEqualToString:@"iPad2,5"])      return IPAD_MINI_WIFI;
//    if ([hardware isEqualToString:@"iPad2,6"])      return IPAD_MINI;
//    if ([hardware isEqualToString:@"iPad2,7"])      return IPAD_MINI_WIFI_CDMA;
//    if ([hardware isEqualToString:@"iPad3,1"])      return IPAD_3_WIFI;
//    if ([hardware isEqualToString:@"iPad3,2"])      return IPAD_3_WIFI_CDMA;
//    if ([hardware isEqualToString:@"iPad3,3"])      return IPAD_3;
//    if ([hardware isEqualToString:@"iPad3,4"])      return IPAD_4_WIFI;
//    if ([hardware isEqualToString:@"iPad3,5"])      return IPAD_4;
//    if ([hardware isEqualToString:@"iPad3,6"])      return IPAD_4_GSM_CDMA;
//    
//    if ([hardware isEqualToString:@"i386"])         return SIMULATOR;
//    if ([hardware isEqualToString:@"x86_64"])       return SIMULATOR;
    
    return platform;
    
}


@end

//////--------------------------------------------------------------------------------
//////--------------------------------------------------------------------------------
//////--------------------------------------------------------------------------------
//////--------------------------------------------------------------------------------
//////--------------------------------------------------------------------------------
@interface SKStoreProductViewController (StatusBarFixing)

@end

@implementation SKStoreProductViewController (StatusBarFixing)

-(BOOL) prefersStatusBarHidden { return YES; }

@end

//////--------------------------------------------------------------------------------
//////--------------------------------------------------------------------------------
//////--------------------------------------------------------------------------------
//////--------------------------------------------------------------------------------
//////--------------------------------------------------------------------------------
@interface SocialDelegateBridge : NSObject<SKStoreProductViewControllerDelegate>
{
    UIViewController *m_RootViewController;
    UIActivityIndicatorView *activityIndicator;
}
- (void)SetRootViewController:(UIViewController*)rootViewController;
- (void)presentAppStoreForID:(NSNumber *)appStoreID inView:(UIView *)view withDelegate:(id<SKStoreProductViewControllerDelegate>)delegate withURL:(NSURL *)appStoreURL;
- (void)productViewControllerDidFinish:(SKStoreProductViewController *)viewController;
//+ (NSString *) deviceModel;
@end

static SocialDelegateBridge* s_SDDelegateBridge = nil;
static SocialX* s_pSocialX = NULL;

SocialX* SocialX::getInstance()
{
    if (s_pSocialX == NULL)
    {
        s_pSocialX = new SocialX();

        s_SDDelegateBridge = [[SocialDelegateBridge alloc] init];
        UIViewController* rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
        [s_SDDelegateBridge SetRootViewController:rootViewController];
    }
    return s_pSocialX;
}

//void SocialX::openActionSheetWithScore(int score)
//{
//    openActionSheetWithScore(score, NULL);
//}

DevicePerformance SocialX::getDevicePerformance()
{
    NSString *hardware = [UIDeviceHardware platformString];
    
    if ([hardware isEqualToString:@"iPhone1,1"] ||
        [hardware isEqualToString:@"iPhone1,2"] ||
        [hardware isEqualToString:@"iPhone2,1"] ||
        [hardware isEqualToString:@"iPhone3,1"] ||
        [hardware isEqualToString:@"iPhone3,2"] ||
        [hardware isEqualToString:@"iPhone3,3"] ||
        [hardware isEqualToString:@"iPod1,1"] ||
        [hardware isEqualToString:@"iPod2,1"] ||
        [hardware isEqualToString:@"iPod3,1"] ||
        [hardware isEqualToString:@"iPad1,1"] ||
        [hardware isEqualToString:@"iPad1,2"] ||
        [hardware isEqualToString:@"iPad2,1"] ||
        [hardware isEqualToString:@"iPad2,2"] ||
        [hardware isEqualToString:@"iPad2,3"] ||
        [hardware isEqualToString:@"iPad2,4"] ||
        [hardware isEqualToString:@"i386"] ||
        [hardware isEqualToString:@"x86_64"]) {
        //
        // Low Performance:
        // NO VIDEO SHARING
        // NO BACKGROUND FLOWING
        //
        return NoRecording_NoFlowing;
    }
    else if ([hardware isEqualToString:@"iPad3,1"] ||
        [hardware isEqualToString:@"iPad3,2"] ||
        [hardware isEqualToString:@"iPad3,3"] ||
        [hardware isEqualToString:@"iPod4,1"] ||
        [hardware isEqualToString:@"iPod5,1"]) {
        //
        // Middle Performance:
        // VIDEO SHARING
        // NO BACKGROUND FLOWING
        //
        
        return WithRecording_NoFlowing;
    }
    else if ([hardware isEqualToString:@"iPhone4,1"] ||
        [hardware isEqualToString:@"iPhone5,1"] ||
        [hardware isEqualToString:@"iPhone5,2"] ||
        [hardware isEqualToString:@"iPhone5,3"] ||
        [hardware isEqualToString:@"iPhone5,4"] ||
        [hardware isEqualToString:@"iPhone6,1"] ||
        [hardware isEqualToString:@"iPhone6,2"] ||
        [hardware isEqualToString:@"iPad2,5"] ||
        [hardware isEqualToString:@"iPad2,6"] ||
        [hardware isEqualToString:@"iPad2,7"] ||
        [hardware isEqualToString:@"iPad3,4"] ||
        [hardware isEqualToString:@"iPad3,5"] ||
        [hardware isEqualToString:@"iPad3,6"]) {
        //
        // High Performace:
        // VIDEO SHARING
        // BACKGROUND FLOWING
        //
        return WithRecording_WithFlowing;
    }

    // default
    return WithRecording_WithFlowing;
}

//void SocialX::openActionSheetWithScore(int score, unsigned char *buffer, int leng)
//{
//    UIViewController* rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
//
//    UIImage *image = [UIImage imageWithData:[NSData dataWithBytes:buffer length:leng]];
////    UIImage *image=[UIImage imageNamed:@"Icon@2x.png"];
//    NSString *string = [NSString stringWithFormat:@"OMG! Freesh has escaped from Timon and scored %d. -> %@", score, AppStoreLink];
//    NSArray *postItems=@[string, image];
//    UIActivityViewController *activityViewController = [[UIActivityViewController alloc] initWithActivityItems:postItems
//                                                                                         applicationActivities:nil];
//
//    //if iPhone
//    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
//    {
//        [rootViewController presentViewController:activityViewController
//                                         animated:YES
//                                       completion:nil];
//    }
//    //if iPad
//    else
//    {
//        // Change Rect to position Popover
//        UIView *_view = rootViewController.view;
//        UIPopoverController *popup = [[UIPopoverController alloc] initWithContentViewController:activityViewController];
//        [popup presentPopoverFromRect:CGRectMake(_view.frame.size.width/2, _view.frame.size.height/4, 0, 0) inView:_view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
//    }
//}

void SocialX::openActionSheetWithMessage(const char* message, const char* path)
{
    //
//    NSString *string = nil;
//    if (score == -1) {
//        string = [NSString stringWithFormat:@"Check this cool wallpaper obtained from #timefish. -> %@", AppStoreLink];
//    }
//    else {
//        string = [NSString stringWithFormat:@"OMG! Freesh has escaped from Timon and scored %d. #timefish -> %@", score, AppStoreLink];
//    }
    NSString *string = [NSString stringWithFormat:@"%@ -> %@", [NSString stringWithUTF8String:message], AppStoreLink];

    UIImage *image=[[UIImage alloc] initWithContentsOfFile:[NSString stringWithFormat:@"%s", path]];

    NSArray *postItems=(image == nil)?@[string]:@[string, image];
    UIActivityViewController *activityViewController = [[UIActivityViewController alloc] initWithActivityItems:postItems
                                                                                         applicationActivities:nil];
    NSArray *excludeActivities = @[UIActivityTypeAssignToContact];
    activityViewController.excludedActivityTypes = excludeActivities;

    
    
    
    [(UIActivityViewController *)activityViewController setCompletionHandler:^(NSString *activityType, BOOL completed){
        NSLog(@"type: %@", activityType);
    }];

    
    
    
    UIViewController* rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;

    //if iPhone
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
    {
        [rootViewController presentViewController:activityViewController
                                         animated:YES
                                       completion:nil];
    }
    //if iPad
    else
    {
        // Change Rect to position Popover
        UIView *_view = rootViewController.view;
        UIPopoverController *popup = [[UIPopoverController alloc] initWithContentViewController:activityViewController];
        [popup presentPopoverFromRect:CGRectMake(_view.frame.size.width/2, _view.frame.size.height/4, 0, 0) inView:_view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
    }
}

//void SocialX::openActionSheetWithPoster(const char* message, const char* path)
//{
//    openActionSheetWithScore(message, path);
//}

void SocialX::rateApp()
{
    //
    // Note: It looks like user cannot write a review with the following methods:
    //
//    NSNumber *storeId = [NSNumber numberWithInt:960150767];
//    [s_SDDelegateBridge presentAppStoreForID:storeId inView:nil withDelegate:s_SDDelegateBridge withURL:[NSURL URLWithString:AppStoreLink]];

    //
    // Use this instead:
    //
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:ReviewLink]];
}

void SocialX::openFacebookPage()
{
    BOOL isInstalled = [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"fb://"]];
    if (isInstalled) {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:FacebookDeepLink]];
    } else {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:FacebookLink]];
    }
}

void SocialX::openHomepage()
{
    BOOL isInstalled = [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"fb://"]];
    if (isInstalled) {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:HomepageDeepLink]];
    } else {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:HomepageLink]];
    }
}

void SocialX::openArtworkPage()
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:BehanceLink]];
}

void SocialX::openYoutubePage()
{
    BOOL isInstalled = [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"vnd.youtube:"]];
    if (isInstalled) {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:YoutubeDeepLink]];
    } else {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:YoutubeLink]];
    }
}

float SocialX::getCurrentTimeSinceLastBootInSec()
{
    //
    // Note: This code snippet is from...
    //
    // http://bendodson.com/weblog/2013/01/29/ca-current-media-time/
    // http://www.opensource.apple.com/source/shell_cmds/shell_cmds-56/w/w.c
    //
    struct timeval boottime;
    int mib[2] = {CTL_KERN, KERN_BOOTTIME};
    size_t size = sizeof(boottime);
    time_t now;
    time_t uptime = -1;
    
    (void)time(&now);
    
    if (sysctl(mib, 2, &boottime, &size, NULL, 0) != -1 && boottime.tv_sec != 0)
    {
        uptime = now - boottime.tv_sec;
    }
    return uptime;

    //
    // Note: CACurrentMediaTime will be paused during sleep.
    //
//    return CACurrentMediaTime();
}

std::time_t SocialX::getCurrentTime()
{
//    NSTimeInterval intval = [[NSDate date] timeIntervalSince1970];
    std::time_t timeCheckIn = std::time(0);

    return timeCheckIn;
}

bool SocialX::getAudioInUse()
{
    NSError *error;
    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryAmbient error:&error];
    
    bool inUse = [[AVAudioSession sharedInstance] isOtherAudioPlaying];
    return inUse;
}

//void SocialX::tweetTapped()
//{
//    UIViewController* rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
//
//    if ([SLComposeViewController isAvailableForServiceType:SLServiceTypeTwitter])
//    {
//        SLComposeViewController *tweetSheet = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeTwitter];
//        [tweetSheet setInitialText:@"This is a tweet!"];
//
//        [rootViewController presentViewController:tweetSheet animated:YES completion:nil];
//        
//    }
//    else
//    {
//        UIAlertView *alertView = [[UIAlertView alloc]
//                                  initWithTitle:@"Sorry"
//                                  message:@"You can't send a tweet right now, make sure your device has an internet connection and you have at least one Twitter account setup"
//                                  delegate:rootViewController
//                                  cancelButtonTitle:@"OK"
//                                  otherButtonTitles:nil];
//        [alertView show];
//    }
//}
//
//void SocialX::facebookTapped()
//{
//    
//}

std::string SocialX::getDeviceUUID()
{
    //
    // NOTE: UUID는 현재 안드로이드만 지원하므로
    // iOS에서는 테스트용으로 하드 코딩된 아래 코드를 리턴함
    //
    return "fe2e3a4d-1748-41f1-b30c-92a35bdfcbd5";
}

void SocialX::submitScore()
{
    //
    // android 전용이므로, 여기서는 아무것도 하지 않음
    //
}

void SocialX::showAlert()
{
    NSString *msg = [NSString stringWithFormat:@"showAlert"];
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:nil message:msg delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alertView show];
    [alertView release];
}

//////--------------------------------------------------------------------------------
//////--------------------------------------------------------------------------------
//////--------------------------------------------------------------------------------
//////--------------------------------------------------------------------------------
//////--------------------------------------------------------------------------------
@implementation SocialDelegateBridge

- (void)SetRootViewController:(UIViewController*)rootViewController
{
    if( m_RootViewController != nil )
    {
        [m_RootViewController release];
        m_RootViewController = nil;
    }
    
    m_RootViewController = rootViewController;
    
    [m_RootViewController retain];
}

- (void)presentAppStoreForID:(NSNumber *)appStoreID inView:(UIView *)view withDelegate:(id<SKStoreProductViewControllerDelegate>)delegate withURL:(NSURL *)appStoreURL
{
    activityIndicator = [[UIActivityIndicatorView alloc] initWithFrame:CGRectMake(0, 0, 32, 32)];
    
    m_RootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
    
    [activityIndicator setCenter:m_RootViewController.view.center];
    [activityIndicator setActivityIndicatorViewStyle:UIActivityIndicatorViewStyleWhiteLarge];
    [m_RootViewController.view addSubview : activityIndicator];
    [activityIndicator startAnimating];
    
    //    NSLog(@"%@", [[UIDevice currentDevice] model]);
    //    NSLog(@"%@", [[UIDevice currentDevice] name]);
    //    NSLog(@"%@", [[UIDevice currentDevice] systemName]);
    //    NSLog(@"%@", [[UIDevice currentDevice] systemVersion]);
    
    if(NSClassFromString(@"SKStoreProductViewController")) { // iOS6 이상인지 체크
        
        SKStoreProductViewController *storeController = [[SKStoreProductViewController alloc] init];
        storeController.delegate = delegate; // productViewControllerDidFinish
        
        NSDictionary *productParameters = @{ SKStoreProductParameterITunesItemIdentifier : appStoreID };
        
        
        [storeController loadProductWithParameters:productParameters completionBlock:^(BOOL result, NSError *error) {
            if (result) {
                //                [self presentViewController:storeController animated:YES completion:nil];
                UIViewController* rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
                [rootViewController presentViewController:storeController animated: YES completion:nil];
                
            } else {
                [[[UIAlertView alloc] initWithTitle:@"Connection Fail" message:@"Cannot find the application." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles: nil] show];
            }

            if (s_pSocialX->getDelegate()) {
                s_pSocialX->getDelegate()->onFinishLoading();
                [activityIndicator stopAnimating];
            }
        }];
    } else { // iOS6 이하일 때
        [[UIApplication sharedApplication] openURL:appStoreURL];
    }
}
- (void)productViewControllerDidFinish:(SKStoreProductViewController *)viewController
{
    [m_RootViewController dismissViewControllerAnimated:YES completion:nil];
    //    [viewController dismissModalViewControllerAnimated:YES];
}

//+ (NSString *) deviceModel {
//    size_t size;
//    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
//    char *model = (char*)malloc(size);
//    sysctlbyname("hw.machine", model, &size, NULL, 0);
//    NSString *deviceModel = [NSString stringWithCString:model encoding:NSUTF8StringEncoding];
//    free(model);
//    
//    return deviceModel;
//}

@end
