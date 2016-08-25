//
//  Popup.cpp
//  Timefish
//
//  Created by midchow on 2015. 1. 22..
//
//

#include "Popup.h"


bool PopupBase::init()
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

void PopupBase::setEnbleTouch(bool flag)
{
    if (flag) {
        // setTouchEnabled
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true);
        touchListener->onTouchBegan = CC_CALLBACK_2(PopupBase::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(PopupBase::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(PopupBase::onTouchEnded, this);
        touchListener->onTouchCancelled = CC_CALLBACK_2(PopupBase::onTouchCancelled, this);
        dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
        // set keyInput enabled
        auto listener = EventListenerKeyboard::create();
        listener->onKeyReleased = CC_CALLBACK_2(PopupBase::onKeyReleased, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        ownsPriority = true;
    }
    else {
        // setTouchDisabled
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->removeEventListenersForTarget(this);
        
        ownsPriority = false;
    }
}

void PopupBase::setBackground(float _height)
{
    mainLayer = Layer::create();
    addChild(mainLayer, 1);

    topPosY = visibleSizeHalf.height + _height * 0.5;
    btmPosY = visibleSizeHalf.height - _height * 0.5;

    // bg top
    auto top = Sprite::create("popup_round.png");
    top->setFlippedY(true);
    top->setAnchorPoint(Vec2(0.5, 0));
    top->setPosition(Vec2(visibleSizeHalf.width, topPosY) + origin);
    mainLayer->addChild(top, 1);
    
    // bg middle
    auto mid = Sprite::create("popup_middle.png");
    mid->setAnchorPoint(Vec2(0.5, 1));
    mid->setPosition(Vec2(visibleSizeHalf.width, topPosY) + origin);
    mid->setScaleY(_height);
    mainLayer->addChild(mid, 1);
    
    // bg btm
    auto btm = Sprite::create("popup_round.png");
    btm->setAnchorPoint(Vec2(0.5, 1));
    btm->setPosition(Vec2(visibleSizeHalf.width, btmPosY) + origin);
    mainLayer->addChild(btm, 1);
    
    //
    popupWidth = top->getContentSize().width;
}

void PopupBase::removeSelf(Ref* pSender)
{
    //
//    FlurryX::getInstance()->logEvent("PopupNothing");
    
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);
    
    removeFromParent();
}

bool PopupBase::onTouchBegan(Touch *touch, Event *event)
{
    return true;
}

void PopupBase::onTouchMoved(Touch *touch, Event *event)
{
}

void PopupBase::onTouchEnded(Touch *touch, Event *event)
{
}

void PopupBase::onTouchCancelled(Touch *touch, Event *event)
{
}

void PopupBase::onKeyReleased( EventKeyboard::KeyCode keycode, Event *event )
{
//    log("MainUIStatus: PopupBase::onKeyReleased: %d", ownsPriority);
    if (ownsPriority && keycode == EventKeyboard::KeyCode::KEY_BACK) {
        event->stopPropagation();
        removeSelf(NULL);
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool HiddenskinPopup::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initBackground();
    
    return true;
}

void HiddenskinPopup::initBackground()
{
    //
    setBackground(500);

    //
    // buttons
    //
    float posX[2] = {
        -1.1,
        1.1
    };
    const char* fileNames[] = {
        "button_icon_share.png",
        "icon_play.png"
    };

    auto callback1 = [this](Ref* pSender) {
        //
        SoundManager::getInstance()->playSoundEffect(SoundButton, false);

        if (shareCallback) {
            shareCallback();
        }
    };
    auto callback2 = [this](Ref* pSender) {
        //
        SoundManager::getInstance()->playSoundEffect(SoundButton, false);
        
        //
        UserInfo::getInstance()->setSkinNumWithTypeSet(_skinNum);
        UserInfo::getInstance()->setRandomSkin(false);

        //
        if (nextCallback) {
            nextCallback();
        }
    };
    std::function<void(Ref* pSender)> buttonCallbacks[2] = {
        callback1,
        callback2
//        CC_CALLBACK_1(HiddenskinPopup::removeSelf, this)
    };

    //
    for (int i=0; i<2; i++) {
        auto menuItem = MenuItemImageButton::create();
        menuItem->setNormalImage(Sprite::createWithSpriteFrameName("button_middle_white.png"));
        menuItem->setSelectedImage(Sprite::createWithSpriteFrameName("button_middle_white_click.png"));
        menuItem->setCallback(buttonCallbacks[i]);

        Size s = menuItem->getContentSize();
        
        auto mainBtn = Menu::create(menuItem, NULL);
        mainBtn->setPosition(Vec2(visibleSizeHalf.width + posX[i] * s.width * 0.5, btmPosY + s.height * 0.5) + origin);
        addChild(mainBtn, 1);
        
        //
        // add icon
        auto icon = Sprite::createWithSpriteFrameName(fileNames[i]);
        icon->setPosition(Vec2(s) * 0.5);
        menuItem->addChild(icon, 1);

        //
        menuItem->setLabelChild(icon);
    }
}

void HiddenskinPopup::setSkinInfo(int skinIdx)
{
    _skinNum = skinIdx + 1;
    
    //
    std::stringstream stream;
    stream << getFreeshKeyCode(skinIdx);
    std::string keyCode = stream.str();
    
    //
    stream.str("");
    stream.clear();
    stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
    std::string nameInfo = stream.str();
    
    Label *nameLabel = Label::createWithTTF(nameInfo, UserInfo::getInstance()->getFontPath(), 50);
    nameLabel->setAnchorPoint(Vec2(0.5, 1));
    nameLabel->setPosition(Vec2(visibleSizeHalf.width, topPosY + 12) + origin);
    nameLabel->setTextColor(Color4B(255, 61, 1, 255));
    Size s = nameLabel->getContentSize();
    addChild(nameLabel, 1);

    //
    IdleFreeshLayer *freeshSkin = IdleFreeshLayer::getIdleFreeshSkin(skinIdx + 1);
    freeshSkin->setAnchorPoint(Vec2(0.5, 0));
    freeshSkin->setPosition(Vec2(visibleSizeHalf.width, visibleSizeHalf.height) + origin);
    addChild(freeshSkin, 10);
    freeshSkin->release();
}

void HiddenskinPopup::startIntro()
{
    this->setVisible(true);

    setEnbleTouch(true);

    //
    runPopupEffect();
}

void HiddenskinPopup::runPopupEffect()
{
    //
    // popup effect
    //
    this->setScale(0.5);
    this->runAction(EaseElasticOut::create(ScaleTo::create(0.55, 1.0)));
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool GoldenfishInfoPopup::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initBackground();
    
    //
    setEnbleTouch(true);
    
    return true;
}

void GoldenfishInfoPopup::initBackground()
{
    //
    setBackground(500);
    
    //
    goldenfishSkinIdx = UserInfo::getInstance()->getGoldenFishSkinIndex();
    
    //
    // button
    //
    auto callback = [this](Ref* pSender) {
        //
        SoundManager::getInstance()->playSoundEffect(SoundButton, false);
        
        if (purchaseCallback) {
            purchaseCallback(goldenfishSkinIdx);
        }

        if (closeCallback) {
            closeCallback();
        }

        removeFromParent();
    };

    //
    {
        auto menuItem = MenuItemImageButton::create();
        menuItem->setNormalImage(Sprite::createWithSpriteFrameName("button_center_white.png"));
        menuItem->setSelectedImage(Sprite::createWithSpriteFrameName("button_center_white_click.png"));
        menuItem->setCallback(callback);
        
        Size s = menuItem->getContentSize();
        
        auto mainBtn = Menu::create(menuItem, NULL);
        mainBtn->setPosition(Vec2(visibleSizeHalf.width, btmPosY + s.height * 0.5) + origin);
        addChild(mainBtn, 1);

        //
        // Bring the price tag from App store
        //
        std::stringstream stream;
        std::string price = LocalizationManager::getInstance()->getPriceTagByProductName(getNormalSkinName(goldenfishSkinIdx));
        if (price == std::string("0")) {
            stream << LocalizationManager::getInstance()->getLocalizationString(btnStringKeys[3]);
        }
        else {
            stream << price;
        }

        std::string nameInfo = stream.str();
        Label *btnLabel = NULL;
        float fontSize = (UserInfo::getInstance()->currLangType == LanguageType::JAPANESE)? 41:
        (UserInfo::getInstance()->currLangType == LanguageType::ENGLISH)?51:
        (UserInfo::getInstance()->currLangType == LanguageType::CHINESE)?52:55;
        //
        // NOTE: Japanese font doesn't have Yen mark.
        //
        if (UserInfo::getInstance()->currLangType == LanguageType::JAPANESE) {
            btnLabel = Label::createWithTTF(nameInfo, EnglishFontPath, 51 * 0.8);
        }
        else {
            btnLabel = Label::createWithTTF(nameInfo, UserInfo::getInstance()->getFontPath(), fontSize * 0.8);
        }
        btnLabel->setTextColor(Color4B(255, 61, 1, 255));
        btnLabel->setPosition(Vec2(s) * 0.5);
        menuItem->addChild(btnLabel, 1);

        //
        menuItem->setLabelChild(btnLabel);
        
        //
        //
        auto bonus = BonusTooltipLayer::create();
        bonus->initWithAmount(GoldenFishBuyBonus);
        bonus->setPosition(Vec2(s.width, s.height));
        menuItem->addChild(bonus, 10);
        
        //
        std::stringstream sstream;
        sstream << LocalizationManager::getInstance()->getLocalizationString("GoldfishDesc");
        std::string descStr = sstream.str();
        
        TTFConfig config(UserInfo::getInstance()->getFontPath(), 32);
        Label *lbl = Label::createWithTTF(config, descStr, TextHAlignment::CENTER, visibleSize.width);
        Size lblSize = lbl->getContentSize();
        lbl->setTextColor(Color4B::BLACK);
        lbl->setAnchorPoint(Vec2(0.5, 1));
        lbl->setPosition(Vec2(visibleSizeHalf.width, visibleSizeHalf.height - 20) + origin);
        addChild(lbl, 1);
    }

    //
    setSkinInfo();

    //
    // Close button
    //
    {
        auto closeBtnItem = MenuItemImageButton::create();
        closeBtnItem->setNormalImage(Sprite::createWithSpriteFrameName("button_short_white.png"));
        closeBtnItem->setSelectedImage(Sprite::createWithSpriteFrameName("button_short_white_click.png"));
        closeBtnItem->setCallback(CC_CALLBACK_1(GoldenfishInfoPopup::removeSelf, this));
        
        closeBtnItem->setScale(0.7);

        Size s = closeBtnItem->getContentSize();
        closeBtnItem->setPosition(Vec2(visibleSizeHalf.width + popupWidth*0.5 - 25, topPosY) + origin);

        auto closeBtn = Menu::create(closeBtnItem, NULL);
        closeBtn->setPosition(Vec2::ZERO);
        addChild(closeBtn, 1);
        
        // add icon
        auto icon = Sprite::createWithSpriteFrameName("TF_UI_x.png");
        icon->setPosition(Vec2(s) * 0.5);
        icon->setColor(Color3B(255, 4, 4));
        closeBtnItem->addChild(icon, 1);
        
        //
        closeBtnItem->setLabelChild(icon);
    }
}

void GoldenfishInfoPopup::setSkinInfo()
{
    //
    std::stringstream stream;
    stream << getFreeshKeyCode(goldenfishSkinIdx);
    std::string keyCode = stream.str();
    
    //
    stream.str("");
    stream.clear();
    stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
    std::string nameInfo = stream.str();
    
    Label *nameLabel = Label::createWithTTF(nameInfo, UserInfo::getInstance()->getFontPath(), 50);
    nameLabel->setAnchorPoint(Vec2(0.5, 1));
    nameLabel->setPosition(Vec2(visibleSizeHalf.width, topPosY + 12) + origin);
    nameLabel->setTextColor(Color4B(255, 61, 1, 255));
    Size s = nameLabel->getContentSize();
    addChild(nameLabel, 1);
    
    //
    IdleFreeshLayer *freeshSkin = IdleFreeshLayer::getIdleFreeshSkin(goldenfishSkinIdx + 1);
    freeshSkin->setAnchorPoint(Vec2(0.5, 0));
    freeshSkin->setPosition(Vec2(visibleSizeHalf.width - 20, visibleSizeHalf.height + 90) + origin);
    freeshSkin->showParticle(true);
    addChild(freeshSkin, 10);
    freeshSkin->release();
}

void GoldenfishInfoPopup::removeSelf(Ref* pSender)
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

    if (closeCallback) {
        closeCallback();
    }

    removeFromParent();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool BestScorePopup::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initBackground();
    
    //
    setEnbleTouch(true);

    return true;
}

void BestScorePopup::initBackground()
{
    //
    auto blackLayer = LayerColor::create(Color4B(0,0,0,160));
    addChild(blackLayer, 1);

    //
    setBackground(400);

    //
    // buttons
    //
    float posX[2] = {
        -1.1,
        1.1
    };
    const char* fileNames[] = {
        "button_icon_share.png",
        "icon_play.png"
    };
    
    auto callback1 = [this](Ref* pSender) {
        //
        SoundManager::getInstance()->playSoundEffect(SoundButton, false);
        
        if (shareCallback) {
            shareCallback();
        }
    };
    auto callback2 = [this](Ref* pSender) {
        //
        SoundManager::getInstance()->playSoundEffect(SoundButton, false);

        //
        removeSelf(this);
    };
    std::function<void(Ref* pSender)> buttonCallbacks[2] = {
        callback1,
        callback2
//        CC_CALLBACK_1(HiddenskinPopup::removeSelf, this)
    };
    
    //
    for (int i=0; i<2; i++) {
        auto menuItem = MenuItemImageButton::create();
        menuItem->setNormalImage(Sprite::createWithSpriteFrameName("button_middle_white.png"));
        menuItem->setSelectedImage(Sprite::createWithSpriteFrameName("button_middle_white_click.png"));
        menuItem->setCallback(buttonCallbacks[i]);
        
        Size s = menuItem->getContentSize();
        
        auto mainBtn = Menu::create(menuItem, NULL);
        mainBtn->setPosition(Vec2(visibleSizeHalf.width + posX[i] * s.width * 0.5, btmPosY + s.height * 0.5) + origin);
        mainLayer->addChild(mainBtn, 1);
        
        //
        // add icon
        auto icon = Sprite::createWithSpriteFrameName(fileNames[i]);
        icon->setPosition(Vec2(s) * 0.5);
        menuItem->addChild(icon, 1);
        
        //
        btnPosTopY = btmPosY + s.height;
        
        //
        menuItem->setLabelChild(icon);
    }
}

void BestScorePopup::setScoreInfo(int score)
{
    float titlePosY = topPosY - 30;
    //
    Label *titleLabel = Label::createWithTTF("Best Score!", EnglishFontPath, 60);
    titleLabel->setPosition(Vec2(visibleSizeHalf.width, titlePosY) + origin);
    titleLabel->setTextColor(Color4B(255, 61, 1, 255));
    Size s = titleLabel->getContentSize();
    mainLayer->addChild(titleLabel, 1);

    titlePosY -= s.height * 0.5;

    //
    std::stringstream stream;
//    stream << std::setfill('0') << std::setw(6) << score;
    stream << score;
    std::string scoreInfo = stream.str();
    auto bestScoreLabel = Label::createWithTTF(scoreInfo, NumberFontPath, 80);
    bestScoreLabel->setPosition(Vec2(visibleSizeHalf.width, (titlePosY - btnPosTopY) * 0.5 + btnPosTopY) + origin);
    mainLayer->addChild(bestScoreLabel, 1);

    //
    Color4F colors[3] = {
        Color4F(RGBValue(0), RGBValue(138), RGBValue(255), 1),
        Color4F(RGBValue(251), RGBValue(217), RGBValue(60), 1),
        Color4F(RGBValue(255), RGBValue(27), RGBValue(0), 1),
    };
    for (int i=0; i<3; i++) {
        ParticleSystemQuad *partyPtcl = ParticleSystemQuad::create("particle/VendingMachinePopper_particle/VendingMachinePopper_particle.plist");
        partyPtcl->setPosition(Vec2(visibleSizeHalf.width, visibleSize.height * 1.2) + origin);
        partyPtcl->setStartColor(colors[i]);
        partyPtcl->setEndColor(colors[i]);
        mainLayer->addChild(partyPtcl, 100);
    }
}

void BestScorePopup::runPopupEffect()
{
    //
    // popup effect
    //
    mainLayer->setVisible(false);
    mainLayer->setScale(0.5);
    auto elastic = EaseElasticOut::create(ScaleTo::create(0.55, 1.0));
    auto seq = Sequence::create(DelayTime::create(0.1), Show::create(), elastic, NULL);
    mainLayer->runAction(seq);
}
