//
//  SocialX_android.cpp
//  Timefish
//
//  Created by midchow on 2015. 2. 4..
//
//

#include "SocialX.h"
#include "C2DXSocialJni.h"
#include "UserInfo.h"

static SocialX* s_pSocialX = nullptr;

SocialX* SocialX::getInstance()
{
    if (s_pSocialX == nullptr)
    {
        s_pSocialX = new SocialX();
    }
    return s_pSocialX;
}

void SocialX::openActionSheetWithMessage(const char* message, const char* path)
{
    openActionSheetWithMessageJNI(message, path);
}
void SocialX::rateApp()
{
//    rateAppJNI();
}

void SocialX::openFacebookPage()
{
//    openFacebookPageJNI();
}
void SocialX::openHomepage()
{
//    openHomepageJNI();
}
void SocialX::openArtworkPage()
{
//    openArtworkPageJNI();
}

void SocialX::openYoutubePage()
{
//    openYoutubePageJNI();
}
void SocialX::showExitPopup()
{
    showExitPopupJNI();
}

std::string SocialX::getDeviceUUID()
{
    return getDeviceUUIDJNI();
}

DevicePerformance SocialX::getDevicePerformance()
{
    return NoRecording_WithFlowing;
}

float SocialX::getCurrentTimeSinceLastBootInSec()
{
    return getCurrentTimeSinceLastBootInSecJNI();
}

std::time_t SocialX::getCurrentTime()
{
    std::time_t timeCheckIn = std::time(0);

    return timeCheckIn;
}

bool  SocialX::getAudioInUse()
{
    return false;
}


//void SocialX::tweetTapped()
//{
//}
//
//
//void SocialX::facebookTapped()
//{
//}
//
