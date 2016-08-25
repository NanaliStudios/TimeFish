//
//  FreeshMakerLayer.h
//  Timefish
//
//  Created by midchow on 2015. 7. 19..
//
//

#ifndef __Timefish__FreeshMakerLayer__
#define __Timefish__FreeshMakerLayer__

#include "cocos2d.h"
#include "Global.h"

#include <spine/spine-cocos2dx.h>

#include "IdleFreeshLayer.h"
#include "LocalizationManager.h"

#include "MenuItemSameImage.h"
#include "MenuItemAutoGray.h"
#include "MenuItemImageButton.h"

#define StartBtnTag     0
#define PlayBtnTag      1
#define ShareBtnTag     2
#define RetryBtnTag     3
#define CloseBtnTag     4
#define ImageShareBtnTag     5
#define VideoShareBtnTag     6

#define ResultFreeshNameTag       9998
#define ResultFreeshTag           9999

USING_NS_CC;

class FreeshMakerLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(FreeshMakerLayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;
    
    void setCloseCallback(const std::function<void()>& _closeCallback) {
        closeCallback = _closeCallback;
    }
    void setReplayCallback(const std::function<void()>& _replayCallback) {
        replayCallback = _replayCallback;
    }
    void setShareCallback(const std::function<void()>& _shareCallback) {
        shareCallback = _shareCallback;
    }
    void setUiChangeCallback(const std::function<void(int)>& _uiChangeCallback) {
        uiChangeCallback = _uiChangeCallback;
    }
    void setVideoCallbacks(
                           const std::function<void()>& _startVideoCallback,
                           const std::function<void()>& _finishVideoCallback,
                           const std::function<void()>& _shareVideocallback) {
        
        startVideoCallback = _startVideoCallback;
        finishVideoCallback = _finishVideoCallback;
        shareVideocallback = _shareVideocallback;
    }

//    bool onTouchBegan(Touch *touch, Event *event);
//    void onTouchMoved(Touch *touch, Event *event);
//    void onTouchEnded(Touch *touch, Event *event);
//    void onTouchCancelled(Touch *touch, Event *event);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    virtual void onKeyReleased( EventKeyboard::KeyCode keycode, Event *event );
#endif

    bool ownsTouchPriority() {
        return ownsPriority;
    }

    void hideLayer() {
        ownsPriority = false;

        this->setVisible(false);
        this->pause();

        startBtn->setVisible(false);
        playBtn->setVisible(true);
        shareBtn->setVisible(false);
        videoShareBtn->setVisible(false);
        retryBtn->setVisible(false);
        closeBtn->setVisible(false);
        resultLabel->setVisible(false);
    }
    void showLayer() {
        this->setVisible(true);
        
        //
//        enableLayerTouch();

        sNode->setVisible(true);
        sNode->setAnimation(0, "idle", true);
        sNode->setCompleteListener( [this] (int trackIndex, int loopCount) {
            this->startMachine();
        });
        sNode->setEventListener( [this] (int trackIndex, spEvent* event) {
            //
            // Set Event Name
            //
            if (!strcmp(event->data->name, "ball_pop")) {
                this->showResult();
                
                //
                for (int i=0; i<3; i++) {
                    partyPtcl[i]->resetSystem();
                }
                
                //
                lNode->setVisible(true);
                lNode->setAnimation(0, "animation", false);
            }
        });
        
        setMachineSkin();
        
        startBtn->setVisible(true);
        playBtn->setVisible(false);

        this->resume();
    }
    
    void showResultUI(int skinIdxSelected, bool onlyEarnResult = false, bool withCloseBtn = true, bool restoredSkin = false);
    
private:
    int skinNum;
    
    bool ownsPriority;

    std::function<void()> closeCallback;
    std::function<void()> replayCallback;
    std::function<void()> shareCallback;
    std::function<void(int)> uiChangeCallback;

    //
    std::function<void()> startVideoCallback;
    std::function<void()> finishVideoCallback;
    std::function<void()> shareVideocallback;

    int currAni;
    
    float scaleFactor;

    Menu *startBtn, *playBtn, *shareBtn, *videoShareBtn, *retryBtn, *closeBtn;
    Label *resultLabel;

    spine::SkeletonAnimation *sNode;
    spine::SkeletonAnimation *lNode;
    
    ParticleSystemQuad *partyPtcl[3];

    void initMainLayer();
    void setMachineSkin();
    void initParticles();
    void setMainResultType();
    void btnCallback(Ref* pSender);
    
    void startMachine();
    void processToStartMachine();
    void showResult();
    
    void initToRedrawSkin() {
        sNode->setVisible(true);
        sNode->setAnimation(0, "idle", true);

        //
        setMachineSkin();

        startBtn->setVisible(true);
        hideResultButtons();
        
        //
//        enableLayerTouch();
    }
    void hideResultButtons() {
        playBtn->setVisible(false);
        shareBtn->setVisible(false);
        MenuItemImage *btn = (MenuItemImage*)shareBtn->getChildByTag(ImageShareBtnTag);
        if (btn) {
            btn->setTag(ShareBtnTag);
        }
        videoShareBtn->setVisible(false);
        retryBtn->setVisible(false);
        closeBtn->setVisible(false);
        resultLabel->setVisible(false);
    }
    
    void removeFreeshSkin() {
        //
        removeChildByTag(ResultFreeshNameTag);
        removeChildByTag(ResultFreeshTag);
    }
    
    void startCaptureAndShare(MenuItemImage *btn) {
        videoShareBtn->setVisible(false);
        btn->setTag(ShareBtnTag);
        if (shareCallback) {
            shareCallback();
        }
    }

    void playThis();
    void hideThis();

    //
//    void enableLayerTouch();
//    void disableLayerTouch();
};

#endif /* defined(__Timefish__FreeshMakerLayer__) */
