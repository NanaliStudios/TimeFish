//
//  FirebaseX.h
//  Timefish
//
//  Created by Midchow on 2016. 12. 5..
//
//

#ifndef FirebaseX_h
#define FirebaseX_h

#include "cocos2d.h"

USING_NS_CC;

class FirebaseXDelegate
{
public:
    virtual ~FirebaseXDelegate() {}
    virtual void admobInterstitialReady(bool success) {}
    virtual void admobInterstitialClosed() {}
};

class FirebaseX{
    
public:
    static FirebaseX* getInstance();
    static void purgeFirebaseX();
    
    void startSession();
    
    bool isPossibleOSVersion();
    
    //
    void showBanner();
    void hideBanner();
    
    //
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    bool isInterstitialReady();
#else
    void checkInterstitialReady();
#endif
    void showInterstitial();
    
    void setDelegate(FirebaseXDelegate* delegate) { m_delegate = delegate; }
    FirebaseXDelegate* getDelegate() { return m_delegate; }
    
private:
    FirebaseX()
    : m_delegate(nullptr)
    {
    }
    
    FirebaseXDelegate* m_delegate;
};

#endif /* FirebaseX_h */
