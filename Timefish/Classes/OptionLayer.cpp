//
//  OptionLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 7. 29..
//
//

#include "OptionLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

bool OptionLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();

    //
    soundBtnCnt = 0;
    
    ownsPriority = false;

    //
    initMainLayer();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // set keyInput enabled
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(OptionLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif

    return true;
}

void OptionLayer::initMainLayer()
{
    //
    // Main Layer
    //
    mainLayer = Layer::create();
    mainLayer->setPosition(Vec2::ZERO);
    addChild(mainLayer, 0);

    int mult[] = { -1, 0, 1 };

    //
    // up buttons
    //

    // sound button
    {
        auto btnItem1 = MenuItemImage::create();
        btnItem1->setNormalImage(Sprite::createWithSpriteFrameName("button_middle_white.png"));
        btnItem1->setSelectedImage(Sprite::createWithSpriteFrameName("button_middle_white_click.png"));
        
        auto btnItem2 = MenuItemImage::create();
        btnItem2->setNormalImage(Sprite::createWithSpriteFrameName("button_middle_white.png"));
        btnItem2->setSelectedImage(Sprite::createWithSpriteFrameName("button_middle_white_click.png"));
        
        auto menuItem = MenuItemToggle::createWithCallback( CC_CALLBACK_1(OptionLayer::setToggleButton, this),
                                                           btnItem1, btnItem2, nullptr );
        //
        menuItem->setTag(0);

        int seledtedIdx = SoundManager::getInstance()->getMuted();
        menuItem->setSelectedIndex(seledtedIdx);
        Size s = menuItem->getContentSize();

        auto onIcon = Sprite::createWithSpriteFrameName("icon_sound.png");
        onIcon->setPosition(Vec2(s) * 0.5);
        onIcon->setTag(0);
        menuItem->addChild(onIcon, 100);

        auto offIcon = Sprite::createWithSpriteFrameName("icon_sound_off.png");
        offIcon->setPosition(Vec2(s) * 0.5);
        offIcon->setTag(1);
        menuItem->addChild(offIcon, 100);
        
        if (seledtedIdx == 0) {
            onIcon->setVisible(true);
            offIcon->setVisible(false);
        }
        else {
            onIcon->setVisible(false);
            offIcon->setVisible(true);
        }

        //
        menuItem->setPosition(Vec2(visibleSizeHalf.width - (s.width + 14), visibleSizeHalf.height + s.height) + origin);
        
        
        auto soundBtn = Menu::create(menuItem, NULL);
        soundBtn->setPosition(Vec2::ZERO);
        addChild(soundBtn, 1);
        
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("OptionSound");
        std::string btnInfo = stream.str();

        TTFConfig config(UserInfo::getInstance()->getFontPath(), 32);
        auto *lbl = Label::createWithTTF(config, btnInfo, TextHAlignment::CENTER, 550);
//        auto lbl = Label::createWithTTF(btnInfo, UserInfo::getInstance()->getFontPath(), 32);
        lbl->setColor(Color3B::RED);
        lbl->setAnchorPoint(Vec2(0.5, 1));
        lbl->setPosition(Vec2(s.width * 0.5, 0));
        menuItem->addChild(lbl, 1);
    }
    
    // reminder button
    {
        auto btnItem1 = MenuItemImage::create();
        btnItem1->setNormalImage(Sprite::createWithSpriteFrameName("button_middle_white.png"));
        btnItem1->setSelectedImage(Sprite::createWithSpriteFrameName("button_middle_white_click.png"));

        auto btnItem2 = MenuItemImage::create();
        btnItem2->setNormalImage(Sprite::createWithSpriteFrameName("button_middle_white.png"));
        btnItem2->setSelectedImage(Sprite::createWithSpriteFrameName("button_middle_white_click.png"));

        auto menuItem = MenuItemToggle::createWithCallback( CC_CALLBACK_1(OptionLayer::setToggleButton, this),
                       btnItem1, btnItem2, nullptr );
        //
        menuItem->setTag(1);

        int seledtedIdx = 1 - UserInfo::getInstance()->getNoti();
        menuItem->setSelectedIndex(seledtedIdx);
        Size s = menuItem->getContentSize();

        auto onIcon = Sprite::createWithSpriteFrameName("icon_reminder.png");
        onIcon->setPosition(Vec2(s) * 0.5);
        onIcon->setTag(0);
        menuItem->addChild(onIcon, 100);
        
        auto offIcon = Sprite::createWithSpriteFrameName("icon_reminder_off.png");
        offIcon->setPosition(Vec2(s) * 0.5);
        offIcon->setTag(1);
        menuItem->addChild(offIcon, 100);
        
        if (seledtedIdx == 0) {
            onIcon->setVisible(true);
            offIcon->setVisible(false);
        }
        else {
            onIcon->setVisible(false);
            offIcon->setVisible(true);
        }

        //
        menuItem->setPosition(Vec2(visibleSizeHalf.width, visibleSizeHalf.height + s.height) + origin);
        
        
        auto soundBtn = Menu::create(menuItem, NULL);
        soundBtn->setPosition(Vec2::ZERO);
        addChild(soundBtn, 1);
        
        //
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("OptionNoti");
        std::string btnInfo = stream.str();

        TTFConfig config(UserInfo::getInstance()->getFontPath(), 32);
        auto *lbl = Label::createWithTTF(config, btnInfo, TextHAlignment::CENTER, 300);
//        auto lbl = Label::createWithTTF(btnInfo, UserInfo::getInstance()->getFontPath(), 32);
        lbl->setColor(Color3B::RED);
        lbl->setAnchorPoint(Vec2(0.5, 1));
        lbl->setPosition(Vec2(s.width * 0.5, 0));
        menuItem->addChild(lbl, 1);
    }
    
    // restore button
    {
        auto btnItem = MenuItemImageButton::create();
        btnItem->setNormalImage(Sprite::createWithSpriteFrameName("button_middle_white.png"));
        btnItem->setSelectedImage(Sprite::createWithSpriteFrameName("button_middle_white_click.png"));
        btnItem->setCallback(CC_CALLBACK_1(OptionLayer::btnCallback, this));

        //
        btnItem->setTag(2);

        Size s = btnItem->getContentSize();
        btnItem->setPosition(Vec2(visibleSizeHalf.width + (s.width + 14), visibleSizeHalf.height + s.height) + origin);
        
        auto btn = Menu::create(btnItem, NULL);
        btn->setPosition(Vec2::ZERO);
        mainLayer->addChild(btn, 1);
        
        // add icon
        auto icon = Sprite::createWithSpriteFrameName("icon_restore.png");
        icon->setPosition(Vec2(s) * 0.5);
        btnItem->addChild(icon, 1);

        //
        btnItem->setLabelChild(icon);

        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("OptionRestore");
        std::string btnInfo = stream.str();
        

        TTFConfig config(UserInfo::getInstance()->getFontPath(), 32);
        auto *lbl = Label::createWithTTF(config, btnInfo, TextHAlignment::CENTER, 550);
//        auto lbl = Label::createWithTTF(btnInfo, UserInfo::getInstance()->getFontPath(), 32);
        lbl->setColor(Color3B::RED);
        lbl->setAnchorPoint(Vec2(0.5, 1));
        lbl->setPosition(Vec2(s.width * 0.5, 0));
        btnItem->addChild(lbl, 1);
    }

    //
    // bottom buttons
    //
    const char *labelStr[] = {
        "Tutorial",
        "Facebook",
        "Credit"
    };
    const char *fileNames[] = {
        "icon_tutorial.png",
        "icon_facebook.png",
        "icon_credit.png"
    };

    for (int i=0; i<3; i++) {
        auto btnItem = MenuItemImageButton::create();
        btnItem->setNormalImage(Sprite::createWithSpriteFrameName("button_middle_white.png"));
        btnItem->setSelectedImage(Sprite::createWithSpriteFrameName("button_middle_white_click.png"));
        btnItem->setCallback(CC_CALLBACK_1(OptionLayer::btnCallback, this));
        //
        btnItem->setTag(3 + i);
        
        Size s = btnItem->getContentSize();
        btnItem->setPosition(Vec2(visibleSizeHalf.width + mult[i] * (s.width + 14), visibleSizeHalf.height - s.height) + origin);

        auto btn = Menu::create(btnItem, NULL);
        btn->setPosition(Vec2::ZERO);
        mainLayer->addChild(btn, 1);

        // add icon
        auto icon = Sprite::createWithSpriteFrameName(fileNames[i]);
        icon->setPosition(Vec2(s) * 0.5);
        btnItem->addChild(icon, 1);
        
        //
        btnItem->setLabelChild(icon);

        //
        std::stringstream stream;
        stream << "Option" << labelStr[i];
        std::string keyCode = stream.str();

        //
        stream.str("");
        stream.clear();
        stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
        std::string btnInfo = stream.str();

        TTFConfig config(UserInfo::getInstance()->getFontPath(), 32);
        auto *lbl = Label::createWithTTF(config, btnInfo, TextHAlignment::CENTER, 550);
//        auto lbl = Label::createWithTTF(btnInfo, UserInfo::getInstance()->getFontPath(), 32);
        lbl->setColor(Color3B::RED);
        lbl->setAnchorPoint(Vec2(0.5, 1));
        lbl->setPosition(Vec2(s.width * 0.5, 0));
        btnItem->addChild(lbl, 1);
    }
    
    //
    // Close Button
    //
    {
        // close button
        auto closeBtnItem = MenuItemSameImage::create("TF_UI_x.png", CC_CALLBACK_0(OptionLayer::hideThis, this));
        Size closeBtnSize = closeBtnItem->getContentSize();
        closeBtnItem->setOpacity(150);
        
//        float topPosY = visibleSize.height - closeBtnSize.height;
        
        auto closeBtn = Menu::create(closeBtnItem, NULL);
//        closeBtn->setPosition(Vec2(closeBtnSize.width, topPosY) + origin);
        closeBtn->setPosition(Vec2(closeBtnSize.width * 0.5 + 20, visibleSize.height - closeBtnSize.height* 0.5 - 20) + origin);
        mainLayer->addChild(closeBtn, 10);
    }
}

void OptionLayer::btnCallback(Ref *pSender)
{
    MenuItemImage *btn = (MenuItemImage*)pSender;

    int idx = btn->getTag();

    if (btnCallbacks[idx]) {
        btnCallbacks[idx](this);
        
        //
        TapjoyX::getInstance()->logEventInUIFlowOption(idx);
    }
}

void OptionLayer::setToggleButton(Ref *pSender)
{
    MenuItemToggle *menuItem = (MenuItemToggle*)pSender;
    
    int seledtedIdx = menuItem->getSelectedIndex();

    auto onIcon = (Sprite*)menuItem->getChildByTag(0);
    auto offIcon = (Sprite*)menuItem->getChildByTag(1);
    if (seledtedIdx == 0) {
        onIcon->setVisible(true);
        offIcon->setVisible(false);
    }
    else if (seledtedIdx == 1) {
        onIcon->setVisible(false);
        offIcon->setVisible(true);
    }

    int idx = menuItem->getTag();
    if (idx == 0) {
        //
        // Sound Achievement
        //
        if (!UserInfo::getInstance()->getAchievementInfoWithType(AchievementType08)) {
            soundBtnCnt++;
            
            std::stringstream stream;
            stream << soundBtnCnt;
            std::string soundBtnCntInfo = stream.str();
            
            auto soundLabel = Label::createWithTTF(soundBtnCntInfo, UserInfo::getInstance()->getFontPath(), 45);
            Size s = soundLabel->getContentSize();
//            soundLabel->setString(soundBtnCntInfo);
//        soundLabel->setScale(0.5);
            soundLabel->setColor(Color3B(15, 105, 254));
            soundLabel->setPosition(menuItem->getPosition() + Vec2(0, s.height * 0.7));
            addChild(soundLabel, 1);
            
            float rx = rand_minus1_1() * 25;
            auto seq = Sequence::create(
                                        Spawn::create(
                                                      MoveBy::create(0.5, Vec2(rx, 150)),
                                                      FadeOut::create(0.55),
                                                      NULL),
                                        RemoveSelf::create(),
                                        NULL);
            soundLabel->runAction(seq);
            
            if (soundBtnCnt >= 100) {
                //
                // set achievement
                //
                if (UserInfo::getInstance()->setAchievementInfoWithType(AchievementType08)) {
//                achLayer->showNewMark(true);
//                achLayer->getPageLayer()->updatePages();
//                    achSelection->showNewMark(true);
//                    achSelection->getPageLayer()->updatePages();
                }
            }
        }
    }

    //
    // callback
    //
    btnCallbacks[idx](pSender);
}

void OptionLayer::setVisible(bool visible)
{
    ownsPriority = visible;

    Layer::setVisible(visible);
}

//
// Called from close button or backbutton(Android)
// btnCallbacks should not play button sound
//
void OptionLayer::hideThis()
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    if (btnCallbacks[6]) {
        btnCallbacks[6](this);
    }

    ownsPriority = false;

//    removeFromParent();
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void OptionLayer::onKeyReleased( EventKeyboard::KeyCode keycode, Event *event )
{
//    log("MainUIStatus: OptionLayer::onKeyReleased: %d", ownsPriority);
    if (ownsPriority && keycode == EventKeyboard::KeyCode::KEY_BACK) {
        event->stopPropagation();
        hideThis();
    }
}
#endif
