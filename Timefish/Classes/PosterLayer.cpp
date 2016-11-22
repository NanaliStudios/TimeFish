//
//  PosterLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 3. 20..
//
//

#include "PosterLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

#define SelectionTag 1000

bool PosterLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();

    //
    closeCallback = NULL;

    ownsPriority = false;

    return true;
}

void PosterLayer::initMainLayerWithType(int userSelectedPoster)
{
    //
    removeAllChildren();
    
    posterIdx = userSelectedPoster - 1;
    
    // background color
    Color3B _bgColor = posterBgColor[posterIdx];
    auto bg = LayerColor::create(Color4B(_bgColor.r, _bgColor.g, _bgColor.b, 255));
    addChild(bg, 0);

    // poster
    {
        std::stringstream stream;
        stream << "TF_gameposter_" << std::setfill('0') << std::setw(2);
        Sprite *poster = NULL;
        if (userSelectedPoster == 11 || userSelectedPoster == 12) {
            stream << userSelectedPoster <<".jpg";
            std::string fileName = stream.str();
            poster = Sprite::create(fileName.c_str());
        }
        else if (userSelectedPoster == 10 || userSelectedPoster == 29 ||
                 userSelectedPoster == 30 || userSelectedPoster == 31) {
            stream << userSelectedPoster <<".png";
            std::string fileName = stream.str();
            poster = Sprite::create(fileName.c_str());
        }
        else {
            stream << userSelectedPoster <<".png";
            std::string fileName = stream.str();
            poster = Sprite::createWithSpriteFrameName(fileName.c_str());
        }
        
        poster->setPosition(Vec2(visibleSizeHalf) + origin);
        addChild(poster, 10);
        
        if (userSelectedPoster == 16 || userSelectedPoster == 17 || userSelectedPoster == 18) {
            poster->setScale(0.7);
        }
        
        //
        if (userSelectedPoster == 20) {
            auto posterBack = Sprite::createWithSpriteFrameName("TF_gameposter_20-2.png");
            posterBack->setAnchorPoint(Vec2(1, 0.5));
            posterBack->setPosition(Vec2(visibleSize.width, visibleSizeHalf.height) + origin);
            addChild(posterBack, 9);
        }
    }
    
    // logo
    if (userSelectedPoster != 10 && userSelectedPoster != 11 && userSelectedPoster != 12 &&
        userSelectedPoster != 29 && userSelectedPoster != 30 && userSelectedPoster != 31) { // except full screen poster
        Color3B logoColor[] = {
            Color3B(255,255,255), // 1
            Color3B(255,255,255), // 2
            Color3B(255,255,255), // 3
            Color3B(239,125, 27), // 4
            Color3B(198,198,198), // 5
            Color3B(244,200, 48), // 6
            Color3B(120,255,209), // 7
            Color3B(  7, 94, 63), // 8
            Color3B(251,228, 71), // 9
            Color3B(255,255,255), // 10
            Color3B(255,255,255), // 11
            Color3B( 22,126,251), // 12
            Color3B(255,255,255), // 13
            Color3B(255,255,255), // 14
            Color3B(255,255,255), // 15
            Color3B(255,255,255), // 16
            Color3B(255,255,255), // 17
            Color3B(255,255,255), // 18
            Color3B(255,237,196), // 19
            Color3B(255,255,255), // 20
            Color3B( 76,130, 72), // 21
            Color3B(255,255,255), // 22
            Color3B(255,189, 64), // 23
            Color3B(  0,  0,  0), // 24
            Color3B(255,255,255), // 25
            Color3B(255,255,255), // 26
            Color3B(  0,181,156), // 27
            Color3B(255,253,106), // 28
            Color3B(255,255,255), // 29
            Color3B(255,255,255), // 30
            Color3B(255,255,255), // 31
        };
        
        auto logo = Sprite::createWithSpriteFrameName("TF_gameposter_logo.png");
        logo->setColor(logoColor[posterIdx]);
        logo->setPosition(Vec2(visibleSizeHalf.width, visibleSize.height * 0.14) + origin);
        addChild(logo, 10);
        
        //
        if (userSelectedPoster == 8) {
            auto logoBg = Sprite::createWithSpriteFrameName("TF_gameposter_08-2.png");
            logoBg->setPosition(logo->getPosition() + origin);
            addChild(logoBg, 9);
        }
    }
}

#define PosterBtmPosRate 0.20636363636
void PosterLayer::initUILayer()
{
    //
    uiLayer = Layer::create();
    uiLayer->setPosition(Vec2::ZERO + origin);
    addChild(uiLayer, 150);

    //
    // Buttons
    //
    float topPosY = 0;
    
    // close button
    auto closeBtnItem = MenuItemSameImage::create("TF_UI_x.png", CC_CALLBACK_0(PosterLayer::removeThis, this));
    Size closeBtnSize = closeBtnItem->getContentSize();
    closeBtnItem->setOpacity(0);
    
    topPosY = visibleSize.height - closeBtnSize.height;
    
    auto closeBtn = Menu::create(closeBtnItem, NULL);
    closeBtn->setPosition(Vec2(closeBtnSize.width * 0.5 + 20, visibleSize.height - closeBtnSize.height* 0.5 - 20));
//    closeBtn->setPosition(Vec2(visibleSize.width - closeBtnSize.width, topPosY));
    uiLayer->addChild(closeBtn, 1);

    //
    // Top Layer
    //
    float topHeight = closeBtnSize.height * 2;//visibleSize.height * 0.0628169014;
    LayerColor *top = LayerColor::create(Color4B::BLACK, visibleSize.width, topHeight);
    top->setPosition(Vec2(0, visibleSize.height - topHeight));
    top->setOpacity(0);
    uiLayer->addChild(top);

    //
    // Bottom Layer
    //
    float btmHeight = visibleSize.height * 0.26408450704;
    LayerColor *bottom = LayerColor::create(Color4B::BLACK, visibleSize.width, btmHeight);
    bottom->setPosition(Vec2(0, btmHeight));
    bottom->setPosition(Vec2(0, 0));
    bottom->setOpacity(0);
    uiLayer->addChild(bottom);

    // setTitle button
    auto setBtnItem = MenuItemSameImage::create("TF_timeposter_main.png", CC_CALLBACK_1(PosterLayer::setTitle, this));
    Size s = setBtnItem->getContentSize();
    setBtnItem->setOpacity(0);
    
    setBtn = Menu::create(setBtnItem, NULL);
    float posX = visibleSizeHalf.width;
    setBtn->setPosition(Vec2(posX, s.height*1.5));
    uiLayer->addChild(setBtn, 1);

    // check mark
    checkMark = Sprite::createWithSpriteFrameName("TF_timeposter_v.png");
    checkMark->setPosition(setBtn->getPosition());
    uiLayer->addChild(checkMark, 1);
    checkMark->setOpacity(0);
    checkMark->setVisible(UserInfo::getInstance()->getSelectedPosterType() - 1 == posterIdx);

    //
    // Achievement Info
    //
    std::stringstream stream;
    stream << "AchievementName";
    stream << std::setfill('0') << std::setw(2) << posterIdx+1;
    std::string keyCode1 = stream.str();
    
    //
    stream.str("");
    stream.clear();
    stream << LocalizationManager::getInstance()->getLocalizationString(keyCode1);
    std::string achName = stream.str();
    
//    auto achievementName = Label::createWithSystemFont(achName, "Arial-BoldMT", 30);
    auto achievementName = Label::createWithTTF(achName, UserInfo::getInstance()->getFontPath(), 33);
    achievementName->setPosition(Vec2(visibleSizeHalf.width, topPosY));
    achievementName->setOpacity(0);
    uiLayer->addChild(achievementName, 1);
    
    //
    Size s2 = achievementName->getContentSize();

//    auto achievementName = Label::createWithSystemFont(getAchievementName(posterIdx), "Arial-BoldMT", 30);
//    achievementName->setPosition(Vec2(visibleSizeHalf.width, topPosY));
//    achievementName->setOpacity(0);
//    uiLayer->addChild(achievementName, 1);
    
    //
    stream.str("");
    stream.clear();
    stream << "AchievementDesc";
    stream << std::setfill('0') << std::setw(2) << posterIdx+1;
    std::string keyCode2 = stream.str();
    
    //
    stream.str("");
    stream.clear();
    stream << LocalizationManager::getInstance()->getLocalizationString(keyCode2);
    std::string achInfo = stream.str();
    
//    auto achievementInfo = Label::createWithSystemFont(achInfo, "ArialMT", 35);
    auto achievementInfo = Label::createWithTTF(achInfo, UserInfo::getInstance()->getFontPath(), 32);
    achievementInfo->setPosition(Vec2(visibleSizeHalf.width, s2.height));
    achievementInfo->setOpacity(0);
    uiLayer->addChild(achievementInfo, 1);

//    auto achievementInfo = Label::createWithSystemFont(getAchievementInfo(posterIdx), "ArialMT", 35);
//    Size s2 = achievementName->getContentSize();
//    achievementInfo->setPosition(Vec2(visibleSizeHalf.width, s2.height));
//    achievementInfo->setOpacity(0);
//    uiLayer->addChild(achievementInfo, 1);

    //
    // Fade in
    //
    uiVisible = true;
    float fadeInTime = 0.7;
    top->runAction(FadeTo::create(fadeInTime, 150));
    bottom->runAction(FadeTo::create(fadeInTime, 150));
    closeBtnItem->runAction(FadeTo::create(fadeInTime, 150));
    setBtnItem->runAction(FadeIn::create(fadeInTime));

    checkMark->runAction(FadeIn::create(fadeInTime));
    achievementName->runAction(FadeIn::create(fadeInTime));
    achievementInfo->runAction(FadeIn::create(fadeInTime));
}

void PosterLayer::removeThis()
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    if (closeCallback) {
        closeCallback(this);
    }
    removeFromParent();
}

void PosterLayer::setTitle(Ref *pSender)
{
    MenuItemSameImage *btnItem = (MenuItemSameImage*)pSender;
    if (btnItem) {
        SoundManager::getInstance()->playSoundEffect(SoundButton, false);

        int posterType = posterIdx + 1;

        checkMark->setVisible(true);
        UserInfo::getInstance()->setSelectedPosterType(posterType);
        
        TooltipLayer *tooltip = TooltipLayer::create();
        Vec2 pos = Vec2(setBtn->getPositionX(), setBtn->getPositionY() + btnItem->getContentSize().height);
        tooltip->initWithStringKey(pos, btnItem->getContentSize().width, 0);
        tooltip->enableLayerTouch();
        uiLayer->addChild(tooltip, 100);
        
        //
        FlurryX::getInstance()->logPosterSelection("PosterTitled", posterType);
        //
        TapjoyX::getInstance()->logEventInUIFlow("SharePoster", posterType);
    }
}

void PosterLayer::enableLayerTouch()
{
    // setTouchEnabled
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(PosterLayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(PosterLayer::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(PosterLayer::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(PosterLayer::onTouchCancelled, this);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // set keyInput enabled
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = CC_CALLBACK_2(PosterLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    ownsPriority = true;
#endif
}

void PosterLayer::disableLayerTouch()
{
    // setTouchDisabled
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->pauseEventListenersForTarget(this);

    ownsPriority = false;
}

bool PosterLayer::onTouchBegan(Touch *touch, Event *event)
{
    return true;
}

void PosterLayer::onTouchMoved(Touch *touch, Event *event)
{
}

void PosterLayer::onTouchEnded(Touch *touch, Event *event)
{
    uiVisible = !uiVisible;
    uiLayer->setVisible(uiVisible);
}

void PosterLayer::onTouchCancelled(Touch *touch, Event *event)
{
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void PosterLayer::onKeyReleased( EventKeyboard::KeyCode keycode, Event *event )
{
//    log("MainUIStatus: PosterLayer::onKeyReleased: %d", ownsPriority);
    //
    // Process back button for Android Devices...
    //
    if (ownsPriority && keycode == EventKeyboard::KeyCode::KEY_BACK) {
        event->stopPropagation();
        event->stopPropagation();
        removeThis();
    }
}
#endif
