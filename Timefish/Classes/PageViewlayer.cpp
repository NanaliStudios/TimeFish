//
//  PageViewlayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 3. 19..
//
//

#include "PageViewlayer.h"

#include <iostream>
#include <iomanip>
#include <string>

#define NewTag          100
#define LockedTag       1000
#define SelectionTag    2000

bool PageViewlayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();

    // inits
    currPage = 0;
    pageNumbers = 4;
    colsPage = 3;
    rowsPage = 3;
    itemsPerPage = colsPage*rowsPage;

    ownsBackButtonPriority = false;

    updateCallback = NULL;

    return true;
}

void PageViewlayer::initLayersWithPosY(float posY)
{
    initMainLayer();
    initMovingLayer(posY);

    //
    setCurrPage(0);
}

void PageViewlayer::initMainLayer()
{
    //
    // Main Layer
    //
    mainLayer = Layer::create();
    addChild(mainLayer, 0);

    // Dots
    auto dot = Sprite::createWithSpriteFrameName("TF_timeposter_comma.png");
    float space = dot->getContentSize().width * 1.5;
    float basePosX = visibleSizeHalf.width - (float)(pageNumbers - 1)*0.5 * space;
    for (int i=0; i<pageNumbers; i++) {
        auto dot = Sprite::createWithSpriteFrameName("TF_timeposter_comma.png");
        dot->setPosition(Vec2(basePosX + space*i, visibleSize.height * 0.14) + origin);
        dot->setOpacity(125);
        dot->setScale(0.85);
        dot->setTag(i);
        mainLayer->addChild(dot);
    }
    
    // Count Label
    countLabel = Label::createWithTTF("0", NumberFontPath, 45);
    Size s = countLabel->getContentSize();
    countLabel->setPosition(Vec2(visibleSize.width - 20, visibleSize.height - 15) + origin);
    countLabel->setAnchorPoint(Vec2(1, 1));
    countLabel->setTextColor(Color4B(255, 7, 7, 255));
    addChild(countLabel, 1);
    
    //
    // GameCenter Connect for Achievements
    //
    // NOTE: At Zplay version, hide gamecenter related buttons.
    //
    {
//        auto menuItem = MenuItemImageButton::create();
//        menuItem->setNormalImage(Sprite::createWithSpriteFrameName("button_short_white.png"));
//        menuItem->setSelectedImage(Sprite::createWithSpriteFrameName("button_short_white_click.png"));
//        menuItem->setCallback(CC_CALLBACK_0(PageViewlayer::showGameCenterAchievement, this));
//
//        Size s = menuItem->getContentSize();
//        menuItem->setPosition(Vec2(visibleSize.width - s.width * 0.5 - 20, s.height* 0.5 + 20) + origin);
//
//        auto gcBtn = Menu::create(menuItem, NULL);
//        gcBtn->setPosition(Vec2::ZERO);
//        addChild(gcBtn, 10);
//        
//        //
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//        auto icon = Sprite::create("gp/icon_gamecenter_google.png");
//#else
//        auto icon = Sprite::createWithSpriteFrameName("icon_gamecenter.png");
//#endif
//        icon->setPosition(s * 0.5);
//        menuItem->addChild(icon);
//
//        //
//        menuItem->setLabelChild(icon);

        //
        // NOTE: AchievementType27 카운팅이 잘못되는 버그가 있어서 여기서 체크해준다.
        //
        if (UserInfo::getInstance()->getDoneAchievementNumbers() >=
            UserInfo::getInstance()->getAchievementPassBase(AchievementType27)) {
            UserInfo::getInstance()->setAchievementInfoWithType(AchievementType27);
        }
    }
}

void PageViewlayer::initMovingLayer(float posY)
{
    //
    // Moving Layer
    //
    movingLayer = Layer::create();
    movingLayer->setPosition(Vec2::ZERO + origin);
    addChild(movingLayer, 10);

    basePosY = posY;

    //
    updatePages();
}

void PageViewlayer::updatePages()
{
    //
    movingLayer->removeAllChildren();

    const char *fileName[] = {
        "TF_timeposter_box-deco1.png",
        "TF_timeposter_box-deco2.png",
        "TF_timeposter_box-deco3.png"
    };

    int unlockedAchievements = 0;
    int totalAchievements = 0;
    
    //
    // pages
    //
    pageWidth = visibleSize.width;
    int selectedPosterIdx = UserInfo::getInstance()->getSelectedPosterType() - 1;
    for (int i=0; i<pageNumbers; i++) {
        for (int j=0; j<itemsPerPage; j++) {
            //
            Vec2 pos = getAchievemenetIconPos(i*itemsPerPage + j);

            int posterType = i*itemsPerPage + j+1;
            
            // Selection Marker
            if (selectedPosterIdx>=0 && posterType == selectedPosterIdx + 1) {
                setSelectionMarker(pos);
            }
            
            // Out Box
            auto outBox = Sprite::createWithSpriteFrameName("TF_timeposter_box.png");
            outBox->setPosition(pos);
            movingLayer->addChild(outBox, 1);

            // In Box
            auto inBox = Sprite::createWithSpriteFrameName("TF_timeposter_box.png");

            //
            // NOTE: 업적이 언락돼있으면 Icon을 보여준다.
            //
            if (UserInfo::getInstance()->getAchievementInfoWithType((AchievementType)(1<<(posterType-1)))) {
                //
                unlockAchievement(outBox, inBox, pos, (AchievementType)posterType);

                //
                unlockedAchievements++;
            }
            else {
                //
                outBox->setColor(Color3B(134, 124, 103));
                outBox->setTag(LockedTag + posterType);

                //
                inBox->setPosition(pos);
                inBox->setScale(0.86);
                inBox->setColor(Color3B(77, 65, 52));
                movingLayer->addChild(inBox, 2);
                
                //
                int rowNum = j/3;
                auto icon = Sprite::createWithSpriteFrameName(fileName[rowNum]);
                icon->setPosition(pos);
                movingLayer->addChild(icon, 3);
            }

            //
            //
            //
            totalAchievements++;
            if (totalAchievements >= NumOfAchievement) {
                break;
            }
        }
    }
    
    //
    // Update Count Label
    //
    std::stringstream stream;
//    stream << std::setprecision(2);
    stream << std::setfill('0') << std::setw(2) << unlockedAchievements;
    stream << "/";
    stream << std::setfill('0') << std::setw(2) << totalAchievements;
    std::string countInfo = stream.str();
    
    countLabel->setString(countInfo);
}

Vec2 PageViewlayer::getAchievemenetIconPos(int posterIdx)
{
    auto box = Sprite::createWithSpriteFrameName("TF_timeposter_box.png");
    Size s = box->getContentSize();
    
    //
    float spaceX = s.width * 1.25;
    float spaceY = s.height * 1.25;
    Vec2 posDelta[] = {
        Vec2(-spaceX, 0),
        Vec2(      0, 0),
        Vec2( spaceX, 0),
        Vec2(-spaceX, -spaceY),
        Vec2(      0, -spaceY),
        Vec2( spaceX, -spaceY),
        Vec2(-spaceX, -spaceY * 2),
        Vec2(      0, -spaceY * 2),
        Vec2( spaceX, -spaceY * 2),
    };

    basePosY = visibleSizeHalf.height + s.height + spaceY;
    
    // inits
    int i = (int)(posterIdx/itemsPerPage);
    int j = (int)(posterIdx%itemsPerPage);

    //
    // base item's position is at TOP among other 9 items!!!
    //
    Vec2 basePos = Vec2(visibleSizeHalf.width + origin.x + pageWidth*i, basePosY - s.height);
    Vec2 pos = basePos + posDelta[j];

    return pos;
}

void PageViewlayer::unlockAchievement(Sprite *outBox, Sprite *inBox, Vec2 pos, AchievementType posterType)
{
    int posterIdx = posterType - 1;

    //
    outBox->setColor(Color3B(255, 217, 103));
    outBox->setTag(SelectionTag + posterType);
    
    //
    Size inBoxSize = inBox->getContentSize()*0.86;
    bool needABackground = false;
    
    //
    std::stringstream stream;
    stream << "TF_gameposter_" << std::setfill('0') << std::setw(2);
    Sprite *posterIcon = NULL;
    // file name variation
    if (posterType == 11 || posterType == 12) {
        stream << posterType <<".jpg";
        std::string fileName = stream.str();
        posterIcon = Sprite::create(fileName.c_str());
    }
//    else if (posterType == 10) {
    else if (posterType == 10 || posterType == 29 || posterType == 30 || posterType == 31) {
        stream << posterType <<".png";
        std::string fileName = stream.str();
        posterIcon = Sprite::create(fileName.c_str());
    }
    else {
        needABackground = true;
        
        stream << posterType <<".png";
        std::string fileName = stream.str();
        posterIcon = Sprite::createWithSpriteFrameName(fileName.c_str());
    }

    Size clippingSize = inBoxSize;//inBox->getContentSize();
    float _scale = 0.86;

    // use inbox as a background, if needed.
    if (needABackground) {
        inBox->setPosition(pos);
        inBox->setScale(_scale);
        inBox->setColor(posterBgColor[posterIdx]);
        movingLayer->addChild(inBox, 2);
        
        clippingSize = clippingSize * _scale;
    }

    //
    auto clipper = ClippingNode::create();
    clipper->setAlphaThreshold(0.05);
    clipper->setContentSize(clippingSize);
    clipper->setInverted(false);
    clipper->setPosition(pos + Vec2(-clippingSize.width*0.5, -clippingSize.height*0.5));
    movingLayer->addChild(clipper, 3);
    
    
    auto mask = Sprite::createWithSpriteFrameName("TF_timeposter_box.png");
    mask->setScale(_scale);
    mask->setPosition(Vec2(clippingSize) * 0.5);
    clipper->setStencil(mask);
    
    
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
    posterIcon->setPosition(Vec2(clippingSize.width*0.5, clippingSize.height*0.5) + deltaPos);
    
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
    clipper->addChild(posterIcon);
    
//    // scale variation
//    if (posterType == 16 || posterType == 17 || posterType == 18) {
//        posterIcon->setScale(0.7);
//    }
//    else if (posterType == 21) {
//        posterIcon->setScale(0.5);
//    }
//    else {
//        posterIcon->setScale(0.53);
//    }
//    
//    //
//    auto clipper = ClippingRectangleNode::create();
//    clipper->setClippingRegion(Rect(-inBoxSize.width*0.5, -inBoxSize.height*0.5, inBoxSize.width+1, inBoxSize.height+1));
//    clipper->setPosition(pos);
//    movingLayer->addChild(clipper, 3);
//    
//    posterIcon->setPosition(Vec2(clipper->getContentSize()) * 0.5 + posterIconPosDelta[posterIdx]);
//    clipper->addChild(posterIcon);
    
    if (UserInfo::getInstance()->getNewAchievement[posterIdx]) {
        auto newMark = Sprite::createWithSpriteFrameName("TF_timeposter_new.png");
        Size markSize = newMark->getContentSize();
        newMark->setPosition(pos + Vec2(inBoxSize.width - markSize.width, inBoxSize.height*0.5));
        movingLayer->addChild(newMark, 5);
        
        newMark->setTag(NewTag + posterIdx);

        auto seq = Sequence::create(
                                    ScaleTo::create(0.2, 1.0),
                                    ScaleTo::create(0.2, 0.8),
                                    NULL);
        newMark->runAction(RepeatForever::create(seq));
        newMark->setVisible(UserInfo::getInstance()->haveNewAchievement());
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void PageViewlayer::setCurrPage(int newPage)
{
    // previous dot
    Sprite *prevDot = (Sprite*)mainLayer->getChildByTag(currPage);
    if (prevDot) {
        prevDot->setColor(Color3B::WHITE);
        prevDot->setOpacity(125);
    }

    //
    currPage = newPage;
    Sprite *currDot = (Sprite*)mainLayer->getChildByTag(newPage);
    if (currDot) {
        currDot->setColor(Color3B::RED);
        currDot->setOpacity(255);
    }
}

void PageViewlayer::enableLayerTouch()
{
    // setTouchEnabled
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(PageViewlayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(PageViewlayer::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(PageViewlayer::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(PageViewlayer::onTouchCancelled, this);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void PageViewlayer::disableLayerTouch()
{
    // setTouchDisabled
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->pauseEventListenersForTarget(this);
}

bool PageViewlayer::onTouchBegan(Touch *touch, Event *event)
{
    //
    if (tooltip) {
        tooltip->removeThis(0);
        tooltip = NULL;
    }

    Vec2 touchPos = touch->getLocation();
    
    if (basePosY < touchPos.y) {
        return false;
    }

    prevPos = touchPos;
    beginPos = prevPos;
    isMoving = false;

    movingLayer->stopAllActions();

    //
    //
    //
    btnSelected = false;
    for( const auto &child: movingLayer->getChildren()) {
        if (child->getBoundingBox().containsPoint(touchPos)) {
            btnSelected = true;
            break;
        }
    }

    return true;
}

void PageViewlayer::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = touch->getLocation();
    Vec2 beginDiff = touchPos - beginPos;
    Vec2 movingDiff = touchPos - prevPos;
    prevPos = touchPos;

    //
    //
    //
    if (btnSelected) {
        if (movingDiff.length() < 1) {
            isMoving = false;
            return;
        }
        else {
            btnSelected = false;
        }
    }

    //
    isMoving = true;

    if (movingDiff.x<0) {
        // to prevent rapid change of direction
        if (movingLeft == false && fabsf(beginDiff.x) > visibleSize.width * 0.5) {
            movingLeft = false;
        }
        else {
            movingLeft = true;
        }

        if (isThisLastPage()) {
            movingDiff *= 0.3;
        }
    }
    else {
        // to prevent rapid change of direction
        if (movingLeft == true && fabsf(beginDiff.x) > visibleSize.width * 0.5) {
            movingLeft = true;
        }
        else {
            movingLeft = false;
        }

        if (isThisFirstPage()) {
            movingDiff *= 0.3;
        }
    }

    //
    movingLayer->setPosition(movingLayer->getPosition() + Vec2(movingDiff.x, 0)); // only get x component
}

void PageViewlayer::onTouchEnded(Touch *touch, Event *event)
{
    Vec2 touchPos = touch->getLocation();
    
    if (isMoving) {
        //
        // Scroll is Done!!!
        //
        Vec2 diff = touchPos - beginPos;
        int targetPage = 0;
        if (fabsf(diff.x) < visibleSize.width * 0.1) {
            // to prevent tiny move
            targetPage = currPage;
        }
        else {
            if (movingLeft) {
                targetPage = MIN(currPage + 1, pageNumbers - 1);
            }
            else{
                targetPage = MAX(currPage - 1, 0);
            }
        }
        
        Vec2 moveToPos = Vec2(-pageWidth * targetPage, 0) + origin;
        
        //
        movingLayer->runAction(Sequence::create(
                                                MoveTo::create(0.25, moveToPos),
                                                CallFunc::create( CC_CALLBACK_0(PageViewlayer::finishPageMoving,this)),
                                                NULL));
    }
    else {
        //
        // Button Selection!!!
        //
        Mat4 worldToNodeTM = movingLayer->getWorldToNodeTransform();
        Vec3 localPos = Vec3(touchPos.x, touchPos.y, 0);
        worldToNodeTM.transformPoint(&localPos);
        touchPos.x = localPos.x;
        touchPos.y = localPos.y;

        int idx = 0;
        for( const auto &child: movingLayer->getChildren()) {
            int tag = child->getTag();
            if (child->getBoundingBox().containsPoint(touchPos)) {
                //
                // if selected the unlocked achievement
                //
                if (tag > SelectionTag) {
                    //
                    FlurryX::getInstance()->logEvent("ViewPoster");

                    //
                    SoundManager::getInstance()->playSoundEffect(SoundButton, false);

                    int posterType = tag - SelectionTag;

                    //
                    // Open poster Layer
                    //
                    PosterLayer *poster = PosterLayer::create();
                    poster->initMainLayerWithType(posterType);
                    poster->initUILayer();
                    poster->enableLayerTouch();
                    poster->setCloseCallback(CC_CALLBACK_1(PageViewlayer::posterCloseCallback, this));
                    addChild(poster, 100);
                    
                    ownsBackButtonPriority = true;
                    
                    //
                    // Remove New Mark
                    //
                    int posterIdx = posterType - 1;
                    UserInfo::getInstance()->getNewAchievement[posterIdx] = false;
                    Node *newMark = movingLayer->getChildByTag(NewTag + posterIdx);
                    if (newMark) {
                        newMark->setVisible(false);
                    }
                    
                    if (updateCallback) {
                        updateCallback(this);
                    }
                    
                    //
                    TapjoyX::getInstance()->logEventInUIFlow("Poster", posterType);
                }
                //
                // if selected the locked achievement
                //
                else if (tag > LockedTag) {
                    //
                    FlurryX::getInstance()->logEvent("AchievementLocked");

                    //
                    SoundManager::getInstance()->playSoundEffect(SoundDisabledButton, false);

                    int posterType = tag - LockedTag;

                    tooltip = TooltipLayer::create();
                    Vec2 pos = Vec2(child->getPositionX(), child->getPositionY() + child->getContentSize().height * 0.85);
                    tooltip->initWithAchievementType(pos, child->getContentSize().width, posterType - 1);
                    tooltip->setCloseCallback(CC_CALLBACK_0(PageViewlayer::tooltipCloseCallback, this));
//                    tooltip->enableLayerTouch();
                    movingLayer->addChild(tooltip, 100);

                    //
                    TapjoyX::getInstance()->logEventInUIFlow("AchievementDesc", posterType);
                }
            }

            idx++;
        }
    }
}

void PageViewlayer::tooltipCloseCallback()
{
    tooltip = NULL;
}

void PageViewlayer::posterCloseCallback(Ref *pSender)
{
    int posterIdx = UserInfo::getInstance()->getSelectedPosterType() - 1;

    Vec2 pos = getAchievemenetIconPos(posterIdx);
    if (selectionMarker) {
        selectionMarker->setPosition(pos);
    }
    else {
        setSelectionMarker(pos);
    }
}

void PageViewlayer::setSelectionMarker(Vec2 pos)
{
    selectionMarker = Sprite::createWithSpriteFrameName("TF_timeposter_box.png");
    selectionMarker->setPosition(pos);
    selectionMarker->setScale(1.09);
    selectionMarker->setColor(Color3B(255, 242, 204));
    movingLayer->addChild(selectionMarker, 0);
}

void PageViewlayer::finishPageMoving()
{
    Vec2 pos = movingLayer->getPosition();
    //
    // NOTE with iphone6/6+ pageWidth has slighlty different value with BaseScreenWidth.
    // so, Need to add +1 for page calculation.
    //
    int newPage = (fabsf(pos.x) + 1)/pageWidth;
    
    setCurrPage(newPage);
}

void PageViewlayer::onTouchCancelled(Touch *touch, Event *event)
{
}

void PageViewlayer::showGameCenterAchievement()
{
    //
    FlurryX::getInstance()->logEvent("GameCenterAchievement");
    //
    TapjoyX::getInstance()->logEventInUIFlow("GCAchievement");

    SoundManager::getInstance()->playSoundEffect(SoundButton, false);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //
#else
    //
    if (GameCenterX::getInstance()->isAuthenticated(true)) {
        GameCenterX::getInstance()->showAchievementList();
    }
    else {
//        log("Game Center Unavailable: Please sign in first.");
    }
#endif
}
