//
//  BonusTooltipLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 9. 11..
//
//

#include "BonusTooltipLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

#define SelectionTag 1000

bool BonusTooltipLayer::init()
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

void BonusTooltipLayer::initWithAmount(int bonus)
{
    //
    std::stringstream stream;
    stream << "+" << bonus;
    std::string bonusInfo = stream.str();
    
    auto bonusLabel = Label::createWithTTF(bonusInfo, EnglishFontPath, 35);
    Size s = bonusLabel->getContentSize();
    
    //
    float scale = 0.25;
    auto coinIcon = Sprite::createWithSpriteFrameName("coin_big.png");
    coinIcon->setScale(scale);
    Size coinSize = coinIcon->getContentSize() * scale;
    
    float length = s.width + coinSize.width+ 4;
    
    //
    Vec2 pos = initBackground(length);
    
    //
    bonusLabel->setAnchorPoint(Vec2(0, 0.5));
    bonusLabel->setPosition(Vec2(pos.x - length * 0.5, pos.y));
    addChild(bonusLabel, 1);
    
    //
    coinIcon->setAnchorPoint(Vec2(1, 0.5));
    coinIcon->setPosition(Vec2(pos.x + length * 0.5, pos.y));
    addChild(coinIcon, 1);
}

Vec2 BonusTooltipLayer::initBackground(float length)
{
    //
    // Main Background
    //
    auto tooltipBg = Sprite::create("speech_bubble_middle.png");
    Size s = tooltipBg->getContentSize();
    Vec2 tooltipPos = Vec2(0, -s.height * 0.5);
    tooltipBg->setPosition(Vec2::ZERO);
    tooltipBg->setScaleX(length);
    addChild(tooltipBg);
    
    //
    // Background side(left/right)
    //
    for (int i=0; i<2; i++) {
        Vec2 deltaPos = Vec2(-length * 0.5, 0);
        //
        auto tooltipSideBg = Sprite::create("speech_bubble_round.png");
        tooltipSideBg->setAnchorPoint(Vec2(1, 0.5));
        if (i == 1) {
            deltaPos = -deltaPos;
            tooltipSideBg->setRotation(180);
        }
        tooltipSideBg->setPosition(Vec2::ZERO + deltaPos);
        addChild(tooltipSideBg);
    }
    
    //
    // Tooltip Arrow
    //
    auto tooltipArrow = Sprite::create("speech_bubble_tail.png");
    tooltipArrow->setAnchorPoint(Vec2(0.5, 1.0));
    tooltipArrow->setPosition(tooltipPos);
    addChild(tooltipArrow);
    
    return tooltipBg->getPosition();
}


//void BonusTooltipLayer::initWithAmount(Vec2 pos, int bonus)
//{
//    //
//    std::stringstream stream;
//    stream << "+" << bonus;
//    std::string bonusInfo = stream.str();
//
//    auto bonusLabel = Label::createWithTTF(bonusInfo, EnglishFontPath, 35);
//    Size s = bonusLabel->getContentSize();
//    
//    //
//    float scale = 0.25;
//    auto coinIcon = Sprite::createWithSpriteFrameName("coin_big.png");
//    coinIcon->setScale(scale);
//    Size coinSize = coinIcon->getContentSize() * scale;
//
//    float length = s.width + coinSize.width+ 4;
//
//    //
//    pos = initBackground(length, pos);
//
//    //
//    bonusLabel->setAnchorPoint(Vec2(0, 0.5));
//    bonusLabel->setPosition(Vec2(pos.x - length * 0.5, pos.y) + origin);
//    addChild(bonusLabel, 1);
//
//    //
//    coinIcon->setAnchorPoint(Vec2(1, 0.5));
//    coinIcon->setPosition(Vec2(pos.x + length * 0.5, pos.y) + origin);
//    addChild(coinIcon, 1);
//}
//
//Vec2 BonusTooltipLayer::initBackground(float length, Vec2 pos)
//{
//    //
//    // Main Background
//    //
//    auto tooltipBg = Sprite::create("speech_bubble_middle.png");
//    Size s = tooltipBg->getContentSize();
//    Vec2 tooltipPos = pos + Vec2(0, -s.height * 0.5);
//    tooltipBg->setPosition(pos + origin);
//    tooltipBg->setScaleX(length);
//    addChild(tooltipBg);
//    
//    //
//    // Background side(left/right)
//    //
//    for (int i=0; i<2; i++) {
//        Vec2 deltaPos = Vec2(-length * 0.5, 0);
//        //
//        auto tooltipSideBg = Sprite::create("speech_bubble_round.png");
//        tooltipSideBg->setAnchorPoint(Vec2(1, 0.5));
//        if (i == 1) {
//            deltaPos = -deltaPos;
//            tooltipSideBg->setRotation(180);
//        }
//        tooltipSideBg->setPosition(pos + deltaPos + origin);
//        addChild(tooltipSideBg);
//    }
//    
//    //
//    // Tooltip Arrow
//    //
//    auto tooltipArrow = Sprite::create("speech_bubble_tail.png");
//    tooltipArrow->setAnchorPoint(Vec2(0.5, 1.0));
//    tooltipArrow->setPosition(tooltipPos + origin);
//    addChild(tooltipArrow);
//    
//    return tooltipBg->getPosition();
//}
