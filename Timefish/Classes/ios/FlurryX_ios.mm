//
//  FlurryX_ios.mm
//  Timefish
//
//  Created by midchow on 2015. 1. 23..
//
//

#import <Flurry.h>
#include "FlurryX.h"

#define FlurryAPI_Key @"FWDXF8TBH923PTSPHY7W"

@interface FlurryDelegateBridge
@end

static FlurryX* s_pFlurryX = NULL;

FlurryX* FlurryX::getInstance()
{
    if (s_pFlurryX == NULL)
    {
        s_pFlurryX = new FlurryX();
    }
    return s_pFlurryX;
}

void FlurryX::startSession()
{
    [Flurry startSession:FlurryAPI_Key];

    NSString * appBuildString = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"];
    NSString * appVersionString = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"];
    NSString * versionBuildString = [NSString stringWithFormat:@"%@(%@)", appVersionString, appBuildString];
    [Flurry setAppVersion:versionBuildString];
}

void FlurryX::logEvent(const char *eventName)
{
    NSString *_eventName=[NSString stringWithFormat:@"%s",eventName];
    [Flurry logEvent:_eventName];
}

void FlurryX::logBackgroundEventWithInt(const char *eventName, int value)
{
    NSString *key = @"Plays";
    NSNumber *obj = [NSNumber numberWithInteger:value];
    
    NSDictionary *param = [[NSDictionary alloc] initWithObjectsAndKeys:obj, key, nil];

    NSString *_eventName=[NSString stringWithFormat:@"%s",eventName];
    [Flurry logEvent:_eventName withParameters:param];
}

void FlurryX::logPosterSelection(const char *eventName, int type)
{
    NSString *key = @"Poster";
    NSNumber *obj = [NSNumber numberWithInteger:type];
    
    NSDictionary *param = [[NSDictionary alloc] initWithObjectsAndKeys:obj, key, nil];
    
    NSString *_eventName=[NSString stringWithFormat:@"%s",eventName];
    [Flurry logEvent:_eventName withParameters:param];
}

void FlurryX::logResultEvent(int score, int bestScore, int playCount, int totalCount)
{
    NSString *key1 = @"Score";
    NSString *key2 = @"Best";
    NSString *key3 = @"Plays";
    NSString *key4 = @"TotalPlays";
    NSNumber *obj1 = [NSNumber numberWithInteger:score];
    NSNumber *obj2 = [NSNumber numberWithInteger:bestScore];
    NSNumber *obj3 = [NSNumber numberWithInteger:playCount];
    NSNumber *obj4 = [NSNumber numberWithInteger:totalCount];

    NSDictionary *param = [[NSDictionary alloc] initWithObjectsAndKeys:obj1, key1, obj2, key2, obj3, key3, obj4, key4, nil];
    [Flurry logEvent:@"Result" withParameters:param];
}

