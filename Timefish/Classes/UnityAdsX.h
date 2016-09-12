//
//  UnityAdsX.h
//  Timefish
//
//  Created by Midchow on 2016. 9. 13..
//
//

#ifndef UnityAdsX_h
#define UnityAdsX_h

class UnityAdsXDelegate
{
public:
    virtual ~UnityAdsXDelegate() {}
    
    virtual void onUnityAdsStart() {}

    virtual void onUnityAdsFinish(bool shown) {}
};


class UnityAdsX {
    
public:
    static UnityAdsX* getInstance();
    static void purgeUnityAdsX();
    
    void startSession();
    void showVideo();
    bool isVideoAvailable();
    
    void setDelegate(UnityAdsXDelegate* delegate) { m_delegate = delegate; }
    UnityAdsXDelegate* getDelegate() { return m_delegate; }
    
private:
    UnityAdsX()
    : m_delegate(nullptr)
    {
    }
    
    UnityAdsXDelegate* m_delegate;
};

#endif /* UnityAdsX_h */
