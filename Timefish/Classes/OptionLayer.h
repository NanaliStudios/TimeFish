//
//  OptionLayer.h
//  Timefish
//
//  Created by midchow on 2015. 7. 29..
//
//

#ifndef __Timefish__OptionLayer__
#define __Timefish__OptionLayer__

#include "cocos2d.h"
#include "Global.h"

#include "FlurryX.h"
#include "GameCenterX.h"

#include "LocalizationManager.h"
#include "NotificationX.h"

#include "MenuItemSameImage.h"
#include "MenuItemImageButton.h"

USING_NS_CC;

class OptionLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(OptionLayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;
    
    virtual void setVisible(bool visible);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    virtual void onKeyReleased( EventKeyboard::KeyCode keycode, Event *event );
#endif

    void setButtonCallbacks(const std::function<void(Ref*)>& _setSoundCallback,
                            const std::function<void(Ref*)>& _setNotiCallback,
                            const std::function<void(Ref*)>& _showTutorialCallback,
                            const std::function<void(Ref*)>& _openCreditCallback,
                            const std::function<void(Ref*)>& _setCloseCallback) {
        
        btnCallbacks[0] = _setSoundCallback;
        btnCallbacks[1] = _setNotiCallback;
        btnCallbacks[2] = _showTutorialCallback;
        btnCallbacks[3] = _openCreditCallback;
        btnCallbacks[4] = _setCloseCallback;
    }

private:
    Layer *mainLayer;
    
    int soundBtnCnt;
    
    bool ownsPriority;

    std::function<void(Ref*)> btnCallbacks[5];

    void btnCallback(Ref *pSender);
    void setToggleButton(Ref *pSender);

    void initMainLayer();
    void hideThis();
};

#endif /* defined(__Timefish__OptionLayer__) */
