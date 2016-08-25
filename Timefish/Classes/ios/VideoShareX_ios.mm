//
//  VideoShareX_ios.m
//  Timefish
//
//  Created by midchow on 2015. 8. 4..
//
//

#import <Everyplay/Everyplay.h>

#include "VideoShareX.h"

@interface VideoDelegateBridge : NSObject<EveryplayDelegate>
{
    
}
- (void)everyplayShown;
-(void)everyplayHidden;
-(void)everyplayRecordingStopped;

- (void)everyplayUploadDidStart:(NSNumber *)videoId;
- (void)everyplayUploadDidProgress:(NSNumber *)videoId progress:(NSNumber *)progress;
- (void)everyplayUploadDidComplete:(NSNumber *)videoId;

@end

static VideoDelegateBridge* s_KDDelegateBridge = nil;
static VideoShareX* s_pVideoShareX = NULL;

VideoShareX* VideoShareX::getInstance()
{
    if (s_pVideoShareX == NULL)
    {
        s_pVideoShareX = new VideoShareX();
        
        UIViewController *rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
        
        
        [Everyplay setClientId:@"90ca3a315092df23c38988139150ce6a79e302bb" clientSecret:@"ca9e307dc1af23e6d8a5460f002dc33e67292375" redirectURI:@"https://m.everyplay.com/auth"];
        
        s_KDDelegateBridge = [[VideoDelegateBridge alloc] init];
        
        [Everyplay initWithDelegate:s_KDDelegateBridge andParentViewController:rootViewController];
        
//        [[[Everyplay sharedInstance] capture] autoRecordForSeconds:10 withDelay:2];
    }
    return s_pVideoShareX;
}

void VideoShareX::initVideoShare()
{
}

void VideoShareX::startRecording()
{
    [[[Everyplay sharedInstance] capture] startRecording];
}
void VideoShareX::stopRecording()
{
    [[[Everyplay sharedInstance] capture] stopRecording];
}
void VideoShareX::pauseRecording()
{
    [[[Everyplay sharedInstance] capture] pauseRecording];
}
void VideoShareX::resumeRecording()
{
    [[[Everyplay sharedInstance] capture] resumeRecording];
}
void VideoShareX::showView()
{
    [[Everyplay sharedInstance] showEveryplaySharingModal];
}
void VideoShareX::showEveryplay()
{
    [[Everyplay sharedInstance] showEveryplay];
}


@implementation VideoDelegateBridge
- (void)everyplayShown
{
    NSLog(@"everyplayShown");
}

-(void)everyplayHidden
{
    NSLog(@"everyplayHidden");
}

-(void)everyplayRecordingStopped {
    NSLog(@"everyplayRecordingStopped");

    //
    [[Everyplay sharedInstance] mergeSessionDeveloperData:@{@"gamename" : @"Timefish"}];
}

- (void)everyplayUploadDidStart:(NSNumber *)videoId
{
//    NSLog(@"%d", [videoId intValue]);
}
- (void)everyplayUploadDidProgress:(NSNumber *)videoId progress:(NSNumber *)progress
{
//    NSLog(@"%d", [videoId intValue]);
}
- (void)everyplayUploadDidComplete:(NSNumber *)videoId
{
//    NSLog(@"%d", [videoId intValue]);
    //
    // Maybe this spot is for giving a reward to user
    //
}
@end
