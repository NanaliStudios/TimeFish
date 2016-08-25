//
//  CreditLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 3. 20..
//
//

#include "CreditLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

#define SelectionTag 1000

bool CreditLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();

    scaleFactor = UserInfo::getInstance()->screenScale;

    ownsPriority = false;

    enableLayerTouch();

    //
    closeCallback = NULL;
    
    initBackgroundLayer();
    initMovingLayer();
    initUILayer();

    return true;
}

void CreditLayer::initBackgroundLayer()
{
    // background color
    auto bg = LayerColor::create(Color4B(233, 149, 32, 255));
    addChild(bg, 0);
}

void CreditLayer::initMovingLayer()
{
    Vec2 initPos = Vec2(visibleSizeHalf.width, visibleSize.height * 0.2);

    //
    // Moving Layer
    //
    movingLayer = Layer::create();
    movingLayer->setPosition(initPos);
    addChild(movingLayer, 50);

    float posY = 0;

    //
    auto creditSprite = Sprite::createWithSpriteFrameName("TF_Font_big+shadow.png");
    creditSprite->setPosition(Vec2(0, posY));
    movingLayer->addChild(creditSprite);

    posY -= 100;

    // TITLE
    {
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("OptionCredit");
        std::string nameInfo = stream.str();
        
        Label *nameLabel = Label::createWithTTF(nameInfo, UserInfo::getInstance()->getFontPath(), 58);
        nameLabel->setTextColor(Color4B(122, 50, 190, 255));
        nameLabel->setPosition(Vec2(0, posY));
        movingLayer->addChild(nameLabel, 1);
    }

    //
    // Peoples
    //
    const char* keyNames[] = {
        "CreditPD",
        "CreditPG",
        "CreditAR",
        "CreditGD",
        "CreditSD",
        "CreditMS",
        "CreditBS",
        "CreditTT",
    };
    int pplIdx[] = {1, 2, 3,4,5, 6, 7, 4, 8, 9, 10};
    int peoples[] = {1,1,3,1,1,1,1,2};

    posY -= 100;

    int cnt = 0;
    for (int i=0; i<8; i++) {
        // TITLE
        {
            std::stringstream stream;
            stream << LocalizationManager::getInstance()->getLocalizationString(keyNames[i]);
            std::string nameInfo = stream.str();
            
            Label *nameLabel = Label::createWithTTF(nameInfo, UserInfo::getInstance()->getFontPath(), 40);
            if (i == 6) {
                nameLabel->setTextColor(Color4B(122, 50, 190, 255));
            }
            else {
                nameLabel->setTextColor(Color4B(255, 61, 1, 255));
            }
            nameLabel->setPosition(Vec2(0, posY));
            movingLayer->addChild(nameLabel, 1);
        }
        
        for (int j=0; j< peoples[i]; j++) {
            //
            posY -= 40;

            std::stringstream stream;
            stream << "CreditPPL" << pplIdx[cnt];
            std::string keyName = stream.str();

            //
            stream.str("");
            stream.clear();
            stream << LocalizationManager::getInstance()->getLocalizationString(keyName);
            std::string nameInfo = stream.str();
            
            Label *nameLabel = Label::createWithTTF(nameInfo, UserInfo::getInstance()->getFontPath(), 35);
            nameLabel->setTextColor(Color4B::BLACK);
            nameLabel->setPosition(Vec2(0, posY));
            movingLayer->addChild(nameLabel, 1);

            //
            cnt++;
        }
        
        //
        posY -= 80;
    }
    
    //
    {
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("CreditN");
        std::string nameInfo = stream.str();

        Label *nameLabel = Label::createWithTTF(nameInfo, UserInfo::getInstance()->getFontPath(), 35);
        nameLabel->setTextColor(Color4B::BLACK);
        nameLabel->setPosition(Vec2(0, posY));
        movingLayer->addChild(nameLabel, 1);
        
        //
        posY -= 80;
    }

    //
    float gapY = 50;
    posY -= gapY;
    float dotBasePosY = posY;
    for (int i=0; i<10; i++) {
        Label *nameLabel = Label::createWithTTF(".", UserInfo::getInstance()->getFontPath(), 50);
        nameLabel->setTextColor(Color4B::BLACK);
        nameLabel->setPosition(Vec2(0, posY));
        movingLayer->addChild(nameLabel, 1);

        posY = dotBasePosY - gapY * i;
    }

    //
    {
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("CreditU");
        std::string nameInfo = stream.str();

        youLabel = Label::createWithTTF(nameInfo, UserInfo::getInstance()->getFontPath(), 60);
        youLabel->setTextColor(Color4B(255, 61, 1, 255));
        youLabel->setPosition(Vec2(0, posY));
        movingLayer->addChild(youLabel, 1);
    }

    //
    posY -= youLabel->getContentSize().height;
    {
        auto deepFish = MenuItemSameImage::create("TF_credit_hiddenfish.png", CC_CALLBACK_0(CreditLayer::getAchievement, this));
        deepFish->setTag(1);
        Size s = deepFish->getContentSize();
        
        deepFishBtn = Menu::create(deepFish, NULL);
        deepFishBtn->setPosition(Vec2(0, posY - deepFish->getContentSize().height * 0.5));
        movingLayer->addChild(deepFishBtn, 1);

        if (UserInfo::getInstance()->getAchievementInfoWithType(AchievementType07)) {
            deepFish->setEnabled(false);
        }
        else {
            deepFishBtn->setVisible(false);
        }
    }

    //
    // Move!!!
    //
    Vec2 moveToPos = Vec2(0, fabsf(posY) + visibleSize.height * 0.65);
    auto seq = Sequence::create(
                        DelayTime::create(0.5),
                        MoveBy::create(25, moveToPos),
                        MoveTo::create(0, Vec2(initPos.x, 0)),
                        NULL);
    movingLayer->runAction(RepeatForever::create(seq));
    
    //
    schedule(CC_SCHEDULE_SELECTOR(CreditLayer::checkMovingLayerPos));
}

void CreditLayer::getAchievement()
{
    //
    FlurryX::getInstance()->logEvent("DeepSeaFish");

    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    if(UserInfo::getInstance()->setAchievementInfoWithType(AchievementType07))
        UserInfo::getInstance()->getNewAchievement[6] = true;
    else
        UserInfo::getInstance()->getNewAchievement[6] = false;
    
    if (light) {
        light->removeFromParent();
        deepFishBtn->setEnabled(false);
    }
}

void CreditLayer::checkMovingLayerPos(float dt)
{
    Vec2 pos = movingLayer->convertToWorldSpace(youLabel->getPosition());
//    if (fabsf(pos.y - visibleSize.height*0.65 - origin.y) < 3) { // origin.y !!!
    if (fabsf(pos.y - visibleSize.height*0.65 - origin.y) < 3) { // origin.y !!!
        unschedule(CC_SCHEDULE_SELECTOR(CreditLayer::checkMovingLayerPos));
        movingLayer->stopAllActions();

        deepFishBtn->runAction(Sequence::create(
                                    DelayTime::create(0.3),
                                    Show::create(),
                                    CallFunc::create(CC_CALLBACK_0(CreditLayer::bouncingButton, this)),
                                    NULL));
    }
}

void CreditLayer::bouncingButton()
{
    // no need to show if already achieved.
    if (UserInfo::getInstance()->getAchievementInfoWithType(AchievementType07)) {
        return;
    }

    light = Sprite::createWithSpriteFrameName("TF_Tuto_light.png");
    light->setAnchorPoint(Vec2(0.5, 0));
    light->setRotation(-45);
    movingLayer->addChild(light, 10);
    
    Node *btnItem = deepFishBtn->getChildByTag(1);
    Size s = (btnItem)? btnItem->getContentSize() : Size::ZERO;
    light->setPosition(deepFishBtn->getPosition() + Vec2(-s.width, s.height)*0.5);
    light->runAction(RepeatForever::create(Blink::create(2, 10)));
}

void CreditLayer::initUILayer()
{
    //
    uiLayer = Layer::create();
    uiLayer->setPosition(Vec2(visibleSizeHalf.width, 0));
    addChild(uiLayer, 150);
    
    //
    // Bottom Layer
    //
    float bgPosY = 320/scaleFactor * 0.9;
    LayerColor *bottom = LayerColor::create(Color4B(254, 7, 7, 255), visibleSize.width, bgPosY);
    bottom->setPosition(Vec2(-visibleSizeHalf.width, 0));
    uiLayer->addChild(bottom);

    //
    auto bgTimon = Sprite::createWithSpriteFrameName("TF_credit_timon.png");
    bgTimon->setAnchorPoint(Vec2(0.5, 0));
    bgTimon->setPosition(Vec2(0, bgPosY - 10));
    uiLayer->addChild(bgTimon);

    //
    // Copyright
    std::stringstream stream;
    stream << LocalizationManager::getInstance()->getLocalizationString("CreditCopyRight");
    std::string nameInfo = stream.str();
    
    auto copyright = Label::createWithTTF(nameInfo, EnglishFontPath, 25);
    copyright->setTextColor(Color4B::BLACK);
    copyright->setPosition(Vec2(0, copyright->getContentSize().height * 1.1) + origin);
    copyright->setScale(scaleFactor);
    uiLayer->addChild(copyright, 1);

    //
    // Buttons
    //
//    float baseRatio = ((float)BaseScreenWidth/(float)BaseScreenHeight);
//    float currRatio = visibleSize.width/visibleSize.height;
//    bool ratioCond = currRatio > baseRatio;

    float posY = copyright->getPositionY() + (copyright->getContentSize().height + 50) * scaleFactor;

    // Homepage button
    {
        auto menuItem = MenuItemSameImage::create("TF_credit_UI-nanali.png", CC_CALLBACK_0(CreditLayer::moveToHomepage, this));
        Size s = menuItem->getContentSize() * scaleFactor;
        
        auto homeBtn = Menu::create(menuItem, NULL);
//        float posX = (ratioCond)? s.width * 0.7 - visibleSizeHalf.width : 0 - s.width * 0.7;
//        float buttonPosY = (ratioCond)? posY - s.height * 0.5: posY;
        float posX = - s.width * 0.7;
        float buttonPosY = posY;
        homeBtn->setPosition(Vec2(posX + origin.x, buttonPosY));
        uiLayer->addChild(homeBtn, 1);
        
        menuItem->setScale(scaleFactor);
    }

    // youtube button
    {
        auto menuItem = MenuItemSameImage::create("TF_credit_UI-youtube.png", CC_CALLBACK_0(CreditLayer::moveToYoutube, this));
        Size s = menuItem->getContentSize() * scaleFactor;
        
        auto shareBtn = Menu::create(menuItem, NULL);
//        float posX = (ratioCond)? -s.width * 0.7 + visibleSizeHalf.width : 0 + s.width * 0.7;
//        float buttonPosY = (ratioCond)? posY - s.height * 0.5: posY;
        float posX = s.width * 0.7;
        float buttonPosY = posY;
        shareBtn->setPosition(Vec2(posX + origin.x, buttonPosY));
        uiLayer->addChild(shareBtn, 1);
        
        menuItem->setScale(scaleFactor);

    }
    
    // Nanali Studios
    stream.str("");
    stream.clear();
    stream << LocalizationManager::getInstance()->getLocalizationString("CreditNanali");
    std::string creditInfo = stream.str();
    
    auto nanaliStudio = Label::createWithTTF(creditInfo, EnglishFontPath, 42);
    nanaliStudio->setTextColor(Color4B::WHITE);
    Size s = nanaliStudio->getContentSize();

//    if (ratioCond) {
//        //
//        posY = posY;
//    }
//    else {
        posY += 140 * scaleFactor;
//    }

    nanaliStudio->setPosition(Vec2(origin.x, posY));
    nanaliStudio->setScale(scaleFactor);
    uiLayer->addChild(nanaliStudio);
    
    // Nanali Lab
    {
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("CreditNanaliLab");
        std::string nameInfo = stream.str();
        
        auto nanaliLab = Label::createWithTTF(nameInfo, EnglishFontPath, 42);
        nanaliLab->setTextColor(Color4B::WHITE);
        nanaliLab->setPosition(Vec2(0, nanaliStudio->getPositionY() - s.height * 0.8 * scaleFactor));
        nanaliLab->setScale(scaleFactor);
        uiLayer->addChild(nanaliLab, 1);
    }

    //
    // close button
    //
    {
        auto menuItem = MenuItemSameImage::create("TF_UI_x.png", CC_CALLBACK_0(CreditLayer::removeThis, this));
        Size closeBtnSize = menuItem->getContentSize();
        menuItem->setOpacity(125);
        
        auto closeBtn = Menu::create(menuItem, NULL);
//        closeBtn->setPosition(Vec2(s.width, visibleSize.height - s.height) + origin);
        closeBtn->setPosition(Vec2(closeBtnSize.width * 0.5 + 20, visibleSize.height - closeBtnSize.height* 0.5 - 20) + origin);
        addChild(closeBtn, 1);
    }
}

//
// Called from close button or backbutton(Android)
// closeCallback should not play button sound
//
void CreditLayer::removeThis()
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);
    
    if (closeCallback) {
        closeCallback(this);
    }
    removeFromParent();
}

void CreditLayer::moveToHomepage()
{
    //
    FlurryX::getInstance()->logEvent("Homepage");

    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    SocialX::getInstance()->openHomepage();

//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    FlurryX::getInstance()->openHomepage();
//#else
//    SocialX::getInstance()->openHomepage();
//#endif
}

void CreditLayer::moveToYoutube()
{
    //
    FlurryX::getInstance()->logEvent("Video");

    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    SocialX::getInstance()->openYoutubePage();

//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    FlurryX::getInstance()->openYoutubePage();
//#else
//    SocialX::getInstance()->openYoutubePage();
//#endif
}

void CreditLayer::enableLayerTouch()
{
    // setTouchEnabled
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(CreditLayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(CreditLayer::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(CreditLayer::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(CreditLayer::onTouchCancelled, this);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // set keyInput enabled
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(CreditLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    ownsPriority = true;
#endif
}

void CreditLayer::disableLayerTouch()
{
    // setTouchDisabled
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->pauseEventListenersForTarget(this);

    ownsPriority = false;
}

bool CreditLayer::onTouchBegan(Touch *touch, Event *event)
{
    return true;
}

void CreditLayer::onTouchMoved(Touch *touch, Event *event)
{
}

void CreditLayer::onTouchEnded(Touch *touch, Event *event)
{
}

void CreditLayer::onTouchCancelled(Touch *touch, Event *event)
{
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void CreditLayer::onKeyReleased( EventKeyboard::KeyCode keycode, Event *event )
{
//    log("MainUIStatus: CreditLayer::onKeyReleased: %d", ownsPriority);
    if (ownsPriority && keycode == EventKeyboard::KeyCode::KEY_BACK) {
        event->stopPropagation();
        removeThis();
    }
}
#endif
