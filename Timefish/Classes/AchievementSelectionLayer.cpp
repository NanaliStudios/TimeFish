//
//  AchievementSelectionLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 7. 17..
//
//

#include "AchievementSelectionLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

bool AchievementSelectionLayer::init()
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

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // set keyInput enabled
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(AchievementSelectionLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif

    return true;
}

void AchievementSelectionLayer::initMainLayer()
{
    // inits...
    layerHeight = visibleSize.height * 0.75 + origin.y;
    
    ownsPriority = false;
    
    //
    // Main Layer
    //
//    mainLayer = LayerColor::create(Color4B(239,165, 40,255), visibleSize.width, layerHeight);
    mainLayer = Layer::create();
    mainLayer->setPosition(Vec2::ZERO);
    addChild(mainLayer, 0);

    // Close Button
    {
        // close button
        auto closeBtnItem = MenuItemSameImage::create("TF_UI_x.png", CC_CALLBACK_0(AchievementSelectionLayer::hideThis, this));
        Size closeBtnSize = closeBtnItem->getContentSize();
        closeBtnItem->setOpacity(150);
        
//        float topPosY = visibleSize.height - closeBtnSize.height;
        
        auto closeBtn = Menu::create(closeBtnItem, NULL);
//        closeBtn->setPosition(Vec2(closeBtnSize.width, topPosY) + origin);
        closeBtn->setPosition(Vec2(closeBtnSize.width * 0.5 + 20, visibleSize.height - closeBtnSize.height* 0.5 - 20) + origin);
        mainLayer->addChild(closeBtn, 10);
    }

    // NEW ACHIEVEMENT MARK
//    newMark = Sprite::createWithSpriteFrameName("TF_timeposter_new.png");
//    Size markSize = newMark->getContentSize();
//    newMark->setPosition(Vec2(s.width - markSize.width*0.5, s.height*0.8));
//    mainBtnItem->addChild(newMark);
//    auto seq = Sequence::create(
//                                ScaleTo::create(0.2, 1.2),
//                                ScaleTo::create(0.2, 1),
//                                NULL);
//    newMark->runAction(RepeatForever::create(seq));
//    newMark->setVisible(UserInfo::getInstance()->haveNewAchievement());
//    
    
    //
    // Page Layer
    //
    pageLayer = PageViewlayer::create();
    pageLayer->initLayersWithPosY(visibleSize.height + 300);
    pageLayer->setUpdateCallback(CC_CALLBACK_1(AchievementSelectionLayer::updateLayer, this));
    mainLayer->addChild(pageLayer, 100);
}

void AchievementSelectionLayer::updateLayer(Ref *pSender)
{
    //
    // User checked new mark!
    //
//    showNewMark(UserInfo::getInstance()->haveNewAchievement());
}

void AchievementSelectionLayer::btnCallback(Ref *pSender)
{
    
}

void AchievementSelectionLayer::setVisible(bool visible)
{
//    log("MainUIStatus: AchievementSelectionLayer::showThis, %d", visible);
    ownsPriority = visible;

    Layer::setVisible(visible);
}

void AchievementSelectionLayer::showThis()
{
    setVisible(true);

    pageLayer->enableLayerTouch();
}

//
// Called from close button or backbutton(Android)
// closeCallback should not play button sound
//
void AchievementSelectionLayer::hideThis()
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);
    
    pageLayer->disableLayerTouch();

    if (closeCallback) {
        closeCallback(this);
    }

    ownsPriority = false;
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void AchievementSelectionLayer::onKeyReleased( EventKeyboard::KeyCode keycode, Event *event )
{
//    log("MainUIStatus: AchievementSelectionLayer::onKeyReleased, %d", ownsTouchPriority());
    if (ownsTouchPriority() && keycode == EventKeyboard::KeyCode::KEY_BACK) {
        event->stopPropagation();
        hideThis();
    }
}
#endif
