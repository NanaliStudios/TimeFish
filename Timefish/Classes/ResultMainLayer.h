//
//  ResultMainLayer.h
//  Timefish
//
//  Created by midchow on 2015. 7. 19..
//
//

#ifndef __Timefish__ResultMainLayer__
#define __Timefish__ResultMainLayer__

#include "cocos2d.h"
#include "Global.h"

#include "UserInfo.h"
#include "IdleFreeshLayer.h"

#include "StoreKitX.h"

#include "GraySprite.h"

#include "SkeletonHolder.h"

#include "LocalizationManager.h"

#include "MenuItemSameImage.h"
#include "MenuItemImageButton.h"

#include "BonusTooltipLayer.h"

USING_NS_CC;

#define UIStatusGift            0
#define UIStatusDraw            1
#define UIStatusVideo           2
#define UIStatusRemoveAd        3
#define UIStatusRate            4
#define UIStatusFacebook        5
#define UIStatusShare           6
#define UIStatusPinterest       7
// center only
#define UIStatusBuy             8
#define UIStatusTry             9
#define UIStatusBuyPackage      10
#define UIStatusBuyGoldenFish   11

class ResultMainLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(ResultMainLayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;

    void setButtonCallbacks(const std::function<void()>& callback1,
                            const std::function<void()>& callback2,
                            const std::function<void()>& callback3,
                            const std::function<void()>& callback4,
                            const std::function<void()>& callback5,
                            const std::function<void()>& callback6,
                            const std::function<void()>& callback7,
                            const std::function<void()>& callback8,
                            const std::function<void()>& callback9,
                            const std::function<void()>& callback10,
                            const std::function<void()>& callback11) {
        buttonCallbacks[0] = callback1;
        buttonCallbacks[1] = callback2;
        buttonCallbacks[2] = callback3;
        buttonCallbacks[3] = callback4;
        buttonCallbacks[4] = callback5;
        buttonCallbacks[5] = callback6;
        buttonCallbacks[6] = callback7;
        buttonCallbacks[7] = callback8;
        buttonCallbacks[8] = callback9;
        buttonCallbacks[9] = callback10;
        buttonCallbacks[10] = callback11;
    }
    
    void setWorldLayerUpdater(const std::function<void()>& callback) {
        worldLayerUpdater = callback;
    }

    void initMainLayer(float posY);
    
    void updateMainLayer(bool withGivingCoins = true);
    
    int getSkinIdxSelected() {
        return skinIdxSelected;
    }
    int getPackageIdxSelected() {
        return packageIdxSelected;
    }
    
    void setLayerVisible(bool flag) {
        this->setVisible(flag);
        enableTouch(flag);
    }

    int getResultOneOrThree() {
        return resultOneOrThree;
    }
    
    void setBasePosY(float basePosY) {
        basePosYWithOrigin = basePosY;
        if (resultOneOrThree == 1) {
            basePosYWithOrigin += 20;
        }
    }

    void setMainResultType();
    ResultType getMainResultType() {
        return resultType;
    }
    void setBottomLayerWithLocalString(std::string str, float basePosY, bool withWarning, int amount);
    void addBonusTooltipToMainButton(cocos2d::Size s, int amount);
    void addBonusTooltipToRightBottom(cocos2d::Size s, int amount);

    //
    void startIntroAction();
private:
    ResultType resultType;

    Layer *centerLayer;
    Layer *btmLayer;

    //
    // centerButtonStatus : 0
    //    mainBtn       -> O
    //    leftBtnLabel1 -> X
    //    leftBtnLabel2 -> X
    //
    // centerButtonStatus : 1
    //    mainBtn       -> X
    //    leftBtnLabel1 -> O
    //    leftBtnLabel2 -> X
    //
    // centerButtonStatus : 2
    //    mainBtn       -> X
    //    leftBtnLabel1 -> O
    //    leftBtnLabel2 -> O
    //
    int centerButtonStatus;
    //
    // leftButtonStatus: 1
    //    leftBtnLabel1 -> O
    //    leftBtnLabel2 -> X
    // leftButtonStatus: 2
    //    leftBtnLabel1 -> O
    //    leftBtnLabel2 -> O
    //
    int leftButtonStatus;
    //
    // rightButtonStatus: 1
    //    rightBtnLabel1 -> O
    //    rightBtnLabel2 -> X
    // rightButtonStatus: 2
    //    rightBtnLabel1 -> O
    //    rightBtnLabel2 -> O
    //
    int rightButtonStatus;

    float bottomIconScale, bottomIconSelelectScale;
    
    float scaleFactor;

    // Result UI type: one-button or three-button
    int resultOneOrThree;

    float basePosYWithOrigin;
    float resultButtonPosY;
    float btmButtonPosY;

    int skinIdxSelected;
    int packageIdxSelected;
    
    bool hasShownVideo;

    int listSize;
    Node* actionList[6];

    std::function<void()> worldLayerUpdater;
    std::function<void()> buttonCallbacks[11];
    std::function<void(bool)> uiInitializer[ResultTypeMax];
    
    //
//    Menu *mainBtn;
//    MenuItemImage *mainBtn;
    MenuItemImageButton *mainBtn;

    Label *centerLbl1, *centerLbl2;
    GraySprite *leftIcon, *rightIcon;
    Label *leftBtnLabel1, *leftBtnLabel2;
    Label *rightBtnLabel1, *rightBtnLabel2;
    Sprite *mainIcon;

    //
    // main button : 0
    // left button : 1
    // right button : 2
    //
    int selectedButtonIndex;

    int selectedBottomButton;
    
    int randomPackageIndex;

    //
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    void enableTouch(bool flag);
    
    //
    void setMainButton(int idx, float basePosY);
    void setCenterUI(int idx);

    //
    void setLeftButtonLabel();
    void setLeftUI(int idx);

    //
    void setRightButtonLabel();
    void setRightUI(int idx);

    //
    void showRestTimeInfo(int restHour = -1, int restMin = -1);
    void showRestTimeInfoToLeft(int restHour = -1, int restMin = -1);
    void showRestTimeInfoToRight(int restHour = -1, int restMin = -1);

    //
    void showRestCoinInfo(int toGoCoins);
    void showRestCoinInfoToLeft();
    void showRestCoinInfoToRight();

    //
    void showEarnedCoinInfo();
    void showEarnedCoinInfoToRight();
    void showResultRemoveAdsInfo(bool withoutGivingCoins);
    void showResultPackageBuy(bool withGivingCoins);
    
    //
    void btnCallback(Ref* pSender);
    void selectedLeft();
    void selectedRight();
    bool ableToGetGift() {
        int restHour = 0;
        int restMin = 0;
        UserInfo::getInstance()->getRestGiftTime(restHour, restMin);
        
        if (restHour > 0 || restMin > 0) {
            return false;
        }
        
        return true;
    }

    //
    void setMainResultTypeForFirstTimeUser(int idx);
    void setMainResultTypeForExpUser();
//    void _setMainResultTypeForExpUser();

    void restartIntroAction();

    //
    //
    //
    void setAsResultTypeGift1(bool withCenter = true)
    {
        centerButtonStatus = 0;
        setCenterUI(UIStatusGift);
    }
    void setAsResultTypeGift2(bool withCenter = true)
    {
        if (withCenter) {
            setCenterUI(UIStatusGift);
        }
        setLeftUI(UIStatusDraw);
        setRightUI(UIStatusVideo);
    }
    void setAsResultTypeDraw1(bool withCenter = true)
    {
        setCenterUI(UIStatusDraw);
    }
    void setAsResultTypeDraw2(bool withCenter = true)
    {
        setCenterUI(UIStatusDraw);
        setLeftUI(UIStatusGift);
        setRightUI(UIStatusVideo);
    }
    void setAsResultTypeVideo1(bool withCenter = true)
    {
        setCenterUI(UIStatusVideo);
    }
    void setAsResultTypeVideo2(bool withCenter = true)
    {
        if (withCenter) {
            setCenterUI(UIStatusVideo);
        }
        setLeftUI(UIStatusDraw);
        setRightUI(UIStatusGift);
    }
    void setAsResultTypeBuy1(bool withCenter = true)
    {
        setCenterUI(UIStatusBuy);
    }
    void setAsResultTypeBuy2(bool withCenter = true)
    {
        setCenterUI(UIStatusBuy);
        setLeftUI(UIStatusDraw);
        setRightUI(UIStatusVideo);
    }
    void setAsResultTypeRemoveAd1(bool withCenter = true)
    {
        setCenterUI(UIStatusRemoveAd);
    }
    void setAsResultTypeRemoveAd2(bool withCenter = true)
    {
        setCenterUI(UIStatusRemoveAd);
//        setLeftUI(UIStatusGift);
//        setRightUI(UIStatusDraw);
        setLeftUI(UIStatusDraw);
        setRightUI(UIStatusVideo);
    }
    void setAsResultTypeTry(bool withCenter = true)
    {
        setCenterUI(UIStatusTry);
    }
    void setAsResultTypeRate(bool withCenter = true)
    {
        setCenterUI(UIStatusRate);
    }
    void setAsResultTypeFacebook(bool withCenter = true)
    {
        setCenterUI(UIStatusFacebook);
    }
    void setAsResultTypeShare(bool withCenter = true)
    {
        setCenterUI(UIStatusShare);
    }
    void setAsResultTypeArtwork(bool withCenter = true)
    {
        setCenterUI(UIStatusPinterest);
    }
    void setAsResultTypeBuyPackage(bool withCenter = true)
    {
        setCenterUI(UIStatusBuyPackage);
    }
    void setAsResultTypeGoldenFish(bool withCenter = true)
    {
        setCenterUI(UIStatusBuyGoldenFish);
    }
    
    //
    bool isPrevTypeGift() {
        ResultType prevResultType = UserInfo::getInstance()->getPrevResultType();
        return ((prevResultType == ResultTypeGift1) || (prevResultType == ResultTypeGift2));
    }
    bool isPrevTypeDraw() {
        ResultType prevResultType = UserInfo::getInstance()->getPrevResultType();
        return ((prevResultType == ResultTypeDraw1) || (prevResultType == ResultTypeDraw2));
    }
    bool isPrevTypeVideo() {
        ResultType prevResultType = UserInfo::getInstance()->getPrevResultType();
        return ((prevResultType == ResultTypeVideo1) || (prevResultType == ResultTypeVideo2));
    }
    bool isPrevTypeBuy() {
        ResultType prevResultType = UserInfo::getInstance()->getPrevResultType();
        return ((prevResultType == ResultTypeBuy1) || (prevResultType == ResultTypeBuy2));
    }
    bool isPrevTypeRemove() {
        ResultType prevResultType = UserInfo::getInstance()->getPrevResultType();
        return ((prevResultType == ResultTypeRemoveAD1) || (prevResultType == ResultTypeRemoveAD2));
    }
    
    //
#define ElasticDuration 0.55
    void setMainButtonAction() {
        auto elastic = EaseElasticOut::create(ScaleTo::create(ElasticDuration, scaleFactor));
        auto seq = Sequence::create(ScaleTo::create(0, scaleFactor*0.8), elastic, DelayTime::create(2.0), NULL);
        mainBtn->stopAllActions();
        mainBtn->runAction(RepeatForever::create(seq));
    }
    void setLeftIconAction() {
        if (!leftIcon->getGrayed()) {
            leftIcon->setScale(bottomIconScale);
            auto elastic = EaseElasticOut::create(ScaleTo::create(ElasticDuration, bottomIconScale));
            auto seq = Sequence::create(DelayTime::create(0.6), ScaleTo::create(0, bottomIconScale*0.8), elastic, DelayTime::create(2.0), NULL);
            leftIcon->stopAllActions();
            leftIcon->runAction(RepeatForever::create(seq));
        }
    }
    void setRightIconAction() {
        if (!rightIcon->getGrayed()) {
            rightIcon->setScale(bottomIconScale);
            auto elastic = EaseElasticOut::create(ScaleTo::create(ElasticDuration, bottomIconScale));
            auto seq = Sequence::create(DelayTime::create(1.2), ScaleTo::create(0, bottomIconScale*0.8), elastic, DelayTime::create(2.0), NULL);
            rightIcon->stopAllActions();
            rightIcon->runAction(RepeatForever::create(seq));
        }
    }
};

#endif /* defined(__Timefish__ResultMainLayer__) */
