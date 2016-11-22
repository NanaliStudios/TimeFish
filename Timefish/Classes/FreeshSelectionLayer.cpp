//
//  FreeshSelectionLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 7. 17..
//
//

#include "FreeshSelectionLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

#define SelectableBtnTag    1
#define BuyableBtnTag       2
#define GoldenfishBtnTag    3
#define HiddenBtnTag        4

#define SelectedFreeshTag   99999

bool FreeshSelectionLayer::init()
{
    if ( !ScrollLayerBase::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();

    ownsPriority = false;

    return true;
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void FreeshSelectionLayer::enableKeyInput()
{
    // set keyInput enabled
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(FreeshSelectionLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
#endif


void FreeshSelectionLayer::addItemToScroll(int i)
{
    if (i == 0) {
        //
        // random selection
        //
        freesh[0] = IdleFreeshLayer::create();
        freesh[0]->initRandomSelectionFreesh();
    }
    else if (i > 0){
        //
        // freesh skins
        //
        int skinNum = i;
        freesh[i] = IdleFreeshLayer::getIdleFreeshSkin(skinNum);
        
        //
        //
        //
        int freeshIdx = i - 1;
        bool locked = UserInfo::getInstance()->isLockedSkin(freeshIdx);
        if (locked) {
            if (UserInfo::getInstance()->isThisHiddenSkin(freeshIdx)) {
                freesh[i]->setHidden();
            }
            else {
                freesh[i]->setLocked();
            }
        }
    }

    //
    freesh[i]->setTag(i); // to check selection
    freesh[i]->setLocalZOrder(i);
    freesh[i]->setPosition(Vec2(basePosX + gapX*i, basePosY) + origin);
    freesh[i]->setSelected(false);

    scrollLayer->addChild(freesh[i], 1);
}

void FreeshSelectionLayer::resetScrollItems(int selectedIdx)
{
    if (!UserInfo::getInstance()->isFullSupport()) {
        int diff = endIdx - startIdx + 1;
        if (diff == WindowSize) {
            return;
        }
    }
    
    //
    int halfSize = WindowSize/2;
    int _startIdx = 0;
    int _endIdx = 0;
    if (selectedIdx <= halfSize) {
        _startIdx = 0;
        _endIdx = _startIdx + WindowSize - 1;
    }
    else if ( selectedIdx >= itemSize - 1 - halfSize) {
        _endIdx = itemSize - 1;
        _startIdx = _endIdx - WindowSize + 1;
    }
    else {
        _startIdx = MAX(selectedIdx - halfSize, 0);
        _endIdx = _startIdx + WindowSize - 1;
    }

    //
    if (scrollLayer->getChildrenCount() == 0) {
        for (int i=_startIdx; i<=_endIdx; i++) {
            //
            addItemToScroll(i);
//            log("0.added: %d", i);
        }
    }
    else {
        int diffStartIdx = _startIdx - startIdx;
        if (diffStartIdx > 0) {
            for (int i=startIdx; i<_startIdx; i++) {
                scrollLayer->removeChildByTag(i);
//                log("1.removed: %d", i);
            }
        }
        else if (diffStartIdx < 0){
            for (int i=_startIdx; i<startIdx; i++) {
                addItemToScroll(i);
//                log("1.added: %d", i);
            }
        }
        
        int diffEndInx = _endIdx - endIdx;
        if (diffEndInx > 0) {
            for (int i=endIdx+1; i<=_endIdx; i++) {
                addItemToScroll(i);
//                log("2.added: %d", i);
            }
        }
        else if (diffEndInx < 0){
            for (int i=_endIdx+1; i<=endIdx; i++) {
                scrollLayer->removeChildByTag(i);
//                log("2.removed: %d", i);
            }
        }
    }

    //
    startIdx = _startIdx;
    endIdx = _endIdx;
    
//    log("--------- [%d], (%d, %d) %d", selectedIdx, startIdx, endIdx, (int)scrollLayer->getChildrenCount());
}

void FreeshSelectionLayer::initMainLayer()
{
    //
    WindowSize = FreeshSize + 1;

    //
    initUILayers();
}

//
// Initialize Scrolls
//
void FreeshSelectionLayer::initScrolls()
{
    itemSize = FreeshSize + 1;

    //
    basePosX = 80;
    basePosY = visibleSizeHalf.height;
    gapX = 150 + 7;
    
    //
    // Add items
    //
    resetScrollItems(0);

    //
    scrollMinX = visibleSizeHalf.width - gapX * 0.5;
    scrollMaxX = gapX * (itemSize - 1 - 3) + scrollMinX;
    scrollLength = gapX*itemSize;
    scrollLayer->setPosition(Vec2(scrollMinX, scrollLayer->getPositionY()));
    
    _selectionCallback = CC_CALLBACK_1(FreeshSelectionLayer::selectionCallback, this);
    
    //
    //
    //
    selectedIdx = 0;
    selectionCallback(true); // true means no sound at initization
}

void FreeshSelectionLayer::destroyScrolls()
{
    itemSize = 0;
    startIdx = 0;
    endIdx = 0;
    prevSelectedIdx = -1;
    scrollLayer->removeAllChildren();
    removeChildByTag(SelectedFreeshTag);
}

void FreeshSelectionLayer::initUILayers()
{
    Vec2 btnPos = Vec2(visibleSizeHalf.width, visibleSize.height * 0.2) + origin;
    
    // Button
    {
        btnMenuItem = MenuItemImageButton::create();
        btnMenuItem->setNormalImage(Sprite::createWithSpriteFrameName("button_long_red.png"));
        btnMenuItem->setSelectedImage(Sprite::createWithSpriteFrameName("button_long_red_click.png"));
        btnMenuItem->setCallback(CC_CALLBACK_1(FreeshSelectionLayer::btnCallback, this));
        Size s = btnMenuItem->getContentSize();
        
        //
        btnMenuItem->setTag(SelectableBtnTag); // initially set tag as 1, which means selectable!
        
        auto btn = Menu::create(btnMenuItem, NULL);
        btn->setPosition(btnPos);
        addChild(btn, 1);
        
        //
        std::stringstream stream;
        stream << LocalizationManager::getInstance()->getLocalizationString("Button_Select");
        std::string btnInfo = stream.str();

        btnLabel1 = Label::createWithTTF(btnInfo, UserInfo::getInstance()->getFontPath(), 52);
        btnLabel1->setPosition(Vec2(s.width, s.height) * 0.5);
        btnMenuItem->addChild(btnLabel1, 1);

        //
        // NOTE: btnLabel2 is ONLY for Japanese font which doesn't have Yen mark.
        //
        btnLabel2 = Label::createWithTTF(btnInfo, EnglishFontPath, 52);
        btnLabel2->setPosition(Vec2(s.width, s.height) * 0.5);
        btnLabel2->setVisible(false);
        btnMenuItem->addChild(btnLabel2, 1);

        //
        //
        //
        infoIcon = Sprite::create("icon_info.png");
        infoIcon->setPosition(Vec2(s.width, s.height) * 0.5);
        infoIcon->setVisible(false);
        btnMenuItem->addChild(infoIcon, 1);

        //
        btnMenuItem->setLabelChild(btnLabel1, btnLabel2, infoIcon);
    }

    {
        //
        auto lockBtnMenuItem = MenuItemAutoGray::create("button_long_red.png", "button_long_red_click.png", CC_CALLBACK_1(FreeshSelectionLayer::btnCallback, this));
        Size s = lockBtnMenuItem->getContentSize();
        lockBtnMenuItem->setGrayed();

        //
        lockBtnMenuItem->setTag(HiddenBtnTag);

        lockedBtn = Menu::create(lockBtnMenuItem, NULL);
        lockedBtn->setPosition(btnPos);
        addChild(lockedBtn, 1);

        //
        auto lockIcon = Sprite::createWithSpriteFrameName("icon_locked.png");
        lockIcon->setPosition(Vec2(s.width, s.height) * 0.5);
        lockBtnMenuItem->addChild(lockIcon, 1);

        //
        lockBtnMenuItem->setLabelChild(lockIcon);

        //
        //
        // Hidden skin description Label
        std::string desc = LocalizationManager::getInstance()->getLocalizationString("LockedButton");
        hiddenDescLabel = Label::createWithTTF(desc, UserInfo::getInstance()->getFontPath(), 34);
        hiddenDescLabel->setAnchorPoint(Vec2(0.5, 0));
        hiddenDescLabel->setPosition(Vec2(lockedBtn->getPositionX(), lockedBtn->getPositionY() + s.height * 0.5 + 5));
        hiddenDescLabel->setTextColor(Color4B(255, 7, 7, 255));
        hiddenDescLabel->setVisible(false);
        addChild(hiddenDescLabel, 1);
    }
    
    // Close Button
    {
        auto closeBtnItem = MenuItemSameImage::create("TF_UI_x.png", CC_CALLBACK_0(FreeshSelectionLayer::hideThis, this));
        Size closeBtnSize = closeBtnItem->getContentSize();
        closeBtnItem->setOpacity(150);
        
//        float topPosY = visibleSize.height - closeBtnSize.height;
        
        auto closeBtn = Menu::create(closeBtnItem, NULL);
//        closeBtn->setPosition(Vec2(closeBtnSize.width, topPosY) + origin);
        closeBtn->setPosition(Vec2(closeBtnSize.width * 0.5 + 20, visibleSize.height - closeBtnSize.height* 0.5 - 20) + origin);
        addChild(closeBtn, 1);
    }

    //
    // Shadow
    //
    float orgScale = 1.3;
    auto shadow = Sprite::createWithSpriteFrameName("skin_shadow.png");
    shadow->setPosition(Vec2(visibleSizeHalf.width, visibleSize.height * 0.325) + origin);
    shadow->setScaleY(orgScale);
    addChild(shadow);
    
    auto seq = Sequence::create(ScaleTo::create(0.265, orgScale + 0.1, 1.0),
                                ScaleTo::create(0.265, orgScale, 1.0),
                                NULL);
    shadow->runAction(RepeatForever::create(seq));
    
    //
    // Count Label
    countLabel = Label::createWithTTF("0", NumberFontPath, 45);
    Size s = countLabel->getContentSize();
    countLabel->setPosition(Vec2(visibleSize.width - 20, visibleSize.height - 20) + origin);
    countLabel->setAnchorPoint(Vec2(1, 1));
    countLabel->setTextColor(Color4B(255, 7, 7, 255));
    addChild(countLabel, 1);

    //
    updateCountLabel();

    //
    // Name Label
    nameLabel = Label::createWithTTF("0", UserInfo::getInstance()->getFontPath(), 58);
    nameLabel->setPosition(Vec2(visibleSizeHalf.width, visibleSize.height * 0.8) + origin);
    nameLabel->setTextColor(Color4B(255, 7, 7, 255));
    nameLabel->setVisible(false);
    addChild(nameLabel, 1);
    
    //
    // Music Info Label
    //
    {
        //
        writerInfoLabel = Label::createWithTTF("0", EnglishFontPath, 23);
        Size s = writerInfoLabel->getContentSize();
        writerInfoLabel->setPosition(Vec2(visibleSizeHalf.width, s.height * 1.5) + origin);
        writerInfoLabel->setTextColor(Color4B(255, 7, 7, 255));
        writerInfoLabel->setVisible(false);
        addChild(writerInfoLabel, 1);

        //
        musicInfoLabel = Label::createWithTTF("0", EnglishFontPath, 35);
        musicInfoLabel->setPosition(Vec2(visibleSizeHalf.width, s.height * 2.0) + origin);
        musicInfoLabel->setTextColor(Color4B(255, 7, 7, 255));
        musicInfoLabel->setAnchorPoint(Vec2(0.5, 0));
        musicInfoLabel->setVisible(false);
        addChild(musicInfoLabel, 1);
    }
}

void FreeshSelectionLayer::updateCountLabel()
{
    int unlockedFreesh = UserInfo::getInstance()->getNumberOfUnlockedSkin();
    int totalFreesh = FreeshSize;
    
    //
    // Update Count Label
    //
    std::stringstream stream;
    stream << std::setprecision(2);
    stream << std::setfill('0') << std::setw(2) << unlockedFreesh;
    stream << "/";
    stream << std::setfill('0') << std::setw(2) << totalFreesh;
    std::string countInfo = stream.str();
    
    //
    countLabel->setString(countInfo);
}

void FreeshSelectionLayer::updateNameLabel()
{
    std::stringstream stream;
    if (selectedIdx > 0) {
        stream << getFreeshKeyCode(selectedIdx - 1);
    }
    std::string keyCode = stream.str();
    
    //
    stream.str("");
    stream.clear();
    if (selectedIdx == 0) {
        stream << LocalizationManager::getInstance()->getLocalizationString("Random_Select");
    }
    else if (selectedIdx > 0) {
        stream << LocalizationManager::getInstance()->getLocalizationString(keyCode);
    }
    std::string nameInfo = stream.str();
    
    nameLabel->setVisible(true);
    nameLabel->setString(nameInfo);
}

void FreeshSelectionLayer::updateMusicInfoLabel()
{
    if (selectedIdx < 0) {
        return;
    }

    //
    int songIdx = backgroundMusicIdx[selectedIdx - 1]; // -1 is for randomselection

    //
    if (hasSongWriter(songIdx)) {
        //
        //
        //
        std::stringstream stream;
        stream << "BGMName" << songIdx;
        std::string keyCode = stream.str();
        std::string musicTitle = LocalizationManager::getInstance()->getLocalizationString(keyCode);
        //
        stream.str("");
        stream.clear();
        stream << LocalizationManager::getInstance()->getLocalizationStringWithReplace("BGMName", musicTitle);
        std::string musicTitleInfo = stream.str();
        
        //
        musicInfoLabel->setVisible(true);
        musicInfoLabel->setString(musicTitleInfo);
        
        //
        //
        //
        std::string musicWriter = LocalizationManager::getInstance()->getLocalizationString("SongWriter1");
        std::string musicWriterInfo = LocalizationManager::getInstance()->getLocalizationStringWithReplace("SongWriter", musicWriter);
        writerInfoLabel->setVisible(true);
        writerInfoLabel->setString(musicWriterInfo);

    }
    else {
        musicInfoLabel->setVisible(false);
        writerInfoLabel->setVisible(false);
    }
}

void FreeshSelectionLayer::showHiddenDescLabel(bool flag)
{
    hiddenDescLabel->setVisible(flag);
}

void FreeshSelectionLayer::btnCallback(Ref *pSender)
{
    //
    // NOTE: If user selected a skin while skin-trial status,
    // remove trial skin.
    //
    int trialSkinIdx = UserInfo::getInstance()->getTrialSkinIndex();
    if (trialSkinIdx != -1) {
        UserInfo::getInstance()->initTrialSkinInfo();
        UserInfo::getInstance()->increaseFirstTimeResultTypeIndex();
    }
    
    //
    //
    //
    Node *_node = (Node*)pSender;
    int tag = _node->getTag();
    if (tag == SelectableBtnTag) {
        SoundManager::getInstance()->playSoundEffect(SoundButton, false);
        
        // select
        IdleFreeshLayer *freesh = (IdleFreeshLayer*)getChildByTag(SelectedFreeshTag);

        int selectedSkinNum = -1;
        FreeshType type = freesh->getCurrSkinType();
        int skinNum = freesh->getCurrSkinNumber();
        if (type == FreeshTypeRandom) {
            UserInfo::getInstance()->setRandomSkin(true);
//            UserInfo::getInstance()->setSkinNumForRandom();

//            int skinIdx = UserInfo::getInstance()->getRandomSkinAmongUnlocked();
//            skinNum = skinIdx + 1;
//            type = UserInfo::getInstance()->getSkinTypeBySkinNumber(skinNum);
        }
        else {
            UserInfo::getInstance()->setSkinNumWithTypeSet(skinNum);
            UserInfo::getInstance()->setRandomSkin(false);

            selectedSkinNum = skinNum;
        }

        TapjoyX::getInstance()->logEventInUIFlow("SelectSkin", selectedSkinNum); // -1 is random

        //
        this->setEnableLayer(false);
        
        this->setTag(1);

        if (closeCallback) {
            closeCallback(this);
        }
    }
    else if (tag == BuyableBtnTag) {
//        SoundManager::getInstance()->playSoundEffect(SoundButton, false);
        
        // select
        IdleFreeshLayer *freesh = (IdleFreeshLayer*)getChildByTag(SelectedFreeshTag);
        int selectedSkinNum = freesh->getCurrSkinNumber();

        TapjoyX::getInstance()->logEventInUIFlow("BuySkin", selectedSkinNum);

        //
        // TODO: temp, unlock here
        //
        
        int idx = freesh->getCurrSkinNumber() - 1;
        if (purchaseCallback) {
            purchaseCallback(idx);
        }
//        UserInfo::getInstance()->unlockSkinAt(idx);
//        
//        //
//        this->setEnableLayer(false);
//        
//        this->setTag(1);
//        
//        if (closeCallback) {
//            closeCallback(this);
//        }
    }
    else if (tag == HiddenBtnTag) {
        SoundManager::getInstance()->playSoundEffect(SoundDisabledButton, false);        
    }
    else if (tag == GoldenfishBtnTag) {
        SoundManager::getInstance()->playSoundEffect(SoundButton, false);
        
        if (goldenfishCallback) {
            goldenfishCallback();
        }
    }
}

//
// Called from close button or backbutton(Android)
// closeCallback should not play button sound
//
void FreeshSelectionLayer::hideThis()
{
    SoundManager::getInstance()->playSoundEffect(SoundButton, false);
    
    this->setEnableLayer(false);
    
    this->setTag(0);
    
    if (closeCallback) {
        closeCallback(this);
    }
}

void FreeshSelectionLayer::selectionCallback(bool noSound)
{
    //    log("%d, %d", prevSelectedIdx, selectedIdx);
    if (prevSelectedIdx >= 0 && prevSelectedIdx < FreeshSize + 1 && prevSelectedIdx != selectedIdx) {
        //
        //
        //
        IdleFreeshLayer *freesh = (IdleFreeshLayer*)getChildByTag(SelectedFreeshTag);
        
        if (!freesh) {
            return;
        }
        
        //
        // re-attach
        //
        freesh->retain();
        freesh->removeFromParent();
        freesh->setTag(prevSelectedIdx); // to check selection
        freesh->setLocalZOrder(prevSelectedIdx);
        freesh->setPosition(Vec2(basePosX + gapX*prevSelectedIdx, visibleSizeHalf.height) + origin);
        freesh->setSelected(false);
        scrollLayer->addChild(freesh);
        freesh->release();
        
        prevSelectedIdx = -1;
    }
    
    //
    if (selectedIdx >= 0 && selectedIdx < FreeshSize + 1 && prevSelectedIdx != selectedIdx) {
        
        resetScrollItems(selectedIdx);

        //
        //
        //
        IdleFreeshLayer *freesh = (IdleFreeshLayer*)scrollLayer->getChildByTag(selectedIdx);
        
        if (!freesh) {
            return;
        }
        
        //
        // Change parent
        //
        freesh->retain();
        freesh->removeFromParent();
        freesh->setLocalZOrder(FreeshSize + 1);
        freesh->setPosition(Vec2(visibleSizeHalf) + origin);
        freesh->setSelected(true);
        freesh->setTag(SelectedFreeshTag);
        addChild(freesh);
        freesh->release();
        
        //
        prevSelectedIdx = selectedIdx;
        
        if (!noSound) {
            //
            SoundManager::getInstance()->playSoundEffect(SoundScroll, false);
        }
        
        //
        // Update Name Label
        //
        if (selectedIdx >= 0 && selectedIdx < FreeshSize + 1) {

            //
            updateNameLabel();
            updateMusicInfoLabel();
            
            //
            // button
            //
            int skinIdx = 0;
            bool locked = false;
            bool hidden = false;
            if (selectedIdx > 0) {
                skinIdx = selectedIdx - 1;
                locked = UserInfo::getInstance()->isLockedSkin(skinIdx);
                hidden = UserInfo::getInstance()->isThisHiddenSkin(skinIdx);
            }

            if (locked) {
                if (hidden) {
                    btnMenuItem->setVisible(false);
                    lockedBtn->setVisible(true);

                    //
                    showHiddenDescLabel(true);
                }
                else {
                    if (skinIdx == UserInfo::getInstance()->getGoldenFishSkinIndex()) {
                        //
                        // Goldenfish info button
                        //
                        btnMenuItem->setVisible(true);
                        btnMenuItem->setTag(GoldenfishBtnTag);

                        lockedBtn->setVisible(false);
                        btnLabel1->setVisible(false);
                        btnLabel2->setVisible(false);
                        infoIcon->setVisible(true);
                    }
                    else {
                        //
                        // Other than Goldenfish:
                        //
                        // NOTE: At Zplay version, not able to buy.
                        //
//                        std::string price = LocalizationManager::getInstance()->getPriceTagByProductName(getNormalSkinName(skinIdx));
//                        if (price == std::string("0")) {
                            //
                            // Something Wrong: Lock the button
                            //
                            btnMenuItem->setVisible(false);
                            lockedBtn->setVisible(true);
                            btnLabel1->setVisible(false);
                            btnLabel2->setVisible(false);
//                        }
//                        else {
//                            btnMenuItem->setVisible(true);
//                            lockedBtn->setVisible(false);
//                            
//                            btnMenuItem->setTag(BuyableBtnTag);
//                            btnMenuItem->setEnabled(true);
//                            
//                            //
//                            // NOTE: Japanese font doesn't have Yen mark.
//                            //
//                            if (UserInfo::getInstance()->currLangType == LanguageType::JAPANESE) {
//                                btnLabel1->setVisible(false);
//                                btnLabel2->setString(price);
//                                btnLabel2->setVisible(true);
//                            }
//                            else {
//                                btnLabel1->setVisible(true);
//                                btnLabel1->setString(price);
//                                btnLabel2->setVisible(false);
//                            }
//                        }

                        //
                        infoIcon->setVisible(false);
                    }

                    //
                    showHiddenDescLabel(false);
                }
            }
            else {
                btnMenuItem->setVisible(true);
                lockedBtn->setVisible(false);
                
                btnMenuItem->setTag(SelectableBtnTag);
                btnMenuItem->setEnabled(true);

                std::stringstream stream;
                stream << LocalizationManager::getInstance()->getLocalizationString("Button_Select");
                std::string btnInfo = stream.str();

                btnLabel1->setVisible(true);
                btnLabel1->setString(btnInfo);
                btnLabel2->setVisible(false);
                
                //
                showHiddenDescLabel(false);
                
                //
                infoIcon->setVisible(false);
            }
        }
        else {
            nameLabel->setVisible(false);
        }
    }
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void FreeshSelectionLayer::onKeyReleased( EventKeyboard::KeyCode keycode, Event *event )
{
//    log("MainUIStatus: FreeshSelectionLayer::onKeyReleased, %d", ownsTouchPriority());
    if (ownsTouchPriority() && keycode == EventKeyboard::KeyCode::KEY_BACK) {
        event->stopPropagation();
        hideThis();
    }
}
#endif
