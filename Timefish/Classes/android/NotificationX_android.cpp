//
//  NotificationX_android.cpp
//  Timefish
//
//  Created by midchow on 2015. 8. 19..
//
//

#include "NotificationX.h"
#include "C2DXNotificationJni.h"

#include "UserInfo.h"
#include "LocalizationManager.h"

static NotificationX* s_pNotificationX = nullptr;

NotificationX* NotificationX::getInstance()
{
    if (s_pNotificationX == nullptr)
    {
        s_pNotificationX = new NotificationX();
    }
    return s_pNotificationX;
}

void NotificationX::enableNotification()
{
    
}
void NotificationX::disableNotification()
{
    
}
void NotificationX::cancelAllNotifications()
{
    cancelAllNotificationsJNI();
}

void NotificationX::registerAllLocalNotifications()
{
    //
    // from option
    //
    if (UserInfo::getInstance()->getNoti() == 0) {
        return;
    }

    //
    cancelAllNotifications();
    
    int tag = 1;

    //
    // gift box
    //
    if (UserInfo::getInstance()->getSecondSinceLastBoot() > -1) {
        int restTimeInSec = UserInfo::getInstance()->getRestTimeInSec();
        if (restTimeInSec > 0) {
            int r = rand()%2;
            std::stringstream stream;
            if (r == 0) {
                stream << LocalizationManager::getInstance()->getLocalizationString("PushNoti1");
            }
            else {
                stream << LocalizationManager::getInstance()->getLocalizationString("PushNoti2");
            }
            std::string messageInfo = stream.str();
            registerLocalNotification(messageInfo.c_str(), restTimeInSec + 30, tag); // add 30 seconds for buffer
        }
    }
    
    float oneDayInSec = 24 * 60 * 60;

    //
    // 4 3-day notifications
    //
    float inSec3Days = oneDayInSec * 3;
    int r = rand()%2;
    int notiIdx = r; // 0 or 1
    for (int i=1; i<=4; i++) {
        //
        tag++;

        std::stringstream stream;
        if (notiIdx == 0) {
            stream << LocalizationManager::getInstance()->getLocalizationString("PushNoti3");
        }
        else {
            stream << LocalizationManager::getInstance()->getLocalizationString("PushNoti4");
        }
        std::string messageInfo = stream.str();
        registerLocalNotification(messageInfo.c_str(), inSec3Days * i, tag);
        
        //
        notiIdx = 1 - notiIdx;
    }

    tag++;

    //
    // 15-day notification
    //
    float inSec15Days = oneDayInSec * 15;
    {
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("PushNoti5");
        std::string messageInfo = stream.str();
        registerLocalNotification(messageInfo.c_str(), inSec15Days, tag);
    }

    tag++;

    //
    // 30-day notification
    //
    float inSec30Days = oneDayInSec * 30;
    {
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("PushNoti6");
        std::string messageInfo = stream.str();
        registerLocalNotification(messageInfo.c_str(), inSec30Days, tag);
    }

    tag++;

    //
    // 50-day notification
    //
    float inSec50Days = oneDayInSec * 50;
    {
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("PushNoti7");
        std::string messageInfo = stream.str();
        registerLocalNotification(messageInfo.c_str(), inSec50Days, tag);
    }
}

void NotificationX::registerLocalNotification(const char* message, float seconds, int tag)
{
    registerLocalNotificationJNI(message, (int)seconds, tag);
}
