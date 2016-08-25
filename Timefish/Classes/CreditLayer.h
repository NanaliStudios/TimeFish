//
//  CreditLayer.h
//  Timefish
//
//  Created by midchow on 2015. 3. 20..
//
//

#ifndef __Timefish__CreditLayer__
#define __Timefish__CreditLayer__

#include "cocos2d.h"
#include "Global.h"
#include "UserInfo.h"
#include "SocialX.h"

#include "LocalizationManager.h"

#include "MenuItemSameImage.h"

#include "FlurryX.h"

USING_NS_CC;

class CreditLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(CreditLayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;

    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    virtual void onKeyReleased( EventKeyboard::KeyCode keycode, Event *event );
#endif

    void enableLayerTouch();
    void disableLayerTouch();
    
    void setCloseCallback(const std::function<void(Ref*)>& _closeCallback) {
        closeCallback = _closeCallback;
    }
    
private:
    bool ownsPriority;
    Layer *movingLayer;
    Layer *uiLayer;
    
    float scaleFactor;

    Sprite *light;
    
    Label *youLabel;
    
    Menu *deepFishBtn;
    
    std::function<void(Ref*)> closeCallback;

    void initBackgroundLayer();
    void initMovingLayer();
    void initUILayer();

    void removeThis();

    void getAchievement();
    void checkMovingLayerPos(float dt);

    void moveMovingLayer();
    
    void moveToHomepage();
    void moveToYoutube();
    
    void bouncingButton();
};

#endif /* defined(__Timefish__CreditLayer__) */


