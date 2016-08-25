//
//  NotificationX.m
//  Timefish
//
//  Created by midchow on 2015. 8. 9..
//
//

#include "NotificationX.h"

// iOS8
static const UIUserNotificationType USER_NOTIFICATION_TYPES_REQUIRED = UIUserNotificationTypeAlert | UIUserNotificationTypeSound | UIUserNotificationTypeBadge;
// iOS7 and belows
static const UIRemoteNotificationType REMOTE_NOTIFICATION_TYPES_REQUIRED = UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeBadge;

static NotificationX* s_pNotificationX = NULL;

NotificationX* NotificationX::getInstance()
{
    if (s_pNotificationX == NULL)
    {
        s_pNotificationX = new NotificationX();
    }
    return s_pNotificationX;
}

void NotificationX::enableNotification()
{
    float version = [[[UIDevice currentDevice] systemVersion] floatValue];
    if (version >= 8.0)
    {
        UIApplication *app = [UIApplication sharedApplication];
        
        if ([app respondsToSelector:@selector(registerUserNotificationSettings:)])
        {
            UIUserNotificationSettings *settings = [UIUserNotificationSettings settingsForTypes:USER_NOTIFICATION_TYPES_REQUIRED categories:nil];
            [app registerUserNotificationSettings:settings];
            [app registerForRemoteNotifications];
        }
    }
    else
    {
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes:REMOTE_NOTIFICATION_TYPES_REQUIRED];
    }

    
//    UIApplication *application = [UIApplication sharedApplication];
//    // iOS8이상
//    if ([application respondsToSelector:@selector(registerUserNotificationSettings:)]) {
//        if ([application currentUserNotificationSettings].types == UIUserNotificationTypeNone) {
//            // 꺼짐
//        } else {
//            // 켜짐
//        }
//    } else {
//        // iOS7이하
//        if ([[UIApplication sharedApplication] enabledRemoteNotificationTypes] == UIRemoteNotificationTypeNone) {
//            // 꺼짐
//        } else {
//            // 켜짐
//        }
//    }

}

void NotificationX::disableNotification()
{
    float version = [[[UIDevice currentDevice] systemVersion] floatValue];
    if (version >= 8.0) {
        [UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeNone categories:nil];
    }
    else {
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes:UIRemoteNotificationTypeNone];
    }
}

void NotificationX::registerLocalNotification(const char* message, float seconds, int tag)
{
//    NSLog(@"[%f sec], %@", seconds, [NSString stringWithUTF8String:message]);
    
    UILocalNotification* localNotification = [[UILocalNotification alloc] init];
    [localNotification autorelease];
    
    NSDate* date = [[NSDate date] dateByAddingTimeInterval:seconds];
    localNotification.fireDate = date;
    localNotification.timeZone = [NSTimeZone systemTimeZone];
    
    localNotification.alertBody = [NSString stringWithUTF8String:message];
    localNotification.soundName = @"sounds/button_click.caf";
    localNotification.applicationIconBadgeNumber = 1;

    [[UIApplication sharedApplication] scheduleLocalNotification:localNotification];
}

void NotificationX::cancelAllNotifications()
{
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
    [UIApplication sharedApplication].applicationIconBadgeNumber = 0;
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
            registerLocalNotification(messageInfo.c_str(), restTimeInSec + 30); // add 30 seconds for buffer
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
        std::stringstream stream;
        if (notiIdx == 0) {
            stream << LocalizationManager::getInstance()->getLocalizationString("PushNoti3");
        }
        else {
            stream << LocalizationManager::getInstance()->getLocalizationString("PushNoti4");
        }
        std::string messageInfo = stream.str();
        registerLocalNotification(messageInfo.c_str(), inSec3Days * i);

        //
        notiIdx = 1 - notiIdx;
    }

    //
    // 15-day notification
    //
    float inSec15Days = oneDayInSec * 15;
    {
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("PushNoti5");
        std::string messageInfo = stream.str();
        registerLocalNotification(messageInfo.c_str(), inSec15Days);
    }
    
    //
    // 30-day notification
    //
    float inSec30Days = oneDayInSec * 30;
    {
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("PushNoti6");
        std::string messageInfo = stream.str();
        registerLocalNotification(messageInfo.c_str(), inSec30Days);
    }
    
    //
    // 50-day notification
    //
    float inSec50Days = oneDayInSec * 50;
    {
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("PushNoti7");
        std::string messageInfo = stream.str();
        registerLocalNotification(messageInfo.c_str(), inSec50Days);
    }
}
