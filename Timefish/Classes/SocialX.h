//
//  SocialX.h
//  Timefish
//
//  Created by midchow on 2015. 1. 23..
//
//

#ifndef Timefish_SocialX_h
#define Timefish_SocialX_h

#include <ctime>
#include <string>

typedef enum {
    NoRecording_NoFlowing = 0,
    NoRecording_WithFlowing = 1,
    WithRecording_NoFlowing = 2,
    WithRecording_WithFlowing = 3,
} DevicePerformance;

class SocialXDelegate
{
public:
    virtual ~SocialXDelegate() {}

    virtual void onCancelCallback() {}
    virtual void onFinishLoading() {}
};

class SocialX {
    
public:
    static SocialX* getInstance();
    static void purgeSocialX();

    // iOS only
//    void openActionSheetWithScore(int score);
//    void openActionSheetWithScore(int score, unsigned char *buffer, int leng);
//    void openActionSheetWithScore(const char* message, const char* path);
//    void openActionSheetWithPoster(const char* message, const char* path);
    void openActionSheetWithMessage(const char* message, const char* path);
    
    void rateApp();
    void openFacebookPage();
    void openHomepage();
    void openYoutubePage();
    void openArtworkPage();
    void showExitPopup();
    
    void showAlert();
    
    std::string getDeviceUUID();
    void submitScore();

    DevicePerformance getDevicePerformance();
    
    float getCurrentTimeSinceLastBootInSec();
    std::time_t getCurrentTime();

    bool getAudioInUse();

//    // Android only
//    void tweetTapped();
//    void facebookTapped();

    void setDelegate(SocialXDelegate* delegate) { m_delegate = delegate; }
    SocialXDelegate* getDelegate() { return m_delegate; }
    
private:
    SocialX()
    : m_delegate(nullptr)
    {
    }
    
    SocialXDelegate* m_delegate;
};

#endif
