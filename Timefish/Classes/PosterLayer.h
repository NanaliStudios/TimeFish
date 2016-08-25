//
//  PosterLayer.h
//  Timefish
//
//  Created by midchow on 2015. 3. 20..
//
//

#ifndef __Timefish__PosterLayer__
#define __Timefish__PosterLayer__

#include "cocos2d.h"
#include "Global.h"
#include "UserInfo.h"
#include "SocialX.h"

#include "TooltipLayer.h"
#include "MenuItemSameImage.h"

#include "FlurryX.h"

USING_NS_CC;

class PosterLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(PosterLayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;
    
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    void onKeyReleased( EventKeyboard::KeyCode keycode, Event *event );
#endif

    void enableLayerTouch();
    void disableLayerTouch();
    void initMainLayerWithType(int userSelectedPoster);
    void initUILayer();

    void setCloseCallback(const std::function<void(Ref*)>& _closeCallback) {
        closeCallback = _closeCallback;
    }

private:
    bool ownsPriority;

    Layer *mainLayer;
    Layer *uiLayer;
    Sprite *checkMark;
    Menu *setBtn;

    std::function<void(Ref*)> closeCallback;

    int posterIdx;
    bool uiVisible;

    void addAchievementInfo(float topPosY);

    void removeThis();
    void setTitle(Ref *pSender);
    void setShare();
    void afterCaptured(bool succeed, const std::string& outputFile);
};

#endif /* defined(__Timefish__PosterLayer__) */
