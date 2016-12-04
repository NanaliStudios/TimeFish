//
//  UILayer.h
//  Timefish
//
//  Created by midchow on 2015. 1. 21..
//
//

#ifndef __Timefish__UILayer__
#define __Timefish__UILayer__

#include "cocos2d.h"
#include "Global.h"

#include "StoreKitX.h"

#include "UserInfo.h"
#include "SoundManager.h"

#include "GameCenterX.h"
#include "FlurryX.h"
#include "SocialX.h"
#include "AdColonyX.h"
#include "UnityAdsX.h"

#include "VideoShareX.h"

#include "ScoreLayer.h"
#include "MenuItemSameImage.h"
#include "Popup.h"

#include "ResultMainLayer.h"
#include "ResultUIButtonLayer.h"

#include "CreditLayer.h"
#include "TutorialLayer.h"

#include "FreeshSelectionLayer.h"
#include "AchievementSelectionLayer.h"
#include "FreeGiftLayer.h"
#include "FreeshMakerLayer.h"

#include "PopupManager.h"

#include "ShakeAction.h"

#include "MenuItemImageButton.h"

#include "RibbonLayer.h"

//
#include "OptionLayer.h"

typedef enum {
    MainUIStatusMain,
    MainUIStatusHiddenPopup,
    MainUIStatusTutorial,
    MainUIStatusFreesh,
    MainUIStatusAchievement,
    MainUIStatusPoster,
    MainUIStatusPlay,
    MainUIStatusPause,
    MainUIStatusResult,
    MainUIStatusDraw,
    MainUIStatusGift,
    MainUIStatusOption,
    MainUIStatusCredit,
    MainUIStatusWatchingAd,
    MainUIStatusProcessPurchase,
    MainUIStatusContinueAd,
} MainUIStatus;

USING_NS_CC;
using namespace cocos2d::ui;

class UILayer : public Layer, public StoreKitXDelegate, public AdColonyXDelegate, public SocialXDelegate, public TapjoyXDelegate, public UnityAdsXDelegate
{
public:
    virtual bool init();
    
    CREATE_FUNC(UILayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;

    FreeshSelectionLayer *freeshSelection;
    AchievementSelectionLayer *achSelection;
    
    FreeGiftLayer *freeGift;
    FreeshMakerLayer *freeshMaker;
    
    ResultMainLayer *resultMain;
    ResultUIButtonLayer *resultButtons;
    
    ScoreLayer *scoreLabel;

    PopupManager *popups;

    bool readyToGetOut;

    void initLayers();

    void hideStartUI();
    void showInGameUI();
    
    void startResultOutro();
    void hideInGameUI();

    void showNewLapUI(float posY, int type, int lap);
    
    void showResultMain();
    void showResultUIButtons();
    
    void setColorInformation(int colorIdx);
    
    void setReloadSceneCallback(const std::function<void(Ref*)>& _reloadSceneCallback) {
        reloadSceneCallback = _reloadSceneCallback;
    }
    void setPauseSceneCallback(const std::function<void(Ref*)>& _pauseSceneCallback) {
        pauseSceneCallback = _pauseSceneCallback;
    }
    void setResumeSceneCallback(const std::function<void(Ref*)>& _resumeSceneCallback) {
        resumeSceneCallback = _resumeSceneCallback;
    }
    void setFreeshSkinCallback(const std::function<void()>& _freeshSkinCallback) {
        freeshSkinCallback = _freeshSkinCallback;
    }
    void setUIChangeCallback(const std::function<void(int)>& _uiChangeCallback) {
        uiChangeCallback = _uiChangeCallback;
    }
    void setStartGameCallback(const std::function<void()>& _startGameCallback) {
        startGameCallback = _startGameCallback;
    }
    void setNotContinueCallback(const std::function<void()>& _notContinueCallback) {
        notContinueCallback = _notContinueCallback;
    }
    void setContinueCallback(const std::function<void()>& _continueCallback) {
        continueCallback = _continueCallback;
    }
    
    void forceToPauseGame();

    void updateCoinLabel();
    void updateComboCountLabel(int cnt, bool scoreBoosted = false);
    void comboCountAction(Ref *pSender);
    

    bool exitPopuped;

    bool visitFacebook;

    void setGameStatus(Status _s) {
        status = _s;
    }
    
    int checkHiddenSkin();
    void showHiddenSkinPopups(float dt);
    
    void showTutorial(Ref *pSender);

    void updateUIAfterWatchingAd();
    void showReward(float dt);

    void showVideoReward();
    
    void showContinuePopup();
protected:
    MainUIStatus uiStatus;
    void setUIStatus(MainUIStatus _stat) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        if (_stat == MainUIStatusMain ||
            _stat == MainUIStatusPause ||
            _stat == MainUIStatusResult ||
            _stat == MainUIStatusPlay) {
            enableKeyInput();
        }
        else {
            disableKeyInput();
        }
#endif
        
        uiStatus = _stat;
//        log("MainUIStatus: %d", uiStatus);
    }

    Layer *mainLayer;
    Layer *blackLayer;
    
    Status status;

    bool paused;
    bool resultBtnDisabled; // to prevent double touch

    int currScore;

    Color3B colors[3];

    Label *newLapLabel, *comboCountLabel, *currCoinsLabel;
    Sprite *gameTitle, *coinIcon, *comboCountUI, *comboDots[5];
    MenuItemSameImage *pauseItem;

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    RibbonLayer *ribbon;
#endif
    Menu *freeshSelectionBtn, *posterSelectionBtn;
    Menu *pauseBtn, *quitBtn, *resumeBtn;

    PopupBase *emptyPopup;
    ContinuePopup *cPopup;

    OptionLayer *optionLayer;
    float iconSpace;
    
    int currCombo;
    
    SkeletonAnimation *fingerNode;

    std::function<void(Ref*)> reloadSceneCallback;
    std::function<void(Ref*)> pauseSceneCallback;
    std::function<void(Ref*)> resumeSceneCallback;
    std::function<void()> freeshSkinCallback;
    std::function<void(int)> uiChangeCallback;
    std::function<void()> startGameCallback;
    std::function<void()> notContinueCallback;
    std::function<void()> continueCallback;


    void initUILayer();
    void initSubLayers();
    void initMainButtons();
    void initInGameUI();
    void initPauseLayer();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    virtual void onKeyReleased( EventKeyboard::KeyCode keycode, Event *event );
    void enableKeyInput();
    void disableKeyInput();
    void setGoogleGameServiceLogin(Ref *pSender);
#endif
    void onCancelCallback();

    //
    void showFreeshSelection();
    void showFreeshSelectionCallback(Ref *pSender);
    void showPosterSelectionCallback(Ref *pSender);

    void hideFreeshSelection(Ref *pSender);
    void hidePosterSelection(Ref *pSender);
    void hideOptionLayer(Ref *pSender);

    //
//    void processToFacebook(Ref *pSender);
    void setSound(Ref* pSender);
    void setNotification(Ref* pSender);
    void restorePurchasedItems(Ref *pSender);
    void openCredit(Ref* pSender);

//    void setAd(Ref* pSender);
    void pauseGame(Ref* pSender);
    void quitGame(Ref* pSender);
    void resumeGame(Ref* pSender);
//    void shareGame(Ref* pSender);
//    void rateGame(Ref* pSender);

    void hideFreeshPurchaseResult();
    void afterPurchaseFreeshSkin(int skinIdx, bool restored);

    void tryFreeshSkin();
    void replayGame();
    
    void showOptionLayer();
    void showGameCenterLeaderboard();
    
    void pauseAllSound(int id, const std::string& filePath);
    void stopAllSound(int id, const std::string& filePath);

    void afterCapturedResult(bool succeed, const std::string& outputFile);
    void afterCapturedFreesh(bool succeed, const std::string& outputFile);

    //
    void processSkinPurchaseFromTrial();
    void processSkinPurchaseFromResult();
    void processSkinPurchase(int skinIdx);

    void processPurchasePackage();

    void processPurchaseRemoveAds();
    void finishPurchase(bool purchased);
    void finishRestore(bool success);
    void finishRestoreAll(bool success);

    void moveToArtworkPage();

    //
    void tutorialCloseCallback();
    void creditCloseCallback();

    void showFreeshMaker();
    void showGiftLayer();
    void showVideoLayer();
    void showVideo();
    void rateGame();
    void processToFacebookPage(Ref *pSender);
    void processToFacebook();
    
    void shareGameToVideo();
//    void shareGame();
    void shareGameScore();
    void shareGameFreesh();


    void hideGiftLayer();
    void hideFreeshMaker();
    
    //
    void setMainUIVisible(bool flag);
    void setResultUIVisible(bool flag, bool withUIUpdate = true);
    void setResultUIForSkinPurchase();
    
    //
    void callUiChangeCallback(int _type);
    void showCoinsCallback(int _type);

private:
    bool ownsPriority;
    
    bool finishPurchaseValue;
    void processFinishPurchase(float dt);
    bool finishRestoreValue;
    void processFinishRestore(float dt);

    ParticleSystemQuad *coinPtcl;
    void initParticles();
    void showCoins(int giftType);

    void showEmptyPopup(bool flag);

    void showGoldenFishInfoPopup();
    
    //
    // SocialXDelegate
    //
    void onFinishLoading();
    
    //
    // UnityAdsXDelegate
    //
    void onUnityAdsStart();
    void onUnityAdsFinish(bool shown);

    //
    // AdColonyXDelegate
    //
    void onAdColonyAdStartedInZone(const char* zoneID);
    void onAdColonyAdAttemptFinished(bool shown, const char* zoneID);
    void onAdColonyAdAvailabilityChange(bool available, const char* zoneID);
    
    //
    // TapjoyXDelegate methods
    //
    void requestDidSucceed();
    void requestDidFail();
    void contentIsReady(const char *placementName);
    void contentDidAppear();
    void contentDidDisappear(const char *placementName);
};

#endif /* defined(__Timefish__UiLayer__) */
