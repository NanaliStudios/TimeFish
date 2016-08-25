//
//  FlurryX.h
//  Timefish
//
//  Created by midchow on 2015. 1. 23..
//
//

#ifndef Timefish_FlurryX_h
#define Timefish_FlurryX_h

class FlurryXDelegate
{
public:
    virtual ~FlurryXDelegate() {}
};

class FlurryX {

public:
    static FlurryX* getInstance();
    static void purgeFlurryX();

    void startSession();
    void logEvent(const char *eventName);
    void logPosterSelection(const char *eventName, int type);
    void logBackgroundEventWithInt(const char *eventName, int value);
    void logResultEvent(int score, int bestScore, int playCount, int totalCount);
    
    void setDelegate(FlurryXDelegate* delegate) { m_delegate = delegate; }
    FlurryXDelegate* getDelegate() { return m_delegate; }
    
private:
    FlurryX()
    : m_delegate(nullptr)
    {
    }
    
    FlurryXDelegate* m_delegate;
};

#endif
