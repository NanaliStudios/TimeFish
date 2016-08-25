//
//  AchievementBannerLayer.h
//  Timefish
//
//  Created by midchow on 2015. 3. 25..
//
//

#ifndef __Timefish__AchievementBannerLayer__
#define __Timefish__AchievementBannerLayer__

#include "cocos2d.h"
#include "Global.h"
#include "UserInfo.h"
#include "SocialX.h"

#include "LocalizationManager.h"

#include "MenuItemSameImage.h"

USING_NS_CC;

class AchievementBannerIcon : public Layer
{
public:
    virtual bool init();

    CREATE_FUNC(AchievementBannerIcon);
    
    void initIcons(int achievemenetIdx, cocos2d::Size inBoxSize, float _scale, bool loadNow = false);

    bool preloaded;
};

class AchievementBannerLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(AchievementBannerLayer);

    cocos2d::Size visibleSize, visibleSizeHalf;

    void setBannerWith(int achievemenetIdx);

    void setCloseCallback(const std::function<void(Ref*)>& _closeCallback) {
        closeCallback = _closeCallback;
    }

private:
    LayerColor *top;
    float _scale;
    cocos2d::Size inBoxSize;
    float layerSize;

    std::function<void(Ref*)> closeCallback;

    void initMainLayer();
    void initIcons(int achievemenetIdx);

    void hideBanner();
    void closeBanner();
    
    int currIdx;

    Label *achievementName, *achievementInfo;
    AchievementBannerIcon *icons[NumOfAchievement];

    //
//    int languageIndexDelta;
};

#endif /* defined(__Timefish__AchievementBannerLayer__) */
