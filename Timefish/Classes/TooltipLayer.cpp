//
//  TooltipLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 3. 24..
//
//

#include "TooltipLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

#define SelectionTag 1000

bool TooltipLayer::init()
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
    
    return true;
}

//
// Duff's device:
// http://kldp.org/node/38269
//
void TooltipLayer::util_add_comma_to_num2(const char *str, char *buf, int buflen)
{
    size_t len = strlen(str);
    
    assert(len > 0);
    assert(buflen >= len + ((len - 1) / 3) + 1);
    
    switch ((len - 1) % 3) {
        case 3: /* fake label to make gcc happy */
            while (*str) {
                *buf++ = ',';
            case 2: *buf++ = *str++;
            case 1: *buf++ = *str++;
            case 0: *buf++ = *str++;
            }
    }
    *buf = '\0';
}

void TooltipLayer::initWithAchievementType(Vec2 pos, float iconWidth, int posterIdx)
{
    int col = posterIdx%3;
    
    //
    std::stringstream stream;
    stream << "AchievementDesc";
    stream << std::setfill('0') << std::setw(2) << posterIdx+1;
    std::string keyCode = stream.str();
    
    //
    stream.str("");
    stream.clear();
    stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
    std::string achInfo = stream.str();

//    auto achievementInfo = Label::createWithSystemFont(achInfo, "Arial-BoldMT", 23);
//    Size achievementInfoSize = achievementInfo->getContentSize();
//    float length = achievementInfoSize.width + 20;
    auto achievementInfo = Label::createWithTTF(achInfo, UserInfo::getInstance()->getFontPath(), 23);
    Size achievementInfoSize = achievementInfo->getContentSize();
    float length = achievementInfoSize.width + 20;

    //
    pos = initBackground(length, iconWidth, pos, col);

    //
    // TODO: 12 is hard-coded!!!
    // which means....accumulative achievements starting index
    //
    if (posterIdx >= 12 && posterIdx != 30) {
        pos = pos + Vec2(0, achievementInfoSize.height * 0.5);

        //
        // add current state
        //
        int curr = UserInfo::getInstance()->getAchievementCurr((AchievementType)(1<<posterIdx));
        int base = UserInfo::getInstance()->getAchievementPassBase((AchievementType)(1<<posterIdx));
        if (posterIdx < 15) {
            base *= 1000000;
        }
        
        if (curr > base) {
            curr = base;
        }

        // curr number
        std::stringstream stream1;
        stream1 << curr;
        std::string currNumber = stream1.str();
        char toStr[20] = {}; // NOTE: static size!!!
        util_add_comma_to_num2(currNumber.c_str(), toStr, 20);

        std::stringstream stream3;
        stream3 << toStr;

        // base number
        std::stringstream stream2;
        stream2 << base;
        std::string baseNumber = stream2.str();
        char toStr2[20] = {}; // NOTE: static size!!!
        util_add_comma_to_num2(baseNumber.c_str(), toStr2, 20);

        // final string
        stream3 << " / " << toStr2;
        std::string statusInfo = stream3.str();

//        auto achievementStausInfo = Label::createWithSystemFont(statusInfo, "Arial-BoldMT", 22);
        auto achievementStausInfo = Label::createWithTTF(statusInfo, UserInfo::getInstance()->getFontPath(), 22);
        Vec2 pos2 = pos - Vec2(0, achievementInfoSize.height);
        achievementStausInfo->setPosition(pos2);
        achievementStausInfo->setColor(Color3B::BLACK);
        addChild(achievementStausInfo, 1);
    }

    //
    achievementInfo->setPosition(pos);
    achievementInfo->setColor(Color3B(7, 130, 255));
    addChild(achievementInfo, 1);
    
    //
    scheduleOnce(CC_SCHEDULE_SELECTOR(TooltipLayer::removeThis), 2);
}

void TooltipLayer::initWithStringKey(Vec2 pos, float iconWidth, int stringKey)
{
    //
    std::stringstream stream;
    stream << LocalizationManager::getInstance()->getLocalizationString("TitleMessage");
    std::string msg = stream.str();

    //
//    auto message = Label::createWithSystemFont(msg, "Arial-BoldMT", 23);
    auto message = Label::createWithTTF(msg, UserInfo::getInstance()->getFontPath(), 23);
    Size messageSize = message->getContentSize();
    float length = messageSize.width + 20;

    //
    initBackground(length, iconWidth, pos, 1);
    
    //
    message->setPosition(pos);
    message->setColor(Color3B(7, 130, 255));
    addChild(message, 1);

    //
    scheduleOnce(CC_SCHEDULE_SELECTOR(TooltipLayer::removeThis), 2);
}

Vec2 TooltipLayer::initBackground(float length, float iconWidth, Vec2 pos, int col)
{
    //
    // Main Background
    //
    auto tooltipBg = Sprite::create("TF_timeposter_speechbubble00.png");
    Size s = tooltipBg->getContentSize();
    Vec2 tooltipPos = pos + Vec2(0, -s.height * 0.5);
    if ( col == 0 ) {
        pos += Vec2((length - iconWidth*1.2) * 0.5 ,0);
    }
    else if ( col == 2 ) {
        pos += Vec2((iconWidth*1.2 - length) * 0.5 ,0);
    }
    tooltipBg->setPosition(pos);
    tooltipBg->setScaleX(length);
    addChild(tooltipBg);

    //
    // Background side(left/right)
    //
    for (int i=0; i<2; i++) {
        Vec2 deltaPos = Vec2(-length * 0.5, 0);
        //
        auto tooltipSideBg = Sprite::create("TF_timeposter_speechbubble01.png");
        tooltipSideBg->setAnchorPoint(Vec2(1, 0.5));
        if (i == 1) {
            deltaPos = -deltaPos;
            tooltipSideBg->setRotation(180);
        }
        tooltipSideBg->setPosition(pos + deltaPos);
        addChild(tooltipSideBg);
    }

    //
    // Tooltip Arrow
    //
    auto tooltipArrow = Sprite::create("TF_timeposter_speechbubble02.png");
    if ( col == 0 ) {
        tooltipPos += Vec2(-iconWidth * 0.3 ,0);
        tooltipArrow->setFlippedX(true);
    }
    else if ( col == 1 ) {
        tooltipArrow->setRotation(-5);
    }
    else if ( col == 2 ) {
        tooltipPos += Vec2(iconWidth * 0.3 ,0);
    }
    tooltipArrow->setPosition(tooltipPos);
    addChild(tooltipArrow);

    return tooltipBg->getPosition();
}

void TooltipLayer::removeThis(float dt)
{
    if (closeCallback) {
        closeCallback(this);
    }
    removeFromParent();
}

void TooltipLayer::enableLayerTouch()
{
    // setTouchEnabled
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(TooltipLayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(TooltipLayer::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(TooltipLayer::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(TooltipLayer::onTouchCancelled, this);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void TooltipLayer::disableLayerTouch()
{
    // setTouchDisabled
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->pauseEventListenersForTarget(this);
}

bool TooltipLayer::onTouchBegan(Touch *touch, Event *event)
{
    return true;
}

void TooltipLayer::onTouchMoved(Touch *touch, Event *event)
{
}

void TooltipLayer::onTouchEnded(Touch *touch, Event *event)
{
    disableLayerTouch();
    removeThis(0);
}

void TooltipLayer::onTouchCancelled(Touch *touch, Event *event)
{
}
