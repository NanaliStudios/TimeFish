#include "NativeUtils.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include <android/log.h>
#define CLASS_NAME "com/carlospinan/utils/NativeUtils"
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "PlayGameSingleton.h"
#endif

using namespace cocos2d;

#pragma mark - Sign in and Sign out.
bool NativeUtils::isSignedIn()
{	
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return JniHelpers::jniCommonBoolCall(
		"isSignedIn", 
		CLASS_NAME);
#endif
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return PlayGameSingleton::sharedInstance().isSignedIn();
#endif
    
	return false;
}

void NativeUtils::signIn()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelpers::jniCommonVoidCall(
		"gameServicesSignIn", 
		CLASS_NAME);
#endif
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    PlayGameSingleton::sharedInstance().authenticate();
#endif
}

void NativeUtils::signOut()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelpers::jniCommonVoidCall(
		"gameServicesSignOut", 
		CLASS_NAME);
#endif
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    PlayGameSingleton::sharedInstance().signOut();
#endif	
}

#pragma mark - Submit score and achievements.
void NativeUtils::submitScore(long score)
{
    //
    // TODO: Following leaderboard id is hard-coded
    //
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelpers::jniCommonVoidCall(
		"submitScore", 
		CLASS_NAME,
		"CgkI5oqtpJ4VEAIQHQ",
		score);
#endif
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    PlayGameSingleton::sharedInstance().submitScore(score, leaderboardID);
#endif
}

void NativeUtils::unlockAchievement(int achievementIdx)
{
    //
    // TODO: Fix/ Add later
    // 31 achievements ids are hard-coded...
    //
    const char* achievementID[] = {
        "CgkI5oqtpJ4VEAIQAg",
        "CgkI5oqtpJ4VEAIQAw",
        "CgkI5oqtpJ4VEAIQBA",
        "CgkI5oqtpJ4VEAIQBQ",
        "CgkI5oqtpJ4VEAIQBg",
        "CgkI5oqtpJ4VEAIQBw",
        "CgkI5oqtpJ4VEAIQCA",
        "CgkI5oqtpJ4VEAIQCQ",
        "CgkI5oqtpJ4VEAIQCg",
        "CgkI5oqtpJ4VEAIQCw",
        "CgkI5oqtpJ4VEAIQDA",
        "CgkI5oqtpJ4VEAIQDQ",
        "CgkI5oqtpJ4VEAIQDg",
        "CgkI5oqtpJ4VEAIQDw",
        "CgkI5oqtpJ4VEAIQEA",
        "CgkI5oqtpJ4VEAIQEQ",
        "CgkI5oqtpJ4VEAIQEg",
        "CgkI5oqtpJ4VEAIQEw",
        "CgkI5oqtpJ4VEAIQFA",
        "CgkI5oqtpJ4VEAIQFQ",
        "CgkI5oqtpJ4VEAIQFg",
        "CgkI5oqtpJ4VEAIQFw",
        "CgkI5oqtpJ4VEAIQGA",
        "CgkI5oqtpJ4VEAIQGQ",
        "CgkI5oqtpJ4VEAIQGg",
        "CgkI5oqtpJ4VEAIQGw",
        "CgkI5oqtpJ4VEAIQHA",
        //
        "CgkI5oqtpJ4VEAIQHg",
        "CgkI5oqtpJ4VEAIQHw",
        "CgkI5oqtpJ4VEAIQIA",
        "CgkI5oqtpJ4VEAIQIQ"
    };

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelpers::jniCommonVoidCall(
		"unlockAchievement", 
		CLASS_NAME,
		achievementID[achievementIdx]);
#endif
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    PlayGameSingleton::sharedInstance().unlockAchievement(achievementID);
#endif
}

void NativeUtils::incrementAchievement(const char* achievementID, int numSteps)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelpers::jniCommonVoidCall(
		"incrementAchievement", 
		CLASS_NAME,
		achievementID,
		numSteps);
#endif
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    PlayGameSingleton::sharedInstance().incrementAchievement(numSteps, achievementID);
#endif
}

#pragma mark - Show achievements, leaderboards and single leaderboard.
void NativeUtils::showAchievements()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelpers::jniCommonVoidCall(
		"showAchievements", 
		CLASS_NAME);
#endif
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    PlayGameSingleton::sharedInstance().showAchievements();
#endif
}

void NativeUtils::showLeaderboards()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelpers::jniCommonVoidCall(
		"showLeaderboards", 
		CLASS_NAME);
#endif
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    PlayGameSingleton::sharedInstance().showLeaderboards();
#endif
    
}

//void NativeUtils::showLeaderboard(const char* leaderboardID)
void NativeUtils::showLeaderboard()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelpers::jniCommonVoidCall(
		"showLeaderboard", 
		CLASS_NAME,
		"CgkI5oqtpJ4VEAIQHQ");
#endif
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    PlayGameSingleton::sharedInstance().showSingleLeaderboard(leaderboardID);
#endif
}

void NativeUtils::initAd()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    PlayGameSingleton::sharedInstance().initAd();
#endif
}

void NativeUtils::showAd()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelpers::jniCommonVoidCall(
                                  "showAd",
                                  CLASS_NAME);
#endif
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    PlayGameSingleton::sharedInstance().showAd();
#endif
}

void NativeUtils::hideAd()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelpers::jniCommonVoidCall(
                                  "hideAd",
                                  CLASS_NAME);
#endif
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    PlayGameSingleton::sharedInstance().hideAd();
#endif
}
