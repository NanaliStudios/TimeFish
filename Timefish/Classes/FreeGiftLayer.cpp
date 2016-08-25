//
//  FreeGiftLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 7. 19..
//
//

#include "FreeGiftLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

using namespace spine;

bool FreeGiftLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();
    
    //
    initMainLayer();

    //
    initParticles();
    
    return true;
}

void FreeGiftLayer::initMainLayer()
{
    //
    sNode = SkeletonAnimation::createWithData(SkeletonHolder::getInstance()->giftSkeleton);
    sNode->setDebugSlotsEnabled(false);
    sNode->setPosition(Vec2(visibleSizeHalf) + origin);
    addChild(sNode, 100);

    sNode->setAnimation(0, "idle", true);

    //
    {
        nextBtnItem = MenuItemImageButton::create();
        nextBtnItem->setNormalImage(Sprite::createWithSpriteFrameName("button_center_white.png"));
        nextBtnItem->setSelectedImage(Sprite::createWithSpriteFrameName("button_center_white_click.png"));
        nextBtnItem->setCallback(CC_CALLBACK_1(FreeGiftLayer::btnCallback, this));
        Size s = nextBtnItem->getContentSize();
        nextBtnItem->setPosition(Vec2(visibleSizeHalf.width, 70 + s.height * 0.5) + origin);

        nextBtn = Menu::create(nextBtnItem, NULL);
        nextBtn->setPosition(Vec2::ZERO);
        nextBtn->setVisible(false);
        addChild(nextBtn, 1);

        // add icon
        auto icon = Sprite::createWithSpriteFrameName("icon_play.png");
        icon->setPosition(Vec2(s) * 0.5);
        nextBtnItem->addChild(icon, 1);

        //
        nextBtnItem->setLabelChild(icon);
    }
    
    //
    {
        resultCoinLabel = Label::createWithTTF("0", NumberFontPath, 105);
        resultCoinLabel->setColor(Color3B(254, 246, 108));
        resultCoinLabel->setAnchorPoint(Vec2(0, 0.5));
        resultCoinLabel->setVisible(false);
        addChild(resultCoinLabel, 1);

        //
        coinIcon = Sprite::createWithSpriteFrameName("coin_big.png");
        coinIcon->setAnchorPoint(Vec2(0, 0.5));
        coinIcon->setScale(0.7);
        coinIcon->setVisible(false);
        addChild(coinIcon, 1);
    }
}

void FreeGiftLayer::initParticles()
{
    //
    Color4F colors[3] = {
        Color4F(RGBValue(0), RGBValue(138), RGBValue(255), 1),
        Color4F(RGBValue(251), RGBValue(217), RGBValue(60), 1),
        Color4F(RGBValue(255), RGBValue(27), RGBValue(0), 1),
    };
    for (int i=0; i<3; i++) {
        partyPtcl[i] = ParticleSystemQuad::create("particle/party_popper_particle/party popper.plist");
        partyPtcl[i]->setPosition(Vec2(visibleSizeHalf) + origin);
        partyPtcl[i]->setStartColor(colors[i]);
        partyPtcl[i]->setEndColor(colors[i]);
        partyPtcl[i]->stopSystem();
        addChild(partyPtcl[i], 0);
    }
}

void FreeGiftLayer::setRandomSkin()
{
    //
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

void FreeGiftLayer::showLayer(int _type, int _amount)
{
    //
    giftType = _type;
    amount = _amount;
    poped = false;
    
    setRandomSkin();
    sNode->setAnimation(0, "idle", true);
    sNode->setEventListener( [this] (int trackIndex, spEvent* event) {
        //
        // Set Event Name
        //
        if (!strcmp(event->data->name, "sound_idle_01")) {
            //
            SoundManager::getInstance()->playSoundEffect(SoundGift1, false);
        }
        else if (!strcmp(event->data->name, "sound_idle_02")) {
            //
            SoundManager::getInstance()->playSoundEffect(SoundGift2, false);
        }
        else if (!strcmp(event->data->name, "sound_idle_03")) {
            //
            SoundManager::getInstance()->playSoundEffect(SoundGift3, false);
        }
        else if (!strcmp(event->data->name, "coin_rain")) {
            if (showCoinsCallback) {
                showCoinsCallback(giftType);
            }
//            int eRate[] = {500, 1000, 1000, 2000, 4000};
//            coinPtcl->setEmissionRate(eRate[giftType]);
//            coinPtcl->resetSystem();
        }
        else if (!strcmp(event->data->name, "party_popper")) {
            //
            SoundManager::getInstance()->playSoundEffect(SoundGiftOpen, false);
            
            //
            for (int i=0; i<3; i++) {
                partyPtcl[i]->resetSystem();
            }
            showResultUI();
        }
    });
    
    this->setVisible(true);
    nextBtn->setVisible(false);

    setEnableLayer(true);

    this->resume();
}

void FreeGiftLayer::setEnableLayer(bool flag)
{
    this->setVisible(flag);
    if (flag) {
        // setTouchEnabled
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true);
        touchListener->onTouchBegan = CC_CALLBACK_2(FreeGiftLayer::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(FreeGiftLayer::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(FreeGiftLayer::onTouchEnded, this);
        touchListener->onTouchCancelled = CC_CALLBACK_2(FreeGiftLayer::onTouchCancelled, this);
        dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    }
    else {
        // setTouchDisabled
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->removeEventListenersForTarget(this);
    }
}

void FreeGiftLayer::popTheBox()
{
    //
    sNode->setAnimation(0, "open", false);
}

void FreeGiftLayer::showResultUI()
{
    //
    // showGetButton
    //
    nextBtn->setVisible(true);
    
    //
    // Result
    //
    resultCoinLabel->setString(std::to_string(amount));
    Size s1 = resultCoinLabel->getContentSize();
    Size s2 = coinIcon->getContentSize();
    float gap = 16;
    float totalWidth = s1.width + s2.width * coinIcon->getScale() + gap;
    float basePosX = visibleSizeHalf.width - totalWidth * 0.5;
    resultCoinLabel->setPosition(Vec2(basePosX, visibleSizeHalf.height) + origin);
    coinIcon->setPosition(Vec2(basePosX + s1.width + gap, visibleSizeHalf.height) + origin);
    
    resultCoinLabel->setVisible(true);
    coinIcon->setVisible(true);
    
    //
    // Initialize the Timer!!!
    //
    int restHour = 0;
    int restMin = 0;
    UserInfo::getInstance()->setGiftTime(restHour, restMin);

    //
    //
//    std::stringstream stream;
//    stream << std::setfill('0') << std::setw(2) << restHour;
//    std::string hrInfo = stream.str();
    std::string hrInfo = std::to_string(restHour);

    std::stringstream stream;
    stream << std::setfill('0') << std::setw(2) << restMin;
    std::string minInfo = stream.str();

    //
    stream.str("");
    stream.clear();
    stream << LocalizationManager::getInstance()->getLocalizationStringWithReplaceTwo("FreeGift", hrInfo, minInfo);
    std::string timeInfo = stream.str();

    TTFConfig config(UserInfo::getInstance()->getFontPath(), 50);
    auto *lbl = Label::createWithTTF(config, timeInfo, TextHAlignment::CENTER, 550);
//    auto lbl = Label::createWithTTF(timeInfo, UserInfo::getInstance()->getFontPath(), 50);
    lbl->setAnchorPoint(Vec2(0.5, 0));
    lbl->setPosition(Vec2(visibleSizeHalf.width, nextBtnItem->getPositionY() + nextBtnItem->getContentSize().height));
    lbl->setColor(Color3B(255, 7, 7));
    addChild(lbl, 1);
}

//
// Called from close button
// closeCallback should not play button sound
//
void FreeGiftLayer::btnCallback(Ref* pSender)
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    //
    for (int i=0; i<3; i++) {
        partyPtcl[i]->stopSystem();
    }

    //
    nextBtn->setVisible(false);
    resultCoinLabel->setVisible(false);
    coinIcon->setVisible(false);
    setEnableLayer(false);

    if (closeCallback) {
        closeCallback();
    }
}

bool FreeGiftLayer::onTouchBegan(Touch *touch, Event *event)
{
    return true;
}

void FreeGiftLayer::onTouchMoved(Touch *touch, Event *event)
{
}

void FreeGiftLayer::onTouchEnded(Touch *touch, Event *event)
{
    if (!poped) {
        //
        SoundManager::getInstance()->playSoundEffect(SoundGiftTap, false);
        
        //
        poped = true;
        popTheBox();
    }
}

void FreeGiftLayer::onTouchCancelled(Touch *touch, Event *event)
{
}
