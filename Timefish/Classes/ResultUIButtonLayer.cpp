//
//  ResultUIButtonLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 7. 19..
//
//

#include "ResultUIButtonLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

bool ResultUIButtonLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

void ResultUIButtonLayer::initMainLayerWithCallbacks(const ccMenuCallback& callback1,
                                const ccMenuCallback& callback2,
                                const ccMenuCallback& callback3,
                                const std::function<void()>& _finishCallback)
{
    MenuItemImageButton *menuItem[2];
    menuItem[0] = MenuItemImageButton::create();
    menuItem[0]->setNormalImage(Sprite::createWithSpriteFrameName("button_middle_white.png"));
    menuItem[0]->setSelectedImage(Sprite::createWithSpriteFrameName("button_middle_white_click.png"));
    menuItem[0]->setCallback(callback1);
    
    menuItem[1] = MenuItemImageButton::create();
    menuItem[1]->setNormalImage(Sprite::create("button_verylong_red.png"));
    menuItem[1]->setSelectedImage(Sprite::create("button_verylong_red_click.png"));
    menuItem[1]->setCallback(callback3);
    
    Size s1 = menuItem[0]->getContentSize();
    Size s2 = menuItem[1]->getContentSize();
    float spaceBetween = 9;
    float spaceSide = (visibleSize.width - (s1.width + s2.width + spaceBetween)) * 0.5;
    
    
    const char* fileNames[] = {
        "button_icon_option.png",
        "button_icon_retry.png"
    };
    float posX = spaceSide;
    posY = spaceSide + s1.height * 0.5;
    for (int i=0; i<2; i++) {
        Size s = menuItem[i]->getContentSize();
        menuItem[i]->setPosition(Vec2::ZERO);
        menuItem[i]->setTag(ResultBtnTag);
        
        // button
        mainBtn[i] = Menu::create(menuItem[i], NULL);
        mainBtn[i]->setPosition(Vec2( posX + s.width * 0.5, -posY) + origin);
        addChild(mainBtn[i], 1);
        
        // add icon
        auto icon = Sprite::createWithSpriteFrameName(fileNames[i]);
        icon->setPosition(Vec2(s) * 0.5);
        menuItem[i]->addChild(icon, 1);
        
        //
        menuItem[i]->setLabelChild(icon);
        
        // set next positon
        posX += (s.width + spaceBetween);
    }

    finishCallback = _finishCallback;
}

void ResultUIButtonLayer::showButtons()
{
    btnCnt = 0;

    for (int i=0; i<2; i++) {
        // run action
        float delayDuration = CCRANDOM_0_1() * 0.4f;
        float moveDuration = 0.4f + CCRANDOM_0_1() * 0.25f;

        float posX = mainBtn[i]->getPositionX();
        Vec2 targetPos = Vec2(posX, posY) + origin;
        
        auto callback = CallFunc::create( [this]() {
            //
            btnCnt++;
            if (btnCnt >= 2 && finishCallback) {
                finishCallback();
            }
        });
        auto seq = Sequence::create(DelayTime::create(delayDuration),
                                    EaseBounceOut::create(MoveTo::create(moveDuration, Vec2(posX, posY) + origin)), callback,
                                    NULL);
        mainBtn[i]->runAction(seq);
    }
}
