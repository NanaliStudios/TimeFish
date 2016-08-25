//
//  AdColonyX.h
//  Timefish
//
//  Created by midchow on 2015. 1. 23..
//
//

#ifndef Timefish_AdColonyX_h
#define Timefish_AdColonyX_h

class AdColonyXDelegate
{
public:
    virtual ~AdColonyXDelegate() {}

    // Is called when AdColony has taken control of the device screen and is about to begin showing an ad
    // Apps should implement app-specific code such as pausing a game and turning off app music
    virtual void onAdColonyAdStartedInZone(const char* zoneID) {}

    // Is called when AdColony has finished trying to show an ad, either successfully or unsuccessfully
    // If shown == YES, an ad was displayed and apps should implement app-specific code such as unpausing a game and restarting app music
    virtual void onAdColonyAdAttemptFinished(bool shown, const char* zoneID) {}
    
    virtual void onAdColonyAdAvailabilityChange(bool available, const char* zoneID) {}
};


class AdColonyX {
    
public:
    static AdColonyX* getInstance();
    static void purgeAdColonyX();
    
    void startSession();
    void showVideo();
    bool isVideoAvailable();
    
    void setDelegate(AdColonyXDelegate* delegate) { m_delegate = delegate; }
    AdColonyXDelegate* getDelegate() { return m_delegate; }
    
private:
    AdColonyX()
    : m_delegate(nullptr)
    {
    }
    
    AdColonyXDelegate* m_delegate;
};

#endif
