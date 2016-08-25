//
//  FreeGiftLayer.h
//  Timefish
//
//  Created by midchow on 2015. 7. 19..
//
//

#ifndef __Timefish__FreeGiftLayer__
#define __Timefish__FreeGiftLayer__

#include "cocos2d.h"
#include "Global.h"
#include "UserInfo.h"
#include "LocalizationManager.h"
#include "SkeletonHolder.h"
#include "MenuItemImageButton.h"

#include <spine/spine-cocos2dx.h>

USING_NS_CC;

class FreeGiftLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(FreeGiftLayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;
    
    void setCloseCallback(const std::function<void()>& _closeCallback) {
        closeCallback = _closeCallback;
    }
    void setShowCoinsCallback(const std::function<void(int)>& _showCoinsCallback) {
        showCoinsCallback = _showCoinsCallback;
    }

    void hideLayer() {
        setVisible(false);
        pause();
    }
    void showLayer(int _type, int _amount);

private:
    std::function<void()> closeCallback;
    std::function<void(int)> showCoinsCallback;

    bool poped;
    int giftType;
    int amount;

    Menu *nextBtn;
    MenuItemImageButton *nextBtnItem;
    Label *resultCoinLabel;
    Sprite *coinIcon;

    spine::SkeletonAnimation *sNode;
    
    ParticleSystemQuad *partyPtcl[3];

    void setEnableLayer(bool flag);
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);

    void initMainLayer();
    void setMainResultType();
    void btnCallback(Ref* pSender);
    
    void showResultUI();

    void setRandomSkin();
    void initParticles();

    void popTheBox();
};

#endif /* defined(__Timefish__FreeGiftLayer__) */
