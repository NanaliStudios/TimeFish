//
//  ResultMainLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 7. 19..
//
//

#include "ResultMainLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

#define CC_SAFE_REMOVE(p)          if(p) { (p)->removeFromParent();\
(p) = NULL;\
}

bool ResultMainLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();
    
    //
    hasShownVideo = false;

    //
    uiInitializer[0] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeGift1, this));
    uiInitializer[1] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeGiftZPlay, this));
    uiInitializer[2] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeDraw1, this));
    uiInitializer[3] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeDrawZPlay, this));

    return true;
}

void ResultMainLayer::enableTouch(bool flag)
{
    if (flag) {
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true);
        touchListener->onTouchBegan = CC_CALLBACK_2(ResultMainLayer::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(ResultMainLayer::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(ResultMainLayer::onTouchEnded, this);
        touchListener->onTouchCancelled = CC_CALLBACK_2(ResultMainLayer::onTouchCancelled, this);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    }
    else {
        // setTouchDisabled
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->pauseEventListenersForTarget(this);
    }
}

void ResultMainLayer::initMainLayer(float _posY)
{
    resultButtonPosY = _posY;
    btmButtonPosY = _posY + 153; // bottom posY of left/right icon

    scaleFactor = UserInfo::getInstance()->screenScale;
    bottomIconScale = 0.5 * scaleFactor;
    bottomIconSelelectScale = 0.6 * scaleFactor;

    hasShownVideo = false;

    centerLayer = Layer::create();
    centerLayer->setScale(scaleFactor);
    addChild(centerLayer);

    int idx = (int)resultType;

    uiInitializer[idx](true);
}

void ResultMainLayer::setMainResultType()
{
    int firstTimeResultTypeIdx = UserInfo::getInstance()->getFirstTimeResultTypeIndex();
    if (firstTimeResultTypeIdx >= FirstTimeResultTypeIndexMax) {
        //
        // set resultType for the experienced User
        //
        setMainResultTypeForExpUser();
    }
    else {
        //
        // set resultType for the first-time User
        //
        setMainResultTypeForFirstTimeUser(firstTimeResultTypeIdx);
    }
#pragma mark -
#pragma mark ResultTypeTest
//    resultType = ResultTypeBuy1;
    
#ifdef ProductPurchaseCapture
    int _packageIdxSelected = UserInfo::getInstance()->getRandomPackageIndex();
    if (_packageIdxSelected > -1) {
        resultType = ResultTypeBuyPackage;
    }
    else {
        resultType = ResultTypeBuy1;
    }
#endif

    //
    UserInfo::getInstance()->setPrevResultType(resultType);

    //
    //
    //
    TapjoyX::getInstance()->logEventInUIFlowResult("GameResult", resultType);

    //
    // Decide Result UI type: one-button or three-button
    //
    if (resultType == ResultTypeGiftZPlay ||
        resultType == ResultTypeDrawZPlay) {
        resultOneOrThree = 1;
    }
    else {
        resultOneOrThree = 0;
    }
    
    //
    //
    //
    UserInfo::getInstance()->setHaveSeenVideo(false);
}

void ResultMainLayer::setMainResultTypeForFirstTimeUser(int idx)
{
    resultType = ResultTypeGift1;
    UserInfo::getInstance()->increaseFirstTimeResultTypeIndex();
}

void ResultMainLayer::setMainResultTypeForExpUser()
{
    bool isGiftAvailable = ableToGetGift();
    if (isGiftAvailable) {
        resultType = ResultTypeGiftZPlay;
    }
    else {
        resultType = ResultTypeDrawZPlay;
    }
}

void ResultMainLayer::btnCallback(Ref* pSender)
{
    //
    // get Callback index by ResultType
    //
    int callbackIdx =(resultType == ResultTypeGift1 || resultType == ResultTypeGiftZPlay)? 0:
                    (resultType == ResultTypeDraw1 || resultType == ResultTypeDrawZPlay)? 1: 2;

    //
    selectedButtonIndex = 0;

    //
    // Callback
    //
    if (buttonCallbacks[callbackIdx]) {
        //
        TapjoyX::getInstance()->logEventInResultFlowResultButton(callbackIdx, skinIdxSelected);

        buttonCallbacks[callbackIdx]();
    }
}

void ResultMainLayer::selectedLeft()
{
    int btnTag = leftIcon->getTag();
    selectedButtonIndex = 1;

    if (btnTag == UIStatusDraw) {
        buttonCallbacks[1]();
    }
    else if (btnTag == UIStatusGift) {
        buttonCallbacks[0]();
    }
}

void ResultMainLayer::selectedRight()
{
    int btnTag = rightIcon->getTag();
    selectedButtonIndex = 2;
    
    if (btnTag == UIStatusGift) {
        buttonCallbacks[0]();
    }
    else if (btnTag == UIStatusDraw) {
        buttonCallbacks[1]();
    }
    else if (btnTag == UIStatusShare) {
        buttonCallbacks[2]();
    }
}

#pragma mark -
#pragma mark Updaters
void ResultMainLayer::updateMainLayer(bool withoutGivingCoins)
{
    if (resultType == ResultTypeGift1) {
        int totalCount = UserInfo::getInstance()->getTotalPlayCount();
        if (totalCount == 1) {
            //
            // FOR THE FIRST-TIME USER EXPERIENCE:
            //

            // clear first
            centerLayer->removeAllChildren();
            CC_SAFE_REMOVE(mainBtn);

            resultType = ResultTypeDraw1;
            UserInfo::getInstance()->setPrevResultType(ResultTypeDraw1);

            //
            resultOneOrThree = 0;
            setAsResultTypeDraw1(false);
        }

        //
        if (worldLayerUpdater) {
            worldLayerUpdater();
        }
        updateMainLayer();
    }
    /////////////////////////////////////////
    /////////////////////////////////////////
    /////////////////////////////////////////
    else if (resultType == ResultTypeGiftZPlay) {
        //
        // Update center button
        //
        int restHour = 0;
        int restMin = 0;
        UserInfo::getInstance()->getRestGiftTime(restHour, restMin);
        if (restHour > 0 || restMin > 0) {
            showRestTimeInfo(restHour, restMin);
            
            centerButtonStatus = 2;
        }
        else {
            centerButtonStatus = 0;
        }
        
        
        //
        // Update left button
        //
        if (UserInfo::getInstance()->hasEnoughCoinsToDraw()) {
            // Able to Draw
            leftIcon->setColored();
            int idx = leftIcon->getTag();
            
            //
            std::stringstream stream;
            stream << LocalizationManager::getInstance()->getLocalizationString(getIconKeyStrings(idx));
            std::string stringInfo = stream.str();
            
            leftBtnLabel1->setString(stringInfo);
            leftBtnLabel1->setTextColor(Color4B(255, 61, 1, 255));
            
            //
            leftButtonStatus = 1;
        }
        else {
            // Unable to Draw
            leftIcon->setGrayed();
            showRestCoinInfoToLeft();
            
            //
            leftButtonStatus = 2;
        }
        //
        // Update right button
        //
        if (selectedButtonIndex != 2) {
            if (!hasShownVideo && UserInfo::getInstance()->isVideoAvailable()) {
                rightIcon->setColored();
            }
            else {
                rightIcon->setGrayed();
            }
            
            //
            rightButtonStatus = 1;
        }
        else {
            rightIcon->setGrayed();
            showEarnedCoinInfoToRight();
            
            //
            rightButtonStatus = 2;
        }
        
        //
        // Antion!
        //
        restartIntroAction();
    }
    /////////////////////////////////////////
    /////////////////////////////////////////
    /////////////////////////////////////////
    else if (resultType == ResultTypeDraw1) {
        //
        int _restCoins = GoldNeededToDraw - UserInfo::getInstance()->getCoins();
        if (_restCoins > 0) {
            showRestCoinInfo(_restCoins);
            
            //
            centerButtonStatus = 2;
        }
        else {
            //
            // nothing to do
            //
            
            //
            centerButtonStatus = 0;
        }
        
        //
        // Antion!
        //
        restartIntroAction();
    }
}

void ResultMainLayer::showRestTimeInfo(int restHour, int restMin)
{
    //
    {
        //
        //
        //
        if (!centerLbl1) {
            //
            std::stringstream stream;
            stream << LocalizationManager::getInstance()->getLocalizationString("ResultNextGift");
            std::string labelInfo = stream.str();

            centerLbl1 = Label::createWithTTF(labelInfo, UserInfo::getInstance()->getFontPath(), 45);
            centerLbl1->setTextColor(Color4B::BLACK);
            centerLbl1->setAnchorPoint(Vec2(0.5, 0));
            centerLbl1->setPosition(Vec2(visibleSizeHalf.width + origin.x, basePosYWithOrigin - 132 * scaleFactor));
            addChild(centerLbl1, 1);
            
//            basePosY -= centerLbl1->getContentSize().height * 0.8;
        }
        
        
        //
        // Update Time info
        //
        {
            //
            std::stringstream stream;
            stream << restHour;
            std::string hrInfo = stream.str();
            
            stream.str("");
            stream.clear();
            stream << restMin;
            std::string minInfo = stream.str();

            //
            stream.str("");
            stream.clear();
            stream << LocalizationManager::getInstance()->getLocalizationStringWithReplaceTwo("ResultGiftTime", hrInfo, minInfo);
            std::string timeInfo = stream.str();

            if (centerLbl2) {
                centerLbl2->setString(timeInfo);
            }
            else {
                //
                centerLbl2 = Label::createWithTTF(timeInfo, UserInfo::getInstance()->getFontPath(), 60);
                centerLbl2->setAnchorPoint(Vec2(0.5, 1));
                centerLbl2->setPosition(Vec2(visibleSizeHalf.width + origin.x, basePosYWithOrigin - 138 * scaleFactor));
                centerLbl2->setTextColor(Color4B::WHITE);
                addChild(centerLbl2, 1);
            }
        }

    }
}

void ResultMainLayer::showRestTimeInfoToLeft(int restHour, int restMin)
{
    float centerPosX = leftIcon->getPositionX();
    float basePosY = btmButtonPosY;
    
    //
    {
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("ResultNextGift");
        std::string labelInfo = stream.str();

        if (leftBtnLabel1) {
            leftBtnLabel1->setString(labelInfo);
            leftBtnLabel1->setTextColor(Color4B(3, 191, 255, 255));
        }
        else if (!leftBtnLabel1) {
            leftBtnLabel1 = Label::createWithTTF(labelInfo, UserInfo::getInstance()->getFontPath(), 40);
            leftBtnLabel1->setTextColor(Color4B(255, 61, 1, 255));
            leftBtnLabel1->setAnchorPoint(Vec2(0.5, 1));
            leftBtnLabel1->setPosition(Vec2(centerPosX, basePosY) + origin);
            leftBtnLabel1->setScale(scaleFactor);
            addChild(leftBtnLabel1, 1);
        }
        
        basePosY -= leftBtnLabel1->getContentSize().height;
        
        //
        // Update Time info
        //
        {
            //
            std::stringstream stream;
            stream << restHour;
            std::string hrInfo = stream.str();
            
            stream.str("");
            stream.clear();
            stream << restMin;
            std::string minInfo = stream.str();
            
            //
            stream.str("");
            stream.clear();
            stream << LocalizationManager::getInstance()->getLocalizationStringWithReplaceTwo("ResultGiftTime", hrInfo, minInfo);
            std::string timeInfo = stream.str();

            //
            if (leftBtnLabel2) {
                leftBtnLabel2->setString(timeInfo);
            }
            else {
                //
                leftBtnLabel2 = Label::createWithTTF(timeInfo, UserInfo::getInstance()->getFontPath(), 35);
                leftBtnLabel2->setAnchorPoint(Vec2(0.5, 1));
                leftBtnLabel2->setPosition(Vec2(centerPosX, basePosY) + origin);
                leftBtnLabel2->setScale(scaleFactor);
                leftBtnLabel2->setTextColor(Color4B::WHITE);
                addChild(leftBtnLabel2, 1);
            }
        }

        leftButtonStatus  = 2;
    }
}

void ResultMainLayer::showRestTimeInfoToRight(int restHour, int restMin)
{
    float centerPosX = rightIcon->getPositionX();
    float basePosY = btmButtonPosY;
    
    //
    {
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("ResultNextGift");
        std::string labelInfo = stream.str();

        if (rightBtnLabel1) {
            rightBtnLabel1->setString(labelInfo);
            rightBtnLabel1->setTextColor(Color4B(3, 191, 255, 255));
        }
        else if (!rightBtnLabel1) {
            rightBtnLabel1 = Label::createWithTTF(labelInfo, UserInfo::getInstance()->getFontPath(), 40);
            rightBtnLabel1->setTextColor(Color4B(255, 61, 1, 255));
            rightBtnLabel1->setAnchorPoint(Vec2(0.5, 1));
            rightBtnLabel1->setPosition(Vec2(centerPosX, basePosY) + origin);
            rightBtnLabel1->setScale(scaleFactor);
            addChild(rightBtnLabel1, 1);
        }
        
        basePosY -= rightBtnLabel1->getContentSize().height;
        
        //
        // Update Time info
        //
        {
            //
            std::stringstream stream;
            stream << restHour;
            std::string hrInfo = stream.str();
            
            stream.str("");
            stream.clear();
            stream << restMin;
            std::string minInfo = stream.str();
            
            //
            stream.str("");
            stream.clear();
            stream << LocalizationManager::getInstance()->getLocalizationStringWithReplaceTwo("ResultGiftTime", hrInfo, minInfo);
            std::string timeInfo = stream.str();
            
            //
            if (rightBtnLabel2) {
                rightBtnLabel2->setString(timeInfo);
            }
            else {
                //
                rightBtnLabel2 = Label::createWithTTF(timeInfo, UserInfo::getInstance()->getFontPath(), 35);
                rightBtnLabel2->setAnchorPoint(Vec2(0.5, 1));
                rightBtnLabel2->setPosition(Vec2(centerPosX, basePosY) + origin);
                rightBtnLabel2->setTextColor(Color4B::WHITE);
                rightBtnLabel2->setScale(scaleFactor);
                addChild(rightBtnLabel2, 1);
            }
            
            rightButtonStatus  = 2;
        }
    }
}

void ResultMainLayer::showRestCoinInfo(int toGoCoins)
{
//    float basePosY = visibleSizeHalf.height - 85;
    
    if (!centerLbl1) {
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("ResultDraw");
        std::string labelInfo = stream.str();

        centerLbl1 = Label::createWithTTF(labelInfo, UserInfo::getInstance()->getFontPath(), 51);
        centerLbl1->setTextColor(Color4B::BLACK);
        centerLbl1->setAnchorPoint(Vec2(0.5, 0));
        centerLbl1->setPosition(Vec2(visibleSizeHalf.width + origin.x, basePosYWithOrigin - 132 * scaleFactor));
        addChild(centerLbl1, 1);
        
//        basePosY -= centerLbl1->getContentSize().height * 0.8;
    }
    else {
        centerLbl1->setVisible(true);
    }
    
    //
    std::stringstream stream;
    stream << LocalizationManager::getInstance()->getLocalizationStringWithReplace("ResultDrawCoin", toGoCoins);
    std::string coinInfo = stream.str();
    
    if (centerLbl2) {
        centerLbl2->setString(coinInfo);
        centerLbl2->setVisible(true);
    }
    else {
        centerLbl2 = Label::createWithTTF(coinInfo, UserInfo::getInstance()->getFontPath(), 45);
        centerLbl2->setAnchorPoint(Vec2(0.5, 1));
        centerLbl2->setPosition(Vec2(visibleSizeHalf.width + origin.x, basePosYWithOrigin - 138 * scaleFactor));
        centerLbl2->setTextColor(Color4B::WHITE);
        addChild(centerLbl2, 1);
    }
}

void ResultMainLayer::showRestCoinInfoToLeft()
{
    int _restCoins = GoldNeededToDraw - UserInfo::getInstance()->getCoins();
    if (_restCoins <= 0) {
        return;
    }
    
    //
    float centerPosX = leftIcon->getPositionX();
    float basePosY = btmButtonPosY;
    if (leftBtnLabel1) {
        leftBtnLabel1->setTextColor(Color4B(3, 191, 255, 255));
    }
    else {
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("ResultDraw");
        std::string labelInfo = stream.str();

        leftBtnLabel1 = Label::createWithTTF(labelInfo, UserInfo::getInstance()->getFontPath(), 40);
        leftBtnLabel1->setTextColor(Color4B(3, 191, 255, 255));
        leftBtnLabel1->setAnchorPoint(Vec2(0.5, 1));
        leftBtnLabel1->setPosition(Vec2(centerPosX, basePosY) + origin);
        leftBtnLabel1->setScale(scaleFactor);
        addChild(leftBtnLabel1, 1);
    }
    
    basePosY -= leftBtnLabel1->getContentSize().height;
    
    {
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationStringWithReplace("ResultDrawCoin", _restCoins);
        std::string coinInfo = stream.str();
        
        if (leftBtnLabel2) {
            leftBtnLabel2->setString(coinInfo);
        }
        else {
            leftBtnLabel2 = Label::createWithTTF(coinInfo, UserInfo::getInstance()->getFontPath(), 35);
            leftBtnLabel2->setAnchorPoint(Vec2(0.5, 1));
            leftBtnLabel2->setPosition(Vec2(centerPosX, basePosY) + origin);
            leftBtnLabel2->setScale(scaleFactor);
            leftBtnLabel2->setTextColor(Color4B::WHITE);
            addChild(leftBtnLabel2, 1);
        }

        leftButtonStatus  = 2;
    }
}

void ResultMainLayer::showRestCoinInfoToRight()
{
    int _restCoins = GoldNeededToDraw - UserInfo::getInstance()->getCoins();
    if (_restCoins <= 0) {
        return;
    }
    
    //
    float centerPosX = rightIcon->getPositionX();
    float basePosY = btmButtonPosY;
    if (rightBtnLabel1) {
        rightBtnLabel1->setTextColor(Color4B(3, 191, 255, 255));
    }
    else {
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("ResultDraw");
        std::string labelInfo = stream.str();

        rightBtnLabel1 = Label::createWithTTF(labelInfo, UserInfo::getInstance()->getFontPath(), 40);
        rightBtnLabel1->setTextColor(Color4B(3, 191, 255, 255));
        rightBtnLabel1->setAnchorPoint(Vec2(0.5, 1));
        rightBtnLabel1->setPosition(Vec2(centerPosX, basePosY) + origin);
        rightBtnLabel1->setScale(scaleFactor);
        addChild(rightBtnLabel1, 1);
    }
    
    basePosY -= rightBtnLabel1->getContentSize().height;
    
    {
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationStringWithReplace("ResultDrawCoin", _restCoins);
        std::string coinInfo = stream.str();

        if (rightBtnLabel2) {
            rightBtnLabel2->setString(coinInfo);
        }
        else {
            rightBtnLabel2 = Label::createWithTTF(coinInfo, UserInfo::getInstance()->getFontPath(), 35);
            rightBtnLabel2->setAnchorPoint(Vec2(0.5, 1));
            rightBtnLabel2->setPosition(Vec2(centerPosX, basePosY) + origin);
            rightBtnLabel2->setTextColor(Color4B::WHITE);
            rightBtnLabel2->setScale(scaleFactor);
            addChild(rightBtnLabel2, 1);
        }

        rightButtonStatus  = 2;
    }
}

void ResultMainLayer::showEarnedCoinInfo()
{
    if (!centerLbl1) {
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("ResultVideo2");
        std::string labelInfo = stream.str();
        
        centerLbl1 = Label::createWithTTF(labelInfo, UserInfo::getInstance()->getFontPath(), 45);
        centerLbl1->setTextColor(Color4B::BLACK);
        centerLbl1->setAnchorPoint(Vec2(0.5, 0));
        centerLbl1->setPosition(Vec2(visibleSizeHalf.width + origin.x, basePosYWithOrigin - 120 * scaleFactor));
        addChild(centerLbl1, 1);
    }
    else {
        centerLbl1->setVisible(true);
    }
    
    //
    std::stringstream stream;
    stream << LocalizationManager::getInstance()->getLocalizationStringWithReplace("ResultVideoReward", VideoRewardGold);
    std::string coinInfo = stream.str();
    
    if (centerLbl2) {
        centerLbl2->setString(coinInfo);
        centerLbl2->setVisible(true);
    }
    else {
        centerLbl2 = Label::createWithTTF(coinInfo, UserInfo::getInstance()->getFontPath(), 60);
        centerLbl2->setAnchorPoint(Vec2(0.5, 1));
        centerLbl2->setPosition(Vec2(visibleSizeHalf.width + origin.x, basePosYWithOrigin - 120 * scaleFactor));
        centerLbl2->setTextColor(Color4B::WHITE);
        addChild(centerLbl2, 1);
    }
}

void ResultMainLayer::showEarnedCoinInfoToRight()
{
    float centerPosX = rightIcon->getPositionX();
    float basePosY = btmButtonPosY;

    //
    std::stringstream stream;
    stream << LocalizationManager::getInstance()->getLocalizationString("ResultVideo2");
    std::string labelInfo = stream.str();

    if (rightBtnLabel1) {
        
        rightBtnLabel1->setString(labelInfo);
        rightBtnLabel1->setTextColor(Color4B(3, 191, 255, 255));
    } else {
        //
        rightBtnLabel1 = Label::createWithTTF(labelInfo, UserInfo::getInstance()->getFontPath(), 40);
        rightBtnLabel1->setTextColor(Color4B::BLACK);
        rightBtnLabel1->setAnchorPoint(Vec2(0.5, 1));
        rightBtnLabel1->setPosition(Vec2(centerPosX, basePosY) + origin);
        rightBtnLabel1->setScale(scaleFactor);
        addChild(rightBtnLabel1, 1);
    }

    basePosY -= rightBtnLabel1->getContentSize().height;

    {
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationStringWithReplace("ResultVideoReward", VideoRewardGold);
        std::string coinInfo = stream.str();
        
        //
        rightBtnLabel2 = Label::createWithTTF(coinInfo, UserInfo::getInstance()->getFontPath(), 36);
        rightBtnLabel2->setAnchorPoint(Vec2(0.5, 1));
        rightBtnLabel2->setPosition(Vec2(centerPosX, basePosY + 2) + origin);
        rightBtnLabel2->setTextColor(Color4B::WHITE);
        rightBtnLabel2->setScale(scaleFactor);
        addChild(rightBtnLabel2, 1);

        rightButtonStatus  = 2;
    }
}

void ResultMainLayer::showResultRemoveAdsInfo(bool withGivingCoins)
{
//    float basePosY = visibleSizeHalf.height - 85;
    
    if (!centerLbl1) {
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("ResultAD");
        std::string labelInfo = stream.str();
        
        centerLbl1 = Label::createWithTTF(labelInfo, UserInfo::getInstance()->getFontPath(), 45);
        centerLbl1->setTextColor(Color4B::BLACK);
        centerLbl1->setAnchorPoint(Vec2(0.5, 0));
        centerLbl1->setPosition(Vec2(visibleSizeHalf.width + origin.x, basePosYWithOrigin - 132 * scaleFactor));
        addChild(centerLbl1, 1);
        
//        basePosY -= centerLbl1->getContentSize().height * 0.8;
    }
    else {
        centerLbl1->setVisible(true);
    }

    if (withGivingCoins) {
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationStringWithReplace("ResultReward", RemoveAdsRewardGold);
        std::string coinInfo = stream.str();
        
        if (centerLbl2) {
            centerLbl2->setString(coinInfo);
            centerLbl2->setVisible(true);
        }
        else {
            centerLbl2 = Label::createWithTTF(coinInfo, UserInfo::getInstance()->getFontPath(), 35);
            centerLbl2->setAnchorPoint(Vec2(0.5, 1));
            centerLbl2->setPosition(Vec2(visibleSizeHalf.width + origin.x, basePosYWithOrigin - 138 * scaleFactor));
            centerLbl2->setTextColor(Color4B::WHITE);
            addChild(centerLbl2, 1);
        }
    }
    else {
        if (centerLbl2) {
            centerLbl2->removeFromParent();
            centerLbl2 = NULL;
        }
    }
}

void ResultMainLayer::showResultPackageBuy(bool withGivingCoins)
{
//    float basePosY = visibleSizeHalf.height - 85;
    
    if (!centerLbl1) {
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("PackageResult");
        std::string labelInfo = stream.str();
        
        centerLbl1 = Label::createWithTTF(labelInfo, UserInfo::getInstance()->getFontPath(), 45);
        centerLbl1->setTextColor(Color4B::BLACK);
        centerLbl1->setAnchorPoint(Vec2(0.5, 0));
        centerLbl1->setPosition(Vec2(visibleSizeHalf.width + origin.x, basePosYWithOrigin - 132 * scaleFactor));
        addChild(centerLbl1, 1);
        
//        basePosY -= centerLbl1->getContentSize().height * 0.8;
    }
    else {
        centerLbl1->setVisible(true);
    }
    
    if (withGivingCoins) {
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationStringWithReplace("ResultReward", SkinPackageBuyBonus);
        std::string coinInfo = stream.str();
        
        if (centerLbl2) {
            centerLbl2->setString(coinInfo);
            centerLbl2->setVisible(true);
        }
        else {
            centerLbl2 = Label::createWithTTF(coinInfo, UserInfo::getInstance()->getFontPath(), 35);
            centerLbl2->setAnchorPoint(Vec2(0.5, 1));
            centerLbl2->setPosition(Vec2(visibleSizeHalf.width + origin.x, basePosYWithOrigin - 138 * scaleFactor));
            centerLbl2->setTextColor(Color4B::WHITE);
            addChild(centerLbl2, 1);
        }
    }
    else {
        if (centerLbl2) {
            centerLbl2->removeFromParent();
            centerLbl2 = NULL;
        }
    }
}

#pragma mark -
void ResultMainLayer::setMainButton(int idx, float basePosY)
{
    mainBtn = MenuItemImageButton::create();
    mainBtn->setNormalImage(Sprite::createWithSpriteFrameName("button_center_white.png"));
    mainBtn->setSelectedImage(Sprite::createWithSpriteFrameName("button_center_white_click.png"));
    mainBtn->setCallback(CC_CALLBACK_1(ResultMainLayer::btnCallback, this));

    mainBtn->setTag(idx);
    mainBtn->setScale(scaleFactor);
    Size s = mainBtn->getContentSize();
    
    auto mainMenu = Menu::create(mainBtn, NULL);
    mainMenu->setPosition(Vec2(visibleSizeHalf.width + origin.x, basePosYWithOrigin - 145 * scaleFactor));
    addChild(mainMenu, 1);

    std::stringstream stream;
    stream << LocalizationManager::getInstance()->getLocalizationString(btnStringKeys[idx]);

    float fontSize = (UserInfo::getInstance()->currLangType == LanguageType::JAPANESE)? 41:
                        (UserInfo::getInstance()->currLangType == LanguageType::ENGLISH)?46:
                        (UserInfo::getInstance()->currLangType == LanguageType::CHINESE)?52:50;

    std::string labelString = stream.str();
    Label *btnLabel = Label::createWithTTF(labelString, UserInfo::getInstance()->getFontPath(), fontSize);
    btnLabel->setTextColor(Color4B(255, 61, 1, 255));
    btnLabel->setPosition(Vec2(s) * 0.5);
    mainBtn->addChild(btnLabel, 1);
    
    //
    mainBtn->setLabelChild(btnLabel);
}

void ResultMainLayer::setBottomLayerWithLocalString(std::string str, float basePosY, bool withWarning, int amount)
{
    //
    // Bottom Layer
    //
    btmLayer = Layer::create();
    btmLayer->setScale(scaleFactor);
    addChild(btmLayer);
    
    //
    auto pocketIcon = Sprite::createWithSpriteFrameName("icon_coinpocket.png");
    Size iconSize = pocketIcon->getContentSize();
    btmLayer->addChild(pocketIcon, 1);
    
    {
        std::stringstream stream;
        stream << "+" << amount;
        std::string nameInfo = stream.str();
        
        std::string bonusInfo = stream.str();
        Label *btnLabel = Label::createWithTTF(bonusInfo, NumberFontPath, 32);
        btnLabel->setTextColor(Color4B(255, 61, 1, 255));
        btnLabel->setPosition(Vec2(iconSize.width * 0.45, iconSize.height * 0.35));
        btnLabel->setAnchorPoint(Vec2(0.5, 1));
        pocketIcon->addChild(btnLabel, 1);
    }
    
    std::stringstream sstream;
    sstream << str;
    std::string descStr = sstream.str();

    float fontSize = 40;

    TTFConfig config(UserInfo::getInstance()->getFontPath(), fontSize);
    Label *lbl = Label::createWithTTF(config, descStr, TextHAlignment::CENTER, visibleSize.width);
    Size lblSize = lbl->getContentSize();
    
    float gapX = 10;
    float width = iconSize.width + lblSize.width + gapX;
    float halfWidth = width * 0.5;
    
    //
    // positioning
    //
    float iconPosY = (basePosY - resultButtonPosY) * 0.45 + resultButtonPosY;
    
    pocketIcon->setAnchorPoint(Vec2(0, 0.5));
    pocketIcon->setPosition(Vec2(visibleSizeHalf.width - halfWidth + origin.x, iconPosY));
    
    lbl->setAnchorPoint(Vec2(0, 0.5));
    lbl->setPosition(Vec2(pocketIcon->getPositionX() + iconSize.width + gapX + origin.x, iconPosY - 18));
    
    //
    if (!withWarning) {
        lbl->setTextColor(Color4B(255, 188, 4, 255));
        btmLayer->addChild(lbl, 1);
    }
    else if (withWarning) {
        lbl->setTextColor(Color4B(255, 61, 1, 255));
        
        cocos2d::Node* _target = NodeGrid::create();
        _target->addChild(lbl);
        btmLayer->addChild(_target, 1);
        
        auto shaky = Shaky3D::create(5, Size(15,10), 4, false);
        _target->runAction(RepeatForever::create(shaky));
    }
}

void ResultMainLayer::addBonusTooltipToMainButton(Size s, int amount)
{
    //
    auto bonus = BonusTooltipLayer::create();
//    bonus->initWithAmount(Vec2(s.width, s.height) , amount);
    bonus->initWithAmount(amount);
    bonus->setPosition(Vec2(s.width, s.height));
    mainBtn->addChild(bonus, 1000);
}

void ResultMainLayer::addBonusTooltipToRightBottom(Size s, int amount)
{
    //
    auto bonus = BonusTooltipLayer::create();
//    bonus->initWithAmount(Vec2(rightIcon->getPositionX()+50, rightIcon->getPositionY() + 100) , amount);
//    bonus->initWithAmount(Vec2(50, 100) , amount);
    bonus->initWithAmount(amount);
    bonus->setPosition(Vec2(s.width * 0.8, s.height * 0.75));
    bonus->setAnchorPoint(Vec2(0, 0));
    bonus->setScale(1/bottomIconScale);
    rightIcon->addChild(bonus, 1000);
}

#pragma mark -
void ResultMainLayer::setCenterUI(int idx)
{
    //
    float basePosY = visibleSizeHalf.height - 85;

    //
    // MAIN ICON
    //
    {
        mainIcon = Sprite::createWithSpriteFrameName(getIconFileName(idx));
        mainIcon->setAnchorPoint(Vec2(0.5, 0));
        mainIcon->setPosition(Vec2(visibleSizeHalf.width + origin.x, basePosYWithOrigin - 45 * scaleFactor));
        centerLayer->addChild(mainIcon, 1);
    }
    
    //
    // MAIN BUTTON
    //
    if (idx == UIStatusGift) {
        //
        int restHour = 0;
        int restMin = 0;
        UserInfo::getInstance()->getRestGiftTime(restHour, restMin);
        if (restHour > 0 || restMin > 0) {
            showRestTimeInfo(restHour, restMin);
            
            centerButtonStatus = 2;
        }
        else {
            setMainButton(idx, basePosY);

            centerButtonStatus = 0;
        }
    }
    else if (idx == UIStatusDraw) {
        if (UserInfo::getInstance()->hasEnoughCoinsToDraw()) {
            setMainButton(idx, basePosY);
            
            centerButtonStatus = 0;
        }
        else {
            int _restCoins = GoldNeededToDraw - UserInfo::getInstance()->getCoins();
            if (_restCoins > 0) {
                showRestCoinInfo(_restCoins);
            }
            
            centerButtonStatus = 2;
        }
    }
    else {
        setMainButton(idx, basePosY);

        centerButtonStatus = 0;
    }

    //
    centerLayer->setVisible(false);
    if (mainBtn) {
        mainBtn->setVisible(false);
    }
    if (centerLbl1) {
        centerLbl1->setVisible(false);
    }
    if (centerLbl2) {
        centerLbl2->setVisible(false);
    }
}

void ResultMainLayer::setLeftButtonLabel()
{
    int idx = leftIcon->getTag();
    
    if (!leftBtnLabel1) {
        Size s = leftIcon->getContentSize();

        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString(getIconKeyStrings(idx));
        std::string stringInfo = stream.str();

        TTFConfig config(UserInfo::getInstance()->getFontPath(), 40);
        leftBtnLabel1 = Label::createWithTTF(config, stringInfo, TextHAlignment::CENTER, s.width);
        leftBtnLabel1->setAnchorPoint(Vec2(0.5, 1));
        leftBtnLabel1->setTextColor(Color4B(255, 61, 1, 255));
        leftBtnLabel1->setPosition(Vec2(leftIcon->getPositionX(), btmButtonPosY) + origin);
        leftBtnLabel1->setScale(scaleFactor);
        addChild(leftBtnLabel1, 1);
    }
}

#define LeftRightMargin 0.225
void ResultMainLayer::setLeftUI(int idx)
{
    if (idx != UIStatusDraw && idx != UIStatusGift) {
        return;
    }

    //
    // button icon
    //
    leftIcon = GraySprite::createWithSpriteFrameName(getIconFileName(idx));
    Size s = leftIcon->getContentSize();
    leftIcon->setAnchorPoint(Vec2(0.5, 0));
    leftIcon->setScale(bottomIconScale);
    leftIcon->setPosition(Vec2(visibleSize.width * LeftRightMargin, btmButtonPosY + 8) + origin);
    leftIcon->setTag(idx);
    leftIcon->setColorRate(0.8);
    addChild(leftIcon, 1);

    //
    setLeftButtonLabel();

    if (idx == UIStatusDraw) {
        if (UserInfo::getInstance()->hasEnoughCoinsToDraw()) {
            //
            // Able to Draw
            //
            leftIcon->setColored();
        }
        else {
            //
            // Unable to Draw
            //
            leftIcon->setGrayed();

            //
            showRestCoinInfoToLeft();
        }
    }
    else if (idx == UIStatusGift) {
        //
        int restHour = 0;
        int restMin = 0;
        UserInfo::getInstance()->getRestGiftTime(restHour, restMin);
        if (restHour > 0 || restMin > 0) {
            //
            // Unable to get gift
            //
            leftIcon->setGrayed();

            //
            showRestTimeInfoToLeft(restHour, restMin);
        }
        else {
            //
            // Able to get gift
            //
            leftIcon->setColored();
        }
    }
    
    
    if (leftIcon) {
        leftIcon->setVisible(false);
    }
    
    if (leftBtnLabel1) {
        leftBtnLabel1->setVisible(false);
    }
    if (leftBtnLabel2) {
        leftBtnLabel2->setVisible(false);
    }
}

void ResultMainLayer::setRightButtonLabel()
{
    int idx = rightIcon->getTag();

    if (!rightBtnLabel1) {
        Size s = rightIcon->getContentSize();

        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString(getIconKeyStrings(idx));
        std::string stringInfo = stream.str();

        TTFConfig config(UserInfo::getInstance()->getFontPath(), 40);
        rightBtnLabel1 = Label::createWithTTF(config, stringInfo, TextHAlignment::CENTER, s.width);
        rightBtnLabel1->setAnchorPoint(Vec2(0.5, 1));
        rightBtnLabel1->setTextColor(Color4B(255, 61, 1, 255));
        rightBtnLabel1->setPosition(Vec2(rightIcon->getPositionX(), btmButtonPosY) + origin);
        rightBtnLabel1->setScale(scaleFactor);
        addChild(rightBtnLabel1, 1);
    }
}

void ResultMainLayer::setRightUI(int idx)
{
    //
    // button icon
    //
    rightIcon = GraySprite::createWithSpriteFrameName(getIconFileName(idx));
    Size s = rightIcon->getContentSize();
    rightIcon->setAnchorPoint(Vec2(0.5, 0));
    rightIcon->setScale(bottomIconScale);
    rightIcon->setPosition(Vec2(visibleSize.width * (1 - LeftRightMargin), btmButtonPosY + 8) + origin);
    rightIcon->setTag(idx);
    rightIcon->setColorRate(0.8);
    addChild(rightIcon, 1);

    //
    setRightButtonLabel();
    
    if (idx == UIStatusShare) {
        //
        rightIcon->setColored();
        if (rightBtnLabel1) {
            rightBtnLabel1->setTextColor(Color4B(3, 191, 255, 255));
        }
    }
    else if (idx == UIStatusDraw || idx == UIStatusGift) {
        //
        if (idx == UIStatusDraw) {
            if (UserInfo::getInstance()->hasEnoughCoinsToDraw()) {
                //
                // Able to Draw
                //
                rightIcon->setColored();
            }
            else {
                //
                // Unable to Draw
                //
                rightIcon->setGrayed();
                
                //
                showRestCoinInfoToRight();
            }
        }
        else if (idx == UIStatusGift) {
            //
            int restHour = 0;
            int restMin = 0;
            UserInfo::getInstance()->getRestGiftTime(restHour, restMin);
            if (restHour > 0 || restMin > 0) {
                //
                // Unable to get gift
                //
                rightIcon->setGrayed();
                
                //
                showRestTimeInfoToRight(restHour, restMin);
            }
            else {
                //
                // Able to get gift
                //
                rightIcon->setColored();
            }
        }
    }
    
    if (rightIcon) {
        rightIcon->setVisible(false);
    }
    
    if (rightBtnLabel1) {
        rightBtnLabel1->setVisible(false);
    }
    if (rightBtnLabel2) {
        rightBtnLabel2->setVisible(false);
    }
}

void ResultMainLayer::startIntroAction()
{
    if (resultType == ResultTypeGiftZPlay ||
        resultType == ResultTypeDrawZPlay) {
        //
        // run actions for 3 UIs
        //
        auto bounceCenter = CallFunc::create( [this]() {
            if (centerLayer) {
                centerLayer->setScale(0.4 * scaleFactor);
                centerLayer->setVisible(true);
                centerLayer->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, scaleFactor), 0.25));
            }
        });
        auto bounceCenterBtn = CallFunc::create( [this]() {
            if (centerButtonStatus == 0) {
                if (mainBtn) {
                    mainBtn->setScale(0.4 * scaleFactor);
                    mainBtn->setVisible(true);

                    //
                    setMainButtonAction();
                }
            }
            else {
                if (centerButtonStatus == 2) {
                    if (centerLbl2) {
                        centerLbl2->setScale(0.4 * scaleFactor);
                        centerLbl2->setVisible(true);
                        centerLbl2->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, scaleFactor), 0.25));
                    }
                }
                
                if (centerLbl1) {
                    centerLbl1->setScale(0.4 * scaleFactor);
                    centerLbl1->setVisible(true);
                    centerLbl1->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, scaleFactor), 0.25));
                }
            }
        });
        
        auto bounceLeftBtn = CallFunc::create( [this]() {
            if (leftIcon) {
                leftIcon->setScale(bottomIconScale * 0.4);
                leftIcon->setVisible(true);
                leftIcon->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, bottomIconScale), 0.25));

                //
                setLeftIconAction();
            }
            
            if (leftButtonStatus == 2) {
                if (leftBtnLabel2) {
                    leftBtnLabel2->setScale(scaleFactor * 0.4);
                    leftBtnLabel2->setVisible(true);
                    leftBtnLabel2->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, scaleFactor), 0.25));
                }
            }

            if (leftBtnLabel1) {
                leftBtnLabel1->setScale(scaleFactor * 0.4);
                leftBtnLabel1->setVisible(true);
                leftBtnLabel1->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, scaleFactor), 0.25));
            }
        });
        
        auto bounceRightBtn = CallFunc::create( [this]() {
            if (rightIcon) {
                rightIcon->setScale(bottomIconScale * 0.4);
                rightIcon->setVisible(true);
                rightIcon->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, bottomIconScale), 0.25));

                //
                setRightIconAction();
            }

            if (rightButtonStatus == 2) {
                if (rightBtnLabel2) {
                    rightBtnLabel2->setScale(scaleFactor * 0.4);
                    rightBtnLabel2->setVisible(true);
                    rightBtnLabel2->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, scaleFactor), 0.25));
                }
            }

            if (rightBtnLabel1) {
                rightBtnLabel1->setScale(scaleFactor * 0.4);
                rightBtnLabel1->setVisible(true);
                rightBtnLabel1->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, scaleFactor), 0.25));
            }
        });

        auto centerBtnSound = CallFunc::create( [this]() {
            SoundManager::getInstance()->playSoundEffect(SoundButtonAppear, false);
        });
        
        //
        auto seq1 = Sequence::create(
                                     DelayTime::create(0.18),
                                     bounceLeftBtn,
                                     NULL);
        auto seq2 = Sequence::create(
                                     DelayTime::create(0.33),
                                     bounceRightBtn,
                                     NULL);
        auto seq3 = Sequence::create(
                                     DelayTime::create(0.5),
                                     bounceCenter,
                                     NULL);
        auto seq4 = Sequence::create(
                                     DelayTime::create(0.8),
                                     bounceCenterBtn,
                                     centerBtnSound,
                                     NULL);
        auto spwn = Spawn::create(seq1, seq2, seq3, seq4, NULL);
        this->runAction(spwn);
    }
    else {
        //
        // run actions for 1 UI
        //
        auto bounceCenter = CallFunc::create( [this]() {
            if (centerLayer) {
                centerLayer->setScale(0.4 * scaleFactor);
                centerLayer->setVisible(true);
                centerLayer->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, scaleFactor), 0.25));
            }
        });
        auto bounceCenterBtn = CallFunc::create( [this]() {
            if (centerButtonStatus == 0) {
                if (mainBtn) {
                    mainBtn->setScale(0.4 * scaleFactor);
                    mainBtn->setVisible(true);

                    //
                    setMainButtonAction();
                }
            }
            else {
                if (centerButtonStatus == 2) {
                    if (centerLbl2) {
                        centerLbl2->setScale(0.4 * scaleFactor);
                        centerLbl2->setVisible(true);
                        centerLbl2->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, scaleFactor), 0.25));
                    }
                }
                
                if (centerLbl1) {
                    centerLbl1->setScale(0.4 * scaleFactor);
                    centerLbl1->setVisible(true);
                    centerLbl1->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, scaleFactor), 0.25));
                }
            }
        });
        auto centerBtnSound = CallFunc::create( [this]() {
            SoundManager::getInstance()->playSoundEffect(SoundButtonAppear, false);
        });

        //
        auto seq1 = Sequence::create(
                                    DelayTime::create(0.3),
                                    bounceCenter,
                                    NULL);
        auto seq2 = Sequence::create(
                                    DelayTime::create(0.5),
                                    bounceCenterBtn,
                                    centerBtnSound,
                                    NULL);
        auto spwn = Spawn::create(seq1, seq2, NULL);
        this->runAction(spwn);
    }
}

void ResultMainLayer::restartIntroAction()
{
    centerLayer->setVisible(false);
    if(mainBtn) mainBtn->setVisible(false);
    if(centerLbl1) centerLbl1->setVisible(false);
    if(centerLbl2) centerLbl2->setVisible(false);
    
    if(leftIcon) leftIcon->setVisible(false);
    if(leftBtnLabel1) leftBtnLabel1->setVisible(false);
    if(leftBtnLabel2) leftBtnLabel2->setVisible(false);
    
    if(rightIcon) rightIcon->setVisible(false);
    if(rightBtnLabel1) rightBtnLabel1->setVisible(false);
    if(rightBtnLabel2) rightBtnLabel2->setVisible(false);
    
    startIntroAction();
}

#pragma mark -
#pragma mark Touch Event
bool ResultMainLayer::onTouchBegan(Touch *touch, Event *event)
{
    Vec2 pos = touch->getLocation();
    
    if (leftIcon && leftIcon->isVisible() && leftIcon->getBoundingBox().containsPoint(pos)) {
        if (leftIcon->getGrayed()) {
            SoundManager::getInstance()->playSoundEffect(SoundDisabledButton, false);
            leftIcon->setScale(bottomIconScale);
            selectedBottomButton = 0;
        }
        else {
            leftIcon->setScale(bottomIconSelelectScale);
            leftIcon->stopAllActions();
            selectedBottomButton = 0;
        }
    }
    else if (rightIcon && rightIcon->isVisible() && rightIcon->getBoundingBox().containsPoint(pos)) {
        if (rightIcon->getGrayed()) {
            SoundManager::getInstance()->playSoundEffect(SoundDisabledButton, false);
            rightIcon->setScale(bottomIconScale);
            selectedBottomButton = 1;
        }
        else {
            rightIcon->setScale(bottomIconSelelectScale);
            rightIcon->stopAllActions();
            selectedBottomButton = 1;
        }
    }

    return true;
}

void ResultMainLayer::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 pos = touch->getLocation();
    if (selectedBottomButton == 0 && leftIcon&& leftIcon->isVisible()  && !leftIcon->getGrayed() && leftIcon->getBoundingBox().containsPoint(pos)) {
        leftIcon->setScale(bottomIconSelelectScale);
    }
    else if (leftIcon) {
        leftIcon->setScale(bottomIconScale);
    }

    if (selectedBottomButton == 1 && rightIcon && rightIcon->isVisible() && !rightIcon->getGrayed() && rightIcon->getBoundingBox().containsPoint(pos)) {
        rightIcon->setScale(bottomIconSelelectScale);
    }
    else if (rightIcon) {
        rightIcon->setScale(bottomIconScale);
    }
}

void ResultMainLayer::onTouchEnded(Touch *touch, Event *event)
{
    Vec2 pos = touch->getLocation();
    if (selectedBottomButton == 0 && leftIcon && leftIcon->isVisible()) {
        
        if (leftIcon->getBoundingBox().containsPoint(pos)) {
            if (!leftIcon->getGrayed()) {
                //
                selectedLeft();
            }
            
            leftIcon->setScale(bottomIconScale);
        }
        else {
            setLeftIconAction();
        }
    }
    else if (selectedBottomButton == 1 && rightIcon && rightIcon->isVisible()) {
        
        if (rightIcon->getBoundingBox().containsPoint(pos)) {
            if (!rightIcon->getGrayed()) {
                //
                selectedRight();
            }
            
            rightIcon->setScale(bottomIconScale);
        }
        else {
            setRightIconAction();
        }
    }
}

void ResultMainLayer::onTouchCancelled(Touch *touch, Event *event)
{
}
