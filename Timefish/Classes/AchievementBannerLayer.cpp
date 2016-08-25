//
//  AchievementBannerLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 3. 25..
//
//

#include "AchievementBannerLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

bool AchievementBannerIcon::init()
{
    if ( !Layer::init() )
    {
        preloaded = false;

        return false;
    }

    return true;
}

void AchievementBannerIcon::initIcons(int achievemenetIdx, Size inBoxSize, float _scale, bool loadNow)
{
    if (preloaded) {
        return;
    }

    //
    auto inBox = Sprite::createWithSpriteFrameName("TF_timeposter_box.png");
    
    //
    int posterType = achievemenetIdx + 1;
    bool needABackground = false;
    
    std::stringstream stream;
    stream << "TF_gameposter_" << std::setfill('0') << std::setw(2);
    Sprite *posterIcon = NULL;
    // file name variation
    if (posterType == 11 || posterType == 12) {
        stream << posterType <<".jpg";
        std::string fileName = stream.str();
        posterIcon = Sprite::create(fileName.c_str());
        
        preloaded = true;
    }
    else if (posterType == 10 || posterType == 29 || posterType == 30 || posterType == 31) {
        stream << posterType <<".png";
        std::string fileName = stream.str();
        posterIcon = Sprite::create(fileName.c_str());
        
        preloaded = true;
    }
    else {
        needABackground = true;

        stream << posterType <<".png";
        std::string fileName = stream.str();
        posterIcon = Sprite::createWithSpriteFrameName(fileName.c_str());
        
        preloaded = false;
    }

    //
    if (loadNow && preloaded) {
        return;
    }
    else if ((!preloaded && !loadNow)) {
        return;
    }

    // use inbox as a background, if needed.
    if (needABackground) {
        inBox->setColor(posterBgColor[posterType - 1]);
        inBox->setScale(_scale);
        addChild(inBox, 2);
    }
    
    // scale variation
    if (posterType == 16 || posterType == 17 || posterType == 18) {
        _scale *= 0.7;
    }
    else if (posterType == 21) {
        _scale *= 0.5;
    }
    else {
        _scale *= 0.53;
    }
    
    // set scale
    posterIcon->setScale(_scale);

    //
    auto clipper = ClippingRectangleNode::create();
    clipper->setClippingRegion(Rect(-inBoxSize.width*0.5, -inBoxSize.height*0.5, inBoxSize.width, inBoxSize.height));
    addChild(clipper, 3);

    Vec2 deltaPos = posterIconPosDelta[posterType -1];
    if (posterType == 10) {
        deltaPos.x -= 15;
        deltaPos.y -= 32.5;
    }
    else if (posterType == 11) {
        deltaPos.y -= 20;
    }
    else if (posterType == 13 || posterType == 14 || posterType == 15) {
        deltaPos.x -= 10;
    }
    posterIcon->setPosition(Vec2(clipper->getContentSize()) * 0.5 + deltaPos);
    clipper->addChild(posterIcon);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

bool AchievementBannerLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;

    //
    closeCallback = NULL;

//    languageIndexDelta = 0;
//    if (UserInfo::getInstance()->currLangType == LanguageType::KOREAN) {
//        languageIndexDelta = NumOfAchievement;
//    }
    
    //
    initMainLayer();

    return true;
}

#define DefaultScale 0.7
void AchievementBannerLayer::initMainLayer()
{
    _scale = DefaultScale * (visibleSize.height / BaseScreenHeight);
    auto inBox = Sprite::createWithSpriteFrameName("TF_timeposter_box.png");
    inBoxSize = inBox->getContentSize() * _scale;
    inBox->setScale(_scale);
    
    layerSize = inBoxSize.height + 30;
    top = LayerColor::create(Color4B(255, 255, 255, 220), visibleSize.width, layerSize);
    top->setPosition(Vec2(0, visibleSize.height));
    addChild(top);

    //
    // Preloading!!!
    //
    for (int i=0; i<NumOfAchievement; i++) {
        if (!UserInfo::getInstance()->getAchievementInfoWithType((AchievementType)(1<<i))) {
            icons[i] = AchievementBannerIcon::create();
            Vec2 pos = Vec2(inBoxSize.width * 0.6, layerSize*0.5);
            icons[i]->setPosition(pos);
            icons[i]->initIcons(i, inBoxSize, _scale);
            icons[i]->setVisible(false);
            top->addChild(icons[i]);
        }
        else {
            icons[i] = NULL;
        }
    }
    
    //
    // Achievement Info
    //
    achievementName = Label::createWithTTF("AchievementName", UserInfo::getInstance()->getFontPath(), 30);
//    achievementName = Label::createWithSystemFont("AchievementName", UserInfo::getInstance()->getFontPath(), 30);
    float h = achievementName->getContentSize().height * 0.5;
    achievementName->setPosition(Vec2(inBoxSize.width * 1.3, layerSize*0.5 + h));
    achievementName->setAnchorPoint(Vec2(0, 0.5));
    achievementName->setColor(Color3B::BLACK);
    top->addChild(achievementName, 1);
    
    //
    achievementInfo = Label::createWithTTF("AchievementInfo", UserInfo::getInstance()->getFontPath(), 25);
//    achievementInfo = Label::createWithSystemFont("AchievementInfo", UserInfo::getInstance()->getFontPath(), 25);
    achievementInfo->setPosition(Vec2(inBoxSize.width * 1.3, layerSize*0.5 - h));
    achievementInfo->setAnchorPoint(Vec2(0, 0.5));
    achievementInfo->setColor(Color3B::BLACK);
    top->addChild(achievementInfo, 1);
    
    //
    // Here, pre-setString for the optimization purpose.
    //
    // TODO: Find why!!!
    // A clue found so far:
    // if languageIndexDelta is different between pre-setString and setString,
    // this does not help the optimizations!
    //
//    int arryIdx = languageIndexDelta;

//    //
//    {
//        std::stringstream stream;
//        stream << "AchievementName";
//        stream << std::setfill('0') << std::setw(2) << selectedIdx+1;
//        std::string keyCode = stream.str();
//        
//        //
//        stream.str("");
//        stream.clear();
//        stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
//        std::string nameInfo = stream.str();
//        
//        achievementName->setString(achName);
//    }
//    std::stringstream stream;
//    stream << LocalizationManager::getInstance()->getLocalizationString("AchievementName01");
//    std::string achName = stream.str();
//
//    //
//    achievementInfo->setString(getAchievementInfo(arryIdx));
}

void AchievementBannerLayer::setBannerWith(int achievemenetIdx)
{
    //
    currIdx = achievemenetIdx;
    
    //
    if (icons[achievemenetIdx]) {
        icons[achievemenetIdx]->initIcons(achievemenetIdx, inBoxSize, _scale, true);
        icons[achievemenetIdx]->setVisible(true);
    }
    else {
        //
        // NOTE: should not reach here! Add this lines, just in case.
        //
        icons[achievemenetIdx] = AchievementBannerIcon::create();
        Vec2 pos = Vec2(inBoxSize.width * 0.6, layerSize*0.5);
        icons[achievemenetIdx]->setPosition(pos);
        icons[achievemenetIdx]->initIcons(achievemenetIdx, inBoxSize, _scale, true);
        icons[achievemenetIdx]->setVisible(true);
        icons[achievemenetIdx]->setVisible(false);
        top->addChild(icons[achievemenetIdx]);
    }

//    int arryIdx = achievemenetIdx + languageIndexDelta;

    //
    // Achievement Info
    //
    {
        std::stringstream stream;
        stream << "AchievementName";
        stream << std::setfill('0') << std::setw(2) << achievemenetIdx+1;
        std::string keyCode = stream.str();
        
        //
        stream.str("");
        stream.clear();
        stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
        std::string achName = stream.str();
        
        achievementName->setString(achName);//getAchievementName(arryIdx));
    }

    //
    {
        std::stringstream stream;
        stream << "AchievementDesc";
        stream << std::setfill('0') << std::setw(2) << achievemenetIdx+1;
        std::string keyCode = stream.str();
        
        //
        stream.str("");
        stream.clear();
        stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
        std::string achInfo = stream.str();
        
        achievementInfo->setString(achInfo);
    }

    //
    // Start to Hide
    //
    top->runAction(Sequence::create(
                                    Show::create(),
                                    EaseSineOut::create(MoveBy::create(0.2, Vec2(0, -layerSize))),
                                    DelayTime::create(1.5),
                                    CallFuncN::create(CC_CALLBACK_0(AchievementBannerLayer::hideBanner, this)),
                                    NULL));
}

void AchievementBannerLayer::hideBanner()
{
    top->runAction(Sequence::create(
                                    EaseSineOut::create(MoveBy::create(0.2, Vec2(0, layerSize))),
                                    CallFuncN::create(CC_CALLBACK_0(AchievementBannerLayer::closeBanner, this)),
                                    NULL));
}

void AchievementBannerLayer::closeBanner()
{
    //
//    icons[currIdx]->setVisible(false);
    icons[currIdx]->removeFromParentAndCleanup(true);

    top->setVisible(false);

    if (closeCallback) {
        closeCallback(this); // to wake up next banner
    }
    
//    removeFromParent();
}
