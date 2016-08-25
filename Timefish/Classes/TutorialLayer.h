//
//  TutorialLayer.h
//  Timefish
//
//  Created by midchow on 2015. 7. 30..
//
//

#ifndef __Timefish__TutorialLayer__
#define __Timefish__TutorialLayer__

#include "cocos2d.h"
#include "Global.h"

#include "FlurryX.h"
#include "GameCenterX.h"

#include "LocalizationManager.h"

#include "MenuItemSameImage.h"
#include "MenuItemImageButton.h"

USING_NS_CC;

class TutorialLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(TutorialLayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;

    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void onTouchCancelled(Touch *touch, Event *event);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    virtual void onKeyReleased( EventKeyboard::KeyCode keycode, Event *event );
#endif

    void setCloseCallback(const std::function<void(Ref*)>& _closeCallback) {
        closeCallback = _closeCallback;
    }

private:
    bool ownsPriority;

    Layer *pageLayer[5];
    Layer *uiLayer;

    float scaleFactor;

//    Sprite *pageDots[5];
    
    int currentPage;
    float topPosY;

    std::function<void(Ref*)> closeCallback;

    void setPage1();
    void setPage2();
    void setPage3();
    void setPage4();
    void setPage5();

    void btnCallback(Ref *pSender);
    
    void initMainLayer();
    void hideThis();
};


#endif /* defined(__Timefish__TutorialLayer__) */
