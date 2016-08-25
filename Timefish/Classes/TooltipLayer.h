//
//  TooltipLayer.h
//  Timefish
//
//  Created by midchow on 2015. 3. 24..
//
//

#ifndef __Timefish__TooltipLayer__
#define __Timefish__TooltipLayer__

#include "cocos2d.h"

#include "Global.h"
#include "UserInfo.h"
#include "SocialX.h"
#include "LocalizationManager.h"

#include "MenuItemSameImage.h"

USING_NS_CC;

class TooltipLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(TooltipLayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;

    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    
    void enableLayerTouch();
    void disableLayerTouch();
    void initWithAchievementType(Vec2 pos, float iconWidth, int posterIdx);
    void initWithStringKey(Vec2 pos, float iconWidth, int stringKey);
    
    void setCloseCallback(const std::function<void(Ref*)>& _closeCallback) {
        closeCallback = _closeCallback;
    }
    void removeThis(float dt);

private:
    Layer *mainLayer;
    
    std::function<void(Ref*)> closeCallback;

    Vec2 initBackground(float length, float iconWidth, Vec2 pos, int col);

    void addAchievementInfo(float topPosY);
    
    void util_add_comma_to_num2(const char *str, char *buf, int buflen);
};

#endif /* defined(__Timefish__TooltipLayer__) */
