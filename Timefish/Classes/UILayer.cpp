//
//  UILayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 1. 21..
//
//

#include "UILayer.h"

#include <iostream>
#include <iomanip>
#include <string>

bool UILayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();

    //
    ownsPriority = true;
    paused = false;
    readyToGetOut = false;
    exitPopuped = false;
    visitFacebook = false;
    resultBtnDisabled = false;

    //
    setUIStatus(MainUIStatusMain);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    enableKeyInput();
#endif
    
    //
    SocialX::getInstance()->setDelegate(this);
    
    //
    UnityAdsX::getInstance()->setDelegate(this);

    //
    AdColonyX::getInstance()->setDelegate(this);

    //
    TapjoyX::getInstance()->setDelegate(this);
    
    TapjoyX::getInstance()->requestPlacement("App Launch");

    StoreKitX::getInstance()->setDelegate(this);

    initUILayer();
    initInGameUI();

    return true;
}

#define TopPosRate 0.86363636363 // (1 - 600/4400)
#define BtmPosRate 0.15636363636 // 1 - TopPosRate
void UILayer::initUILayer()
{
    emptyPopup = PopupBase::create();
    emptyPopup->setEnbleTouch(true);
    addChild(emptyPopup, 10000);


    // title
    gameTitle = Sprite::createWithSpriteFrameName("TF_logo_shadow.png");
    gameTitle->setPosition(origin + Vec2(visibleSizeHalf.width, visibleSize.height * TopPosRate));
    gameTitle->setScale(0.9);
    addChild(gameTitle, 1);

    //
    fingerNode = SkeletonAnimation::createWithData(SkeletonHolder::getInstance()->fingerSkeleton);
    fingerNode->setDebugSlotsEnabled(false);
    fingerNode->setPosition(Vec2(visibleSizeHalf.width * 1.1, visibleSize.height * 0.26) + origin);
    fingerNode->setAnimation(0, "finger", true);
    fingerNode->setRotation(-25);
    addChild(fingerNode, 10);

    //
    float scale = 0.3;
    coinIcon = Sprite::createWithSpriteFrameName("coin_big.png");
    Size s = coinIcon->getContentSize();
    coinIcon->setPosition(origin + Vec2(visibleSize) - Vec2(s) * 0.75 * scale);
    coinIcon->setScale(scale);
    addChild(coinIcon);

    //
    currCoinsLabel = Label::createWithTTF("0", NumberFontPath, 40);
    currCoinsLabel->setPosition(Vec2(coinIcon->getPositionX() - s.width* scale * 0.65, coinIcon->getPositionY()));
    currCoinsLabel->setAnchorPoint(Vec2(1, 0.5));
    addChild(currCoinsLabel, 1);
    
    updateCoinLabel();

    //
    initMainButtons();

    //
    initSubLayers();
    
    //
    initParticles();
}

void UILayer::setColorInformation(int colorIdx)
{
    colors[0] = getFreeshColor(colorIdx, ColorOrderFirst);
    colors[1] = getFreeshColor(colorIdx, ColorOrderSecond);
    colors[2] = getFreeshColor(colorIdx, ColorOrderThird);
}

void UILayer::initParticles()
{
    //
    coinPtcl = ParticleSystemQuad::create("particle/coin_rain_particle/coin_particle.plist");
    coinPtcl->setPosition(Vec2(visibleSizeHalf.width, visibleSize.height * 1.4) + origin);
    coinPtcl->stopSystem();
    addChild(coinPtcl, 500);
}

void UILayer::showCoins(int giftType)
{
    //
    if (giftType < 2) {
        SoundManager::getInstance()->playSoundEffect(SoundCoinRain1, false);
    }
    else if(giftType < 4) {
        SoundManager::getInstance()->playSoundEffect(SoundCoinRain2, false);
    }
    else {
        SoundManager::getInstance()->playSoundEffect(SoundCoinRain3, false);
    }

    //
    int eRate[] = {1000, 2000, 2000, 3500, 7000};
    coinPtcl->setEmissionRate(eRate[giftType]);
    coinPtcl->resetSystem();
}

void UILayer::initMainButtons()
{
    // Freesh Selection Button
    {
        auto menuItem = MenuItemImageButton::create();
        menuItem->setNormalImage(Sprite::createWithSpriteFrameName("button_long_white.png"));
        menuItem->setSelectedImage(Sprite::createWithSpriteFrameName("button_long_white_click.png"));
        menuItem->setCallback(CC_CALLBACK_1(UILayer::showFreeshSelectionCallback, this));
        Size s = menuItem->getContentSize();
        
        freeshSelectionBtn = Menu::create(menuItem, NULL);
        freeshSelectionBtn->setPosition(Vec2(s.width * 0.565, s.height * 0.6) + origin);
        addChild(freeshSelectionBtn, 1);
        
        //
        auto icon = Sprite::createWithSpriteFrameName("button_icon_skin.png");
        icon->setPosition(Vec2(s.width, s.height) * 0.5);
        menuItem->addChild(icon);

        //
        menuItem->setLabelChild(icon);
    }

    // Poster Selection Button
    {
        auto menuItem = MenuItemImageButton::create();
        menuItem->setNormalImage(Sprite::createWithSpriteFrameName("button_long_white.png"));
        menuItem->setSelectedImage(Sprite::createWithSpriteFrameName("button_long_white_click.png"));
        menuItem->setCallback(CC_CALLBACK_1(UILayer::showPosterSelectionCallback, this));
        Size s = menuItem->getContentSize();
        
        posterSelectionBtn = Menu::create(menuItem, NULL);
        posterSelectionBtn->setPosition(Vec2(visibleSize.width - s.width * 0.565, s.height * 0.6) + origin);
        addChild(posterSelectionBtn, 1);
        
        //
        auto icon = Sprite::createWithSpriteFrameName("button_icon_achievement.png");
        icon->setPosition(Vec2(s.width, s.height) * 0.5);
        menuItem->addChild(icon);

        //
        menuItem->setLabelChild(icon);
    }
}

void UILayer::initSubLayers()
{
    //
    freeshSelection = FreeshSelectionLayer::create();
    freeshSelection->initMainLayer();
    freeshSelection->setCloseCallback(CC_CALLBACK_1(UILayer::hideFreeshSelection, this));
    freeshSelection->setPurchaseCallback(CC_CALLBACK_1(UILayer::processSkinPurchase, this));
    freeshSelection->setShareCallback(CC_CALLBACK_0(UILayer::shareGameFreesh, this));
    freeshSelection->setGoldenfishCallback(CC_CALLBACK_0(UILayer::showGoldenFishInfoPopup, this));
    freeshSelection->hideLayer();//setVisible(false);
    addChild(freeshSelection);
    
    //
    achSelection = AchievementSelectionLayer::create();
    achSelection->setCloseCallback(CC_CALLBACK_1(UILayer::hidePosterSelection, this));
    achSelection->setVisible(false);
    addChild(achSelection);

    //
    //
    //
    resultMain = ResultMainLayer::create();
    resultMain->setCascadeOpacityEnabled(true);
    resultMain->setLayerVisible(false);
    addChild(resultMain);
    
    //
    resultMain->setButtonCallbacks(CC_CALLBACK_0(UILayer::showGiftLayer, this),
                                   CC_CALLBACK_0(UILayer::showFreeshMaker, this),
                                   CC_CALLBACK_0(UILayer::shareGameScore, this));
    //
    resultButtons = ResultUIButtonLayer::create();
    resultButtons->setVisible(false);
    std::function<void()> finishIntrocallback = [this]() {
        //
        VideoShareX::getInstance()->stopRecording();
        
        //
        int totalPlayCount = UserInfo::getInstance()->getTotalPlayCount();
        int _currScore = UserInfo::getInstance()->getCurrScore();
        int _bestScore = UserInfo::getInstance()->getBestScore();
        if (totalPlayCount>3 && _currScore == _bestScore) {
            BestScorePopup *p = BestScorePopup::create();
            p->setShareCallback(CC_CALLBACK_0(UILayer::shareGameScore, this));
            p->setScoreInfo(_bestScore);
            p->runPopupEffect();
            addChild(p, 1000);
        }
    };
    std::function<void()> shareVideocallback = [this]() {
        //
        VideoShareX::getInstance()->showView();
    };
    resultButtons->initMainLayerWithCallbacks(CC_CALLBACK_0(UILayer::showOptionLayer, this),
                                              CC_CALLBACK_0(UILayer::showGameCenterLeaderboard, this),
                                              CC_CALLBACK_0(UILayer::replayGame, this),
                                              CC_CALLBACK_0(UILayer::shareGameScore, this),
                                              finishIntrocallback);
    addChild(resultButtons);

    //
    freeGift = FreeGiftLayer::create();
    freeGift->setCloseCallback(CC_CALLBACK_0(UILayer::hideGiftLayer, this));
    freeGift->setShowCoinsCallback(CC_CALLBACK_1(UILayer::showCoinsCallback, this));
    freeGift->hideLayer();
    addChild(freeGift);

    //
    freeshMaker = FreeshMakerLayer::create();
    freeshMaker->setReplayCallback(CC_CALLBACK_0(UILayer::replayGame, this));
    freeshMaker->setUiChangeCallback(CC_CALLBACK_1(UILayer::callUiChangeCallback, this));
    freeshMaker->setShareCallback(CC_CALLBACK_0(UILayer::shareGameFreesh, this));

    std::function<void()> finishRecordingcallback = [this]() {
        //
        VideoShareX::getInstance()->stopRecording();
    };
    std::function<void()> startVideocallback = [this]() {
        //
        if (UserInfo::getInstance()->isRecordingEnabled()) {
            VideoShareX::getInstance()->startRecording();
        }
    };
    freeshMaker->setVideoCallbacks(startVideocallback, finishRecordingcallback, shareVideocallback);
    freeshMaker->hideLayer();
    addChild(freeshMaker);

    //
    optionLayer = OptionLayer::create();
    optionLayer->setVisible(false);
    optionLayer->setButtonCallbacks(CC_CALLBACK_1(UILayer::setSound, this),
                                    CC_CALLBACK_1(UILayer::setNotification, this),
//                                    CC_CALLBACK_1(UILayer::restorePurchasedItems, this),
                                    CC_CALLBACK_1(UILayer::showTutorial, this),
//                                    CC_CALLBACK_1(UILayer::processToFacebookPage, this),
                                    CC_CALLBACK_1(UILayer::openCredit, this),
                                    CC_CALLBACK_1(UILayer::hideOptionLayer, this));
    addChild(optionLayer);
}

void UILayer::showGoldenFishInfoPopup()
{
    blackLayer = LayerColor::create(Color4B(0,0,0,160));
    addChild(blackLayer, 10);

    //
    std::function<void()> closeCallback = [this]() {
        //
        blackLayer->removeFromParent();
    };

    //
    GoldenfishInfoPopup *p = GoldenfishInfoPopup::create();
    p->setPurchaseCallback(CC_CALLBACK_1(UILayer::processSkinPurchase, this));
    p->setCloseCallback(closeCallback);
    addChild(p, 1000);
}

void UILayer::callUiChangeCallback(int _type)
{
    if (uiChangeCallback) {
        uiChangeCallback(_type);
    }
}

void UILayer::showCoinsCallback(int _type)
{
    showCoins(_type);
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void UILayer::onKeyReleased( EventKeyboard::KeyCode keycode, Event *event )
{
//    log("MainUIStatus: UILayer::onKeyReleased, %d, %d, %d", ownsPriority, status, uiStatus);
    if (ownsPriority && keycode == EventKeyboard::KeyCode::KEY_BACK) {
        if (status == StatusReady) {
            
            if (uiStatus == MainUIStatusMain) {
                //
                // Exit popup
                //
                exitPopuped = true;
                
                UserInfo::getInstance()->saveUserData();
                SocialX::getInstance()->showExitPopup();
            }
        }
        else if (status == StatusGameOver && readyToGetOut) {
            //
            // Reload game
            //
            quitGame(NULL);
        }
        else if (status == StatusNone || status == StatusJump || status == StatusPause) {
            //
            // In-Game
            //
            if (paused) {
                resumeGame(NULL);
            }
            else {
                pauseGame(NULL);
            }
        }

        //
        event->stopPropagation();
    }
}

void UILayer::enableKeyInput()
{
    // set keyInput enabled
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(UILayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void UILayer::disableKeyInput()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}
#endif

void UILayer::onCancelCallback()
{
    exitPopuped = false;
}

void UILayer::initPauseLayer()
{
    blackLayer = LayerColor::create(Color4B(0,0,0,160));
    addChild(blackLayer, 10);

    // quit button
    {
        auto menuItem = MenuItemSameImage::create("TF_UI_Out.png", CC_CALLBACK_1(UILayer::quitGame, this));
        Size s = menuItem->getContentSize();
        menuItem->setPosition(Vec2(visibleSizeHalf.width - s.width, visibleSizeHalf.height) + origin);
        
        quitBtn = Menu::create(menuItem, NULL);
        quitBtn->setPosition(Vec2::ZERO);
        blackLayer->addChild(quitBtn, 50);
    }
    
    // resume button
    {
        auto menuItem = MenuItemSameImage::create("TF_UI_Play.png", CC_CALLBACK_1(UILayer::resumeGame, this));
        Size s = menuItem->getContentSize();
        menuItem->setPosition(Vec2(visibleSizeHalf.width + s.width, visibleSizeHalf.height) + origin);
        
        resumeBtn = Menu::create(menuItem, NULL);
        resumeBtn->setPosition(Vec2::ZERO);
        blackLayer->addChild(resumeBtn, 50);
    }
}

void UILayer::hideStartUI()
{
    //
    gameTitle->setVisible(false);
    fingerNode->setVisible(false);

    freeshSelectionBtn->setVisible(false);
    posterSelectionBtn->setVisible(false);

    //
    freeshSelection->removeFromParentAndCleanup(true);
    freeshSelection= NULL;
    achSelection->removeFromParentAndCleanup(true);
    achSelection = NULL;
}

void UILayer::initInGameUI()
{
    //
    pauseItem = MenuItemSameImage::create("TF_UI_Stop.png", CC_CALLBACK_1(UILayer::pauseGame, this));
    Size s = Size(40, 43);//pauseItem->getContentSize();
    pauseItem->setPosition(Vec2::ZERO);
    //
    // NOTE:
    // To expand the touch area,
    // Make Rect rect() method a virtual member fuction!
    // (changed cocos2d-x engine code : CCMenuItem.h !!!)
    // And set touchAreaScale variable by following,
    // and multiply it within rect function.
    //
    pauseItem->setTouchAreaScale(3);

    pauseBtn = Menu::create(pauseItem, NULL);
    pauseBtn->setPosition(Vec2(s.width * 0.5 + 20, visibleSize.height - s.height* 0.5 - 20) + origin);
    pauseBtn->setVisible(false);
    addChild(pauseBtn, 1);
    
    //
    scoreLabel = ScoreLayer::create();
    scoreLabel->setLabelPositionY(pauseBtn->getPositionY(),
                                  visibleSize.height * 0.85);
//                                  (coinIcon->getPositionY() - visibleSizeHalf.height - 270) * 0.5 + visibleSizeHalf.height + 270);
    scoreLabel->setVisible(false);
    scoreLabel->setInGameUIPosition();
    addChild(scoreLabel);

    //
    newLapLabel = Label::createWithTTF("0", NumberFontPath, 85);
    newLapLabel->setVisible(false);
    addChild(newLapLabel, 1);

    //
    // Combo UI
    //
    float comboUiPosY = scoreLabel->getPositionY() - scoreLabel->getLabelSize().height;
//    float comboUiPosY = scoreLabel[4]->getPositionY() - scoreLabel[4]->getContentSize().height;
    float deltaX = 30;
    float dotPosX[5] = { -3, -2, -1, 0, 1};
    for (int i=0; i<5; i++) {
        comboDots[i] = Sprite::createWithSpriteFrameName("TF_timeposter_comma.png");
        comboDots[i]->setPosition(Vec2(visibleSizeHalf.width + deltaX*dotPosX[i], comboUiPosY));
        comboDots[i]->setVisible(false);
        comboDots[i]->setScale(0.5);
        comboDots[i]->setOpacity(125);
        addChild(comboDots[i]);
    }

    //
    comboCountUI = Sprite::createWithSpriteFrameName("TF_UI_mult.png");
    comboCountUI->setAnchorPoint(Vec2(1, 0.5));
    comboCountUI->setScale(0.75);
    comboCountUI->setPosition(Vec2(comboDots[4]->getPositionX() + comboCountUI->getContentSize().width * 1.2, comboUiPosY));
    comboCountUI->setVisible(false);
    addChild(comboCountUI, 1);

    //
    comboCountLabel = Label::createWithTTF("0", NumberFontPath, 43);
    comboCountLabel->setPosition(Vec2(comboCountUI->getPositionX() + 5, comboCountUI->getPositionY()));
    comboCountLabel->setAnchorPoint(Vec2(0, 0.5));
    comboCountLabel->setVisible(false);
//    addChild(comboCountLabel, 1);
    cocos2d::Node* _target = NodeGrid::create();
    _target->addChild(comboCountLabel);
    addChild(_target, 1);
}

void UILayer::showInGameUI()
{
    //
    setUIStatus(MainUIStatusPlay);

    //
    pauseBtn->setVisible(true);
    scoreLabel->setVisible(true);
    coinIcon->setVisible(true);

    comboCountLabel->setVisible(true);
    comboCountUI->setVisible(true);
    for (int i=0; i<5; i++) {
        comboDots[i]->setVisible(true);
    }

    //
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    enableKeyInput();
//#endif
}

void UILayer::startResultOutro()
{
    currScore = UserInfo::getInstance()->getCurrScore();

    //
    // Start Ending scene
    //
    int _currScore = UserInfo::getInstance()->getCurrScore();
    int _bestScore = UserInfo::getInstance()->getBestScore();
    scoreLabel->setResultUIPosition(_currScore, _bestScore);

    //
    hideInGameUI();
}

void UILayer::hideInGameUI()
{
    //
    pauseBtn->setVisible(false);
//    for (int i=0; i<6; i++) {
//        scoreLabel[i]->setVisible(false);
//    }

    comboCountLabel->setVisible(false);
    comboCountUI->setVisible(false);
    for (int i=0; i<5; i++) {
        comboDots[i]->setVisible(false);
    }
    
    //
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    disableKeyInput();
//#endif

}

void UILayer::showNewLapUI(float posY, int type, int lap)
{
    newLapLabel->setVisible(true);
    newLapLabel->setOpacity(255);
    newLapLabel->setString(std::to_string(lap));
    newLapLabel->setScale(0);
    newLapLabel->setColor(colors[type]);
    newLapLabel->setPosition(Vec2(visibleSizeHalf.width, posY + newLapLabel->getContentSize().height));

    auto bounce = Repeat::create(Sequence::create(
                                                  ScaleTo::create(0.2, 1),
                                                  ScaleTo::create(0.2, 1.2),
                                                  NULL), 2);

    //
    auto seq = Sequence::create(
                   Spawn::create(
                         MoveBy::create(0.1, Vec2(0, newLapLabel->getContentSize().height * 0.3)),
                         ScaleTo::create(0.1, 1.2),
                         NULL),
                    bounce,
                    FadeOut::create(0.4),
                    NULL);
    newLapLabel->runAction(seq);
}

void UILayer::updateCoinLabel()
{
    currCoinsLabel->setString(std::to_string(UserInfo::getInstance()->getCoins()));
}

void UILayer::comboCountAction(Ref *pSender)
{
    Sprite *comboDot = (Sprite*)pSender;
    auto seq = Sequence::create(
                                ScaleTo::create(0.2, 1),
                                ScaleTo::create(0.2, 0.5),
                                NULL);
    comboDot->runAction(RepeatForever::create(seq));
}

void UILayer::updateComboCountLabel(int cnt, bool scoreBoosted)
{
    if (currCombo == cnt) {
        return;
    }

    currCombo = cnt;

    //
    // Combo Count Label
    //
    comboCountLabel->setString(std::to_string(cnt));

    //
    // Depending on scoreBoosted, set actions
    //
    if (scoreBoosted) {
        comboCountLabel->stopAllActions();
        comboCountLabel->setScale(1.5);
        comboCountLabel->setColor(Color3B::RED);

        //
        // Note: Shaky3D is toooooo slow
        //
        cocos2d::Node* _target = comboCountLabel->getParent();
//        auto shaky = Shaky3D::create(5, Size(15,10), 4, false);
//        _target->runAction(RepeatForever::create(shaky));
        
        auto shaky = Shake::actionWithDuration(0.2, 4);
        _target->runAction(RepeatForever::create(shaky));

        //
        for (int i=0; i<5; i++) {
            comboDots[i]->setColor(Color3B::RED);
        }
        comboCountUI->setColor(Color3B::RED);
    }
    else {
        if (cnt == 1) {
            comboCountLabel->stopAllActions();
            comboCountLabel->setScale(1.0);
            comboCountLabel->setColor(Color3B::WHITE);
            
            //
            cocos2d::Node* _target = comboCountLabel->getParent();
            _target->stopAllActions();
            
            //
            for (int i=0; i<5; i++) {
                comboDots[i]->setColor(Color3B::WHITE);
            }
            comboCountUI->setColor(Color3B::WHITE);
        }
        else if (cnt > 1) {
            auto callback = CallFunc::create( [this]() {
                auto bounce = RepeatForever::create(Sequence::create(
                                                                     ScaleTo::create(0.2, 1),
                                                                     ScaleTo::create(0.2, 1.2),
                                                                     NULL));
                comboCountLabel->runAction(bounce);
            });
            auto scaleSeq = Sequence::create(ScaleTo::create(0, 1),
                                             ScaleTo::create(0.1, 1.5),
                                             callback,
                                             NULL);
            
            //
            comboCountLabel->stopAllActions();
            comboCountLabel->runAction(scaleSeq);
        }
    }
    
    //
    // Combo Dots
    //
    float delay = 0;
    float delayDelta = 0.05;
    int idxMax = MIN(cnt, 5);
    for (int i=0; i<idxMax; i++) {
        comboDots[i]->setOpacity(255);
//        comboDots[i]->setScale(1);
        
        //
        comboDots[i]->runAction(Sequence::create(
                                                 DelayTime::create(delay),
                                                 CallFuncN::create(CC_CALLBACK_1(UILayer::comboCountAction, this)),
                                                 NULL));
        delay += delayDelta;
    }
    for (int i=cnt; i<5; i++) {
        comboDots[i]->setOpacity(125);
        comboDots[i]->setScale(0.5);
        comboDots[i]->stopAllActions();
    }
}

void UILayer::showResultMain()
{
    resultMain->initMainLayer(resultButtons->getPosY());
    resultMain->setLayerVisible(true);

    //
    //
    //
    resultMain->startIntroAction();
//    resultMain->runAction(FadeIn::create(0.5));
}

void UILayer::setResultUIVisible(bool flag, bool withUIUpdate)
{
    scoreLabel->setVisible(flag);

    currCoinsLabel->setVisible(flag);
    coinIcon->setVisible(flag);
    
    resultMain->setLayerVisible(flag);
    if (flag && withUIUpdate) {
        resultMain->updateMainLayer();
    }

    resultButtons->setVisible(flag);
}

void UILayer::setResultUIForSkinPurchase()
{
    scoreLabel->setVisible(false);
    
    currCoinsLabel->setVisible(true);
    coinIcon->setVisible(true);

    resultMain->setLayerVisible(false);
    freeshMaker->setVisible(true);
    
    resultButtons->setVisible(false);
}

void UILayer::showFreeshMaker()
{
    //
    setUIStatus(MainUIStatusDraw);

    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    setResultUIVisible(false);

    freeshMaker->setCloseCallback(CC_CALLBACK_0(UILayer::hideFreeshMaker, this));
    freeshMaker->showLayer();

    //
    if (uiChangeCallback) {
        uiChangeCallback(1);
    }
}

void UILayer::showGiftLayer()
{
    //
    setUIStatus(MainUIStatusGift);

    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    setResultUIVisible(false);

    //
    int giftType = 0;

    int amount = UserInfo::getInstance()->setGiftAmount(giftType);
    freeGift->showLayer(giftType, amount);

    //
    updateCoinLabel();

    //
    if (uiChangeCallback) {
        uiChangeCallback(1);
    }
}

void UILayer::showVideoLayer()
{
    showEmptyPopup(true);

    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    UserInfo::getInstance()->setHaveSeenVideo(true);

    //
    // video watching: UnityAds has a priority!!!
    //
    if (UnityAdsX::getInstance()->isVideoAvailable()) {
        uiStatus = MainUIStatusWatchingAd;
        UnityAdsX::getInstance()->showVideo();
    }
    else if (AdColonyX::getInstance()->isVideoAvailable()) {
        uiStatus = MainUIStatusWatchingAd;
        AdColonyX::getInstance()->showVideo();
    }
    else if (TapjoyX::getInstance()->isVideoReady()) {
        uiStatus = MainUIStatusWatchingAd;
        TapjoyX::getInstance()->showPlacement("VideoRewardPlacement");
    }
}

void UILayer::showResultUIButtons()
{
    //
    setUIStatus(MainUIStatusResult);

    readyToGetOut = true;

    resultButtons->setVisible(true);
    resultButtons->showButtons();
}

void UILayer::setMainUIVisible(bool flag)
{
    gameTitle->setVisible(flag);
    fingerNode->setVisible(flag);
//    tapLabel->setVisible(flag);
    currCoinsLabel->setVisible(flag);
    coinIcon->setVisible(flag);

    freeshSelectionBtn->setVisible(flag);
    posterSelectionBtn->setVisible(flag);
}

void UILayer::showFreeshSelection()
{
    //
    setMainUIVisible(false);

    //
    setUIStatus(MainUIStatusFreesh);

    //
    freeshSelection->initScrolls();
    freeshSelection->showLayer();

    //
    if (uiChangeCallback) {
        uiChangeCallback(2);
    }
}

void UILayer::showFreeshSelectionCallback(Ref *pSender)
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    //
    TapjoyX::getInstance()->logEventInUIFlow("SkinList");

    //
    showFreeshSelection();
}

void UILayer::hideFreeshSelection(Ref *pSender)
{
    Node *sender = (Node*)pSender;
    
    int tag = sender->getTag();
    
    //
    // close popup
    //
    if (tag == 0) {
        //
        setUIStatus(MainUIStatusMain);

        //
        setMainUIVisible(true);

        //
        freeshSelection->destroyScrolls();
        freeshSelection->hideLayer();

        //
        if (uiChangeCallback) {
            uiChangeCallback(0);
        }
    }
    //
    // reload scene
    //
    else {
        if (freeshSkinCallback) {
            freeshSkinCallback();
        }
    }
}

void UILayer::showPosterSelectionCallback(Ref *pSender)
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    //
    TapjoyX::getInstance()->logEventInUIFlow("AchievementList");

    //
    setMainUIVisible(false);

    //
    ownsPriority = false;
    achSelection->showThis();

    //
    if (uiChangeCallback) {
        uiChangeCallback(2);
    }
}

void UILayer::hidePosterSelection(Ref *pSender)
{
    //
    setMainUIVisible(true);

    //
    ownsPriority = true;
    achSelection->setVisible(false);

    //
    if (uiChangeCallback) {
        uiChangeCallback(0);
    }
}

void UILayer::hideGiftLayer()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    enableKeyInput();
#endif

    //
    setResultUIVisible(true);
    
    //
    freeGift->hideLayer();

    //
    if (uiChangeCallback) {
        uiChangeCallback(3);
    }
}

void UILayer::hideFreeshMaker()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    enableKeyInput();
#endif

    //
    freeshMaker->hideLayer();

    //
    setResultUIVisible(true);

    //
    updateCoinLabel();

    //
    if (uiChangeCallback) {
        uiChangeCallback(4);
    }
}

void UILayer::processToFacebookPage(Ref *pSender)
{
    //
    FlurryX::getInstance()->logEvent("FacebookFanPage");
    
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    SocialX::getInstance()->openFacebookPage();
}

void UILayer::processToFacebook()
{
    visitFacebook = true;

    //
    // Assume user liked the facebook page
    //
    UserInfo::getInstance()->setLiked(1);

    processToFacebookPage(NULL);
}

void UILayer::moveToArtworkPage()
{
    //
    FlurryX::getInstance()->logEvent("ArtworkPage");
    
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    SocialX::getInstance()->openArtworkPage();
}

void UILayer::setSound(Ref* pSender)
{
    MenuItemToggle *menuItem = (MenuItemToggle*)pSender;
    
    int seledtedIdx = menuItem->getSelectedIndex();
    SoundManager::getInstance()->setMute(seledtedIdx);
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    //
    if (seledtedIdx == 0) {
        FlurryX::getInstance()->logEvent("Mute");
    }
    else {
        FlurryX::getInstance()->logEvent("Unmute");
    }
}

void UILayer::setNotification(Ref* pSender)
{
    MenuItemToggle *menuItem = (MenuItemToggle*)pSender;
    
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);
    
    int seledtedIdx = menuItem->getSelectedIndex();

    if (seledtedIdx == 0) {
        UserInfo::getInstance()->setNoti(1);
    }
    else {
        UserInfo::getInstance()->setNoti(0);
    }
}

void UILayer::showTutorial(Ref *pSender)
{
    //
    setUIStatus(MainUIStatusTutorial);

    // openCredit
    TutorialLayer *tutorial = TutorialLayer::create();
    if (!pSender) {
        tutorial->setCloseCallback(CC_CALLBACK_0(UILayer::tutorialCloseCallback, this));
    }
    else {
        SoundManager::getInstance()->playSoundEffect(SoundButton, false);
        tutorial->setCloseCallback(CC_CALLBACK_0(UILayer::creditCloseCallback, this));
    }

    addChild(tutorial, 100);
    
    optionLayer->setVisible(false);

    setMainUIVisible(false);

    //
    if (uiChangeCallback) {
        uiChangeCallback(6);
    }
}

void UILayer::openCredit(Ref *pSender)
{
    //
    setUIStatus(MainUIStatusCredit);

    //
    FlurryX::getInstance()->logEvent("OpenCredit");

    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    optionLayer->setVisible(false);

    // openCredit
    CreditLayer *credit = CreditLayer::create();
    credit->setCloseCallback(CC_CALLBACK_0(UILayer::creditCloseCallback, this));
    addChild(credit, 100);

    //
    if (uiChangeCallback) {
        uiChangeCallback(6);
    }
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void UILayer::setGoogleGameServiceLogin(Ref *pSender)
{
    if (!NativeUtils::isSignedIn()) {
        NativeUtils::signIn();
    }
    else {
        NativeUtils::signOut();
    }
}
#endif

void UILayer::tutorialCloseCallback()
{
    //
    setUIStatus(MainUIStatusMain);

    setMainUIVisible(true);

    //
    if (uiChangeCallback) {
        uiChangeCallback(0);
    }
}

void UILayer::creditCloseCallback()
{
    //
    setUIStatus(MainUIStatusResult);

    optionLayer->setVisible(true);

    //
    if (uiChangeCallback) {
        uiChangeCallback(2);
    }
}

void UILayer::forceToPauseGame()
{
    paused = true;
    pauseItem->setEnabled(false);
    initPauseLayer();
    pauseSceneCallback(this);
}

void UILayer::pauseGame(Ref* pSender)
{
    paused = true;

    //
    setUIStatus(MainUIStatusPause);

    //
    FlurryX::getInstance()->logEvent("Pause");
    TapjoyX::getInstance()->logEventInUIFlow("Pause");

    SoundManager::getInstance()->playSoundEffect(SoundButton, false);
//    SoundManager::getInstance()->playSoundEffectWithCallback(SoundButton, false,
//                                                             CC_CALLBACK_2(UILayer::pauseAllSound, this));
    pauseItem->setEnabled(false);

    initPauseLayer();

    pauseSceneCallback(this);
}

void UILayer::pauseAllSound(int id, const std::string& filePath)
{
//    SoundManager::getInstance()->pauseAllSoundEffect();
}

void UILayer::quitGame(Ref* pSender)
{
    //
    FlurryX::getInstance()->logEvent("Quit");
    TapjoyX::getInstance()->logEventInUIFlow("Quit");

    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    reloadSceneCallback(this);
}

void UILayer::resumeGame(Ref* pSender)
{
    //
    setUIStatus(MainUIStatusPlay);

    paused = false;

    //
    FlurryX::getInstance()->logEvent("Resume");
    TapjoyX::getInstance()->logEventInUIFlow("Resume");

    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    pauseItem->setEnabled(true);

//    disableTouch();
    blackLayer->removeFromParent();
    
    resumeSceneCallback(this);
}

void UILayer::shareGameToVideo()
{
    if (resultBtnDisabled) {
        return;
    }

    VideoShareX::getInstance()->showView();
}

void UILayer::shareGameScore()
{
    if (resultBtnDisabled) {
        return;
    }

//    //
//    SoundManager::getInstance()->playSoundEffect(SoundButton, false);
    
    //
    utils::captureScreen(CC_CALLBACK_2(UILayer::afterCapturedResult, this), "share.png");

    //
    //
    //
//    auto callback = [&](RenderTexture* rt, const std::string& path)
//    {
//        log("%s", path.c_str());
//        auto sprite = Sprite::createWithSpriteFrameName(path);
//        addChild(sprite);
//        sprite->setScale(0.5f);
//        sprite->setPosition(Vec2(visibleSizeHalf));
//        
//        SocialX::getInstance()->openActionSheetWithScore(currScore, path.c_str());
//    };
//
//    RenderTexture *_target = RenderTexture::create((int)visibleSize.width, (int)visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8_OES);
//    _target->retain();
//    _target->setPosition(Vec2(visibleSizeHalf));
//    _target->saveToFile("test.png", Image::Format::PNG, true, callback);
//    addChild(_target, -1);

    

    //
    //
    //
//    RenderTexture* texture = RenderTexture::create((int)visibleSize.width, (int)visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8_OES);
//    texture->setPosition(Vec2(visibleSizeHalf));
//    texture->clear(RGBValue(11), RGBValue(19), RGBValue(37), RGBValue(255));
//    texture->begin();
//    Director::getInstance()->getRunningScene()->visit();
//    texture->end();

//    Image *img = texture->newImage(false);
//    unsigned char *data = img->getData();
//    int dataSize = (int)img->getDataLen();

//    SocialX::getInstance()->openActionSheetWithScore(currScore, data, dataSize);
//
//    
//    delete img;
}

void UILayer::afterCapturedResult(bool succeed, const std::string& outputFile)
{
    std::stringstream stream;
    stream << LocalizationManager::getInstance()->getLocalizationStringWithReplace("Share1", currScore);
    std::string scoreShare = stream.str();
    
    if (succeed) {
        SocialX::getInstance()->openActionSheetWithMessage(scoreShare.c_str(), outputFile.c_str());
    }
    else {
        SocialX::getInstance()->openActionSheetWithMessage(scoreShare.c_str(), NULL);
    }
}

void UILayer::shareGameFreesh()
{
    if (resultBtnDisabled) {
        return;
    }

    //
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);
    
    //
    utils::captureScreen(CC_CALLBACK_2(UILayer::afterCapturedFreesh, this), "share.png");
}

void UILayer::afterCapturedFreesh(bool succeed, const std::string& outputFile)
{
    std::stringstream stream;
    stream << LocalizationManager::getInstance()->getLocalizationString("Share2");
    std::string freeshShare = stream.str();
    
    if (succeed) {
        SocialX::getInstance()->openActionSheetWithMessage(freeshShare.c_str(), outputFile.c_str());
    }
    else {
        SocialX::getInstance()->openActionSheetWithMessage(freeshShare.c_str(), NULL);
    }
}

void UILayer::rateGame()
{
    if (resultBtnDisabled) {
        return;
    }

    //
    FlurryX::getInstance()->logEvent("Rate");

    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    SocialX::getInstance()->rateApp();

    //
    UserInfo::getInstance()->setRated(1);
}

void UILayer::tryFreeshSkin()
{
    int trialSkinIdx = UserInfo::getInstance()->getTrialSkinIndex();
    int trialCount = UserInfo::getInstance()->getTrialCount();
    if (trialSkinIdx != -1 && trialCount == 0) {
        //
        // Trial Play
        //
        replayGame();
    }
    else {
//        log("tryFreeshSkin: %d, %d", trialSkinIdx, trialCount);
        processSkinPurchaseFromTrial();
    }
}

void UILayer::replayGame()
{
    if (resultBtnDisabled) {
        return;
    }

    //
    // NOTE: While the first-time user experice flow,
    // if user hit the replay button instead of the try button at ResultTypeTrial,
    // skip trial!
    //
//    int firstTimeResultTypeIdx = UserInfo::getInstance()->getFirstTimeResultTypeIndex();
//    int trialSkinIdx = UserInfo::getInstance()->getTrialSkinIndex();
//    if (firstTimeResultTypeIdx == 3 && trialSkinIdx==-1 && resultMain->getMainResultType() == ResultTypeTrial) {
//        UserInfo::getInstance()->increaseFirstTimeResultTypeIndex();
//    }

    //
    resultBtnDisabled = true;
    
    //
    FlurryX::getInstance()->logEvent("Replay");
    //
    TapjoyX::getInstance()->logEventInUIFlow("Replay");

    //
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);
//    SoundManager::getInstance()->playSoundEffectWithCallback(SoundButton, false,
//                                                             CC_CALLBACK_2(UILayer::stopAllSound, this));
    
    //
    reloadSceneCallback(this);
}

void UILayer::stopAllSound(int id, const std::string& filePath)
{
    SoundManager::getInstance()->pauseAllSoundEffect();
}

void UILayer::showOptionLayer()
{
    if (resultBtnDisabled) {
        return;
    }

    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    //
    TapjoyX::getInstance()->logEventInUIFlow("Option");

    setResultUIVisible(false);

    optionLayer->setVisible(true);

    //
    if (uiChangeCallback) {
        uiChangeCallback(2);
    }
}

void UILayer::hideOptionLayer(Ref *pSender)
{
    setResultUIVisible(true, false);

    optionLayer->setVisible(false);

    //
    if (uiChangeCallback) {
        uiChangeCallback(7);
    }
}

void UILayer::showGameCenterLeaderboard()
{
    if (resultBtnDisabled) {
        return;
    }

    //
    FlurryX::getInstance()->logEvent("GameCenter");
    //
    TapjoyX::getInstance()->logEventInUIFlow("GCLeaderBoard");

    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (!NativeUtils::isSignedIn()) {
        NativeUtils::signIn();
    }
    else {
        NativeUtils::showLeaderboard();
    }
#else
    //
    if (GameCenterX::getInstance()->isAuthenticated(true)) {
        GameCenterX::getInstance()->showLeaderboard();
    }
    else {
        log("Game Center Unavailable: Please sign in first.");
    }
#endif
}

int UILayer::checkHiddenSkin()
{
    int cnt = 0;
    int hiddenSkin = UserInfo::getInstance()->getHiddenSkinIndex(); // the first hidden skin index

    //
    // Init popup manager
    //
    popups = PopupManager::create();
    
    std::function<void()> callback = [this]() {
        //
        if (startGameCallback) {
            startGameCallback();
        }
    };
    popups->setEndOfPopupCallback(callback);
    addChild(popups, 1000);

    //
    // Checkin unlock conditions
    //
    bool unlockCond[NumOfHiddenSkins];
    for (int i=0; i<NumOfHiddenSkins; i++) {
        unlockCond[i] = (UserInfo::getInstance()->isLockedSkin(hiddenSkin + i) && UserInfo::getInstance()->getHiddenSkinUnlockCond(i));
    }

    //
    // Add hidden skin unlock popups depending on conditions
    //
    for (int i=0; i<NumOfHiddenSkins; i++) {
        if (unlockCond[i]) {
            cnt++;
            //
            // Unlock reward skin
            //
            UserInfo::getInstance()->unlockSkinAt(hiddenSkin);

            std::function<void()> callback = [this]() {
                //
                popups->showNextPopup();
            };
            
            //
            HiddenskinPopup *popup = HiddenskinPopup::create();
            popup->setShareCallback(CC_CALLBACK_0(UILayer::shareGameFreesh, this));
            popup->setNextCallback(callback);
            popup->setSkinInfo(hiddenSkin);

            //
            popups->pushPopup(popup);

            //
            TapjoyX::getInstance()->logEventInUIFlow("HiddenSkinUnlock", HiddenSkinStartIndexForTapJoy + i);
        }
        
        hiddenSkin++;
    }
    
//    //
//    // Hidden Skin : Wizard
//    // Facebook visit reward
//    //
//    if (UserInfo::getInstance()->getLiked() && UserInfo::getInstance()->isLockedSkin(hiddenSkin)) {
//        cnt++;
//        //
//        // Unlock reward skin
//        //
//        UserInfo::getInstance()->unlockSkinForFacebookReward();
//        int skinNum = hiddenSkin + 1;
//
//        //
//        FreeshType skinType = UserInfo::getInstance()->getSkinTypeBySkinNumber(skinNum);
//
//        //
//        UserInfo::getInstance()->skinType = skinType;
//        UserInfo::getInstance()->skinNum = skinNum;
//
//        std::function<void()> callback = [this]() {
//            //
//            popups->showNextPopup();
//        };
//
//        //
//        PopupLayer *popup = PopupLayer::create();
//        popup->setShareCallback(CC_CALLBACK_0(UILayer::shareGame, this));
//        popup->setNextCallback(callback);
//        popup->setSkinInfo(hiddenSkin);
//
//        //
//        popups->pushPopup(popup);
//    }

    //
    //
    //
    if (cnt > 0) {
        scheduleOnce(CC_SCHEDULE_SELECTOR(UILayer::showHiddenSkinPopups), 2.0);
    }
    else {
        showEmptyPopup(false);
    }

    return cnt;
}

void UILayer::showHiddenSkinPopups(float dt)
{
    showEmptyPopup(false);
    popups->showNextPopup();
}

#pragma mark -
#pragma mark IAP
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void UILayer::showEmptyPopup(bool flag)
{
    if (flag) {
        if (emptyPopup) {
            emptyPopup->setEnbleTouch(true);
        }
    }
    else {
        if (emptyPopup) {
            emptyPopup->setEnbleTouch(false);
        }
    }
}

void UILayer::processSkinPurchaseFromTrial()
{
    uiStatus = MainUIStatusProcessPurchase;

    //
    int trialSkinIdx = UserInfo::getInstance()->getTrialSkinIndex();
    
    //
    // Skin Purchase
    //
    processSkinPurchase(trialSkinIdx);
}

void UILayer::processSkinPurchaseFromResult()
{
    int skinIdx = resultMain->getSkinIdxSelected();
    
    //
    // Skin Purchase
    //
    processSkinPurchase(skinIdx);
}

void UILayer::processSkinPurchase(int skinIdx)
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    //
    if (StoreKitX::getInstance()->initIAP()) {
        uiStatus = MainUIStatusProcessPurchase;

        //
        // TO block touch, add empty popup
        //
        showEmptyPopup(true);

        //
        TapjoyX::getInstance()->logEventInIAP("StartIAPSkin", skinIdx);

        //
        // Purchase
        //
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        if (skinIdx == UserInfo::getInstance()->getGoldenFishSkinIndex()) {
            StoreKitX::getInstance()->beginPurchaseFreeshSkin("goldfish");
        }
        else {
            StoreKitX::getInstance()->beginPurchaseFreeshSkin(getNormalSkinName(skinIdx));
        }
#else
        StoreKitX::getInstance()->beginPurchaseFreeshSkin(getNormalSkinName(skinIdx));
#endif
    }
}

void UILayer::processPurchasePackage()
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    //
    if (StoreKitX::getInstance()->initIAP()) {
        uiStatus = MainUIStatusProcessPurchase;
        
        int packageIdx = resultMain->getPackageIdxSelected();

        //
        // TO block touch, add empty popup
        //
        showEmptyPopup(true);
        
        //
        TapjoyX::getInstance()->logEventInIAP("StartIAPPackage", packageIdx);
        
        //
        // Purchase
        //
        std::stringstream stream;
        stream << std::setprecision(2);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        stream << "package" << std::setfill('0') << std::setw(2) << packageIdx + 1;
#else
        stream << "Package" << std::setfill('0') << std::setw(2) << packageIdx + 1;
#endif
        std::string packageID = stream.str();
        StoreKitX::getInstance()->beginPurchasePackage(packageID.c_str());
    }
}

void UILayer::processPurchaseRemoveAds()
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    //
    FlurryX::getInstance()->logEvent("IAP");

    if (StoreKitX::getInstance()->initIAP()) {
        uiStatus = MainUIStatusProcessPurchase;

        // TO block touch, add empty popup
        showEmptyPopup(true);

        //
        TapjoyX::getInstance()->logEventInIAP("StartIAPRemove");

        //
        // Purchase
        //
        StoreKitX::getInstance()->beginPurchaseRemoveAds();
    }
}

void UILayer::restorePurchasedItems(Ref *pSender)
{
    if (StoreKitX::getInstance()->initIAP()) {
        //
        showEmptyPopup(true);
        
        //
        // Restore
        //
        StoreKitX::getInstance()->restorePurchasesItems();
    }
}


void UILayer::finishPurchase(bool purchased)
{
//    log("UILayer::finishPurchase: %d", purchased);

    finishPurchaseValue = purchased;

    if (finishPurchaseValue) {
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
        // if ios, call immediately!
        processFinishPurchase(0);
#else
        scheduleOnce(CC_SCHEDULE_SELECTOR(UILayer::processFinishPurchase), 1);
#endif
    }
    else {
        processFinishPurchase(0);
    }
}

void UILayer::processFinishPurchase(float dt)
{
    bool failed = false;
    
    // successful
    if (finishPurchaseValue) {
        
        int listSize = (int)StoreKitX::getInstance()->itemIdxListToProvide.size();
        if (listSize == 1) {
            int idx = -1;
            bool restored = false;
            StoreKitX::getInstance()->popItemIdxListToProvide(idx, restored);
            
            if (idx > -1) {
                std::string productName = StoreKitX::getInstance()->getProductNameByIndex(idx);
                int skinIdx = getSkinIndexBySkinName(productName.c_str());
                if (skinIdx != -1) {
//                    log("%s: %d", skinName.c_str(), skinIdx);
                    
                    //
                    // After skin purchased,
                    // process providing freesh skin.
                    //
                    afterPurchaseFreeshSkin(skinIdx, restored);
                    
                    //
                    TapjoyX::getInstance()->logEventInIAP("SuccessIAPSkin");
                }
                else if (idx == StoreKitX::getInstance()->removeAdsItemIndex){
                    //
                    // After RemoveAds purchased,
                    // process providing reward coins.
                    //
                    showCoins(4);
                    UserInfo::getInstance()->addEarnedCoinsForRemoveAds();
                    updateCoinLabel();
                    //
                    UserInfo::getInstance()->setAdRemoveOptionWithSave(1);
                    //
                    resultMain->updateMainLayer();
                    //
                    showEmptyPopup(false);
                    //
                    TapjoyX::getInstance()->logEventInIAP("SuccessIAPRemove");
                }
                else {
                    //
                    // Package Item
                    //
                    showCoins(3);
                    UserInfo::getInstance()->addEarnedCoinsForPackage();
                    updateCoinLabel();
                    //
                    std::string packageID = productName.substr(7); // PackageXX
                    int packageIdx = atoi(packageID.c_str()) - 1;
                    UserInfo::getInstance()->unlockPackageSkins(packageIdx);
                    //
                    resultMain->updateMainLayer();
                    //
                    showEmptyPopup(false);
                    //
                    TapjoyX::getInstance()->logEventInIAP("SuccessIAPPackage");
                }
            }
            else {
                // Data Error???
                failed = true;
            }
        }
    }
    else {
        failed = true;
    }
    
    if (failed) {
        //
        showEmptyPopup(false);
        
        //
        TapjoyX::getInstance()->logEventInIAP("FailIAP");
    }
}

void UILayer::finishRestore(bool success)
{
    finishRestoreValue = success;

//#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    // if ios, call immediately!
    processFinishRestore(0);
//#endif
}

void UILayer::processFinishRestore(float dt)
{
    // successful
    if (finishRestoreValue) {
        int listSize = (int)StoreKitX::getInstance()->itemIdxListToProvide.size();
        if (listSize == 1) {
            int idx = -1;
            bool restored = false;
            StoreKitX::getInstance()->popItemIdxListToProvide(idx, restored);

            if (idx > -1) {
                std::string productName = StoreKitX::getInstance()->getProductNameByIndex(idx);
                int skinIdx = getSkinIndexBySkinName(productName.c_str());
                if (skinIdx != -1) {
                    //
                    // After skin restored,
                    // process providing freesh skin.
                    //
                    afterPurchaseFreeshSkin(skinIdx, restored);

                    //
                    TapjoyX::getInstance()->logEventInIAP("RestoreIAPSkin");
                }
                else if (idx == StoreKitX::getInstance()->removeAdsItemIndex){
                    //
                    // After RemoveAds restored,
                    // Processs remove ads + Don't provide any coins!
                    //
                    UserInfo::getInstance()->setAdRemoveOptionWithSave(1);
                    resultMain->updateMainLayer(false); // withGivingCoins
                    //
                    TapjoyX::getInstance()->logEventInIAP("RestoreIAPRemove");
                    //
                    showEmptyPopup(false);
                }
                else {
                    //
                    // Package Item
                    //
                    std::string packageID = productName.substr(7); // PackageXX
                    int packageIdx = atoi(packageID.c_str()) - 1;
                    UserInfo::getInstance()->unlockPackageSkins(packageIdx);
                    //
                    resultMain->updateMainLayer(false); // withGivingCoins
                    //
                    TapjoyX::getInstance()->logEventInIAP("RestoreIAPPackage");
                    //
                    showEmptyPopup(false);
                }
            }
            else {
                // Data Error???
            }
        }
    }
    else {
        //
        showEmptyPopup(false);
    }
}

void UILayer::finishRestoreAll(bool success)
{
    //
    showEmptyPopup(false);

    // successful
    if (success) {
        int listSize = (int)StoreKitX::getInstance()->itemIdxListToProvide.size();
        for (int i=0; i<listSize; i++) {
            int idx = -1;
            bool restored = false;
            StoreKitX::getInstance()->popItemIdxListToProvide(idx, restored);

            if (idx > -1) {
                std::string productName = StoreKitX::getInstance()->getProductNameByIndex(idx);
                int skinIdx = getSkinIndexBySkinName(productName.c_str());
                if (skinIdx != -1) {
//                    log("UILayer::finishRestoreAll: Skin %d, %d, %d", idx, restored, skinIdx);
                    if (skinIdx != -1) {
                        //
                        // After skin restored,
                        //
                        UserInfo::getInstance()->unlockSkinAt(skinIdx);
                    }
                }
                else if (idx == StoreKitX::getInstance()->removeAdsItemIndex) {
//                    log("UILayer::finishRestoreAll: RemoveAds %d, %d", idx, restored);
                    //
                    // Remove Ads
                    //
                    UserInfo::getInstance()->setAdRemoveOptionWithSave(1);
                }
                else {
                    //
                    // Package Item
                    //
                    std::string packageID = productName.substr(7); // PackageXX
                    int packageIdx = atoi(packageID.c_str()) - 1;
                    UserInfo::getInstance()->unlockPackageSkins(packageIdx);
                }
            }
        }
        
        //
        TapjoyX::getInstance()->logEventInIAP("SuccessRestoreAll");
    }
    
    //
    reloadSceneCallback(this);
}

void UILayer::hideFreeshPurchaseResult()
{
    //
    freeshMaker->hideLayer();

    //
    showFreeshSelection();
}

void UILayer::afterPurchaseFreeshSkin(int skinIdx, bool restored)
{
    int trialSkinIdx = UserInfo::getInstance()->getTrialSkinIndex();
    if (trialSkinIdx == -1) {
        //
        //
        //
        setResultUIForSkinPurchase();

        if (freeshSelection) {
            freeshSelection->hideLayer();
        }

        //
        if (status == StatusGameOver) {
            freeshMaker->setCloseCallback(CC_CALLBACK_0(UILayer::hideFreeshMaker, this));
            freeshMaker->showResultUI(skinIdx, true, true, restored); // from result main
        }
        else {
            freeshMaker->setCloseCallback(CC_CALLBACK_0(UILayer::hideFreeshPurchaseResult, this));
            freeshMaker->showResultUI(skinIdx, true, false, restored);  // from freesh selection
        }

        //
        if (!restored && skinIdx == UserInfo::getInstance()->getGoldenFishSkinIndex()) {
            UserInfo::getInstance()->unlockGoldenFishAndGiveRewardAfterPurchase();
            showCoins(4);
            updateCoinLabel();
        }
        else {
            UserInfo::getInstance()->unlockSkinAt(skinIdx);
        }
    }
    else {
        //
        //
        //
        if (restored) {
            //
            // without rewards
            //
            UserInfo::getInstance()->unlockTrialSkinAfterRestore();
        }
        else {
            //
            // with rewards
            //
            UserInfo::getInstance()->unlockTrialSkinAndGiveRewardAfterPurchase();
            showCoins(3);
            updateCoinLabel();
        }
        
        //
        setResultUIForSkinPurchase();

        //
        freeshMaker->setCloseCallback(CC_CALLBACK_0(UILayer::hideFreeshMaker, this));
        freeshMaker->showResultUI(trialSkinIdx, true);
    }
    
    //
    showEmptyPopup(false);
}

void UILayer::onFinishLoading()
{
    resultBtnDisabled = false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void UILayer::onUnityAdsStart()
{
//    CCLOG("UILayer::onUnityAdsStart");
    SoundManager::getInstance()->pauseAmbienceSound();
    Director::getInstance()->stopAnimation();
}

void UILayer::onUnityAdsFinish(bool shown)
{
//    CCLOG("UILayer::onUnityAdsFinish %d", shown);
    
    SoundManager::getInstance()->resumeAllSoundEffect();
    Director::getInstance()->startAnimation();

    if (shown) {
        showVideoReward();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void UILayer::onAdColonyAdStartedInZone(const char* zoneID)
{
//    log("onAdColonyAdStartedInZone");
    
    SoundManager::getInstance()->pauseAmbienceSound();
    Director::getInstance()->stopAnimation();
}

void UILayer::onAdColonyAdAttemptFinished(bool shown, const char* zoneID)
{
//    log("onAdColonyAdAttemptFinished");
    
    SoundManager::getInstance()->resumeAllSoundEffect();
    Director::getInstance()->startAnimation();
    
    //
    // video is done. give the rewards!
    //
    if (shown) {
        showVideoReward();
    }
}

void UILayer::onAdColonyAdAvailabilityChange(bool available, const char* zoneID)
{
//    log("onAdColonyAdAvailabilityChange: %d", available);
}

void UILayer::showVideoReward()
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    // call immediately!
    showReward(0);
#endif
}

void UILayer::updateUIAfterWatchingAd()
{
    //
    // TODO:
    // In Android platforms, it takes some time
    // to come back from video/purchase view to cocos2d view.
    // With that delay, some newly added Textures/Lables are not showed up,
    // or sometimes cause crash.
    // So let's give some delay(1 sec here) before uiLayer updates.
    // And call this at returnToForeground from main PlayScene.
    //
    // This is temporary solution. Need to fix this process later!!!
    //
    if (uiStatus == MainUIStatusWatchingAd) {
        uiStatus = MainUIStatusResult;
        scheduleOnce(CC_SCHEDULE_SELECTOR(UILayer::showReward), 1);
    }
}

void UILayer::showReward(float dt)
{
    //
    showEmptyPopup(false);

    showCoins(0);
    UserInfo::getInstance()->addEarnedCoinsForVideo();
    updateCoinLabel();
    resultMain->updateMainLayer();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void UILayer::requestDidSucceed()
{
//    log("requestDidSucceed");
}
void UILayer::requestDidFail()
{
//    log("requestDidFail");
}
void UILayer::contentIsReady(const char *placementName)
{
//    log("contentIsReady: %s", placementName);
    if (status == StatusReady) {
        TapjoyX::getInstance()->showPlacement("App Launch");
    }
}
void UILayer::contentDidAppear()
{
//    log("contentDidAppear");
}
void UILayer::contentDidDisappear(const char *placementName)
{
//    log("contentDidDisappear");
    
    if (status != StatusReady) {
        
        if (!strcmp(placementName, "VideoRewardPlacement")) {
            //
            // video is done. give the rewards!
            //
            showVideoReward();
        }
    }
}
