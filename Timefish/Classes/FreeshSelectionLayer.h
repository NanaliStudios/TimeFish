//
//  FreeshSelectionLayer.h
//  Timefish
//
//  Created by midchow on 2015. 7. 17..
//
//

#ifndef __Timefish__FreeshSelectionLayer__
#define __Timefish__FreeshSelectionLayer__

#include "cocos2d.h"
#include "Global.h"

#include "LocalizationManager.h"

#include "MenuItemAutoGray.h"
#include "MenuItemSameImage.h"
#include "ScrollLayerBase.h"
//#include "FishLayer.h"
#include "IdleFreeshLayer.h"
#include "SkeletonHolder.h"
#include "MenuItemImageButton.h"

#include "StoreKitX.h"

//
#include <spine/spine-cocos2dx.h>

USING_NS_CC;

class FreeshSelectionLayer : public ScrollLayerBase
{
public:
    virtual bool init();
    
    CREATE_FUNC(FreeshSelectionLayer);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    void enableKeyInput();
    virtual void onKeyReleased( EventKeyboard::KeyCode keycode, Event *event );
#endif

    void initMainLayer();
    void setCloseCallback(const std::function<void(Ref*)>& _closeCallback) {
        closeCallback = _closeCallback;
    }
    void setPurchaseCallback(const std::function<void(int)>& _purchaseCallback) {
        purchaseCallback = _purchaseCallback;
    }
    void setGoldenfishCallback(const std::function<void()>& _goldenfishCallback) {
        goldenfishCallback = _goldenfishCallback;
    }

    bool ownsTouchPriority() {
        return ownsPriority;
    }
    
    void hideLayer() {
        ownsPriority = false;
        this->setVisible(false);
        for (int i=0; i<itemSize; i++) {
            IdleFreeshLayer *freesh = (IdleFreeshLayer*)scrollLayer->getChildByTag(i);
            if (freesh) {
                freesh->pauseAnimation();
            }
        }
        this->pause();
    }
    void showLayer() {
        ownsPriority = true;
        this->setVisible(true);
        this->resume();
        for (int i=0; i<itemSize; i++) {
            IdleFreeshLayer *freesh = (IdleFreeshLayer*)scrollLayer->getChildByTag(i);
            if (freesh) {
                freesh->resumeAnimation();
            }
        }
        this->setEnableLayer(true);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        enableKeyInput();
#endif
        
        //
        // move to current skin
        //
        int idx = UserInfo::getInstance()->getSkinNum() - 1;
        if (UserInfo::getInstance()->getRandomSkin()) {
            idx = -1;
        }
//        if (UserInfo::getInstance()->getFreeshType() == FreeshTypeRandom) {
//            idx = -1;
//        }

        directMoveScrollToIndex(idx);
        
    }
    
    void initScrolls();
    void destroyScrolls();

private:
    int status;
    int WindowSize;
    bool ownsPriority;

    IdleFreeshLayer *freesh[FreeshSize + 1]; // +1 is for random selection
    std::function<void(Ref*)> closeCallback;
    std::function<void(int)> purchaseCallback;
    std::function<void()> goldenfishCallback;
    
    Label *countLabel, *nameLabel, *hiddenDescLabel, *musicInfoLabel, *writerInfoLabel;

    MenuItemImageButton *btnMenuItem;
    Menu *lockedBtn;
    Label *btnLabel1, *btnLabel2;
    Sprite *infoIcon;

    //
    void initUILayers();

    //
    void btnCallback(Ref *pSender);
    void hideThis();
    
    void selectionCallback(bool noSound);
    
    //
    void updateCountLabel();
    void updateNameLabel();
    void updateMusicInfoLabel();
    void showHiddenDescLabel(bool flag);
    
    // dynamic item add/remove
    int startIdx;
    int endIdx;
    void resetScrollItems(int selectedIdx);

    void addItemToScroll(int i);
};

#endif /* defined(__Timefish__FreeshSelectionLayer__) */
