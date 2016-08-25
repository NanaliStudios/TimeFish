//
//  TapjoyX.h
//  Timefish
//
//  Created by midchow on 2015. 8. 5..
//
//

#ifndef __Timefish__TapjoyX__
#define __Timefish__TapjoyX__

class TapjoyXDelegate
{
public:
    virtual ~TapjoyXDelegate() {}
    
    virtual void onCancelCallback() {}
    
    virtual void requestDidSucceed() {}
    virtual void requestDidFail() {}
    virtual void contentIsReady(const char *placementName) {}
    virtual void contentDidAppear() {}
    virtual void contentDidDisappear(const char *placementName) {}
};

class TapjoyX {
    
public:
    static TapjoyX* getInstance();
    static void purgeTapjoyX();
    
    bool isVideoReady();
    bool isPlacementReady(const char *placementName);

    void startSession();
    void endSession();
    void requestPlacement(const char *placementName);
    void showPlacement(const char *placementName);

    void setUserLevel(int lvl);
    void logEventInEconomy(int eventTypeIdx, int param);
    void logEventInUIFlow(const char *eventName, int param = -1);
    void logEventInUIFlowResult(const char *eventName, int resultTypeIdx);
    void logEventInUIFlowOption(int btnIdx);
    void logEventInResultFlowResultButton(int btnIdx, int skinNum);
    void logEventInIAP(const char *eventName, int skinIdx = -1);
    
    void setDelegate(TapjoyXDelegate* delegate) { m_delegate = delegate; }
    TapjoyXDelegate* getDelegate() { return m_delegate; }
    
private:
    TapjoyX()
    : m_delegate(nullptr)
    {
    }
    
    TapjoyXDelegate* m_delegate;
};


#endif /* defined(__Timefish__TapjoyX__) */
