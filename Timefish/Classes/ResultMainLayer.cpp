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
    uiInitializer[1] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeGift2, this));
    uiInitializer[2] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeDraw1, this));
    uiInitializer[3] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeDraw2, this));
    uiInitializer[4] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeVideo1, this));
    uiInitializer[5] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeVideo2, this));
    uiInitializer[6] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeBuy1, this));
    uiInitializer[7] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeBuy2, this));
    uiInitializer[8] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeRemoveAd1, this));
    uiInitializer[9] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeRemoveAd2, this));
    uiInitializer[10] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeTry, this));
    uiInitializer[11] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeRate, this));
    uiInitializer[12] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeFacebook, this));
    uiInitializer[13] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeShare, this));
    uiInitializer[14] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeArtwork, this));
    uiInitializer[15] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeBuyPackage, this));
    uiInitializer[16] = (CC_CALLBACK_1(ResultMainLayer::setAsResultTypeGoldenFish, this));

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
    if (resultType == ResultTypeGift2 ||
        resultType == ResultTypeDraw2 ||
        resultType == ResultTypeVideo2 ||
        resultType == ResultTypeBuy2 ||
        resultType == ResultTypeRemoveAD2 ) {
        resultOneOrThree = 1;
    }
    else {
        resultOneOrThree = 0;
    }
    
    //
    //
    //
    if (!(resultType == ResultTypeVideo1 || resultType == ResultTypeVideo2)) {
        UserInfo::getInstance()->setHaveSeenVideo(false);
    }
}

void ResultMainLayer::setMainResultTypeForFirstTimeUser(int idx)
{
    ResultType firstTimeResultTypes[] = {
        ResultTypeGift1,
        ResultTypeVideo2,
        ResultTypeBuy1,
        ResultTypeTrial,
        ResultTypeFacebook,
        ResultTypeGoldenFish,
        ResultTypeRate,
        ResultTypeBuyPackage
    };
    
    //
    // Rules
    //
    bool firstTime = UserInfo::getInstance()->isThisFirstTimeLaunch();
    bool isGiftAvailable = ableToGetGift();
    bool enoughGoldToDraw = UserInfo::getInstance()->hasEnoughCoinsToDraw();
    bool isVideoAvailable = UserInfo::getInstance()->isVideoAvailable() && UserInfo::getInstance()->isFarEnoughFromLastSeenVideo();
    bool unlockedAll = (UserInfo::getInstance()->getRandomSkinAmongLocked() == -1);
    bool goldenFishUnlocked = UserInfo::getInstance()->isGoldenFishUnlocked();
    bool liked = UserInfo::getInstance()->getLiked();
    bool rated = UserInfo::getInstance()->getRated();
    randomPackageIndex = UserInfo::getInstance()->getRandomPackageIndex();
    bool isPackageAvailable = (randomPackageIndex >= 0);
    int trialCount = UserInfo::getInstance()->getTrialCount();

    ResultType prevResultType = UserInfo::getInstance()->getPrevResultType();
    
    if (firstTime) {
        resultType = firstTimeResultTypes[idx];
        UserInfo::getInstance()->increaseFirstTimeResultTypeIndex();
    }
    if (isGiftAvailable) {
        resultType = ResultTypeGift1;
    }
    else if (enoughGoldToDraw && prevResultType != ResultTypeDraw2) {
        resultType = ResultTypeDraw2;
    }
    else {
        //
        // FOR THE FIRST-TIME USER EXPERIENCE:
        //

        //
        if (idx == 1 && !isVideoAvailable) {
            idx++;
            UserInfo::getInstance()->increaseFirstTimeResultTypeIndex();
        }

        //
        if (idx == 2 && unlockedAll) {
            idx++;
            UserInfo::getInstance()->increaseFirstTimeResultTypeIndex();
        }

        //
        if (idx == 3 && unlockedAll) {
            idx++;
            UserInfo::getInstance()->increaseFirstTimeResultTypeIndex();
        }

        //
        if (idx == 4 && liked) {
            idx++;
            UserInfo::getInstance()->increaseFirstTimeResultTypeIndex();
        }

        //
        if (idx == 5 && goldenFishUnlocked) {
            idx++;
            UserInfo::getInstance()->increaseFirstTimeResultTypeIndex();
        }

        //
        if (idx == 6 && rated) {
            idx++;
            UserInfo::getInstance()->increaseFirstTimeResultTypeIndex();
        }

        //
        if (idx == 7 && !isPackageAvailable) {
            idx++;
            UserInfo::getInstance()->increaseFirstTimeResultTypeIndex();
        }

        // re-evaluate
        if (idx < FirstTimeResultTypeIndexMax) {
            resultType = firstTimeResultTypes[idx];

            if (!(idx == 3 && trialCount < TrialAllowNum)) {
                UserInfo::getInstance()->increaseFirstTimeResultTypeIndex();
            }
        }
        else {
            resultType = ResultTypeArtwork;
        }
    }
}

void ResultMainLayer::setMainResultTypeForExpUser()
{
    bool isGiftAvailable = ableToGetGift();
    bool hasTrialSkin = (UserInfo::getInstance()->getTrialSkinIndex() != -1);
    if (hasTrialSkin) {
        resultType = ResultTypeTrial;
        
        return;
    }
    else if (isGiftAvailable) {
        resultType = ResultTypeGift1;
        
        return;
    }

    //
    //
    //
    ResultType prevResultType = UserInfo::getInstance()->getPrevResultType();

    //
    // Conditions
    //
    bool isVideoAvailable = UserInfo::getInstance()->isVideoAvailable() && UserInfo::getInstance()->isFarEnoughFromLastSeenVideo();
    bool enoughGoldToDraw = UserInfo::getInstance()->hasEnoughCoinsToDraw();
    bool unlockedAll = (UserInfo::getInstance()->getRandomSkinAmongLocked() == -1 );
    bool isRecordingEnabled = UserInfo::getInstance()->isRecordingEnabled();
    randomPackageIndex = UserInfo::getInstance()->getRandomPackageIndex();
    bool isPackageAvailable = (randomPackageIndex >= 0);
    bool goldenFishUnlocked = UserInfo::getInstance()->isGoldenFishUnlocked();
    bool adsRemoved = UserInfo::getInstance()->getNoAdOption();
    bool rated = UserInfo::getInstance()->getRated();
    bool liked = UserInfo::getInstance()->getLiked();

    //
    //
    //
    int resultGroupType = UserInfo::getInstance()->getResultGroupTypeIndex();
    int prob = rand()%999 + 1; // 1 ~ 1000

    //
    // CASE 1
    //
    if (resultGroupType == 0) {
        if (prob < 100 && isVideoAvailable && prevResultType!=ResultTypeGift2) {
            resultType = ResultTypeGift2;
        }
        else if (prob < 200 && enoughGoldToDraw && prevResultType!=ResultTypeDraw1) {
            resultType = ResultTypeDraw1;
        }
        else if (prob < 500 && isVideoAvailable && prevResultType!=ResultTypeDraw2) {
            resultType = ResultTypeDraw2;
        }
        else if (prob < 600 && isVideoAvailable && prevResultType!=ResultTypeVideo1) {
            resultType = ResultTypeVideo1;
        }
        else if (prob < 1000 && isVideoAvailable && prevResultType!=ResultTypeVideo2) {
            resultType = ResultTypeVideo2;
        }
        else if (!unlockedAll) {
            resultType = ResultTypeTrial;
        }
        // exceptions
        else if (prob < 500 && isRecordingEnabled && prevResultType!=ResultTypeShare) {
            resultType = ResultTypeShare;
        }
        else {
            resultType = ResultTypeArtwork;
        }

        //
        UserInfo::getInstance()->increaseResultGroupTypeIndex();
    }
    //
    // CASE 2
    //
    else if (resultGroupType == 1) {
        if (prob < 100 && isPackageAvailable && prevResultType!=ResultTypeBuyPackage) {
            resultType = ResultTypeBuyPackage;
        }
        else if (prob < 500 && !unlockedAll && prevResultType!=ResultTypeBuy1) {
            resultType = ResultTypeBuy1;
        }
        else if (prob < 1000 && !unlockedAll && isVideoAvailable && prevResultType!=ResultTypeBuy2) {
            resultType = ResultTypeBuy2;
        }
        // exceptions
        else if (prob < 500 && isRecordingEnabled && prevResultType!=ResultTypeShare) {
            resultType = ResultTypeShare;
        }
        else {
            resultType = ResultTypeArtwork;
        }
        
        //
        UserInfo::getInstance()->increaseResultGroupTypeIndex();
    }
    //
    // CASE 3
    //
    else if (resultGroupType == 2) {
        if (prob < 100 && !unlockedAll && prevResultType!=ResultTypeTrial) {
            resultType = ResultTypeTrial;
        }
        else if (prob < 300 && isPackageAvailable && prevResultType!=ResultTypeBuyPackage) {
            resultType = ResultTypeBuyPackage;
        }
        else if (prob < 600 && !goldenFishUnlocked && prevResultType!=ResultTypeGoldenFish) {
            resultType = ResultTypeGoldenFish;
        }
        else if (prob < 800 && !adsRemoved && prevResultType!=ResultTypeRemoveAD1) {
            resultType = ResultTypeRemoveAD1;
        }
        else if (prob < 1000 && !adsRemoved && prevResultType!=ResultTypeRemoveAD2) {
            resultType = ResultTypeRemoveAD2;
        }
        // exceptions
        else if (prob < 500 && isRecordingEnabled && prevResultType!=ResultTypeShare) {
            resultType = ResultTypeShare;
        }
        else {
            resultType = ResultTypeArtwork;
        }
        
        //
        UserInfo::getInstance()->increaseResultGroupTypeIndex();
    }
    //
    // CASE 4
    //
    else if (resultGroupType == 3) {
        if (prob < 200 && isVideoAvailable && prevResultType!=ResultTypeDraw2) {
            resultType = ResultTypeDraw2;
        }
        else if (prob < 400 && isVideoAvailable && prevResultType!=ResultTypeVideo1) {
            resultType = ResultTypeVideo1;
        }
        else if (prob < 950 && !liked && prevResultType!=ResultTypeFacebook) {
            resultType = ResultTypeFacebook;
        }
        else if (prob < 1000 && prevResultType!=ResultTypeArtwork) {
            resultType = ResultTypeArtwork;
        }
        // exceptions
        else if (prob < 500 && isRecordingEnabled && prevResultType!=ResultTypeShare) {
            resultType = ResultTypeShare;
        }
        else {
            resultType = ResultTypeArtwork;
        }
        
        //
        UserInfo::getInstance()->increaseResultGroupTypeIndex();
    }
    //
    // CASE 5
    //
    else if (resultGroupType == 4) {
        if (prob < 250 && isVideoAvailable && prevResultType!=ResultTypeVideo2) {
            resultType = ResultTypeVideo2;
        }
        else if (prob < 300 && isRecordingEnabled && prevResultType!=ResultTypeShare) {
            resultType = ResultTypeShare;
        }
        else if (prob < 700 && !rated && prevResultType!=ResultTypeRate) {
            resultType = ResultTypeRate;
        }
        else if (prob < 900 && !liked && prevResultType!=ResultTypeFacebook) {
            resultType = ResultTypeFacebook;
        }
        else if (prob < 1000 && prevResultType!=ResultTypeArtwork) {
            resultType = ResultTypeArtwork;
        }
        // exceptions
        else if (prob < 500 && isRecordingEnabled && prevResultType!=ResultTypeShare) {
            resultType = ResultTypeShare;
        }
        else {
            resultType = ResultTypeArtwork;
        }
        
        //
        UserInfo::getInstance()->increaseResultGroupTypeIndex();
    }
    //
    // CASE 6
    //
    else if (resultGroupType == 5) {
        if (prob < 200 && !unlockedAll && prevResultType!=ResultTypeTrial) {
            resultType = ResultTypeTrial;
        }
        else if (prob < 500 && isPackageAvailable && prevResultType!=ResultTypeBuyPackage) {
            resultType = ResultTypeBuyPackage;
        }
        else if (prob < 700 && !adsRemoved && prevResultType!=ResultTypeRemoveAD2) {
            resultType = ResultTypeRemoveAD2;
        }
        else if (prob < 1000 && !goldenFishUnlocked &&prevResultType!=ResultTypeGoldenFish) {
            resultType = ResultTypeGoldenFish;
        }
        else if (isVideoAvailable) {
            resultType = ResultTypeVideo1;
        }
        // exceptions
        else if (prob < 500 && isRecordingEnabled && prevResultType!=ResultTypeShare) {
            resultType = ResultTypeShare;
        }
        else {
            resultType = ResultTypeArtwork;
        }
        
        //
        UserInfo::getInstance()->increaseResultGroupTypeIndex();
    }
}

void ResultMainLayer::btnCallback(Ref* pSender)
{
    if (resultType == ResultTypeTrial) {
        UserInfo::getInstance()->setTrialSkinIndex(skinIdxSelected);
    }

    //
    // get Callback index by ResultType
    //
    int callbackIdx =(resultType == ResultTypeGift1 || resultType == ResultTypeGift2)? 0:
            (resultType == ResultTypeDraw1 || resultType == ResultTypeDraw2)? 1:
            (resultType == ResultTypeVideo1 || resultType == ResultTypeVideo2)? 2:
            (resultType == ResultTypeBuy1 || resultType == ResultTypeBuy2 ||
             resultType == ResultTypeGoldenFish)? 3:
            (resultType == ResultTypeBuyPackage)? 10:
            (resultType == ResultTypeRemoveAD1 || resultType == ResultTypeRemoveAD2)? 4:
            (resultType == ResultTypeTrial)? 5:
            (resultType == ResultTypeRate)? 6:
            (resultType == ResultTypeFacebook)? 7:
            (resultType == ResultTypeShare)? 8:9;

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
    
    if (btnTag == UIStatusDraw) {
        buttonCallbacks[1]();
    }
    else if (btnTag == UIStatusGift) {
        buttonCallbacks[0]();
    }
    else if (btnTag == UIStatusVideo) {
        hasShownVideo = true;
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
        else {
            //
            // Switch to ResultTypeGift2
            //
            resultType = ResultTypeGift2;
            UserInfo::getInstance()->setPrevResultType(ResultTypeGift2);

            //
            resultOneOrThree = 1;
            basePosYWithOrigin += 20;
            mainIcon->setPositionY(mainIcon->getPositionY() + 60);
            setAsResultTypeGift2(false);
        }

        //
        if (worldLayerUpdater) {
            worldLayerUpdater();
        }
        updateMainLayer();

//        //
//        // Switch to ResultTypeGift2
//        //
//        resultType = ResultTypeGift2;
//        UserInfo::getInstance()->setPrevResultType(ResultTypeGift2);
//
//        //
//        resultOneOrThree = 1;
//        basePosYWithOrigin += 20;
//        if (worldLayerUpdater) {
//            worldLayerUpdater();
//        }
//        mainIcon->setPositionY(mainIcon->getPositionY() + 60);
//        
//        setAsResultTypeGift2(false);
//        updateMainLayer();
    }
    /////////////////////////////////////////
    /////////////////////////////////////////
    /////////////////////////////////////////
    else if (resultType == ResultTypeGift2) {
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
    /////////////////////////////////////////
    /////////////////////////////////////////
    /////////////////////////////////////////
    else if (resultType == ResultTypeDraw2) {
        //
        // Update center button
        //
        if (UserInfo::getInstance()->hasEnoughCoinsToDraw()) {
            if (!mainBtn) {
                float basePosY = visibleSizeHalf.height - 85;
                setMainButton(UIStatusDraw, basePosY);
            }

            //
            centerButtonStatus = 0;
        }
        else {
            int _restCoins = GoldNeededToDraw - UserInfo::getInstance()->getCoins();
            if (_restCoins > 0) {
                showRestCoinInfo(_restCoins);
            }
            
            //
            centerButtonStatus = 2;
        }
        //
        // Update left button
        //
        {
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
    else if (resultType == ResultTypeVideo1) {
        //
        // Switch to ResultTypeGift2
        //
        resultType = ResultTypeVideo2;
        UserInfo::getInstance()->setPrevResultType(ResultTypeVideo2);

        resultOneOrThree = 1;
        basePosYWithOrigin += 20;
        if (worldLayerUpdater) {
            worldLayerUpdater();
        }
        mainIcon->setPositionY(mainIcon->getPositionY() + 60);

        setAsResultTypeVideo2(false);
        updateMainLayer();
    }
    /////////////////////////////////////////
    /////////////////////////////////////////
    /////////////////////////////////////////
    else if (resultType == ResultTypeVideo2) {
        //
        // Update center button
        //
        if (selectedButtonIndex == 0) {
            showEarnedCoinInfo();

            //
            centerButtonStatus = 2;
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
        {
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
                
                //
                rightButtonStatus = 2;
            }
            else {
                //
                // Able to get gift
                //
                rightIcon->setColored();

                //
                rightButtonStatus = 1;
            }
        }

        //
        // Antion!
        //
        restartIntroAction();
    }
    /////////////////////////////////////////
    /////////////////////////////////////////
    /////////////////////////////////////////
    else if (resultType == ResultTypeBuy1) {
        //
        // Clear previous!
        //
        centerLayer->removeAllChildren();
        CC_SAFE_REMOVE(mainBtn);

        //
        // ...And Reset!
        //
        resultType = ResultTypeArtwork;
        UserInfo::getInstance()->setPrevResultType(ResultTypeArtwork);
        setAsResultTypeArtwork(true);
        updateMainLayer();

//        UserInfo::getInstance()->setHaveSeenVideo(true);
    }
    /////////////////////////////////////////
    /////////////////////////////////////////
    /////////////////////////////////////////
    else if (resultType == ResultTypeBuy2) {
        //
        // Update center button
        //
        if (selectedButtonIndex == 0) {
            //
            // Clear previous!
            //
            centerLayer->removeAllChildren();
            CC_SAFE_REMOVE(mainBtn);
            
            //
            // ...And Reset!
            //
            resultType = ResultTypeArtwork;
            UserInfo::getInstance()->setPrevResultType(ResultTypeArtwork);

            resultOneOrThree = 0;
            if (worldLayerUpdater) {
                worldLayerUpdater();
            }

            setAsResultTypeArtwork(true);
            updateMainLayer();

            //
            // No need to go further! Return!
            //
            return;
        }
        else {
            //
            // NOTE: When comeback to result UI,
            // check the selected skin is unlocked by draw or other UI.
            // If so, change it.
            //
            if (!UserInfo::getInstance()->isLockedSkin(skinIdxSelected)) {
                
                centerLayer->removeAllChildren();
                CC_SAFE_REMOVE(mainBtn);

                setCenterUI(UIStatusBuy);
            }
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
    else if (resultType == ResultTypeBuyPackage) {
        //
        btmLayer->removeAllChildren();

        showResultPackageBuy(withoutGivingCoins);

        centerButtonStatus = 2;

        //
        // Antion!
        //
        restartIntroAction();
    }
    /////////////////////////////////////////
    /////////////////////////////////////////
    /////////////////////////////////////////
    else if (resultType == ResultTypeRemoveAD1) {
        showResultRemoveAdsInfo(withoutGivingCoins);

        centerButtonStatus = 2;

        //
        // Antion!
        //
        restartIntroAction();
    }
    /////////////////////////////////////////
    /////////////////////////////////////////
    /////////////////////////////////////////
    else if (resultType == ResultTypeRemoveAD2) {
        //
        // Update center button
        //
        if (selectedButtonIndex == 0) {
            showResultRemoveAdsInfo(withoutGivingCoins);
            
            centerButtonStatus = 2;
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
    else if (resultType == ResultTypeTrial) {
        //
        // Clear previous!
        //
        centerLayer->removeAllChildren();
        CC_SAFE_REMOVE(mainBtn);
        btmLayer->removeAllChildren();
        
        //
        // ...And Reset!
        //
        resultType = ResultTypeArtwork;
        UserInfo::getInstance()->setPrevResultType(ResultTypeArtwork);
        setAsResultTypeArtwork(true);
        updateMainLayer();
    }
    /////////////////////////////////////////
    /////////////////////////////////////////
    /////////////////////////////////////////
    else if (resultType == ResultTypeArtwork) {
        //
        // Antion!
        //
        centerButtonStatus = 0;

        restartIntroAction();
    }
}

void ResultMainLayer::showRestTimeInfo(int restHour, int restMin)
{
//    float basePosY = visibleSizeHalf.height - 85;
    //
    {
//        if (restHour == -1 && restMin == -1) {
//            //
//            // Initialzie Time info
//            //
//            restHour = 0;
//            restMin = 0;
//            UserInfo::getInstance()->getRestGiftTime(restHour, restMin);
//        }
        
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
//        if (restHour == -1 && restMin == -1) {
//            //
//            // Initialzie Time info
//            //
//            restHour = 0;
//            restMin = 0;
//            UserInfo::getInstance()->getRestGiftTime(restHour, restMin);
//        }
        
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
//        if (restHour == -1 && restMin == -1) {
//            //
//            // Initialzie Time info
//            //
//            restHour = 0;
//            restMin = 0;
//            UserInfo::getInstance()->getRestGiftTime(restHour, restMin);
//        }
        
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
    std::stringstream discountStream;
    bool priceTag = false;
    bool discountPriceTag = false;
    if (idx == UIStatusTry) {
        int trialSkinIdx = UserInfo::getInstance()->getTrialSkinIndex();
        if (trialSkinIdx != -1) {
            int trialCount = UserInfo::getInstance()->getTrialCount();
            if (trialCount >= 1) {
                //
                addBonusTooltipToMainButton(s, TrialSkinBuyBonus);

                //
                // setBottomLayer
                //
                if (trialCount < TrialAllowNum) {
                    //
                    // 2nd trial
                    //
                    std::string str = LocalizationManager::getInstance()->getLocalizationStringWithReplace("ResultTry", TrialSkinBuyBonus);
                    setBottomLayerWithLocalString(str, mainMenu->getPositionY(), false, TrialSkinBuyBonus);
                }
                else if (trialCount == TrialAllowNum) {
                    //
                    // The last trial
                    //
                    std::string str = LocalizationManager::getInstance()->getLocalizationStringWithReplace("ResultTryLast", TrialSkinBuyBonus);
                    setBottomLayerWithLocalString(str, mainMenu->getPositionY(), true, TrialSkinBuyBonus);
                }
            }
            
            //
            // Bring the price tag from App store
            //
            std::string price = LocalizationManager::getInstance()->getPriceTagByProductName(getNormalSkinName(trialSkinIdx));
            if (price == std::string("0")) {
                stream << LocalizationManager::getInstance()->getLocalizationString(btnStringKeys[3]);
            }
            else {
                stream << price;
                priceTag = true;
            }
        }
        else {
            stream << LocalizationManager::getInstance()->getLocalizationString(btnStringKeys[idx]);
        }
    }
    else if (idx == UIStatusFacebook) {
        //
        // Bottom Layer
        //
        btmLayer = Layer::create();
        btmLayer->setScale(scaleFactor);
        addChild(btmLayer);
        
        std::stringstream sstream;
        sstream << LocalizationManager::getInstance()->getLocalizationString("ResultLikeNotice");
        std::string descStr = sstream.str();

        float fontSize = (UserInfo::getInstance()->currLangType == LanguageType::JAPANESE)? 32:40;

        TTFConfig config(UserInfo::getInstance()->getFontPath(), fontSize);
        Label *lbl = Label::createWithTTF(config, descStr, TextHAlignment::CENTER, visibleSize.width);
        Size lblSize = lbl->getContentSize();
        lbl->setTextColor(Color4B(3, 191, 255, 255));
        float iconPosY = (mainMenu->getPositionY() - resultButtonPosY) * 0.45 + resultButtonPosY - 18;
        lbl->setPosition(Vec2(visibleSizeHalf.width + origin.x, iconPosY));
        btmLayer->addChild(lbl, 1);
        
        //
        stream << LocalizationManager::getInstance()->getLocalizationString(btnStringKeys[idx]);
    }
    else if (idx == UIStatusRemoveAd) {
        //
        // RemoveAds IAP Price tag
        //
        std::string price = LocalizationManager::getInstance()->getPriceTagByProductName(IAP_ID_RemoveAds);
        if (price == std::string("0")) {
            stream << LocalizationManager::getInstance()->getLocalizationString(btnStringKeys[3]);
        }
        else {
            stream << price;
            priceTag = true;
        }
        
        //
        addBonusTooltipToMainButton(s, RemoveAdsRewardGold);
    }
    else if (idx == UIStatusBuyPackage) {
        //
        // Buy Skin Package IAP Price tag
        //
        std::string originalPrice = LocalizationManager::getInstance()->getPriceTagByProductName("Piggybank");
        std::string discountPrice = LocalizationManager::getInstance()->getPriceTagByProductName("Package01");
        if (discountPrice == std::string("0")) {
            stream << LocalizationManager::getInstance()->getLocalizationString(btnStringKeys[3]);
        }
        else {
            discountStream << originalPrice;
            stream << discountPrice;
//            priceTag = true;
            discountPriceTag = true;
        }
        
        //
        addBonusTooltipToMainButton(s, SkinPackageBuyBonus);
        
        //
        std::string str = LocalizationManager::getInstance()->getLocalizationStringWithReplace("ResultTry", TrialSkinBuyBonus);
        setBottomLayerWithLocalString(str, mainMenu->getPositionY(), false, TrialSkinBuyBonus);
    }
    else if (idx == UIStatusBuy || idx == UIStatusBuyGoldenFish) {
        //
        // Bring the price tag from App store
        //
        std::string price = LocalizationManager::getInstance()->getPriceTagByProductName(getNormalSkinName(skinIdxSelected));
        if (price == std::string("0")) {
            stream << LocalizationManager::getInstance()->getLocalizationString(btnStringKeys[3]);
        }
        else {
            stream << price;
            priceTag = true;
        }

        //
        if (idx == UIStatusBuyGoldenFish) {
            addBonusTooltipToMainButton(s, GoldenFishBuyBonus);
            
            //
            std::string str = LocalizationManager::getInstance()->getLocalizationString("GoldfishDesc");
            setBottomLayerWithLocalString(str, mainMenu->getPositionY(), false, GoldenFishBuyBonus);
        }
    }
    else {
        stream << LocalizationManager::getInstance()->getLocalizationString(btnStringKeys[idx]);
        
        if (idx == UIStatusVideo) {
            //
            addBonusTooltipToMainButton(s, VideoRewardGold);
        }
    }

    float fontSize = (UserInfo::getInstance()->currLangType == LanguageType::JAPANESE)? 41:
                        (UserInfo::getInstance()->currLangType == LanguageType::ENGLISH)?46:
                        (UserInfo::getInstance()->currLangType == LanguageType::CHINESE)?52:50;

    std::string labelString = stream.str();
    Label *btnLabel = NULL;

    //
    if (!discountPriceTag) {
        //
        // NOTE: Japanese font doesn't have Yen mark.
        //
        if (priceTag && UserInfo::getInstance()->currLangType == LanguageType::JAPANESE) {
            btnLabel = Label::createWithTTF(labelString, EnglishFontPath, 51);
        }
        else {
            btnLabel = Label::createWithTTF(labelString, UserInfo::getInstance()->getFontPath(), fontSize);
        }
        btnLabel->setTextColor(Color4B(255, 61, 1, 255));
        btnLabel->setPosition(Vec2(s) * 0.5);
        mainBtn->addChild(btnLabel, 1);
        
        //
        mainBtn->setLabelChild(btnLabel);
    }
    else {
        //
        // Button Lables for display of Package Item
        //
        std::string discountString = discountStream.str();
        Label *discountLabel = NULL;
        //
        // NOTE: Japanese font doesn't have Yen mark.
        //
        if (UserInfo::getInstance()->currLangType == LanguageType::JAPANESE) {
            discountLabel = Label::createWithTTF(discountString, EnglishFontPath, 51 * 0.6);
            btnLabel = Label::createWithTTF(labelString, EnglishFontPath, 51);
        }
        else {
            discountLabel = Label::createWithTTF(discountString, UserInfo::getInstance()->getFontPath(), fontSize * 0.6);
            btnLabel = Label::createWithTTF(labelString, UserInfo::getInstance()->getFontPath(), fontSize);
        }
        btnLabel->setTextColor(Color4B(255, 61, 1, 255));
        btnLabel->setPosition(Vec2(s) * 0.5 - Vec2(0, 13));
        mainBtn->addChild(btnLabel, 1);
        
        discountLabel->setTextColor(Color4B(255, 61, 1, 255));
        discountLabel->setAnchorPoint(Vec2(0, 0.5));
        discountLabel->setPosition(Vec2(20, s.height * 0.75));
        mainBtn->addChild(discountLabel, 1);
        
        auto discountLine = Sprite::create("sale_line.png");
        discountLine->setAnchorPoint(Vec2(0, 0.5));
        discountLine->setScaleX(discountLabel->getContentSize().width + 20); // left 10 + rifht 10
        discountLine->setPosition(Vec2(discountLabel->getPositionX() - 10, discountLabel->getPositionY())); // left 10
        mainBtn->addChild(discountLine, 1);

        //
        mainBtn->setLabelChild(btnLabel, discountLabel, discountLine);
    }
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
//    if (trialCount < TrialAllowNum) {
//        //
//        // 2nd trial
//        //
//        sstream << LocalizationManager::getInstance()->getLocalizationStringWithReplace("ResultTry", TrialSkinBuyBonus);
//    }
//    else if (trialCount == TrialAllowNum) {
//        //
//        // The last trial
//        //
//        sstream << LocalizationManager::getInstance()->getLocalizationStringWithReplace("ResultTryLast", TrialSkinBuyBonus);
//    }
    
    //
    std::string descStr = sstream.str();

    float fontSize = 40;
    if (resultType == ResultTypeGoldenFish) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        fontSize = (UserInfo::getInstance()->currLangType == LanguageType::JAPANESE)? 32 * scaleFactor: 35;
#else
        fontSize = (UserInfo::getInstance()->currLangType == LanguageType::JAPANESE)? 32: 35;
#endif
    }
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
    if (idx == UIStatusBuy || idx == UIStatusTry || idx == UIStatusBuyGoldenFish) {
        //
        int trialSkinIdx = UserInfo::getInstance()->getTrialSkinIndex();
        if (trialSkinIdx != -1) {
            skinIdxSelected = trialSkinIdx;
        }
        else if (idx == UIStatusBuyGoldenFish) {
            skinIdxSelected = UserInfo::getInstance()->getGoldenFishSkinIndex();
        }
        else {
            int tmp = UserInfo::getInstance()->getRandomSkinAmongLocked();
            if (tmp == -1) {
                //
                // If user unlocked all...let's call another.
                //
                setAsResultTypeArtwork();
                
                return;
            }

            //
            skinIdxSelected = tmp;
        }

        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString(getFreeshKeyCode(skinIdxSelected));
        std::string nameInfo = stream.str();
        
        Label *nameLabel = Label::createWithTTF(nameInfo, UserInfo::getInstance()->getFontPath(), 44);
        nameLabel->setAnchorPoint(Vec2(0.5, 0));
        nameLabel->setPosition(Vec2(visibleSizeHalf.width + origin.x, basePosYWithOrigin - 65 * scaleFactor));
        nameLabel->setTextColor(Color4B::BLACK);
        Size s = nameLabel->getContentSize();
//        nameLabel->setScale(scaleFactor);
        centerLayer->addChild(nameLabel, 1);
        
        //
        IdleFreeshLayer *freeshSkin = IdleFreeshLayer::getIdleFreeshSkin(skinIdxSelected + 1);
        freeshSkin->setAnchorPoint(Vec2(0.5, 0));
        freeshSkin->setPosition(Vec2(visibleSizeHalf.width - 10, nameLabel->getPositionY() + s.height + 105));
        centerLayer->addChild(freeshSkin, 1);
        freeshSkin->setSelected(true);
        freeshSkin->showParticle(true); // whether it's a goldenfish or not, call this anyway!
        freeshSkin->release();
    }
    else if (idx == UIStatusBuyPackage) {
        //
        // NOTE: It is dangerous to get new packageIndex here,
        // 'coz we already checked the possibilities at resultType decision state.
        // I.E. getRandomPackageIndex() could be -1 here.
        //
        packageIdxSelected = randomPackageIndex;//UserInfo::getInstance()->getRandomPackageIndex();

#ifdef ProductPurchaseCapture
        UserInfo::getInstance()->incPackageIdx();
#endif
        if (packageIdxSelected >= 0) {
            PackageInfo pInfo = packages[packageIdxSelected];
            
            //
            std::stringstream stream;
            stream << std::setprecision(2);
            stream << "Package" << std::setfill('0') << std::setw(2) << packageIdxSelected + 1;
            std::string keyCode = stream.str();
            
            //
            stream.str("");
            stream.clear();
            
            stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
            std::string nameInfo = stream.str();
            
            Label *nameLabel = Label::createWithTTF(nameInfo, UserInfo::getInstance()->getFontPath(), 44);
            nameLabel->setAnchorPoint(Vec2(0.5, 0));
            nameLabel->setPosition(Vec2(visibleSizeHalf.width + origin.x, basePosYWithOrigin - 65 * scaleFactor));
            nameLabel->setTextColor(Color4B::BLACK);
            Size s = nameLabel->getContentSize();
            centerLayer->addChild(nameLabel, 1);
            
            //
            float basePosY = nameLabel->getPositionY() + s.height + 90;
            Vec2 positions[3] = {
                Vec2(visibleSizeHalf.width - 10      , basePosY + 140),
                Vec2(visibleSizeHalf.width - 10 - 115, basePosY),
                Vec2(visibleSizeHalf.width - 10 + 115, basePosY),
            };
            for (int i=0; i<3; i++) {
                IdleFreeshLayer *freeshSkin = IdleFreeshLayer::getIdleFreeshSkin(pInfo.skinIdx[i] + 1);
                freeshSkin->setAnchorPoint(Vec2(0.5, 0));
                freeshSkin->setPosition(positions[i]);
                centerLayer->addChild(freeshSkin, 1);
                freeshSkin->release();
                
                //
                freeshSkin->setScaleForPackage();
            }
        }
    }
    else if (idx == UIStatusRemoveAd) {
        mainIcon = Sprite::createWithSpriteFrameName(getIconFileName(idx));
        mainIcon->setAnchorPoint(Vec2(0.5, 0));
        float iconPosY = (resultOneOrThree == 0)?basePosYWithOrigin - 45 * scaleFactor : basePosYWithOrigin - 73 * scaleFactor;
        mainIcon->setPosition(Vec2(visibleSizeHalf.width + origin.x, iconPosY));
        centerLayer->addChild(mainIcon, 1);
    }
    else {
        mainIcon = Sprite::createWithSpriteFrameName(getIconFileName(idx));
        mainIcon->setAnchorPoint(Vec2(0.5, 0));
        mainIcon->setPosition(Vec2(visibleSizeHalf.width + origin.x, basePosYWithOrigin - 45 * scaleFactor));
        centerLayer->addChild(mainIcon, 1);
        
        if (idx == UIStatusFacebook) {
            Size s = mainIcon->getContentSize();
            
            //
            //
            auto rewardBG = Sprite::createWithSpriteFrameName("speechbubble_reward.png");
            rewardBG->setPosition(mainIcon->getPosition() + Vec2(s.width * 0.5, s.height * 0.8));
            centerLayer->addChild(rewardBG, 1);
            
            //
            //
            int startIndexOfHiddenSkin = UserInfo::getInstance()->getHiddenSkinIndex();
            //
            IdleFreeshLayer *freeshSkin = IdleFreeshLayer::getIdleFreeshSkin(startIndexOfHiddenSkin + 1);
            freeshSkin->setAnchorPoint(Vec2(0.5, 0));
            freeshSkin->setHidden();
            freeshSkin->setSelected(false);
            freeshSkin->setPosition(Vec2(rewardBG->getPosition()));
            centerLayer->addChild(freeshSkin, 1);
            freeshSkin->release();
        }
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

    if (idx == UIStatusDraw || idx == UIStatusGift) {
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
    else if (idx == UIStatusVideo) {
        if (UserInfo::getInstance()->isVideoAvailable()) {
            rightIcon->setColored();
            
            //
            addBonusTooltipToRightBottom(s, VideoRewardGold);
        }
        else {
            rightIcon->setGrayed();
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
    if (resultType == ResultTypeGift2 ||
        resultType == ResultTypeDraw2 ||
        resultType == ResultTypeVideo2 ||
        resultType == ResultTypeBuy2 ||
        resultType == ResultTypeRemoveAD2) {
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
