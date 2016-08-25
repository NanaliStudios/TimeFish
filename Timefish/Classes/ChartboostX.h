//
//  ChartboostX.h
//  Timefish
//
//  Created by midchow on 2015. 1. 21..
//
//

#ifndef Timefish_ChartboostX_h
#define Timefish_ChartboostX_h

class ChartboostXDelegate
{
public:
    virtual ~ChartboostXDelegate() {}
    
    //
    // Interstitial
    //
    // Called before requesting an interestitial from the back-end
    // In Android, this method is called in UI Thread
    // In Android, the param "location" will always be '\0'
    virtual bool shouldRequestInterstitial(const char* location) { return true; }
    
    // Called when an interstitial has been received, before it is presented on screen
    // Return false if showing an interstitial is currently innapropriate, for example if the user has entered the main game mode.
    // In Android, this method is called in UI Thread
    // In Android, the param "location" will always be '\0'
    virtual bool shouldDisplayInterstitial(const char* location) { return false; }
    
    virtual void didDisplayInterstitial(const char* location) {}
    
    // Called when an interstitial has been received and cached.
    // In Android, this method will only be called if you call ChartboostX::hasCachedInterstitial(const char* location) first
    virtual void didCacheInterstitial(const char* location) {}
    
    // Called when an interstitial has failed to come back from the server
    // In Android, the param "location" will always be '\0'
    virtual void didFailToLoadInterstitial(const char* location, int errorCode) {}
    
    // Called when the user dismisses the interstitial
    // If you are displaying the add yourself, dismiss it now.
    // In Android, the param "location" will always be '\0'
    virtual void didDismissInterstitial(const char* location) {}
    
    // Same as above, but only called when dismissed for a close
    // In Android, the param "location" will always be '\0'
    virtual void didCloseInterstitial(const char* location) {}
    
    // Same as above, but only called when dismissed for a click
    // In Android, the param "location" will always be '\0'
    virtual void didClickInterstitial(const char* location) {}
};


class ChartboostX {
    
public:
    static ChartboostX* getInstance();
    static void purgeChartboostX();

    void startSession();
    void cacheInterstitial(const char* location);
    void showInterstitial(const char* location);
    bool hasCachedInterstitial(const char* location);
    
    void setDelegate(ChartboostXDelegate* delegate) { m_delegate = delegate; }
    ChartboostXDelegate* getDelegate() { return m_delegate; }
    
private:
    ChartboostX()
    : m_delegate(nullptr)
    {
    }
    
    ChartboostXDelegate* m_delegate;
};

#endif
