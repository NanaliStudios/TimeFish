//
//  TutorialLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 7. 30..
//
//

#include "TutorialLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

bool TutorialLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();

    ownsPriority = true;

    //
    initMainLayer();
    
    // setTouchEnabled
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(TutorialLayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(TutorialLayer::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(TutorialLayer::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(TutorialLayer::onTouchCancelled, this);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // set keyInput enabled
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(TutorialLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif
    
    return true;
}

void TutorialLayer::initMainLayer()
{
    //
    // Page Layers
    //
    for (int i=0; i<5; i++) {
        pageLayer[i] = Layer::create();
        pageLayer[i]->setPosition(Vec2::ZERO);
        pageLayer[i]->setVisible(false);
        addChild(pageLayer[i], 0);
    }
    
    //
    // UI Layer
    //
    uiLayer = Layer::create();
    uiLayer->setPosition(Vec2::ZERO);
    addChild(uiLayer, 10);
    
    //
    // skip button
    //
    auto btnItem = MenuItemImageButton::create();
    btnItem->setNormalImage(Sprite::createWithSpriteFrameName("button_middle_white.png"));
    btnItem->setSelectedImage(Sprite::createWithSpriteFrameName("button_middle_white_click.png"));
    btnItem->setCallback(CC_CALLBACK_1(TutorialLayer::btnCallback, this));

    //
    Size s = btnItem->getContentSize();
    btnItem->setPosition(Vec2(visibleSizeHalf.width, s.height*0.5 + 20) + origin);
    
    auto btn = Menu::create(btnItem, NULL);
    btn->setPosition(Vec2::ZERO);
    uiLayer->addChild(btn, 1);
    
    // add icon
    auto icon = Sprite::createWithSpriteFrameName("icon_play.png");
    icon->setPosition(Vec2(s) * 0.5);
    btnItem->addChild(icon, 1);

    //
    btnItem->setLabelChild(icon);

    scaleFactor = UserInfo::getInstance()->screenScale;

    //
    // page dots
    //
//    float deltaX = 48;
//    float dotPosX[5] = { -2, -1, 0, 1, 2};
//    for (int i=0; i<5; i++) {
//        pageDots[i] = Sprite::createWithSpriteFrameName("TF_timeposter_comma.png");
//        pageDots[i]->setPosition(Vec2(btnItem->getPositionX() + deltaX*dotPosX[i], btnItem->getPositionY() + s.height + 5));
//        pageDots[i]->setOpacity(125);
//        pageDots[i]->setColor(Color3B::WHITE);
//        uiLayer->addChild(pageDots[i], 1);
//    }
//    pageDots[0]->setColor(Color3B::RED);
//
    
    //
    // Close Button
    //
    {
        // close button
        auto closeBtnItem = MenuItemSameImage::create("TF_UI_x.png", CC_CALLBACK_0(TutorialLayer::hideThis, this));
        Size closeBtnSize = closeBtnItem->getContentSize();
        closeBtnItem->setOpacity(150);
        
        topPosY = visibleSize.height - closeBtnSize.height;

        auto closeBtn = Menu::create(closeBtnItem, NULL);
        closeBtn->setPosition(Vec2(closeBtnSize.width * 0.5 + 20, visibleSize.height - closeBtnSize.height* 0.5 - 20) + origin);
        uiLayer->addChild(closeBtn, 10);

        //
        topPosY -= closeBtnSize.height;
    }

    topPosY = visibleSize.height * 0.82;

    //
    // Set Pages
    //
    currentPage = 0;
    pageLayer[0]->setVisible(true);
    setPage1();
    setPage2();
    setPage3();
    setPage4();
    setPage5();
}

#define PosRate 0.47
#define FontSize 30
void TutorialLayer::setPage1()
{
    auto img = Sprite::createWithSpriteFrameName("tutorial_image_01.png");
    img->setPosition(Vec2(visibleSizeHalf.width, visibleSize.height * PosRate) + origin);
    pageLayer[0]->addChild(img);

    img->setScale(scaleFactor);

    //
    std::stringstream stream;
    stream << "Tutorial_Script_01";
    std::string keyCode = stream.str();

    //
    stream.str("");
    stream.clear();
    stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
    std::string nameInfo = stream.str();

    TTFConfig config(UserInfo::getInstance()->getFontPath(), FontSize);
    Label *scriptLabel = Label::createWithTTF(config, nameInfo, TextHAlignment::CENTER, 550);
    scriptLabel->setAnchorPoint(Vec2(0.5, 0.5));
    scriptLabel->setPosition(Vec2(visibleSizeHalf.width, topPosY) + origin);
    scriptLabel->setTextColor(Color4B::WHITE);
    pageLayer[0]->addChild(scriptLabel, 1);

    //
    scriptLabel->setScale(scaleFactor);
}

void TutorialLayer::setPage2()
{
    //
    auto img1 = Sprite::createWithSpriteFrameName("tutorial_image_02-1.png");
    img1->setAnchorPoint(Vec2(0.5, 0));
    img1->setPosition(Vec2(visibleSizeHalf.width, visibleSizeHalf.height + 25) + origin);
    pageLayer[1]->addChild(img1);
    
    img1->setScale(scaleFactor);

    //
    auto img2 = Sprite::createWithSpriteFrameName("tutorial_image_02-2.png");
    img2->setAnchorPoint(Vec2(0.5, 1));
    img2->setPosition(Vec2(visibleSizeHalf.width, visibleSizeHalf.height - 25) + origin);
    pageLayer[1]->addChild(img2);

    img2->setScale(scaleFactor);

    //
    auto img3 = Sprite::createWithSpriteFrameName("tutorial_image_02-hand.png");
    Size s3 = img3->getContentSize()*scaleFactor;
    img3->setPosition(Vec2(img2->getPositionX(), img2->getPositionY()) + Vec2(s3.width * 0.7, -s3.height * 0.85));
    pageLayer[1]->addChild(img3);

    img3->setScale(scaleFactor);

    //
    std::stringstream stream;
    stream << "Tutorial_Script_02";
    std::string keyCode = stream.str();
    
    //
    stream.str("");
    stream.clear();
    stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
    std::string nameInfo = stream.str();

    TTFConfig config(UserInfo::getInstance()->getFontPath(), FontSize);
    Label *scriptLabel = Label::createWithTTF(config, nameInfo, TextHAlignment::CENTER, 600);
    scriptLabel->setAnchorPoint(Vec2(0.5, 0.5));
    scriptLabel->setPosition(Vec2(visibleSizeHalf.width, topPosY) + origin);
    scriptLabel->setTextColor(Color4B::WHITE);
    pageLayer[1]->addChild(scriptLabel, 1);

    //
    scriptLabel->setScale(scaleFactor);
}

#define GapY 20
void TutorialLayer::setPage3()
{
    auto img1 = Sprite::createWithSpriteFrameName("tutorial_image_03-1.png");
    img1->setAnchorPoint(Vec2(0.5, 0));
    img1->setPosition(Vec2(visibleSizeHalf.width, visibleSize.height * PosRate + GapY) + origin);
//    Size s1 = img1->getContentSize();
    pageLayer[2]->addChild(img1);
    img1->setScale(scaleFactor);
    
    auto img2 = Sprite::createWithSpriteFrameName("tutorial_image_03-2.png");
    img2->setAnchorPoint(Vec2(0.5, 1));
    img2->setPosition(Vec2(visibleSizeHalf.width, visibleSize.height * PosRate - GapY) + origin);
//    Size s2 = img2->getContentSize();
    pageLayer[2]->addChild(img2);
    img2->setScale(scaleFactor);

    //
    std::stringstream stream;
    stream << "Tutorial_Script_03";
    std::string keyCode = stream.str();
    
    //
    stream.str("");
    stream.clear();
    stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
    std::string nameInfo = stream.str();
    
    TTFConfig config(UserInfo::getInstance()->getFontPath(), FontSize);
    Label *scriptLabel = Label::createWithTTF(config, nameInfo, TextHAlignment::CENTER, 550);
    scriptLabel->setAnchorPoint(Vec2(0.5, 0.5));
    scriptLabel->setPosition(Vec2(visibleSizeHalf.width, topPosY) + origin);
    scriptLabel->setTextColor(Color4B::WHITE);
    pageLayer[2]->addChild(scriptLabel, 1);

    //
    scriptLabel->setScale(scaleFactor);
}

void TutorialLayer::setPage4()
{
    float posY = topPosY;

    //
    std::stringstream stream;
    stream << "Tutorial_Script_04";
    std::string keyCode = stream.str();
    
    //
    stream.str("");
    stream.clear();
    stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
    std::string nameInfo = stream.str();

    TTFConfig config(UserInfo::getInstance()->getFontPath(), FontSize);
    Label *scriptLabel = Label::createWithTTF(config, nameInfo, TextHAlignment::CENTER, 550);
    scriptLabel->setAnchorPoint(Vec2(0.5, 0.5));
    scriptLabel->setPosition(Vec2(visibleSizeHalf.width, posY) + origin);
    scriptLabel->setTextColor(Color4B::WHITE);
    pageLayer[3]->addChild(scriptLabel, 1);
    //
    scriptLabel->setScale(scaleFactor);

    const char* fileNames[] = {
        "tutorial_image_04-1.png",
        "tutorial_image_04-2.png",
        "tutorial_image_04-3.png",
        "tutorial_image_04-4.png",
        "tutorial_image_04-5.png",
    };

    const Color4B colors[] = {
        Color4B(253, 59, 70, 255),
        Color4B(246, 176, 50, 255),
        Color4B(10, 127, 249, 255),
        Color4B(165, 215, 70, 255),
        Color4B(186, 91, 229, 255),
    };

    //
//    posY -= 180;

    float deltaY = 100;
    float multY[] = {2, 1, 0, -1, -2};
    for (int i=0; i<5; i++) {
        
        posY = visibleSizeHalf.height + multY[i] * deltaY;

        //
        auto icon = Sprite::createWithSpriteFrameName(fileNames[i]);
        Size s2 = icon->getContentSize();
        icon->setAnchorPoint(Vec2(1, 0.5));
        pageLayer[3]->addChild(icon);
        icon->setScale(scaleFactor);

        //
        float basePosX = 120;
        icon->setPosition(Vec2(basePosX - 10, posY) + origin);

        //
        {
            std::stringstream stream;
            stream << "Tutorial_Boost_";
            stream << std::setfill('0') << std::setw(2) << i+1;
            std::string keyCode = stream.str();
            
            //
            stream.str("");
            stream.clear();
            stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
            std::string boostTitle = stream.str();
            
            Label *titleLbl = Label::createWithTTF(boostTitle, UserInfo::getInstance()->getFontPath(), FontSize);
            titleLbl->setAnchorPoint(Vec2(0, 0));
            titleLbl->setTextColor(colors[i]);
            pageLayer[3]->addChild(titleLbl, 1);
            //
            titleLbl->setScale(scaleFactor);

            
            //
            titleLbl->setPosition(Vec2(basePosX + 10, posY) + origin);
        }

        //
        {
            std::stringstream stream;
            stream << "Tutorial_Boost_Script_";
            stream << std::setfill('0') << std::setw(2) << i+1;
            std::string keyCode = stream.str();
            
            //
            stream.str("");
            stream.clear();
            stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
            std::string boostInfo = stream.str();

            TTFConfig config(UserInfo::getInstance()->getFontPath(), 25);
            Label *descLbl = Label::createWithTTF(config, boostInfo, TextHAlignment::LEFT, 470);
//            Label *descLbl = Label::createWithTTF(boostInfo, UserInfo::getInstance()->getFontPath(), 25);
            descLbl->setAnchorPoint(Vec2(0, 1));
            descLbl->setTextColor(Color4B::WHITE);
            pageLayer[3]->addChild(descLbl, 1);
            //
            descLbl->setScale(scaleFactor);

            //
            descLbl->setPosition(Vec2(basePosX + 10, posY) + origin);
        }
        
//        posY -= 115;
    }
}

void TutorialLayer::setPage5()
{
    auto img1 = Sprite::createWithSpriteFrameName("tutorial_image_05-1.png");
    img1->setAnchorPoint(Vec2(0.5, 0));
    img1->setPosition(Vec2(visibleSizeHalf.width, visibleSize.height * PosRate + GapY) + origin);
//    Size s1 = img1->getContentSize();
    pageLayer[4]->addChild(img1);
    img1->setScale(scaleFactor);

    auto img2 = Sprite::createWithSpriteFrameName("tutorial_image_05-2.png");
    img2->setAnchorPoint(Vec2(0.5, 1));
    img2->setPosition(Vec2(visibleSizeHalf.width, visibleSize.height * PosRate - GapY) + origin);
//    Size s2 = img2->getContentSize();
    pageLayer[4]->addChild(img2);
    img2->setScale(scaleFactor);

    //
    std::stringstream stream;
    stream << "Tutorial_Script_05";
    std::string keyCode = stream.str();
    
    //
    stream.str("");
    stream.clear();
    stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
    std::string nameInfo = stream.str();
    
    TTFConfig config(UserInfo::getInstance()->getFontPath(), FontSize);
    Label *scriptLabel = Label::createWithTTF(config, nameInfo, TextHAlignment::CENTER, 550);
    scriptLabel->setAnchorPoint(Vec2(0.5, 0.5));
    scriptLabel->setPosition(Vec2(visibleSizeHalf.width, topPosY) + origin);
    scriptLabel->setTextColor(Color4B::WHITE);
    pageLayer[4]->addChild(scriptLabel, 1);

    //
    scriptLabel->setScale(scaleFactor);
}

void TutorialLayer::btnCallback(Ref *pSender)
{
    currentPage++;
    if (currentPage >= 5) {
        hideThis();
    }
    else {
        SoundManager::getInstance()->playSoundEffect(SoundButton, false);

        int prevPage = currentPage - 1;
        pageLayer[prevPage]->setVisible(false);
        pageLayer[currentPage]->setVisible(true);

//        for (int i=0; i<5; i++) {
//            if (currentPage == i) {
//                pageDots[i]->setColor(Color3B::RED);
//            }
//            else {
//                pageDots[i]->setColor(Color3B::WHITE);
//            }
//        }
    }
}

void TutorialLayer::hideThis()
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    UserInfo::getInstance()->haveSeenTutorial = true;
    
    ownsPriority = false;

    if (closeCallback) {
        closeCallback(this);
    }

    removeFromParent();
}

bool TutorialLayer::onTouchBegan(Touch *touch, Event *event)
{
    return true;
}

void TutorialLayer::onTouchMoved(Touch *touch, Event *event)
{
}

void TutorialLayer::onTouchEnded(Touch *touch, Event *event)
{
}

void TutorialLayer::onTouchCancelled(Touch *touch, Event *event)
{
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void TutorialLayer::onKeyReleased( EventKeyboard::KeyCode keycode, Event *event )
{
//    log("MainUIStatus: TutorialLayer::onKeyReleased: %d", ownsPriority);
    if (ownsPriority && keycode == EventKeyboard::KeyCode::KEY_BACK) {
        event->stopPropagation();
        hideThis();
    }
}
#endif
