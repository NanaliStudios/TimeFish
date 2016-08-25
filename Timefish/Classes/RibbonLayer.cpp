//
//  RibbonLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 9. 10..
//
//

#include "RibbonLayer.h"

#include "ScoreLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

bool RibbonLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();
    
    status = 0;
    
    ownsPriority = true;

    //
    // setTouchEnabled
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(RibbonLayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(RibbonLayer::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(RibbonLayer::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(RibbonLayer::onTouchCancelled, this);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    //
    initMainLayer();
    initArrow();

    return true;
}

void RibbonLayer::initMainLayer()
{
    ribbonLayer = Layer::create();
    addChild(ribbonLayer);

    //
    auto ribbonBody = Sprite::create("tag_middle.png");
    ribbonBody->setScaleY(115);
    ribbonBody->setAnchorPoint(Vec2(0.5, 0));
    ribbonBody->setPosition(Vec2::ZERO);
    ribbonLayer->addChild(ribbonBody);

    //
    ribbonHead = Sprite::create("tag_front.png");
    ribbonHead->setAnchorPoint(Vec2(0.5, 1));
    ribbonHead->setPosition(Vec2::ZERO);
    ribbonLayer->addChild(ribbonHead);
    
    //
    {
        float scale = 62.0/102.0;
        auto btnItem = MenuItemImageButton::create();
        btnItem->setNormalImage(Sprite::createWithSpriteFrameName("button_short_white.png"));
        btnItem->setSelectedImage(Sprite::createWithSpriteFrameName("button_short_white_click.png"));
        btnItem->setCallback(CC_CALLBACK_0(RibbonLayer::showEveryplay, this));
        btnItem->setScale(scale);

        Size s = btnItem->getContentSize();
        btnItem->setPosition(Vec2(0, 65));
        
        auto btn = Menu::create(btnItem, NULL);
        btn->setPosition(Vec2::ZERO);
        ribbonLayer->addChild(btn, 1);
        
        // add icon
        auto icon = Sprite::create("button_everyplay.png");
        icon->setPosition(Vec2(s) * 0.5);
        icon->setScale(1/scale);
        btnItem->addChild(icon, 1);
        
        //
        btnItem->setLabelChild(icon);
    }

    //
    runRibbonAction();
}

void RibbonLayer::runRibbonAction()
{
    //
    auto seq = Sequence::create(MoveBy::create(0.5, Vec2(0, -14)),
                                MoveBy::create(0.5, Vec2(0, 14)),
                                NULL);
    ribbonLayer->runAction(RepeatForever::create(seq));
}

void RibbonLayer::initArrow()
{
    arrow = Sprite::create("tag_arrow.png");
    arrow->setPosition(Vec2(0, -75));
    addChild(arrow);
    auto seq = Sequence::create(MoveBy::create(0.3, Vec2(0, -10)),
                                MoveBy::create(0.3, Vec2(0, 10)),
                                NULL);
    auto rep = Repeat::create(seq, 3);
    auto arrowAction = Sequence::create(FadeIn::create(0),
                                        rep,
                                        FadeOut::create(0.2),
                                        DelayTime::create(1.5),
                                        NULL);
    arrow->runAction(RepeatForever::create(arrowAction));
}

void RibbonLayer::showEveryplay()
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);
    VideoShareX::getInstance()->showEveryplay();
}

void RibbonLayer::openRibbon()
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);
    status = 1;
    ribbonLayer->stopAllActions();
    auto callback = CallFunc::create([this](){
        status = 3;
    });
    ribbonLayer->runAction(Sequence::create(MoveTo::create(0.2, Vec2(0, -83)),
                                     callback,
                                     NULL));
    arrow->setVisible(false);
}

void RibbonLayer::closeRibbon()
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);
    status = 2;
    ribbonLayer->stopAllActions();
    auto callback = CallFunc::create([this](){
        status = 0;
        arrow->setVisible(true);
        runRibbonAction();
    });
    ribbonLayer->runAction(Sequence::create(MoveTo::create(0.2, Vec2(0, 0)),
                                     callback,
                                     NULL));
}

bool RibbonLayer::onTouchBegan(Touch *touch, Event *event)
{
    if (!ownsPriority || !isVisible()) {
        return false;
    }

    Vec2 touchPos = touch->getLocation();
    
    Mat4 worldToNodeTM = ribbonLayer->getWorldToNodeTransform();
    Vec3 localPos = Vec3(touchPos.x, touchPos.y, 0);
    worldToNodeTM.transformPoint(&localPos);
    touchPos.x = localPos.x;
    touchPos.y = localPos.y;

    Rect bbox = ribbonHead->getBoundingBox();
    bbox.origin = bbox.origin + Vec2(-30, -30);
    bbox.size = bbox.size + Size(40,80);
    if (bbox.containsPoint(touchPos)) {
        if (status == 0) {
            openRibbon();
        }
        else if (status == 3) {
            closeRibbon();
        }

        return true;
    }

    return false;
}

void RibbonLayer::onTouchMoved(Touch *touch, Event *event)
{
}

void RibbonLayer::onTouchEnded(Touch *touch, Event *event)
{
}

void RibbonLayer::onTouchCancelled(Touch *touch, Event *event)
{
}

