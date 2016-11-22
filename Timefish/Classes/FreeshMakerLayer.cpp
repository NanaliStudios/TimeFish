//
//  FreeshMakerLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 7. 19..
//
//

#include "FreeshMakerLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

using namespace spine;

bool FreeshMakerLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();

    currAni = -1;

    scaleFactor = UserInfo::getInstance()->screenScale;

    //
    initMainLayer();
    
    //
    initParticles();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // set keyInput enabled
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(FreeshMakerLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    ownsPriority = false;
#endif

    return true;
}

void FreeshMakerLayer::initMainLayer()
{
    //
    lNode = SkeletonAnimation::createWithData(SkeletonHolder::getInstance()->lightSkeleton);
    lNode->setDebugSlotsEnabled(false);
    lNode->setPosition(Vec2(visibleSizeHalf) + origin);
    lNode->setVisible(false);
    lNode->setScale(scaleFactor);
    addChild(lNode, 10);

    //
    sNode = SkeletonAnimation::createWithData(SkeletonHolder::getInstance()->drawSkeleton);
    sNode->setDebugSlotsEnabled(false);
    sNode->setPosition(Vec2(visibleSizeHalf) + origin);
    sNode->setScale(scaleFactor);
    addChild(sNode, 100);

    setMachineSkin();

    float basePosY = 0;
    //
    // StartBtn
    //
    {
        auto menuItem = MenuItemImageButton::create();
        menuItem->setNormalImage(Sprite::createWithSpriteFrameName("button_center_red.png"));
        menuItem->setSelectedImage(Sprite::createWithSpriteFrameName("button_center_red_click.png"));
        menuItem->setCallback(CC_CALLBACK_1(FreeshMakerLayer::btnCallback, this));

        menuItem->setTag(StartBtnTag);
        Size s = menuItem->getContentSize();
        
        basePosY = 20 + s.height * 0.5;
        
        startBtn = Menu::create(menuItem, NULL);
        startBtn->setPosition(Vec2(visibleSizeHalf.width, basePosY) + origin);
        addChild(startBtn, 1);
        
        //
        Label *btnLabel = Label::createWithTTF(std::to_string(GoldNeededToDraw), NumberFontPath, 65);
        btnLabel->setAnchorPoint(Vec2(0, 0.5));
        menuItem->addChild(btnLabel, 1);
        
        //
        auto coinIcon = Sprite::createWithSpriteFrameName("coin_big.png");
        coinIcon->setAnchorPoint(Vec2(1, 0.5));
        coinIcon->setScale(0.40);
        menuItem->addChild(coinIcon);
        
        Size s1 = btnLabel->getContentSize();
        Size s2 = coinIcon->getContentSize();
        float width = s1.width + s2.width * coinIcon->getScale() + 4;
        
        //
        btnLabel->setPosition(Vec2((s.width - width)*0.5, s.height * 0.5));
        coinIcon->setPosition(Vec2((s.width + width)*0.5, s.height * 0.5));
        
        //
        menuItem->setLabelChild(btnLabel, coinIcon);
    }

    //
    // playBtn = PlayBtn
    //
    {
        auto playBtnItem = MenuItemImageButton::create();
        playBtnItem->setNormalImage(Sprite::createWithSpriteFrameName("button_middle_white.png"));
        playBtnItem->setSelectedImage(Sprite::createWithSpriteFrameName("button_middle_white_click.png"));
        playBtnItem->setCallback(CC_CALLBACK_1(FreeshMakerLayer::btnCallback, this));

        Size s = playBtnItem->getContentSize();
        playBtnItem->setTag(PlayBtnTag);
        playBtnItem->setPosition(Vec2(visibleSizeHalf.width, basePosY) + origin);
        
        playBtn = Menu::create(playBtnItem, NULL);
        playBtn->setPosition(Vec2::ZERO);
        playBtn->setVisible(false);
        addChild(playBtn, 1);
        
        // add icon
        auto icon = Sprite::createWithSpriteFrameName("icon_play.png");
        icon->setPosition(Vec2(s) * 0.5);
        playBtnItem->addChild(icon, 1);

        //
        playBtnItem->setLabelChild(icon);
    }
    
    //
    // RetyBtn
    //
    {
        auto retryBtnItem = MenuItemAutoGray::create("button_middle_red.png", "button_middle_red_click.png", CC_CALLBACK_1(FreeshMakerLayer::btnCallback, this));
        Size s = retryBtnItem->getContentSize();
        retryBtnItem->setTag(RetryBtnTag);
        retryBtnItem->setPosition(Vec2(visibleSizeHalf.width + s.width * 1.05, basePosY) + origin);

        retryBtn = Menu::create(retryBtnItem, NULL);
        retryBtn->setPosition(Vec2::ZERO);
        retryBtn->setVisible(false);
        addChild(retryBtn, 1);

        //
        Label *btnLabel = Label::createWithTTF(std::to_string(GoldNeededToDraw), NumberFontPath, 60);
        btnLabel->setAnchorPoint(Vec2(0, 0.5));
        retryBtnItem->addChild(btnLabel, 1);

        //
        auto coinIcon = Sprite::createWithSpriteFrameName("coin_big.png");
        coinIcon->setAnchorPoint(Vec2(1, 0.5));
        coinIcon->setScale(0.3);
        retryBtnItem->addChild(coinIcon);
        
        Size s1 = btnLabel->getContentSize();
        Size s2 = coinIcon->getContentSize();
        float width = s1.width + s2.width * coinIcon->getScale() + 2;

        //
        btnLabel->setPosition(Vec2((s.width - width)*0.5, s.height * 0.5));
        coinIcon->setPosition(Vec2((s.width + width)*0.5, s.height * 0.5));
    }

    //
    // Result Label
    //
    {
        resultLabel = Label::createWithTTF(" ", UserInfo::getInstance()->getFontPath(), 60);
        resultLabel->setPosition(Vec2(visibleSizeHalf.width, basePosY + resultLabel->getContentSize().height * 1.5) + origin);
        resultLabel->setColor(Color3B::BLACK);
        resultLabel->setVisible(false);
        addChild(resultLabel, 1);
    }

    //
    // Close Button
    //
    {
        auto closeBtnItem = MenuItemSameImage::create("TF_UI_x.png", CC_CALLBACK_1(FreeshMakerLayer::btnCallback, this));
        Size closeBtnSize = closeBtnItem->getContentSize();
        closeBtnItem->setOpacity(150);
        closeBtnItem->setTag(CloseBtnTag);

//        float topPosY = visibleSize.height - closeBtnSize.height;

        closeBtn = Menu::create(closeBtnItem, NULL);
//        closeBtn->setPosition(Vec2(closeBtnSize.width, topPosY) + origin);
        closeBtn->setPosition(Vec2(closeBtnSize.width * 0.5 + 20, visibleSize.height - closeBtnSize.height* 0.5 - 20) + origin);
        closeBtn->setVisible(false);
        addChild(closeBtn, 1);
    }
}

void FreeshMakerLayer::setMachineSkin()
{
    // set skin
    const char *skinNames[] = {
        "B",
        "R",
        "Y"
    };
    int idx = rand()%3;
    std::stringstream stream;
    stream << skinNames[idx];
    std::string skinName = stream.str();
    sNode->setSkin(skinName);
}

void FreeshMakerLayer::initParticles()
{
    //
    Color4F colors[3] = {
        Color4F(RGBValue(0), RGBValue(138), RGBValue(255), 1),
        Color4F(RGBValue(251), RGBValue(217), RGBValue(60), 1),
        Color4F(RGBValue(255), RGBValue(27), RGBValue(0), 1),
    };
    for (int i=0; i<3; i++) {
        partyPtcl[i] = ParticleSystemQuad::create("particle/VendingMachinePopper_particle/VendingMachinePopper_particle.plist");
        partyPtcl[i]->setPosition(Vec2(visibleSizeHalf.width, visibleSize.height * 1.2) + origin);
        partyPtcl[i]->setStartColor(colors[i]);
        partyPtcl[i]->setEndColor(colors[i]);
        partyPtcl[i]->stopSystem();
        addChild(partyPtcl[i], 0);
    }
}

void FreeshMakerLayer::processToStartMachine()
{
    //
    // Consume 100 Coins
    //
    UserInfo::getInstance()->consumeCoinsForDraw();
    
    //
    SoundManager::getInstance()->playSoundEffect(SoundDrawStart, false);
    
    //
    currAni = 0;
    startBtn->setVisible(false);
    
    startMachine();
}

void FreeshMakerLayer::startMachine()
{
    if (currAni >= 0 && currAni < 4) {
        currAni++;

        if (currAni == 1) {
            //
            // Start recording
            //
            if (startVideoCallback) {
                startVideoCallback();
            }

        }
        else if (currAni == 3) {
            //
            SoundManager::getInstance()->playSoundEffect(SoundDrawZoomin, false);
        }
        
        sNode->setAnimation(0, std::to_string(currAni), false);
    }
}

void FreeshMakerLayer::showResult()
{
    //
    SoundManager::getInstance()->playSoundEffect(SoundDrawPop, false);

    //
    // Pick skin
    //
    int skinIdxSelected = UserInfo::getInstance()->getRandomSkinAmongNotHiddenForDraw();

    //
    showResultUI(skinIdxSelected);
    
    //
    // UNLOCK!
    //
    UserInfo::getInstance()->unlockSkinAt(skinIdxSelected);
    
    //
    TapjoyX::getInstance()->logEventInEconomy(5, skinIdxSelected + 1);
}

void FreeshMakerLayer::showResultUI(int skinIdxSelected, bool onlyEarnResult, bool withCloseBtn, bool restoredSkin)
{
    ownsPriority = true;

    skinNum = skinIdxSelected + 1;

    //
    sNode->setVisible(!onlyEarnResult);

    //
    {
        std::stringstream stream;
        stream << getFreeshKeyCode(skinIdxSelected);
        std::string keyCode = stream.str();
        
        //
        stream.str("");
        stream.clear();
        stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
        std::string nameInfo = stream.str();
        
        //
        // Name Label
        auto nameLabel = Label::createWithTTF(nameInfo, UserInfo::getInstance()->getFontPath(), 60);
        nameLabel->setPosition(Vec2(visibleSizeHalf.width, visibleSize.height * 0.75) + origin);
        nameLabel->setTextColor(Color4B(255, 7, 7, 255));
        nameLabel->setTag(ResultFreeshNameTag);
        addChild(nameLabel, 1);
        
        //
        IdleFreeshLayer *freeshSkin = IdleFreeshLayer::getIdleFreeshSkin(skinNum);
        freeshSkin->setPosition(Vec2(visibleSizeHalf) + origin);
        freeshSkin->setTag(ResultFreeshTag);
        freeshSkin->setSelected(true);
        freeshSkin->showParticle(true); // whether it's a goldenfish or not, call this anyway!
        addChild(freeshSkin, 1);
        freeshSkin->release();
    }

    playBtn->setVisible(true);

    if (!onlyEarnResult) {
        retryBtn->setVisible(true);

        MenuItemAutoGray *itm = (MenuItemAutoGray*)retryBtn->getChildByTag(RetryBtnTag);
        if (UserInfo::getInstance()->hasEnoughCoinsToDraw()) {
            itm->setColored();
        }
        else {
            itm->setGrayed();
        }
        
    }
    if (withCloseBtn) {
        closeBtn->setVisible(true);
    }
    else {
        closeBtn->setVisible(false);
    }
    
    //
    if (!restoredSkin) {
        bool isNewSkin = false;

        std::stringstream stream;
        if (onlyEarnResult || UserInfo::getInstance()->isLockedSkin(skinIdxSelected)) {
            //
            SoundManager::getInstance()->playSoundEffect(SoundSkinNew, false);
            
            //
            for (int i=0; i<3; i++) {
                partyPtcl[i]->setEmissionRate(12);
            }

            //
            stream << LocalizationManager::getInstance()->getLocalizationString("Skin_New");

            isNewSkin = true;
        }
        else {
            //
            SoundManager::getInstance()->playSoundEffect(SoundSkinNotNew, false);

            //
            stream << LocalizationManager::getInstance()->getLocalizationString("Skin_TryAgain");
            
            //
            for (int i=0; i<3; i++) {
                partyPtcl[i]->setEmissionRate(3);
            }
        }
        
        std::string labelString = stream.str();
        resultLabel->setVisible(true);
        resultLabel->setString(labelString);
        if (isNewSkin) {
            auto seq = Sequence::create(ScaleTo::create(0.25, 1.2),
                                        ScaleTo::create(0.25, 1.0),
                                        NULL);
            resultLabel->runAction(RepeatForever::create(seq));
        }
    }
    else {
        //
        // Skin is Restored!!!
        //
        //
        SoundManager::getInstance()->playSoundEffect(SoundSkinNew, false);
        
        //
        for (int i=0; i<3; i++) {
            partyPtcl[i]->setEmissionRate(12);
        }
    }

    //
    // show particles
    //
    for (int i=0; i<3; i++) {
        partyPtcl[i]->resetSystem();
    }
    
    //
    if (uiChangeCallback) {
        uiChangeCallback(2);
    }
}

void FreeshMakerLayer::btnCallback(Ref* pSender)
{
    bool didGacha = false;
    if (currAni != -1) {
        didGacha = true;
    }

    //
    // Finish recording
    //
    if (didGacha) {
        if (finishVideoCallback) {
            finishVideoCallback();
        }
    }

    MenuItemImage *btn = (MenuItemImage*)pSender;
    int tag = btn->getTag();
    if (tag == StartBtnTag) {
        SoundManager::getInstance()->playSoundEffect(SoundButton, false);
        //
        processToStartMachine();
    }
    else if (tag == PlayBtnTag) {
        playThis();
    }
    else if (tag == RetryBtnTag) {
        
        MenuItemAutoGray *btn = (MenuItemAutoGray*)pSender;
        if (btn->getGrayed()) {
            //
            SoundManager::getInstance()->playSoundEffect(SoundDisabledButton, false);
        }
        else {
            SoundManager::getInstance()->playSoundEffect(SoundButton, false);
            
            if (uiChangeCallback) {
                uiChangeCallback(6);
            }
            
            removeFreeshSkin();
            initToRedrawSkin();
        }
    }
    else if (tag == CloseBtnTag) {
        SoundManager::getInstance()->playSoundEffect(SoundButton, false);
        hideThis();
    }
}

void FreeshMakerLayer::playThis()
{
    UserInfo::getInstance()->setRandomSkin(false);
    UserInfo::getInstance()->setSkinNumWithTypeSet(skinNum);
    
    if (replayCallback) {
        replayCallback();
    }
}

void FreeshMakerLayer::hideThis()
{
    hideResultButtons();
    removeFreeshSkin();
    
    ownsPriority = false;

    if (closeCallback) {
        closeCallback();
    }
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void FreeshMakerLayer::onKeyReleased( EventKeyboard::KeyCode keycode, Event *event )
{
//    log("MainUIStatus: FreeshMakerLayer::onKeyReleased: %d", ownsPriority);
    if (ownsPriority && keycode == EventKeyboard::KeyCode::KEY_BACK) {
        event->stopPropagation();
        
        if (closeBtn->isVisible()) {
            SoundManager::getInstance()->playSoundEffect(SoundButton, false);
            hideThis();
        }
        else {
            playThis();
        }
    }
}
#endif
