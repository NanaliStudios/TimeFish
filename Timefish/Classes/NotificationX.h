//
//  NotificationX.h
//  Timefish
//
//  Created by midchow on 2015. 8. 9..
//
//

#ifndef Timefish_NotificationX_h
#define Timefish_NotificationX_h

#include "UserInfo.h"
#include "LocalizationManager.h"

class NotificationXDelegate
{
public:
    virtual ~NotificationXDelegate() {}
};

class NotificationX {
    
public:
    static NotificationX* getInstance();
    static void purgeNotificationX();

    void enableNotification();
    void disableNotification();
    void cancelAllNotifications();
    
    void registerAllLocalNotifications();
    void registerLocalNotification(const char* message, float seconds, int tag = -1);
    
    void setDelegate(NotificationXDelegate* delegate) { m_delegate = delegate; }
    NotificationXDelegate* getDelegate() { return m_delegate; }
    
private:
    NotificationX()
    : m_delegate(nullptr)
    {
    }
    
    NotificationXDelegate* m_delegate;
};

#endif
