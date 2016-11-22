//
//  Popup.h
//  Timefish
//
//  Created by midchow on 2015. 1. 22..
//
//

#ifndef __Timefish__Popup__
#define __Timefish__Popup__

#include "cocos2d.h"
#include "Global.h"

#include "FlurryX.h"
#include "SoundManager.h"

#include "LocalizationManager.h"

#include "IdleFreeshLayer.h"

#include "MenuItemSameImage.h"
#include "MenuItemImageButton.h"
#include "BonusTooltipLayer.h"
#include "MenuItemAutoGray.h"

USING_NS_CC;

class PopupBase : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(PopupBase);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;
    
    void setEnbleTouch(bool flag);

    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void onTouchCancelled(Touch *touch, Event *event);
    virtual void onKeyReleased( EventKeyboard::KeyCode keycode, Event *event );
    virtual void removeSelf(Ref* pSender);

protected:
    float topPosY, btmPosY;
    float popupWidth;

    bool ownsPriority;
    Layer *mainLayer;
    
    void setBackground(float _height);
};

class HiddenskinPopup : public PopupBase
{
public:
    virtual bool init();
    
    CREATE_FUNC(HiddenskinPopup);

    void setNextCallback(const std::function<void()>& _nextCallback) {
        nextCallback = _nextCallback;
    }
    void startIntro();

    void setSkinInfo(int skinIdx);

protected:
    Sprite *bg;
    int _skinNum;

    std::function<void()> nextCallback;

    void initBackground();
    
    void runPopupEffect();
};

class GoldenfishInfoPopup : public PopupBase
{
public:
    virtual bool init();
    
    CREATE_FUNC(GoldenfishInfoPopup);

    void setPurchaseCallback(const std::function<void(Ref*)>& _purchaseCallback) {
        purchaseCallback = _purchaseCallback;
    }
    void setCloseCallback(const std::function<void()>& _closeCallback) {
        closeCallback = _closeCallback;
    }
    
    virtual void removeSelf(Ref* pSender);

protected:
    Sprite *bg;
    int goldenfishSkinIdx;

    std::function<void(Ref*)> purchaseCallback;
    std::function<void()> closeCallback;
    
    void initBackground();
    void setSkinInfo();
};

class BestScorePopup : public PopupBase
{
public:
    virtual bool init();
    
    CREATE_FUNC(BestScorePopup);

    void setShareCallback(const std::function<void()>& _shareCallback) {
        shareCallback = _shareCallback;
    }
    void setScoreInfo(int score);
    void runPopupEffect();

protected:
    void initBackground();
    
    float btnPosTopY;

    std::function<void()> shareCallback;
};

#endif /* defined(__Timefish__Popup__) */
