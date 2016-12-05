//
//  PlayScene.cpp
//  Timefish
//
//  Created by midchow on 2015. 1. 13..
//
//

#include "PlayScene.h"

#include <iostream>
#include <iomanip>
#include <string>

#define DefaultMaxJumpTime 0.5
#define JumpTimeMinLimit 0.3
#define MaxJumpTimeDecRate 0.9

#define VelocityFactor 40
#define VelocityDecRate 0.0 // after crash halt for a second

#define VelocityIncRate 0.015 // increase from zero

// NOTE: At ver. 1.07 : change from 15% to 10%
#define MaxVelocityIncRate 1.10

#define FeverVelocityIncRate 4.0
#define FeverVelocityDecRate 0.875

#define MaxVelocity 80
#define JumpFactor 200  // jumping height

#define DefaultBallAnglePos 100
#define GetByMonsterMinDist 7
#define CloseEnoughToMonsterDist 45

#define IntroActionTime 0.9
#define OutroActionTime 0.65

#define CircleSegments 100

#ifdef AUTOJUMP_TEST
#define TimonVelocityIncRate 0.0175 // increase from zero
#define TEST2 0.99f
#define TEST3 1.02
#else
#define TimonVelocityIncRate VelocityIncRate // increase from zero
#define TEST2 1.0f
#define TEST3 1.0f
#endif

USING_NS_CC;

static int firstLaps[] = {1,1,2,2,1};

Scene* PlayScene::createScene()
{
    auto scene = Scene::create();
    auto layer = PlayScene::create();
    
    scene->addChild(layer, 1, 1);
    
    return scene;
}

bool PlayScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    //
    FlurryX::getInstance()->logEvent("Main");
    
    testBoostType = 0;
    
    //
    // preload sounds
    //
//    SoundManager::getInstance()->preloadSounds();

    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();

//    return true; // 75.1 MB

    //
    // Banner for Achievements...
    //
    banner = AchievementBannerLayer::create();
    banner->setCloseCallback(CC_CALLBACK_0(PlayScene::showNextAchievementsBanner, this));
    banner->setPosition(origin);
    addChild(banner, 5000); // most top
//    for (int i=0; i<NumOfAchievement; i++) {
//        UserInfo::getInstance()->bannerQueue->pushBanner(i);
//    }

//    return true; // 77.1 MB

    //
    // call this before setBackgroundLayer / UILayer inits
    // 'coz freesh skin decides colors!
    //
    setWorldLayer();

//    return true; // 78.5 MB

    //
    setBackgroundLayer();
    
//    return true; // 79.1 MB

    //
    uiLayer = UILayer::create();
    addChild(uiLayer, 100);
    uiLayer->setReloadSceneCallback(CC_CALLBACK_1(PlayScene::showPostInterstitial, this));
    uiLayer->setPauseSceneCallback(CC_CALLBACK_1(PlayScene::pauseScene, this));
    uiLayer->setResumeSceneCallback(CC_CALLBACK_1(PlayScene::resumeScene, this));
    uiLayer->setFreeshSkinCallback(CC_CALLBACK_0(PlayScene::reloadPlayScene, this));
    uiLayer->setUIChangeCallback(CC_CALLBACK_1(PlayScene::setBackgroundLayerTo, this));
    uiLayer->setStartGameCallback(CC_CALLBACK_0(PlayScene::reloadPlayScene, this));
    uiLayer->setColorInformation(colorIdx);
    //
    // check for hidden skin unlock popups...
    //
    if (uiLayer->checkHiddenSkin() == 0) {
        startGameByEnablingTouchEvent();
    }

//    return true; // 92.1 MB

    //
    setGameStatus(StatusReady);

    currLap = 0;
    setNextCapsuleLap();

    //
    currCombo = 1;
    maxComboCount = 0;
    uiLayer->updateComboCountLabel(currCombo);

    //
    UserInfo::getInstance()->initCurrScore();
    ballLap = 0;
    runVelocity = VelocityFactor * VelocityDecRate;
    maxVelocity = VelocityFactor;
    feverVelocity = VelocityFactor;
    maxJumpTime = DefaultMaxJumpTime;
    timonMaxVelocity = 0;
    coinsEarned = 0;
    crashed = false;
    freeshDied = false;

    //
    UserInfo::getInstance()->initJumpCount();
    UserInfo::getInstance()->initCapsuleCount();

    //
    freeshShieldStat = NormalStatus;
    setFreeshToNormalStatus();

    bannerIsOn = false;

    // initial freesh angle position
    angle = -M_PI_2 * 0.25;
    
    //
    // preload ads
    //
    ChartboostX::getInstance()->setDelegate(this);
    ChartboostX::getInstance()->cacheInterstitial("Main Menu");

    //
    // Tutorial!
    //
    if (!UserInfo::getInstance()->haveSeenTutorial) {
        uiLayer->showTutorial(NULL);
    }

    //
    scheduleOnce(CC_SCHEDULE_SELECTOR(PlayScene::showAchievementsBanners), 2);
//    showAchievementsBanners();

    //
    TapjoyX::getInstance()->logEventInUIFlow("Main");

    //
    SoundManager::getInstance()->startAmbienceSound();

    return true;
}

//
// Set current Freesh's skin
// Return current Freesh's skin number
//
int PlayScene::setFreeshSkin()
{
    int currSkin = UserInfo::getInstance()->getSkinNum();

    //
    // Check trial skin
    //
    int trialSkinIdx = UserInfo::getInstance()->getTrialSkinIndex();
    bool randomSkin = UserInfo::getInstance()->getRandomSkin();
    if (trialSkinIdx != -1) {
        int trialCount = UserInfo::getInstance()->getTrialCount();
        if (trialCount < TrialAllowNum) {
            //
            // increase skin trial count
            //
            UserInfo::getInstance()->setTrialCount(trialCount + 1);
            currSkin = trialSkinIdx + 1;
        }
        else {
            //
            // with last trial, initialize.
            //
            UserInfo::getInstance()->initTrialSkinInfo();
            trialSkinIdx = -1;
        }
    }

    //
    // check random skin, if not trial
    //
    if (trialSkinIdx == -1 && randomSkin) { // random selection
        int _skinNum = UserInfo::getInstance()->getRandomSkinAmongUnlockedWithoutPrevious();
        UserInfo::getInstance()->setSkinNumWithTypeSet(_skinNum);

        currSkin = _skinNum;
    }
    
    //
    // Error Check
    //
    if (currSkin <= 0 || currSkin > FreeshSize) {
        currSkin = 1;
    }
    else if(trialSkinIdx == -1 && UserInfo::getInstance()->isLockedSkin(currSkin - 1)) {
        currSkin = 1;
    }

    //
    // skin
    //
    FreeshType skinType = UserInfo::getInstance()->getSkinTypeBySkinNumber(currSkin);
    character->initFreeshWithSkinType(skinType, currSkin);
//    character->setSkin(skinType, currSkin);

    return currSkin;
}

void PlayScene::showAchievementsBanners(float dt)
{
    if (!bannerIsOn) {
        int idx = UserInfo::getInstance()->bannerQueue->popBanner();
//        log("showAchievementsBanners: %d", idx);
        if (idx > -1) {
            bannerIsOn = true;
            banner->setBannerWith(idx);
        }
    }
}

void PlayScene::showNextAchievementsBanner()
{
    bannerIsOn = false;
    showAchievementsBanners(0);
}

void PlayScene::startGameByEnablingTouchEvent()
{
    //
    // setTouchEnabled
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(PlayScene::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(PlayScene::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(PlayScene::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(PlayScene::onTouchCancelled, this);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void PlayScene::setBackgroundSoundVolume(float dt)
{
    if (status == StatusIntro) {
        bgVolume += dt;
        if (bgVolume >= 1) {
            bgVolume = 1;
            unschedule(CC_SCHEDULE_SELECTOR(PlayScene::setBackgroundSoundVolume));
        }

        SoundManager::getInstance()->startBackgroundMusic(colorIdx, bgVolume);
    }
    else if (status == StatusGameOver) {
        bgVolume -= dt*0.85; // more slowly
        if (bgVolume <= 0) {
            bgVolume = 0;
            unschedule(CC_SCHEDULE_SELECTOR(PlayScene::setBackgroundSoundVolume));

            //
            // NOTE: need to initialize backgroundMusicID!!!
            //
            SoundManager::getInstance()->stopBackgroundMusic();
        }
        else {
            SoundManager::getInstance()->startBackgroundMusic(colorIdx, bgVolume);
        }
    }
}

void PlayScene::startIntro(float dt)
{
    //
    if (UserInfo::getInstance()->isRecordingEnabled()) {
        VideoShareX::getInstance()->startRecording();
    }
    
    //
    int skinNum = colorIdx;
    if (UserInfo::getInstance()->getHiddenSkinIndex() <= colorIdx) {
        skinNum = colorIdx + HiddenSkinStartIndexForTapJoy;
    }
    TapjoyX::getInstance()->logEventInUIFlow("InGame", skinNum);

    sTime = time(0); // get start time

    uiLayer->hideStartUI();
//    achLayer->hideLayer();

    //
    SoundManager::getInstance()->playSoundEffect(SoundGameStart, false);

    gameContinuedWithVideo = false;

    jumpCount = 0;
    goldenJumpCount = rand()%10 + 5;
    prevDiffLength = 0;

    //
    bgVolume = 0;
    schedule(CC_SCHEDULE_SELECTOR(PlayScene::setBackgroundSoundVolume));

    //
#ifdef DEVELOPMENT
    auto seq = Sequence::create(
                                CallFunc::create( CC_CALLBACK_0(PlayScene::startEscaping,this)),
                                NULL);
    worldLayer->runAction(seq);
#else

    // TODO: fix following Hard coded position!!!
//    Vec2 moveToPos = Vec2(-279.867493,817.277222); // -197.328125.782.409912
    Vec2 moveToPos = Vec2(-197.328125, 782.409912);

    //
    // NOTE: At ver. 1.07 : change from 0.8 to 0.7
    //
//    float scale = (uiLayer->resultMain->getResultOneOrThree() == 0)? 0.3f: 0.26f;
    auto action = Spawn::create(
                                EaseSineOut::create(MoveTo::create(IntroActionTime, moveToPos)),
                                EaseSineOut::create(ScaleTo::create(IntroActionTime, 0.7)),
                                NULL);
    auto seq = Sequence::create(
                                action,
                                CallFunc::create( CC_CALLBACK_0(PlayScene::startEscaping,this)),
                                DelayTime::create(0.5),
                                NULL);
    worldLayer->runAction(seq);
#endif
}

void PlayScene::startEscaping()
{
    Vec2 newPos = Vec2(cosf(angle), sinf(angle)) * fishRadius;
    Vec2 diff = - newPos;
    
    //
    // set object rotation BEFORE POSITION!!!
    //
    float radian = diff.getAngle();
    float degree = get12OclockDegreeFrom3OclockRadian(radian);

    character->setFishRotation(degree);
    character->setPosition(newPos);// + origin);
    character->setVisible(true);
    character->setOpenAniCallback(CC_CALLBACK_1(PlayScene::startChasing, this));
    std::function<void()> callback = [this]() {
        //
        if(freeshTrail) freeshTrail->setVisible(true);
    };
    character->setFinishStartAniCallback(callback);
    character->setAnimation(AniStart);
}

void PlayScene::startChasing(Ref* pSender)
{
    //
    SoundManager::getInstance()->playSoundEffect(SoundLanding1, false);

    showJumpParticle();

    //
    bgLayer->setTimonStart();

    startMainLoop();
}

void PlayScene::startMainLoop()
{
    //
    setGameStatus(StatusNone);

    uiLayer->showInGameUI();

    schedule(CC_SCHEDULE_SELECTOR(PlayScene::moveCharacter));
}

void PlayScene::setBackgroundLayer()
{
    //
    // Base Background
    //
    background = LayerColor::create(Color4B::WHITE); // initialize background with white
    addChild(background);

    //
    // TODO:
    // Set background flow depending on device fps or performances
    //
    if (UserInfo::getInstance()->isFlowEnabled()) {
        flowTop = SpriteFlow::create("offset_bg_top.png");
        flowTop->setPosition(Vec2(visibleSizeHalf.width, visibleSizeHalf.height) + origin);
        Vec2 velocity = Vec2(-1.66666666667, 0); // 100/60
        flowTop->setDelta(velocity);
        flowTop->setOpacity(25);
        addChild(flowTop, 0);
        
        //
        flowBottom = SpriteFlow::create("offset_bg_bottom.png");
        flowBottom->setPosition(Vec2(visibleSizeHalf.width, visibleSizeHalf.height) + origin);
        velocity = Vec2(-1.33333333333, 0); // 80/60
        flowBottom->setDelta(velocity);
        flowBottom->setOpacity(25);
        addChild(flowBottom, 0);
    }
}

void PlayScene::setBackgroundLayerTo(int stat)
{
    if (stat == 0) {
        BGColorSet bgColorSet = bgColorSets[colorIdx];

        worldLayer->setVisible(true);
        background->setColor(getBGColorSet(colorIdx));

        //
        if (flowTop) {
            flowTop->setOpacity(flowBGOpacityTable[(int)bgColorSet][0]);
            flowBottom->setOpacity(flowBGOpacityTable[(int)bgColorSet][1]);
        }
        else {
//            stayTop->setOpacity(flowBGOpacityTable[(int)bgColorSet][0]);
//            stayBottom->setOpacity(flowBGOpacityTable[(int)bgColorSet][1]);
        }
    }
    //
    // Background for Free Gift or FreeshMaker
    //
    else if (stat == 1) {
        worldLayer->setVisible(false);
    }
    //
    // Background for Freesh Selection or Poster Selection or Option or FreeshMaker Result
    //
    else if (stat == 2) {
        worldLayer->setVisible(false);
        background->setColor(Color3B(240, 166, 33));
        
        if (flowTop) {
            flowTop->setOpacity(46);
            flowBottom->setOpacity(46);
        }
        else {
//            stayTop->setOpacity(46);
//            stayBottom->setOpacity(46);
        }
    }
    //
    // Background for Freesh Selection or Poster Selection or Option or FreeshMaker Result
    //
    else if (stat == 6) {
        worldLayer->setVisible(false);
        background->setColor(getBGColorSet(0));
        
        //
        if (flowTop) {
            flowTop->setOpacity(flowBGOpacityTable[0][0]);
            flowBottom->setOpacity(flowBGOpacityTable[0][1]);
        }
        else {
//            stayTop->setOpacity(flowBGOpacityTable[0][0]);
//            stayBottom->setOpacity(flowBGOpacityTable[0][1]);
        }
    }
    //
    // Background for Free Gift
    //
    else if (stat == 3) {
        worldLayer->setVisible(true);
    }
    //
    // Background for FreeshMaker closing
    //
    else if (stat == 4) {
        worldLayer->setVisible(true);
        
        background->setColor(getBGColorSet(0));

        //
        if (flowTop) {
            flowTop->setOpacity(flowBGOpacityTable[0][0]);
            flowBottom->setOpacity(flowBGOpacityTable[0][1]);
        }
        else {
//            stayTop->setOpacity(flowBGOpacityTable[0][0]);
//            stayBottom->setOpacity(flowBGOpacityTable[0][1]);
        }
    }
    //
    // Background for End of Run
    //
    else if (stat == 5) {
        Color3B _c = getBGColorSet(0);
        background->runAction(TintTo::create(OutroActionTime, _c.r, _c.g, _c.b));

        if (flowTop) {
            flowTop->runAction(FadeTo::create(OutroActionTime, flowBGOpacityTable[0][0]));
            flowBottom->runAction(FadeTo::create(OutroActionTime, flowBGOpacityTable[0][1]));
        }
        else {
//            stayTop->runAction(FadeTo::create(OutroActionTime, flowBGOpacityTable[0][0]));
//            stayBottom->runAction(FadeTo::create(OutroActionTime, flowBGOpacityTable[0][1]));
        }
    }
    //
    // Background to Origina end color ( when closing option )
    //
    else if (stat == 7) {
        worldLayer->setVisible(true);
        background->setColor(getBGColorSet(0));
        
        //
        if (flowTop) {
            flowTop->setOpacity(flowBGOpacityTable[0][0]);
            flowBottom->setOpacity(flowBGOpacityTable[0][1]);
        }
        else {
//            stayTop->setOpacity(flowBGOpacityTable[0][0]);
//            stayBottom->setOpacity(flowBGOpacityTable[0][1]);
        }
    }
}

void PlayScene::setWorldLayer()
{
    //
    watchRadius = 890;

    //
    // World Layer!!
    //
    worldLayer = Layer::create();
    worldLayer->setAnchorPoint(Vec2::ZERO);
    addChild(worldLayer, 40);
    worldLayer->setPosition(Vec2(visibleSizeHalf) + origin);

    //
    //
    //
    float scale = 0.3f * UserInfo::getInstance()->screenScale;
    worldLayer->setScale(scale);
 
    //
    // Character Layer
    //
    character = FishLayer::create();
    character->setPosition(Vec2::ZERO);
    character->setVisible(false);
    int currSkin = setFreeshSkin();
    colorIdx = currSkin - 1;
    character->setPlayableScale();
    worldLayer->addChild(character, 50);

    //
    // Jump Coin for Goldfish
    //
    jCoin = JumpCoin::create();
    worldLayer->addChild(jCoin, 100);

    //
    // Background Layer
    //
    if (colorIdx == 34) { // when sleepy freesh skin
        bgLayer = SleepyTimon::create();
    }
    else {
        bgLayer = BGLayer::create();
    }
    bgLayer->initMainLayerWithColor(colorIdx);
    worldLayer->addChild(bgLayer, 0);
    bgLayer->setNextBg(0);
    bgLayer->setPosition(Vec2::ZERO);
    std::function<void()> callback = [this]() {
        //
        unschedule(CC_SCHEDULE_SELECTOR(PlayScene::moveCharacter));

        //
        uiLayer->showResultMain();
    };
    bgLayer->setFinishCallback(callback);

    //
    // Obstacle Layer
    //
    obsLayer = ObstacleLayer::create();
    obsLayer->initObstaclesWithRadius(watchRadius, colorIdx);
    obsLayer->setPosition(Vec2::ZERO);
    worldLayer->addChild(obsLayer, 10);

    //
    // Ball Layer
    //
    itemLayer = Layer::create();
    worldLayer->addChild(itemLayer, 10);

    //
    fishRadius = watchRadius - 130 * 0.75;

    //
    // Motionstreak Trail
    //
//    float trailWidth = 80.0f;
//    if (colorIdx >= 48) {
//        trailWidth = 15.0f;
//    }
//    freeshTrail = FreeshTrailLayer::create(0.25f, 1.0f, trailWidth, Color3B(255, 255, 255), "TF_freeshTrail.png");
    if (colorIdx != UserInfo::getInstance()->getHiddenSkinIndex() + 6) {
        freeshTrail = FreeshTrailLayer::create(0.175f, 1.0f, 80, Color3B(255, 255, 255), "TF_freeshTrail.png");
        freeshTrail->setVisible(false);
        worldLayer->addChild(freeshTrail, 39);
    }

    //
    // obstacles
    //
    initObstacles();

    //
    // balls
    //
    initBalls();
    
    //
    // Particles
    //
    initParticles();

    // center circle
    {
        centerCircle = Layer::create();
        worldLayer->addChild(centerCircle, 90);

        bool flipX[] = { false, true, true, false };
        bool flipY[] = { false, false, true, true };
        Vec2 anchorPoints[] = {
            Vec2(0, 1),
            Vec2(1, 1),
            Vec2(1, 0),
            Vec2(0, 0)
        };
        for (int i=0; i<4; i++) {
            auto dotcircle = Sprite::create("clock_line_01.png");
            dotcircle->setAnchorPoint(anchorPoints[i]);
            dotcircle->setPosition(Vec2::ZERO);
            dotcircle->setColor(getSkinColorSet(colorIdx, DotColor));
            dotcircle->setFlippedX(flipX[i]);
            dotcircle->setFlippedY(flipY[i]);
            centerCircle->addChild(dotcircle, 3);
            
            auto incircle = Sprite::create("clock_line_02.png");
            incircle->setAnchorPoint(anchorPoints[i]);
            incircle->setPosition(Vec2::ZERO);
            incircle->setColor(getSkinColorSet(colorIdx, InColor));
            incircle->setFlippedX(flipX[i]);
            incircle->setFlippedY(flipY[i]);
            centerCircle->addChild(incircle, 2);
            
            auto outcircle = Sprite::create("clock_line_03.png");
            outcircle->setAnchorPoint(anchorPoints[i]);
            outcircle->setPosition(Vec2::ZERO);
            outcircle->setColor(getSkinColorSet(colorIdx, OutColor));
            outcircle->setFlippedX(flipX[i]);
            outcircle->setFlippedY(flipY[i]);
            centerCircle->addChild(outcircle,1);
            
            
//            // test in border
//            auto inBorder = Sprite::create("clock_line_04.png");
//            inBorder->setAnchorPoint(anchorPoints[i]);
//            inBorder->setPosition(Vec2::ZERO + origin);
//            inBorder->setColor(getSkinColorSet(colorIdx, InColor));
//            inBorder->setFlippedX(flipX[i]);
//            inBorder->setFlippedY(flipY[i]);
//            worldLayer->addChild(inBorder, 120);
//            
//            // test out border
//            auto outBorder = Sprite::create("clock_line_05.png");
//            outBorder->setAnchorPoint(anchorPoints[i]);
//            outBorder->setPosition(Vec2::ZERO + origin);
//            outBorder->setColor(getSkinColorSet(colorIdx, OutColor));
//            outBorder->setFlippedX(flipX[i]);
//            outBorder->setFlippedY(flipY[i]);
//            worldLayer->addChild(outBorder, 100);

        }
    }

    /*
     // circle lines
    {
        float outRadius = watchRadius + 30;
        float inRadius = watchRadius;

        auto stencil = DrawNode::create();
        stencil->drawSolidCircle(Vec2::ZERO + origin, inRadius - 30, CC_DEGREES_TO_RADIANS(90), CircleSegments, 1.0f, 1.0f, Color4F::BLACK);
   
        auto clipper = ClippingNode::create();
        clipper->setAnchorPoint(Point(0.5, 0.5));
        clipper->setStencil(stencil);
        clipper->setInverted(true);
        worldLayer->addChild(clipper, 100);

        //
        auto outLine = DrawNode::create();
        Color4F outColor(RGBValue(209), RGBValue(107), RGBValue(248), 1.0f);
        outLine->drawSolidCircle(Vec2::ZERO + origin, outRadius, CC_DEGREES_TO_RADIANS(90), CircleSegments, 1.0f, 1.0f, outColor);
        clipper->addChild(outLine);

        //
        auto inLine = DrawNode::create();
        Color4F inColor(RGBValue(150), RGBValue(57), RGBValue(185), 1.0f);
        inLine->drawSolidCircle(Vec2::ZERO + origin, inRadius, CC_DEGREES_TO_RADIANS(90), CircleSegments, 1.0f, 1.0f, inColor);
        clipper->addChild(inLine);
    }
     */
    
    // watch borders
    /*
    {
        float outRadius = watchRadius + 32;
        float inRadius = watchRadius - 3;

        for (int i=0; i<360; i+=2) {
            auto inSegment = Sprite::create("clock_in_2do.png");
            inSegment->setScaleX(0.991);
            inSegment->setColor(getSkinColorSet(colorIdx, InColor));
            inSegment->getTexture()->setAntiAliasTexParameters();

            auto outSegment = Sprite::create("clock_out_2do.png");
            outSegment->setScaleX(0.966);
            outSegment->setColor(getSkinColorSet(colorIdx, OutColor));
            outSegment->getTexture()->setAntiAliasTexParameters();

            float _angle = CC_DEGREES_TO_RADIANS(i);
            Vec2 dir = Vec2(cosf(_angle), sinf(_angle));

            //
            Vec2 inPos = dir * inRadius;
            inSegment->setPosition(inPos + origin);
            worldLayer->addChild(inSegment, 120);
            
            //
            Vec2 outPos = dir * outRadius;
            outSegment->setPosition(outPos + origin);
            worldLayer->addChild(outSegment, 100);
            
            Vec2 diff = - dir;
            
            //
            // set object rotation BEFORE POSITION!!!
            //
            float radian = diff.getAngle();
            float degree = get12OclockDegreeFrom3OclockRadian(radian);
            
            inSegment->setRotation(degree);
            outSegment->setRotation(degree);
        }
    }
     */
    
    {
        float outRadius = watchRadius - 19;
        float inRadius = watchRadius - 45;
        
        float _degree = 0;
        for (int i=0; i<8; i++) {
            auto inSegment = Sprite::createWithSpriteFrameName("clock_line_04.png");
            inSegment->setColor(getSkinColorSet(colorIdx, InColor));

            auto outSegment = Sprite::createWithSpriteFrameName("clock_line_05.png");
            outSegment->setColor(getSkinColorSet(colorIdx, OutColor));

            float _angle = CC_DEGREES_TO_RADIANS(_degree);
            Vec2 dir = Vec2(cosf(_angle), sinf(_angle));
            
            //
            Vec2 inPos = dir * inRadius;
            inSegment->setPosition(inPos);
            worldLayer->addChild(inSegment, 120);
            
            //
            Vec2 outPos = dir * outRadius;
            outSegment->setPosition(outPos);
            worldLayer->addChild(outSegment, 100);
            
            Vec2 diff = - dir;
            
            //
            // set object rotation BEFORE POSITION!!!
            //
            float radian = diff.getAngle();
            float degree = get12OclockDegreeFrom3OclockRadian(radian);
            
            inSegment->setRotation(degree);
            outSegment->setRotation(degree);
            
            _degree += 45.0f; // 45 degree
        }
    }
    
//    //
//    character->removeFromParentAndCleanup(true);
//    jCoin->removeFromParentAndCleanup(true);
//    obsLayer->removeFromParentAndCleanup(true);
//    itemLayer->removeFromParentAndCleanup(true);
//    freeshTrail->removeFromParentAndCleanup(true);
//    for (int i=0; i<JumpParticles; i++) {
//        jumpEmitter[i]->removeFromParentAndCleanup(true);
//    }
//    crashEmitter->removeFromParentAndCleanup(true);
}

void PlayScene::initObstacles()
{
    //
    currLap = character->getCurrLap();
    currRound = (int)currLap/15;

    float deltaDegree = 25 - currLap;
    float firstDegree = DefaultBallAnglePos + deltaDegree;
    float currDegree[] = {
        270.0, 200.0, firstDegree // lap1
    };

    int currColor = bgLayer->getCurrCircle();

    //
    // starts from 3 o'clock and move counterclock wise
    //
    for (int i=0; i< 3; i++) {
        float degree = currDegree[i];
        obsLayer->setObstacleAt(degree, currColor, false);
    }
}

void PlayScene::setObstacles()
{
    //
    obsLayer->reinitObstacles();
    
    //
    int obsNums[] = {6, 7, 8, 9, 7, 6, 8, 7, 8, 8, 9, 8, 9, 9};
//    float currDegree[] = {
//        410, 360, 310, 240, 190, 140, // lap2
//        410, 370, 330, 290, 230, 190, 150, // lap3
//        410, 375, 340, 305, 255, 220, 185, 135, // lap4
//        410, 380, 350, 300, 270, 240, 190, 160, 130, // lap5
//        410, 365, 320, 275, 230, 185, 140, // lap6
//        410, 380, 345, 285, 250, 220, // lap 7
//        410, 380, 350, 290, 260, 230, 170, 140, // lap 8
//        410, 360, 315, 275, 240, 210, 185, ///// lap 9
//        410, 385, 355, 320, 280, 235, 185, ///// lap 10
//        410, 375, 340, 305, 245, 215, 185, 155, // lap 11
//        410, 380, 350, 320, 290, 260, 230, 200, 170, // lap 12
//        410, 380, 350, 300, 270, 240, 190, 160, // lap 13
//        410, 380, 350, 310, 280, 250, 210, 180, 150, // lap 14
//        410, 385, 355, 320, 280, 235, 195, 160, 130, // lap 15
//    };

    currLap = character->getCurrLap();
    currRound = MIN((int)currLap/15, 5); // 5 round max
    float startDegree = 405 - currRound * 2;

    float currDegreeDelta[] = { // degree delta value fom starting degree
        0, 50, 100, 170, 220, 270,
        0, 40,  80, 120, 180, 220, 260,
        0, 35,  70, 105, 155, 190, 225, 275,
        0, 30,  60, 110, 140, 170, 220, 250, 280,
        0, 45,  90, 135, 180, 225, 270,
        0, 30,  65, 125, 160, 190,
        0, 30,  60, 120, 150, 180, 240, 270,
        0, 50,  95, 135, 170, 200, 225,
        0, 25,  55,  90, 130, 175, 225, 275,
        0, 35,  70, 105, 165, 195, 225, 255,
        0, 30,  60,  90, 120, 150, 180, 210, 240,
        0, 30,  60, 110, 140, 170, 220, 250,
        0, 30,  60, 100, 130, 160, 200, 230, 260,
        0, 25,  55,  90, 130, 175, 215, 250, 280
    };

    int tracks = (int)sizeof(obsNums)/sizeof(int);
    int idx = (currLap-1) % tracks;
    int obsNum = obsNums[idx];
    int degreeIdx = 0;
    for (int i=0; i<idx; i++) {
        degreeIdx += obsNums[i];
    }

    //
    int coinIdx[3] = {-1, -1, -1}; // bonus coin idx

    //
    if (freeshStat == CoinBoosterStatus) {
        obsLayer->makeObstaclesToCoin(obsNum);

        // init
        setFreeshToNormalStatus();
    }
    else {
        int prob = rand()%100;
        if (prob < 15) { // less than 15%
            //
            // set bonus coin idx
            //
            int bonusCoins = rand()%3 + 1; // 1~3
            for (int i=0; i<bonusCoins; i++) {
                coinIdx[i] = rand()%obsNum;
            }
        }
    }

    //
    // starts from 3 o'clock and move counterclock wise
    //
    int nextColor = bgLayer->getNextCircle();
    int _doubleObsCount = 0;
    for (int i=0; i< obsNum; i++) {
        float degree = startDegree - currDegreeDelta[degreeIdx + i];

        bool asCoin = false;
        for (int j=0; j<3; j++) {
            if (coinIdx[j] == i) {
                asCoin = true;
                break;
            }
        }
        
        //
        bool doubleObstacle = false;
        int prob = rand()%100;
        if (_doubleObsCount < MIN(currRound,4) && prob < currRound*20) { // currRound starts from 0.
            doubleObstacle = true;
            _doubleObsCount++;
//            log("%d -> %d", currRound, prob);
        }

        obsLayer->setObstacleAt(degree, nextColor, doubleObstacle, asCoin);
    }
}

int PlayScene::getNextBallIndex()
{
    //
    ballLap++;
    
    int nextBGColor = bgLayer->getNextCircle() + 1;
    if (nextBGColor > 2) {
        nextBGColor = 0;
    }

    return nextBGColor;
}

void PlayScene::initBalls()
{
    //
    int nextBGColor = getNextBallIndex();

    //
    BallLayer *capsule = BallLayer::create();
    capsule->initMainLayer(colorIdx, nextBGColor);

    //
    // starts from 3 o'clock and move counterclock wise
    //
    float degree = DefaultBallAnglePos;
    float radian = CC_DEGREES_TO_RADIANS(degree);
    float radius = watchRadius - 71;
    Vec2 pos = Vec2(cosf(radian), sinf(radian))* radius;// + origin;
    capsule->setPosition(pos);
    capsule->setAnglePosition(radian);
    capsule->setTag(1000); // TEMP
    itemLayer->addChild(capsule, 0);
    
    balls.pushBack(capsule);
    
    //
    // set object rotation
    //
    {
        Vec2 diff = origin - pos;
        float _radian = diff.getAngle();
        float _degree = get12OclockDegreeFrom3OclockRadian(_radian);
        capsule->setEffectRotation(_degree, radian);
    }
    
    //
    // init capsules
    //
    const char* fileNames[] = {
        "boost_icon_fever.png",
        "boost_icon_coin.png",
        "boost_icon_slow.png",
        "boost_icon_score.png",
        "boost_icon_shield.png"
    };
    for (int i=0; i<5; i++) {
        boostIcon[i] = Sprite::createWithSpriteFrameName(fileNames[i]);
        boostIcon[i]->setVisible(false);
        boostIcon[i]->setPosition(capsule->getPosition());
        worldLayer->addChild(boostIcon[i], 200);
    }
}

void PlayScene::setCapsule()
{
    //
    int nextBGColor = getNextBallIndex();

    //
    if (balls.size() > 0) {
        BallLayer *colorBall = balls.at(balls.size() - 1);
        if (colorBall) {
            colorBall->setCurrType(nextBGColor);

            int boostType = -1; // default
            //
            // Check if Booster Capsule is needed!!!
            //
            bool needBoost = (currLap > 0 && currLap == nextCapsuleLap
                              && freeshStat != ContinueBoosterStatus); // 이어하기로 부스팅된 상태가 아닌어야함
            if (needBoost) {
                int totalPlayCount = UserInfo::getInstance()->getTotalPlayCount();
                bool firstTimeBoost = false;
                int boostProbability = BoostProbability;
                if (totalPlayCount < 5) {
                    //
                    // FOR THE FIRST-TIME USER EXPERIENCE:
                    //
                    int idx = totalPlayCount; // 0 ~ 4
                    if (currLap == firstLaps[idx]) {
                        int firstTimeBoostTypes[] = {1,0,2,3,4};
                        boostType = firstTimeBoostTypes[idx];
                        firstTimeBoost = true;
                    }
                    else {
                        boostProbability = BoostProbabilityForFirstUser;
                    }
                }

                //
                if (!firstTimeBoost) {
                    int boostProb = rand()%1000;
                    if (boostProb < boostProbability && freeshStat == NormalStatus) {
                        //
                        // check distance between freesh and timon
                        //
                        float characterPos = character->getAnglePosition();
                        float timonPos = bgLayer->getCurrDegree();
                        float posDiff = characterPos - timonPos;
                        
                        int freeshLap = character->getCurrLap();
                        int timonLap = bgLayer->getCurrLap();
                        
                        if (freeshLap != timonLap) {
                            posDiff += 360;
                        }
                        
                        //
                        boostProb = rand()%1000;
                        //
                        // if freesh and timon are close enough...
                        //
                        if (posDiff < CloseEnoughToMonsterDist) {
                            //
                            if (boostProb < 300) { // 30%
                                boostType = 0; // Fever Capsule
                            }
                            else if (boostProb < 450) { // 15%
                                boostType = 1; // Coin Capsule
                            }
                            else if (boostProb < 750) { // 30%
                                boostType = 2; // Slow Capsule
                            }
                            else if (boostProb < 850) { // 10%
                                boostType = 3; // Point Capsule
                            }
                            else if (boostProb < 1000) { // 15%
                                boostType = 4; // Shield Capsule
                            }
                        }
                        //
                        // if freesh and timon are far enough...
                        //
                        else {
                            if (boostProb < 300) { // 30%
                                boostType = 0;
                            }
                            else if (boostProb < 550) { // 25%
                                boostType = 1;
                            }
                            else if (boostProb < 850) { // 30%
                                boostType = 3;
                            }
                            else if (boostProb < 1000) { // 15%
                                boostType = 4;
                            }
                        }
#ifdef AUTOJUMP_TEST
                        boostType = testBoostType;
                        testBoostType++;
                        if (testBoostType == 5) {
                            testBoostType = 0;
                        }
#endif
                    }
                }
                
                //
                setNextCapsuleLap();
            }

//            log("%d", boostType);
            colorBall->setAsBooster(boostType); // set -1 if not.
        }
    }
}

void PlayScene::setNextCapsuleLap()
{
    int totalPlayCount = UserInfo::getInstance()->getTotalPlayCount();
    if (totalPlayCount < 5 && currLap == 0) {
        //
        // FOR THE FIRST-TIME USER EXPERIENCE:
        //
        int idx = totalPlayCount; // 0 ~ 4
        nextCapsuleLap = firstLaps[idx];
    }
    else {
        int r = rand()%3 + 2;
        nextCapsuleLap = currLap + r;
    }

//    log("currLap: %d nextCapsuleLap: %d", currLap, nextCapsuleLap);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Main Loop
void PlayScene::moveCharacter(float dt)
{
    //
    if (freeshStat == FeverBoosterStatus || freeshStat == ContinueBoosterStatus) {
        runVelocity = 300;//MaxVelocity * 2.0;
    }
    else {
        if (fabsf(feverVelocity - maxVelocity) > FLT_EPSILON) {
            //
            // While in fever-mode ( not fever capsule but color ball !!!)
            //
            runVelocity = MAX(feverVelocity, maxVelocity);
            feverVelocity *= FeverVelocityDecRate;
            feverVelocity = MAX(feverVelocity, maxVelocity);
        }
        else {
            //
            // normal status
            //
            runVelocity +=  maxVelocity * VelocityIncRate * (dt / 0.0166);
            runVelocity = MIN(runVelocity, maxVelocity);
        }
    }

    float freeshDegreeDelta = dt * runVelocity * TEST2;

    //
    if (bgLayer->isMoveable()) {
        float characterPos = character->getAnglePosition();
        float timonPos = bgLayer->getCurrDegree();
        float posDiff = characterPos - timonPos;
        float _timonMaxVelocity = 0;
        
        if (freeshDied) {
            maxVelocity *= (1.021 + VelocityIncRate);
            _timonMaxVelocity = MIN(maxVelocity, 500); // set Timon's maximum velocity when Freesh's died.
        }
        else {
            //
            // Let Timone start smoothly!!!
            //
            timonMaxVelocity +=  maxVelocity * TimonVelocityIncRate * (dt / 0.0166);
            timonMaxVelocity = MIN(timonMaxVelocity, maxVelocity);

            _timonMaxVelocity = (character->getCurrLap() == bgLayer->getCurrLap() && posDiff < 20)? timonMaxVelocity: timonMaxVelocity * (1 + currRound*0.1);
            if (freeshStat == FeverBoosterStatus) {
                _timonMaxVelocity = runVelocity * 0.99f; // 99% of freesh feverBoost speed
            }
            else if (freeshStat == ContinueBoosterStatus) {
                if (getByMonster(45)) {
                    //
                    // NOTE: 타이몬이 가까우면, 일정 거리만큼 떨어질 때까지 잠시 멈춰준다.
                    //
                    _timonMaxVelocity = 0;
                }
                else {
                    _timonMaxVelocity = runVelocity * 0.80f; // 80% of freesh feverBoost speed
                }
            }
            else if (freeshStat == SlowBoosterStatus) {
                _timonMaxVelocity *= 0.1;
            }
        }
        bgLayer->setNextBg(CC_DEGREES_TO_RADIANS(dt * _timonMaxVelocity * TEST3), freeshDied);
    }
    
    //
    checkObstacles();
    checkCapsule();


    //
    // AFTER DIED, MOVE BGLAYER ONLY!!!
    //
    if (freeshDied) {
        
        //
        // freeshHeight
        //
//        if (freeshHeight > 0) {
//            log("%f", freeshHeight);
//        }
        

        return;
    }

    //
    // Set Freesh Status
    //
    if (freeshStat == FeverBoosterStatus || freeshStat == ContinueBoosterStatus) {
        boostDuration -= freeshDegreeDelta;
        if (boostDuration <= 0) {
            setFreeshToNormalStatus();

            // to warn user the end of FeverBoosterStatus/ContinueBoosterStatus
            setFreeshBlink();
        }
    }
    else if (freeshStat == SlowBoosterStatus) {
        boostDuration -= freeshDegreeDelta;
        if (boostDuration <= 0) {
            bgLayer->setTimonToSlow(false);
            setFreeshToNormalStatus();
        }
    }

    //
    float deltaAngle = CC_DEGREES_TO_RADIANS(freeshDegreeDelta);
    angle -= deltaAngle;

    //
    Vec2 newPos = Vec2(cosf(angle), sinf(angle)) * fishRadius;

    
    
    float trailAngle = 0;
    if (freeshStat == FeverBoosterStatus || freeshStat == ContinueBoosterStatus) {
        trailAngle = angle - CC_DEGREES_TO_RADIANS(3.0);
    }
    else {
        trailAngle = angle + CC_DEGREES_TO_RADIANS(1.0);
    }
//    Vec2 newTrailPos = Vec2(cosf(trailAngle), sinf(trailAngle)) * (fishRadius + 15);
    Vec2 newTrailPos = Vec2(cosf(trailAngle), sinf(trailAngle)) * (fishRadius);

    
    
#ifndef DEVELOPMENT
    //
    // move world layer
    //
    worldLayer->setPosition(setWorldLayerPos(newPos));
    
//    if (Vec2(Vec2(622.752441,4.137451) - worldLayer->getPosition()).length() < 5) {
//        unschedule(CC_SCHEDULE_SELECTOR(PlayScene::moveCharacter));
//    }
#endif
    Vec2 diff = - newPos;
    
    //
    // set object rotation BEFORE POSITION!!!
    //
    float radian = diff.getAngle();
    float degree = get12OclockDegreeFrom3OclockRadian(radian);
    character->setFishRotation(degree);

    float btm = newPos.y;
    
    //
    // set object position
    //
    if (status == StatusJump) {
        currTime += dt;

        if (currTime >= jumpTime) {
            currTime = jumpTime;
            setGameStatus(StatusNone);

            character->setAnimation(AniJump2);
            
            showJumpParticle();

            //
            // End of Jump
            //
            int r = rand()%3;
            SoundManager::getInstance()->playSoundEffect((SoundEffect)(SoundLanding1 + r), false);
        }
        else {
            float t = currTime / jumpTime;
            float frac = fmodf(t, 1.0f );
            float y = JumpFactor * 4 * frac * (1 - frac);
            diff.normalize();
            diff *= MIN(y, JumpFactor);
            newPos += diff;

            newTrailPos += diff;

            if (goingUp) {
                float currDiffLength = diff.length();
                if (prevDiffLength >= currDiffLength) {
                    //
                    // Started to Going Down!
                    //
                    goingUp = false;

                    //
                    // if golden fish
                    //
                    if (colorIdx == UserInfo::getInstance()->getGoldenFishSkinIndex()) {
                        goldenJumpCount--;
                        if (goldenJumpCount <= 0) {
                            goldenJumpCount = rand()%10 + 5;

                            //
                            jCoin->setPosition(newPos);
                            jCoin->setCoinRotation(degree);
                            jCoin->showJumpCoin();

                            //
                            SoundManager::getInstance()->playSoundEffect(SoundGoldenFishCoin, false);

                            //
                            getOneCoin();
                        }
                    }
                }
            }

            prevDiffLength = diff.length();
        }
    }

    //
    character->setPosition(newPos);// + origin);
    
    
#ifdef AUTOJUMP_TEST
    if (status != StatusJump && obsLayer->testGetClosestObstacle(character->getAnglePosition())) {
        setToJump();
    };
#endif
    

//    freeshTrail->setPosition(character->getPosition());

    if(freeshTrail) freeshTrail->setPosition(newTrailPos);// + origin);
//    {
//        int idx = character->getCurrType();
//        Color3B c[3];
//        c[0] = Color3B(255, 7, 7); // red
//        c[1] = Color3B(7, 130, 255); // blue
//        c[2] = Color3B(239, 164, 19); // yellow
//
//        freeshTrail->setColor(c[idx]);
//    }

  
    //
    // height information is needed for the animation transition: from AniDie1 to AniDie2
    //
    freeshHeight = MAX(newPos.y - btm, 0);

    //
    character->setAnglePosition(angle);

    //
    int multiplier = currCombo;//(freeshStat == PointBoosterStatus)? MaxComboCount:currCombo;
    int deltaScore = deltaAngle * 100 * multiplier;
    UserInfo::getInstance()->addCurrScore(deltaScore, 3);
    uiLayer->scoreLabel->updateScore(UserInfo::getInstance()->getCurrScore());

    // check angle position
    if (angle <= -M_PI * 2) {
        angle += M_PI * 2;
    }

    //
    // Check goal in!
    //
    if (angle <= -4.71238898 && fabsf(angle + 4.71238898) < deltaAngle) {
        //
        // increase lap count first!!!
        //
        character->increaseCurrLap();

        setObstacles();

        //
        // Check Achievement for First Step
        //
        if (character->getCurrLap() == 7) {
            UserInfo::getInstance()->setAchievementInfoWithType(AchievementType11);
        }
    }

    //
    // check ending conditions
    //
    deathType = checkEndingConditions();
    if (!freeshDied && deathType != FreeshDeathTypeNone) {
        //
        SoundManager::getInstance()->playSoundEffect(SoundGone, false);

        //
        // NOTE: 비디오 광고를 보고 이어할 수 있는 상황이면, 유저에게 물어본다.
        //
        if (!gameContinuedWithVideo && UserInfo::getInstance()->isVideoAvailable()) {
            askToContinue();
        }
        else {
            setDied();
        }
    }
}

void PlayScene::askToContinue()
{
    if (status == StatusReady || status == StatusGameOver) {
        return;
    }
    
    prevStatus = status;
    setGameStatus(StatusAskContinue);
    
    worldLayer->pause();
    character->pauseAnimation();
    bgLayer->pauseAnimation();

    SoundManager::getInstance()->pauseAmbienceSound();

    unschedule(CC_SCHEDULE_SELECTOR(PlayScene::moveCharacter));
    
    //
    uiLayer->setNotContinueCallback(CC_CALLBACK_0(PlayScene::setNotContinue, this));
    uiLayer->setContinueCallback(CC_CALLBACK_0(PlayScene::setContinue, this));
    uiLayer->showContinuePopup();
}

void PlayScene::setNotContinue()
{
    worldLayer->resume();
    character->resumeAnimation();
    bgLayer->resumeAnimation();
    
    setGameStatus(prevStatus);
    
    SoundManager::getInstance()->resumeAllSoundEffect();
    
    schedule(CC_SCHEDULE_SELECTOR(PlayScene::moveCharacter));
    
    //
    setDied();
}

void PlayScene::setContinue()
{
    //
    // NOTE: 이어하기는 한번만 할 수 있다.
    //
    gameContinuedWithVideo = true;
    
    //
    worldLayer->resume();
    character->resumeAnimation();
    bgLayer->resumeAnimation();
    
    setGameStatus(prevStatus);
    
    SoundManager::getInstance()->resumeAllSoundEffect();

    {
        //
        if (deathType == FreeshDeathTypeByColor) {
            BallLayer *capsule = balls.at(balls.size() - 1);
            character->setType(capsule->getCurrType());
            //
            Vec2 worldPos = character->getPosition();
            Vec2 pos = worldLayer->convertToWorldSpace(worldPos);
            uiLayer->showNewLapUI(pos.y, character->getCurrType(), character->getCurrLap());
        }

        //
        //
        //
        freeshStat = ContinueBoosterStatus;
        //
        SoundManager::getInstance()->playSoundEffect(SoundFeverBoost, false);
        
        boostDuration = 345;
        
        // Freesh's Boost Sound if have....
        if (freeshBoostSoundIdx[colorIdx] >= 0) {
            SoundManager::getInstance()->playFreeshBoostSoundEffect(colorIdx, false);
        }
    }

    
    //
    schedule(CC_SCHEDULE_SELECTOR(PlayScene::moveCharacter));
}

void PlayScene::initParticles()
{
    //
    // set particle colors
    //
    Color3B _c1 = getFreeshColor(colorIdx, ColorOrderFirst);// Color3B(255, 7, 7);
    Color3B _c2 = getFreeshColor(colorIdx, ColorOrderSecond);// Color3B(7, 130, 255);
    Color3B _c3 = getFreeshColor(colorIdx, ColorOrderThird);// Color3B(239, 164, 19);
    ptclColors[0] = Color4F(RGBValue(_c1.r), RGBValue(_c1.g), RGBValue(_c1.b), 1);
    ptclColors[1] = Color4F(RGBValue(_c2.r), RGBValue(_c2.g), RGBValue(_c2.b), 1);
    ptclColors[2] = Color4F(RGBValue(_c3.r), RGBValue(_c3.g), RGBValue(_c3.b), 1);

    //
    // jump particle
    //
    jumpParticlePtr = -1;
    for (int i=0; i<JumpParticles; i++) {
        jumpEmitter[i] = ParticleSystemQuad::create("particle/jump/particle_jump.plist");
        jumpEmitter[i]->stopSystem();
        jumpEmitter[i]->setPositionType(ParticleSystem::PositionType::RELATIVE);
        worldLayer->addChild(jumpEmitter[i], 40);
    }

    //
    // crash particle
    //
    crashEmitter = ParticleSystemQuad::create("particle/crash/particle_texture.plist");
    crashEmitter->stopSystem();
    worldLayer->addChild(crashEmitter, 55);
}

void PlayScene::showJumpParticle()
{
    Vec2 pos = character->getPosition();

    jumpParticlePtr++;
    if (jumpParticlePtr >= JumpParticles) {
        jumpParticlePtr = 0;
    }

    Vec2 newPos = Vec2(cosf(angle), sinf(angle)) * (fishRadius + 20); // 20 pts from freesh's center
    jumpEmitter[jumpParticlePtr]->setPosition(newPos);// + origin);

    int idx = character->getCurrType();
    jumpEmitter[jumpParticlePtr]->setStartColor(ptclColors[idx]);
    jumpEmitter[jumpParticlePtr]->setEndColor(ptclColors[idx]);

    //
    Vec2 diff = origin - pos;
    float radian = diff.getAngle();
    float degree = get12OclockDegreeFrom3OclockRadian(radian);
    jumpEmitter[jumpParticlePtr]->setRotation(degree);

    //
    jumpEmitter[jumpParticlePtr]->resetSystem();
}

void PlayScene::showCrashParticle()
{
    crashEmitter->setPosition(character->getPosition());
    crashEmitter->resetSystem();
}

Vec2 PlayScene::setWorldLayerPos(Vec2 newPos)
{
    Vec2 pos = worldLayer->convertToWorldSpace(newPos);
    Vec2 diff = Vec2(visibleSizeHalf) - pos;
    Vec2 p = worldLayer->getPosition();

    return p + diff + origin;
}

FreeshDeathType PlayScene::checkEndingConditions()
{
    //
    // 0, 1, 2 -> red, blue, yellow
    //
    int currFishColor = character->getCurrType();
    int currBGColor = bgLayer->getCurrCircle();
    int nextBGColor = bgLayer->getNextCircle();

    //
    float characterPos = character->getAnglePosition();
    float timonPos = bgLayer->getCurrDegree();
    float posDiff = characterPos - timonPos;

    int freeshLap = character->getCurrLap();
    int timonLap = bgLayer->getCurrLap();

    bool getByMonster = (freeshLap == timonLap &&
                         posDiff < GetByMonsterMinDist);

    //
    bool enterSameColor = ((freeshLap == timonLap && currFishColor == currBGColor) ||
                           (freeshLap > timonLap && currFishColor == nextBGColor));
    
#ifdef DEVELOPMENT
//    //log("[%d] %f == %f [%d] (%d == %d ) && (%d == %d) || (%d > %d && %d == %d)", getByMonster,
//        character->getAnglePosition(), bgLayer->getCurrDegree(),
//        enterSameColor,
//        freeshLap, timonLap,
//        currFishColor, currBGColor,
//        freeshLap, timonLap,
//        currFishColor, nextBGColor
//        );
#endif
    
    //
    // Death Count
    //
    if (getByMonster) {
        //
        // Get By Timon
        //

        //
        // DataTypeDeathCountRed, DataTypeDeathCountBlue, DataTypeDeathCountYellow
        //
        FreeshColorSet colorSet = bgLayer->getCurrColorSet();
        if (colorSet == FreeshColorSet_Red) {
            UserInfo::getInstance()->addToDeathCount((DataType)(DataTypeDeathCountRed));
        }
        else if (colorSet == FreeshColorSet_Blue) {
            UserInfo::getInstance()->addToDeathCount((DataType)(DataTypeDeathCountBlue));
        }
        else if (colorSet == FreeshColorSet_Yellow) {
            UserInfo::getInstance()->addToDeathCount((DataType)(DataTypeDeathCountYellow));
        }

        //
        // DataTypeDeathCountFirst
        //
        bool firstObstacle = (freeshLap==0);// && obsLayer->getCollidedObstacleIdx() == 2);
        if (firstObstacle) {
            UserInfo::getInstance()->addToDeathCount(DataTypeDeathCountFirst);
        }

        //
        // jumping death3
        //
        bool jumpDeath = (status == StatusJump);
        if (jumpDeath) {
            UserInfo::getInstance()->addToDeathCount(DataTypeDeathCountJump);
        }

        //
        // total death
        //
        UserInfo::getInstance()->addToDeathCount(DataTypeDeathCountTotal);

        return FreeshDeathTypeByTimon;
    }
    else if (enterSameColor) {
        //
        // Enter Same Color
        //

        // death without ball
        UserInfo::getInstance()->addToDeathCount(DataTypeDeathCountWithoutBall);

        // jumping death
        bool jumpDeath = (status == StatusJump);
        if (jumpDeath) {
            UserInfo::getInstance()->addToDeathCount(DataTypeDeathCountJump);
        }
        
        // total death
        UserInfo::getInstance()->addToDeathCount(DataTypeDeathCountTotal);

        return FreeshDeathTypeByColor;
    }

    //
    // play timon's eye and sound effect by distance
    //
    if (freeshLap > timonLap) {
        posDiff = 360 - timonPos + characterPos;
    }
    bgLayer->setTimonByDistance(posDiff, freeshLap, freeshDied);
    
    return FreeshDeathTypeNone;
}

void PlayScene::setDied()
{
    freeshDied = true;
    
    //
    UserInfo::getInstance()->saveLapInfo(character->getCurrLap());

    //
    UserInfo::getInstance()->incPlayCount();
    UserInfo::getInstance()->incTotalPlayCount();

    //
    // Here, Decide result UI type: one-button or three-button
    //
    uiLayer->resultMain->setMainResultType();

    //
    // Start Outro Immediately!!!
    //
    startOutro();

    //
    character->stopAllActions();
    character->runAction(Show::create());
//    character->setDieAniCallback(CC_CALLBACK_1(PlayScene::startOutro, this));
    character->setAnimation(AniDie1);

    //
    // register finish callback to Timon. 'coz Timon is gonna run more
    //
    std::function<void()> callback = [this]() {
        //
        uiLayer->showResultUIButtons();
        //
        centerCircle->setVisible(false);
    };
    bgLayer->setStartToCloeEyeCallback(callback);
    bgLayer->setTimonRunMore();

    // stop ball's effect animation
    for (int i=0 ; i<balls.size(); i++) {
        BallLayer *bNode = balls.at(i);
        if (bNode) {
            bNode->setFinish();
        }
    }
    
    //
    if (coinsEarned > 0) {
        int amountType = (coinsEarned > 999)?4:(coinsEarned > 299)?3:(coinsEarned > 99)?2:(coinsEarned > 49)?1:0;
        TapjoyX::getInstance()->logEventInEconomy(2, amountType);
    }
}

void PlayScene::resetWorldLayerPositionAndScale()
{
    float posYRate = (uiLayer->resultMain->getResultOneOrThree() == 0)? 0.5f : 0.54;
    float scale = (uiLayer->resultMain->getResultOneOrThree() == 0)? 0.3f: 0.26f;
    Vec2 moveToPos = Vec2(visibleSizeHalf.width, visibleSize.height * posYRate)+ origin;

    uiLayer->resultMain->setBasePosY(moveToPos.y);
    scale = scale * UserInfo::getInstance()->screenScale;
    auto action = Spawn::create(
                                EaseSineIn::create(MoveTo::create(0, moveToPos)),
                                EaseSineIn::create(ScaleTo::create(0, scale)),
                                NULL);
    worldLayer->runAction(action);
}

void PlayScene::startOutro()
{
    bgLayer->setTimonSoundoff();

    //
    //
    //
    UserInfo::getInstance()->processResultScore(6);

    uiLayer->startResultOutro();

    //
    outroTime = 0;
    schedule(CC_SCHEDULE_SELECTOR(PlayScene::startOutroLoop));
}

void PlayScene::startOutroLoop(float dt)
{
    circleAlpha += dt;

    outroTime += dt;
    if (outroTime > 0.45f) {
        unschedule(CC_SCHEDULE_SELECTOR(PlayScene::startOutroLoop));

        //
        // time check!
        //
        eTime = time(0); // get end time
        time_t playTime = eTime - sTime;
        UserInfo::getInstance()->playTimeTotal += playTime;

#ifdef DEVELOPMENT
        auto seq = Sequence::create(
                                    CallFunc::create( CC_CALLBACK_0(PlayScene::showScoreUI,this)),
                                    NULL);
        worldLayer->runAction(seq);
#else
        float posYRate = (uiLayer->resultMain->getResultOneOrThree() == 0)? 0.5f : 0.54;
        float scale = (uiLayer->resultMain->getResultOneOrThree() == 0)? 0.3f: 0.26f;
        Vec2 moveToPos = Vec2(visibleSizeHalf.width, visibleSize.height * posYRate)+ origin;
    
        uiLayer->resultMain->setWorldLayerUpdater(CC_CALLBACK_0(PlayScene::resetWorldLayerPositionAndScale, this));
        uiLayer->resultMain->setBasePosY(moveToPos.y);
        
        scale = scale * UserInfo::getInstance()->screenScale;
        auto action = Spawn::create(
                                    EaseSineIn::create(MoveTo::create(OutroActionTime, moveToPos)),
                                    EaseSineIn::create(ScaleTo::create(OutroActionTime, scale)),
                                    NULL);

        auto seq = Sequence::create(
                                    action,
                                    CallFunc::create( CC_CALLBACK_0(PlayScene::showScoreUI,this)),
                                    NULL);
        worldLayer->runAction(seq);
        
        //
        // Smoothly change backgroud color to default color
        //
        setBackgroundLayerTo(5);
#endif

        //
        SoundManager::getInstance()->playSoundEffect(SoundGameOver, false);
    }
}

void PlayScene::showScoreUI()
{
    //
    setGameStatus(StatusGameOver);

    //
    schedule(CC_SCHEDULE_SELECTOR(PlayScene::setBackgroundSoundVolume));

    //
    // game center
    //
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    if (GameCenterX::getInstance()->isAuthenticated(false)) {
        GameCenterX::getInstance()->reportScore(UserInfo::getInstance()->getCurrScore());
    }
    
#else
    SocialX::getInstance()->submitScore();

    if (NativeUtils::isSignedIn()) {
        NativeUtils::submitScore(UserInfo::getInstance()->getCurrScore());
    }
#endif

    //
    FlurryX::getInstance()->logResultEvent(UserInfo::getInstance()->getCurrScore(),
                                           UserInfo::getInstance()->getBestScore(),
                                           UserInfo::getInstance()->getPlayCount(),
                                           UserInfo::getInstance()->getTotalPlayCount());
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void PlayScene::getOneCoin()
{
    UserInfo::getInstance()->addToCoins(1);
    uiLayer->updateCoinLabel();
    coinsEarned++;
}

void PlayScene::checkObstacles()
{
    //
    if (bgLayer->isTimonBlue()) {
        float currDegree = bgLayer->getCurrDegree();
        obsLayer->setObstacleDisappear(currDegree);

        return;
    }

    //
    Vec2 characterPos = character->getPosition();
    ObjectType obsType = obsLayer->checkCollisions(characterPos, crashed, (freeshStat == FeverBoosterStatus || freeshStat == ContinueBoosterStatus));
    if (obsType == ObjectTypeObstacle) {
        //
        // CRASH!!!
        //
        setCrashed();
    }
    else if (obsType == ObjectTypeCoin) {
        //
        // Get Coin
        //
        int r = rand()%4;
        SoundManager::getInstance()->playSoundEffect((SoundEffect)(SoundCoinGet1 + r), false);
        
        //
        character->setAnimation(AniEat);

        //
        getOneCoin();
    }
}

void PlayScene::setFreeshBoosted(ObjectType obsType)
{
    character->setAnimation(AniEat);

    FreeshStatus tmpStat = (FreeshStatus)(obsType - ObjectTypeCapsule1); // set to 0~3

    //
    // Freesh Shield Stat
    //
    if (tmpStat == ShieldBoosterStatus) {
        freeshShieldStat = ShieldBoosterStatus;
        character->setShield(true);
        
        //
        SoundManager::getInstance()->playSoundEffect(SoundShieldBoost, false);
    }
    else {
        freeshStat = tmpStat;
    }

    //
    // Freesh Stat
    //
    if (freeshStat == FeverBoosterStatus) {
        //
        SoundManager::getInstance()->playSoundEffect(SoundFeverBoost, false);

        boostDuration = 345;
        
        // Freesh's Boost Sound if have....
        if (freeshBoostSoundIdx[colorIdx] >= 0) {
            SoundManager::getInstance()->playFreeshBoostSoundEffect(colorIdx, false);
        }
    }
    else if (freeshStat == SlowBoosterStatus) {
        //
        SoundManager::getInstance()->playSoundEffect(SoundSlowBoost, false);

        boostDuration = 120;

        bgLayer->setTimonToSlow(true);
    }
    else if (freeshStat == PointBoosterStatus) {
        //
        SoundManager::getInstance()->playSoundEffect(SoundPointBoost, false);

        //
        // set currCombo to max+1 IMMEDIATELY!!!
        //
        setToMaxCombo(PointBoostBonus); // add for Point Boost Bonus

        uiLayer->updateComboCountLabel(currCombo, true);

        setFreeshToNormalStatus();
    }
    else if (freeshStat == CoinBoosterStatus) {
       //
        SoundManager::getInstance()->playSoundEffect(SoundCoinBoost, false);
    }
}

void PlayScene::setFreeshToNormalStatus()
{
    freeshStat = NormalStatus;

    setBackgroundLayerTo(0);
}

void PlayScene::setCrashed()
{
    //
    if (freeshStat == FeverBoosterStatus || freeshStat == ContinueBoosterStatus) {
        //
        // Crashed but no penalties!!!
        //
        
        //
        int r = rand()%3;
        SoundManager::getInstance()->playSoundEffect((SoundEffect)(SoundFeverCrash1 + r), false);
    }
    else if (freeshShieldStat == ShieldBoosterStatus) {
        //
        // Crashed only remove freesh shield
        //
        
        //
        SoundManager::getInstance()->playSoundEffect(SoundShieldBreak, false);

        character->setShield(false);

        freeshShieldStat = NormalStatus;
        
        //
        showCrashParticle();
    }
    else {
        //
        SoundManager::getInstance()->playSoundEffect(SoundCrash, false);
        
        if (currCombo > MaxComboCount) { // PointBoosterStatus
            SoundManager::getInstance()->playSoundEffect(SoundPointBreak, false);
        }

        //
        showCrashParticle();

        //
        // Penalties!!!
        //
        runVelocity *= VelocityDecRate;
        character->setAnimation(AniCrash);
        
        //
        //
        //
        currCombo = 1;
        maxComboCount = 0;
        uiLayer->updateComboCountLabel(currCombo);

        //
        setFreeshBlink();
    }
}

void PlayScene::setFreeshBlink()
{
    //
    crashed = true;
    if(freeshTrail) freeshTrail->setVisible(false);
    
    //
    auto callback = CallFunc::create([this](){
        crashed = false;
        if(freeshTrail) freeshTrail->setVisible(true);
    });

    //
    character->runAction(Sequence::create(
                                          Blink::create(2, 10),
                                          Show::create(),
                                          callback,
                                          NULL));
}

void PlayScene::checkCapsule()
{
    BallLayer *capsule = balls.at(balls.size() - 1);
    
    bool capsuleIsAvailable = (character->getAnglePosition() > 270);

    if (capsuleIsAvailable &&
        character->getCurrLap() == ballLap &&
        bgLayer->getCurrLap() == character->getCurrLap()) {
        
        //
        // Set new color AND set as a booster capsule if needed.
        //
        setCapsule();
    }

    if (!capsuleIsAvailable) {
        return;
    }
    else {
        Vec2 characterPos = character->getPosition();
        ObjectType objType = capsule->checkCollision(characterPos);
        if (objType != ObjectTypeNone) {
            //
            character->setType(capsule->getCurrType());

            //
            Vec2 worldPos = character->getPosition();
            Vec2 pos = worldLayer->convertToWorldSpace(worldPos);
            uiLayer->showNewLapUI(pos.y, character->getCurrType(), character->getCurrLap()+1);

            //
            // Increase max velocity
            //
            maxVelocity *= MaxVelocityIncRate;
            maxVelocity = MIN(maxVelocity, MaxVelocity);
            feverVelocity = maxVelocity * FeverVelocityIncRate;
            maxJumpTime *= MaxJumpTimeDecRate;
            maxJumpTime = MAX(maxJumpTime, JumpTimeMinLimit);

            //
            character->setAnimation(AniEat);

            //
            //
            //
            if (objType == ObjectTypeCapsule1 ||
                objType == ObjectTypeCapsule2 ||
                objType == ObjectTypeCapsule3 ||
                objType == ObjectTypeCapsule4 ||
                objType == ObjectTypeCapsule5) {
                //
                // EAT BOOST Capsule!!!
                //
                setFreeshBoosted(objType);

                //
                // Show boost icon
                //
                int idx = objType - ObjectTypeCapsule1;
                boostIcon[idx]->setOpacity(255);
                auto seq = Sequence::create(Show::create(),
                                            Spawn::create(ScaleTo::create(0.1, 5), FadeOut::create(0.4), NULL),
                                            Hide::create(),
                                            NULL);
                boostIcon[idx]->runAction(seq);

                //
                UserInfo::getInstance()->addToCapsuleCount();
                UserInfo::getInstance()->addToWitchCapsuleCount();
            }
            else {
                //
                SoundManager::getInstance()->playSoundEffect(SoundCapsule, false);
            }
            
            //
            //
            //
            // Score Boost Status
            if (currCombo == MaxComboCount + PointBoostBonus) {
                //
                // check achievement09
                //
                maxComboCount++;
                if (maxComboCount == 6) {
                    UserInfo::getInstance()->setAchievementInfoWithType(AchievementType09);
                }
            }
            else {
                int tmpCombo = currCombo;
                tmpCombo++;
                if (tmpCombo >= MaxComboCount) {
                    setToMaxCombo();
                }
                else {
                    currCombo = tmpCombo;
                }
                uiLayer->updateComboCountLabel(currCombo);
            }
        };
    }
}

void PlayScene::setToMaxCombo(int bonus)
{
    currCombo = MaxComboCount + bonus;
}

void PlayScene::setToJump()
{
    if (!freeshDied && status == StatusNone && freeshStat != FeverBoosterStatus && freeshStat != ContinueBoosterStatus) {
        currTime = 0;
        jumpTime = maxJumpTime; // sec
        goingUp = true;
        setGameStatus(StatusJump);
        
        character->setAnimation(AniJump1);
        
        //
        // Start of Jump
        //
        SoundManager::getInstance()->playFreeshJumpSoundEffect(colorIdx, false);
        
        showJumpParticle();
        
        //
        // Freesh's jumping voice
        //
        if (freeshVoiceSoundIdx[colorIdx] >= 0) {
            jumpCount++;
            int jumpRand = rand()%5 + 5;
            if (jumpCount > jumpRand) {
                SoundManager::getInstance()->playFreeshVoiceSoundEffect(colorIdx, false);
                jumpCount = 0;
            }
        }

        //
        UserInfo::getInstance()->increaseJumpCount();
    }

}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark TOUCH
bool PlayScene::onTouchBegan(Touch *touch, Event *event)
{
    if (//achLayer->ownsTouchPriority() ||
        (uiLayer->freeshSelection && uiLayer->freeshSelection->ownsTouchPriority()) ||
        (uiLayer->achSelection && uiLayer->achSelection->ownsTouchPriority()) ||
        uiLayer->freeshMaker->ownsTouchPriority()) {
        return false;
    }

    // jump
    setToJump();

#ifdef LEVELDESIGN
    Vec2 touchPos = touch->getLocation();
    Vec2 diff = touchPos - obsLayer->convertToWorldSpace(Vec2::ZERO);
    float radian = diff.getAngle();
    float degree = CC_RADIANS_TO_DEGREES(radian);
    if (degree < 0) {
        degree += 360;
    }
    
    //log("degree: %f", degree);
    
    addObstacleAt(degree);
#endif

    return true;
}

void PlayScene::onTouchMoved(Touch *touch, Event *event)
{
#ifdef LEVELDESIGN
    obsLayer->removeAllChildrenWithCleanup(false);
    obstacles.clear();
#endif
}

void PlayScene::onTouchEnded(Touch *touch, Event *event)
{
    if (status == StatusReady) {

        if (!UserInfo::getInstance()->getAchievementInfoWithType(AchievementType10)) {
            //
            // This should be first run!!!
            //
            UserInfo::getInstance()->setAchievementInfoWithType(AchievementType10);
        }

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        //
        // NOTE: exitPopuped is to prevent the situation that user touches the screen and back button at the same time.
        // exitPopuped should be initialized at the cancel-callback of exit popup!!!
        //
        if ( uiLayer->exitPopuped ) {
            return;
        }
#endif

        //
        SoundManager::getInstance()->playSoundEffect(SoundButton, false);

        setGameStatus(StatusIntro);
        startIntro(0);

        //
        FlurryX::getInstance()->logEvent("Play");
    }
}

void PlayScene::onTouchCancelled(Touch *touch, Event *event)
{
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void PlayScene::showPostInterstitial(Ref* pSender)
{
    if (UserInfo::getInstance()->getNoAdOption() != 1) {
        FirebaseX::getInstance()->hideBanner();
    }

//    int playCount = UserInfo::getInstance()->getPlayCount();
    if (UserInfo::getInstance()->getNoAdOption() == 1 || // user hided/removed ads
        UserInfo::getInstance()->getTotalPlayCount() < 8 || // first time user
        UserInfo::getInstance()->getBestScore() < 300) { //|| // poor user
//        (playCount>0 && playCount%2 != 0)) { // Every 2 times

        //
        UserInfo::getInstance()->hasShowAd = false;
        reloadPlayScene();

        return;
    }

    if (status == StatusPause || // pause -> no post ad
        status == StatusReady){
        //
        //
        UserInfo::getInstance()->hasShowAd = false;
        reloadPlayScene();
    } else {
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
        if (FirebaseX::getInstance()->isInterstitialReady()) {
            doAfterAdmobInterstitialCheck(true);
        }
        else {
            doAfterAdmobInterstitialCheck(false);
        }
#else
        FirebaseX::getInstance()->setDelegate(this);
        FirebaseX::getInstance()->checkInterstitialReady();
#endif
    }
}

void PlayScene::doAfterAdmobInterstitialCheck(bool isAdMobReady)
{
    bool showAd = false;

    //
    // NOTE: AdMob 우선, 다음 Chartboost
    //
    if (isAdMobReady) {
        //
        // AdMob
        //
        showAd = true;
        UserInfo::getInstance()->hasShowAd = true;
        FirebaseX::getInstance()->setDelegate(this);
        FirebaseX::getInstance()->showInterstitial();
    }
    else if (ChartboostX::getInstance()->hasCachedInterstitial("Main Menu")) {
        //
        // CHARTBOOST
        //
        showAd = true;
        UserInfo::getInstance()->hasShowAd = true;
        ChartboostX::getInstance()->showInterstitial("Main Menu");
    }
    else {
        showAd = false;
    }
    
    if (!showAd) {
        //
        UserInfo::getInstance()->hasShowAd = false;
        reloadPlayScene();
    }
}


void PlayScene::reloadPlayScene()
{
//    SoundManager::getInstance()->stopAllSoundEffect();
    SoundManager::getInstance()->stopBackgroundMusic();
    bgLayer->setTimonSoundoff();

    FirebaseX::getInstance()->setDelegate(nullptr);
    ChartboostX::getInstance()->setDelegate(nullptr);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    auto scene = LoadingScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.15, scene, Color3B(0,0,0)));
#else
    auto scene = PlayScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, scene, Color3B(0,0,0)));
#endif
}

void PlayScene::pauseScene(Ref* pSender)
{
    if (status == StatusReady || status == StatusGameOver) {
        return;
    }

    prevStatus = status;
    setGameStatus(StatusPause);

    worldLayer->pause();
    character->pauseAnimation();
    bgLayer->pauseAnimation();

    unschedule(CC_SCHEDULE_SELECTOR(PlayScene::moveCharacter));

//    SoundManager::getInstance()->pauseAllSoundEffect();

    //
    VideoShareX::getInstance()->pauseRecording();
}

void PlayScene::resumeScene(Ref* pSender)
{
    worldLayer->resume();
    character->resumeAnimation();
    bgLayer->resumeAnimation();

    setGameStatus(prevStatus);

    SoundManager::getInstance()->resumeAllSoundEffect();

    schedule(CC_SCHEDULE_SELECTOR(PlayScene::moveCharacter));
    
    //
    VideoShareX::getInstance()->resumeRecording();
}

void PlayScene::enterBackgroundPause()
{
    if (status == StatusPause || status == StatusReady || status == StatusGameOver || status == StatusAdTime || status == StatusAskContinue) {
        return;
    }

    SoundManager::getInstance()->pauseAllSoundEffect();

    uiLayer->forceToPauseGame();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark AdMob-related
void PlayScene::admobInterstitialReady(bool success)
{
    //
    // NOTE: 안드로이드의 경우 1초의 딜레이를 준다.
    // success여부에 따라 CallFunc을 다르게 설정함
    //
    auto callback = (success)? CallFunc::create([this](){
        this->doAfterAdmobInterstitialCheck(true);
    }) : CallFunc::create([this](){
        this->doAfterAdmobInterstitialCheck(false);
    });
    auto seq = Sequence::create(DelayTime::create(1.0),
                                callback, NULL);
    this->runAction(seq);
}

void PlayScene::admobInterstitialClosed()
{
    UserInfo::getInstance()->hasShowAd = true;
    continueGameLogic();
}

void PlayScene::interstitialDidFailToPresentScreen()
{
    if (!UserInfo::getInstance()->hasShowAd && status == StatusGameOver) {
        continueGameLogic();
    }
}

void PlayScene::interstitialWillLeaveApplication()
{
    setGameStatus(StatusAdTime);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Chartboost-related
void PlayScene::continueGameLogic()
{
    reloadPlayScene();
}

bool PlayScene::shouldDisplayInterstitial(const char* location)
{
//    log("about to display interstitial at location %s", location);
    
    if (status == StatusGameOver || status == StatusPause)
        return true;

    return false;
}

void PlayScene::didCacheInterstitial(const char* location)
{
//    log("did cache interstitial at location %s", location);
}

void PlayScene::didFailToLoadInterstitial(const char* location, int errorCode)
{
    //
    // NOTE: Called when FAILED TO SHOW or FAILED TO CACHE!
    //

//    log("did fail to load interstitial at location %s, %d", location, errorCode);
    if (!UserInfo::getInstance()->hasShowAd && status == StatusGameOver) {
//        log("didFailToLoadInterstitial : continueGameLogic");

        continueGameLogic();
    }
}

void PlayScene::didCloseInterstitial(const char* location)
{
//    log("did close interstitial at location %s", location);
    UserInfo::getInstance()->hasShowAd = true;
    continueGameLogic();
}

void PlayScene::didClickInterstitial(const char* location)
{
//    log("did click interstitial at location %s", location);

    setGameStatus(StatusAdTime);
}

void PlayScene::returnToForeground()
{
    if (status == StatusGameOver && uiLayer->visitFacebook) {
        //
        // GO TO MAIN!!!
        //
        reloadPlayScene();
    }
    else {
        //
        // Check Audio in Use
        //

        SoundManager::getInstance()->checkAudioInUseAndSetBackgroundMusic();

//        if (status != StatusReady && status != StatusGameOver) {
//            SoundManager::getInstance()->checkAudioInUseAndSetBackgroundMusic();
//        }
//        else {
//            SoundManager::getInstance()->checkAudioInUse();
//        }

        //
        if (status == StatusAdTime) {
            UserInfo::getInstance()->hasShowAd = true;
            continueGameLogic();
        }
        else {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            uiLayer->updateUIAfterWatchingAd();
#endif
        }
    }
}
