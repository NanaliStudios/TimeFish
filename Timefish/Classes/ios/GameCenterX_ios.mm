//
//  GameCenterX_ios.mm
//  Timefish
//
//  Created by midchow on 2015. 1. 22..
//
//

#import <GameKit/GameKit.h>
#include "GameCenterX.h"

// GameCenterDelegateBridge
@class GKLocalPlayer;

@interface GameCenterDelegateBridge : NSObject<GKGameCenterControllerDelegate>
{
    UIViewController*           m_RootViewController;
    NSString *_leaderboardIdentifier;
    bool _gameCenterEnabled;

    NSMutableDictionary *achievementsDictionary;
};

// currentPlayerID is the value of the playerID last time GameKit authenticated.
@property (retain,readwrite) NSString * currentPlayerID;
@property(nonatomic, retain) NSMutableDictionary *achievementsDictionary;

- (void) SetRootViewController:(UIViewController*)rootViewController;
- (void) AuthenticateLocalPlayer:(int)localAchievementInfo;
- (void) loadAchievementsWith:(int)localAchievementInfo;
- (BOOL) IsAuthenticated:(BOOL)withAlert;
- (void) showLeaderboardAndAchievements:(BOOL)shouldShowLeaderboard;
- (void) reportScore:(NSInteger)_score;
- (void) reportAchievementIdentifier: (NSString*) identifier percentComplete: (float) percent;
- (void) reportAchievementToServer:(NSMutableArray*)achievementsToReport;
- (void) resetAchievements;
@end


static GameCenterDelegateBridge* s_pGKDelegateBridge = nil;
static GameCenterX* s_pGameCenterX = NULL;

GameCenterX* GameCenterX::getInstance()
{
    if (s_pGameCenterX == NULL)
    {
        s_pGameCenterX = new GameCenterX();
        s_pGKDelegateBridge = [[GameCenterDelegateBridge alloc] init];
    }
    return s_pGameCenterX;
}

bool GameCenterX::isAuthenticated(bool withAlert)
{
    return [s_pGKDelegateBridge IsAuthenticated:withAlert];
}

void GameCenterX::authenticateLocalPlayer()
{
    int localAchievementInfo = UserInfo::getInstance()->getAchievementInfo();
    [s_pGKDelegateBridge AuthenticateLocalPlayer:localAchievementInfo];
}

void GameCenterX::showLeaderboard()
{
    [s_pGKDelegateBridge showLeaderboardAndAchievements:YES];
}

void GameCenterX::showAchievementList()
{
    [s_pGKDelegateBridge showLeaderboardAndAchievements:NO];
}

void GameCenterX::reportScore(int _score)
{
    [s_pGKDelegateBridge reportScore:_score];
}

void GameCenterX::reportAchievementIdentifier(int achievementType, float percentage)
{
    //
    // NOTE: Achievement ID should look like this:
    // Timefish.Achievement10
    //
    NSString *achievementID = [NSString stringWithFormat:@"Timefish.Achievement%02d", achievementType];

    [s_pGKDelegateBridge reportAchievementIdentifier:achievementID percentComplete:percentage];
}

void GameCenterX::resetAchievements()
{
    [s_pGKDelegateBridge resetAchievements];
}

@implementation GameCenterDelegateBridge
@synthesize achievementsDictionary;
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

- (void) AuthenticateLocalPlayer:(int)localAchievementInfo {
    GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
    
    localPlayer.authenticateHandler = ^(UIViewController *viewController, NSError *error){
        if (viewController != nil) {
            //
            if (m_RootViewController == nil) {
                UIViewController* rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
                [self SetRootViewController:rootViewController];
            }
            [m_RootViewController presentViewController: viewController animated: YES completion:nil];
        }
        else{
            if ([GKLocalPlayer localPlayer].authenticated) {
                _gameCenterEnabled = YES;
                
                // Get the default leaderboard identifier.
                [[GKLocalPlayer localPlayer] loadDefaultLeaderboardIdentifierWithCompletionHandler:^(NSString *leaderboardIdentifier, NSError *error) {
                    
                    if (error != nil) {
                        //NSLog(@"%@", [error localizedDescription]);
                    }
                    else{
                        _leaderboardIdentifier = leaderboardIdentifier;
                        [_leaderboardIdentifier retain];
                    }
                }];

                achievementsDictionary = [[NSMutableDictionary alloc] init];
                [self loadAchievementsWith:localAchievementInfo];
            }
            
            else{
                _gameCenterEnabled = NO;
                //NSLog(@"_gameCenter not Enabled");
            }
        }
    };
}

- (void) loadAchievementsWith:(int)localAchievementInfo
{
    [GKAchievement loadAchievementsWithCompletionHandler:^(NSArray *achievements, NSError *error)
     {
         if (error == nil)
         {
             NSMutableArray *achievementsToReport = [[NSMutableArray alloc] init];
             int _localAchievementInfo = localAchievementInfo;

             //
             // Remove achievements which is already reported
             //
             for (GKAchievement* achievement in achievements) {
                 [achievementsDictionary setObject: achievement forKey: achievement.identifier];
                 //NSLog(@"%@", achievement.identifier);

                 NSUInteger fromIdx = (achievement.identifier.length - 2);
                 NSString *last = [achievement.identifier substringFromIndex:fromIdx];
                 int type = [last intValue] - 1;
                 int typeValue = 1 << type;

                 if ((_localAchievementInfo & typeValue) == typeValue) {
                     // already reported
                     typeValue = ~typeValue;
                     _localAchievementInfo = _localAchievementInfo & typeValue;
                 }
             }

             //
             // Make report
             //
             for (int i=0; i<NumOfAchievement; i++) {
                 int type = 1<<i;
                 if ((_localAchievementInfo & type) == type) {
                     NSString *achievementID = [NSString stringWithFormat:@"Timefish.Achievement%02d", i+1];
                     GKAchievement *achievement = [[GKAchievement alloc] initWithIdentifier: achievementID];
                     achievement.percentComplete = 100;
                     achievement.showsCompletionBanner = NO;
                     
                     [achievementsToReport addObject:achievement];
                 }
             }

             [self reportAchievementToServer:achievementsToReport];
             
             [achievementsToReport release];
         }
     }];
}

-(BOOL) IsAuthenticated:(BOOL)withAlert
{
    GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
    if (!localPlayer.isAuthenticated && withAlert) {

        UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"Game Center Unavailable" message:@"Player is not signed in." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [alertView show];
        [alertView release];
    }

    return localPlayer.isAuthenticated;
}

- (void) showLeaderboardAndAchievements:(BOOL)shouldShowLeaderboard
{
    GKGameCenterViewController *gcViewController = [[GKGameCenterViewController alloc] init];
    
    gcViewController.gameCenterDelegate = self;

    if (shouldShowLeaderboard) {
        //NSLog(@"%@", _leaderboardIdentifier);
        gcViewController.viewState = GKGameCenterViewControllerStateLeaderboards;
        gcViewController.leaderboardIdentifier = _leaderboardIdentifier;
    }
    else{
        gcViewController.viewState = GKGameCenterViewControllerStateAchievements;
    }

    if (m_RootViewController == nil) {
        UIViewController* rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
        [self SetRootViewController:rootViewController];
    }
    [m_RootViewController presentViewController:gcViewController animated:YES completion:nil];
}

- (void) reportScore:(NSInteger)_score
{
    GKScore *score = [[GKScore alloc] initWithLeaderboardIdentifier:_leaderboardIdentifier];
    score.value = _score;
    
    [GKScore reportScores:@[score] withCompletionHandler:^(NSError *error) {
        if (error != nil) {
            //NSLog(@"%@", [error localizedDescription]);
        }
    }];
}

- (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)gameCenterViewController
{
    [m_RootViewController dismissViewControllerAnimated:YES completion:nil];
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

- (void) reportAchievementIdentifier: (NSString*) identifier percentComplete: (float) percent
{
    GKAchievement *achievement = [[GKAchievement alloc] initWithIdentifier: identifier];
    achievement.percentComplete = 100;
    achievement.showsCompletionBanner = NO;

    NSArray *achievements = [NSArray arrayWithObjects:achievement, nil];
    [GKAchievement reportAchievements:achievements withCompletionHandler:^(NSError *error) {
        if (error != nil) {
            //NSLog(@"Error in reporting achievements: %@", error);
        }
    }];
}

- (void) reportAchievementToServer:(NSMutableArray*)achievementsToReport
{
    int leng = (int)achievementsToReport.count;
    if (leng == 0) {
        return;
    }
    //NSLog(@"reportAchievementToServer: %d", (int)achievementsToReport.count);

    [GKAchievement reportAchievements:achievementsToReport withCompletionHandler:^(NSError *error) {
        if (error != nil) {
            //NSLog(@"Error in reporting achievements: %@", error);
        }
    }];
}

- (void) resetAchievements
{
    //
    // This is debug purpose only.
    //
    // Clear all locally saved achievement objects.
    achievementsDictionary = [[NSMutableDictionary alloc] init];
    // Clear all progress saved on Game Center.
    [GKAchievement resetAchievementsWithCompletionHandler:^(NSError *error) {
        if (error != nil) {
             // handle the error.
        }
     }];
}

@end
