//
//  BGLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 1. 13..
//
//

#include "BGLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

using namespace spine;

// HEAD
#define LayerOrderTimonHeadGlow     50
#define LayerOrderTimonEye          40
#define LayerOrderTimonHead         30

// BODY
#define LayerOrderTimonBodyNext     25

// TAIL
#define LayerOrderTimonTailGlow     20
#define LayerOrderTimonTail         10

//
#define LayerOrderTimonBodyCurrent  5
#define LayerOrderTimonBodyHide     1

#define LayerOrderTimonBody1        3
#define LayerOrderTimonBody2        2
#define LayerOrderTimonBody3        1

bool BGLayer::init()
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

void BGLayer::initMainLayerWithColor(int _colorIdx)
{
    colorIdx = _colorIdx;

    //
    movingLayer = Layer::create();
    movingLayer->setAnchorPoint(Vec2(0,0));
    addChild(movingLayer, LayerOrderTimonEye);

    //
    // Set Glow Color, if needed
    //
    FreeshColorSet colorSet = getFreeshColorSet(colorIdx, ColorOrderFirst);
    if (colorSet == FreeshColorSet_DeepSea1) {
        glowed = 1;

        glowColors[0] = Color3B(196, 251, 236);
        glowColors[1] = Color3B(249, 200, 254);
        glowColors[2] = Color3B( 24,  47, 86);
    }
    else if (colorSet == FreeshColorSet_Ghost1) {
        glowed = 2;

        glowColors[0] = Color3B(255, 255, 255);
        glowColors[1] = Color3B( 67,  79,  80);
        glowColors[2] = Color3B( 89, 176, 187);
    }
    else if (colorSet == FreeshColorSet_Gold1) {
        glowed = 3;

        glowColors[0] = Color3B(255, 255, 164);
        glowColors[1] = Color3B(255, 210,  27);
        glowColors[2] = Color3B(229, 229, 229);
    }
    else {
        glowed = 0;
    }
    

    //
    colors[0] = getFreeshColor(colorIdx, ColorOrderFirst);
    colors[1] = getFreeshColor(colorIdx, ColorOrderSecond);
    colors[2] = getFreeshColor(colorIdx, ColorOrderThird);
    colors[3] = Color3B(3, 191, 255); // color for blued

    //
    blued = false;

    for (int i=0; i<4; i++) {
        //
        circle[i] = ProgressTimer::create(Sprite::createWithSpriteFrameName("white.png"));
        circle[i]->setType(ProgressTimer::Type::RADIAL);
        circle[i]->setColor(colors[i]);
        circle[i]->setPercentage(0);
        
        circle[i]->setScale(10); // !!! hard-coded
        addChild(circle[i], LayerOrderTimonBody1);
    }
    
    //
    //
    //
    baseSprite = Sprite::createWithSpriteFrameName("DS_monster.png");
    baseSprite->setAnchorPoint(Vec2(0.95,0));
    addChild(baseSprite, LayerOrderTimonTail);

    //
    //
    //
    movingSprite = Sprite::createWithSpriteFrameName("DS_monster.png");
    movingSprite->setAnchorPoint(Vec2(0,0));
    movingSprite->setFlippedX(true);
    addChild(movingSprite, LayerOrderTimonHead);
    
    //
    //
    //
    if (glowed > 0) {
        //
        baseGlowSprite = Sprite::createWithSpriteFrameName("Timon_glow.png");
        baseGlowSprite->setAnchorPoint(Vec2(0.95,0));
        addChild(baseGlowSprite, LayerOrderTimonTailGlow);

        //
        movingGlowSprite = Sprite::createWithSpriteFrameName("Timon_glow.png");
        movingGlowSprite->setAnchorPoint(Vec2(0,0));
        movingGlowSprite->setFlippedX(true);
        addChild(movingGlowSprite, LayerOrderTimonHeadGlow);
    }

    //
    currCircle = 2; // yellow
    nextCircle = 0; // red
    circle[currCircle]->setPercentage(100);
    circle[currCircle]->setLocalZOrder(LayerOrderTimonBodyCurrent);
    circle[nextCircle]->setLocalZOrder(LayerOrderTimonBodyNext);

    //
    currAniStatus = TimonAniNone;
    
    currDegree = 0;
    laps = 0;
    angle = -M_PI_2 * 0.875;
    angleUpdated = true;
    baseDelta = 100;
    delta = baseDelta + 1*laps;
    
    //
    timonSoundId = SoundManager::getInstance()->playTimonSoundEffect(colorIdx, 0, true);
    
    //
    currDegree = CC_RADIANS_TO_DEGREES(-angle);

    //
    baseSprite->setColor(colors[nextCircle]);
    if (baseGlowSprite) baseGlowSprite->setColor(glowColors[nextCircle]);

    movingLayer->setRotation(currDegree);

    //
    movingSprite->setColor(colors[nextCircle]);
    movingSprite->setRotation(currDegree);
    if (movingGlowSprite) {
        movingGlowSprite->setColor(glowColors[nextCircle]);
        movingGlowSprite->setRotation(currDegree);
    }
    
    //
    addEyes();
}

void BGLayer::addEyes()
{
    eyeCount = 0;
    
    Vec2 pos[] = {
        Vec2(5, 760),
        Vec2(5, 592),
        Vec2(5, 510)
    };

    for (int i=0; i<3; i++) {
        sNode[i] = SkeletonAnimation::createWithData(SkeletonHolder::getInstance()->timonSkeleton);
        sNode[i]->setDebugSlotsEnabled(false);
        sNode[i]->setVisible(false);
        sNode[i]->setRotation(180);
        sNode[i]->setPosition(pos[i]);
        movingLayer->addChild(sNode[i]);

        // Initialize CompleteListener
        setTimonAnimationCompleteListers(i);

    }
    
    // initial state
    currAniStatus = TimonAniOpen;
    setOpenAni(0);
}

void BGLayer::setTimonAnimationCompleteListers(int idx)
{
    sNode[idx]->setCompleteListener( [this] (int trackIndex, int loopCount) {
        if (currAniStatus == TimonAniOpen) {
            setIdleAniAfterCountThree();
        }
        else if (currAniStatus == TimonAniIdle) {
            //
            // do nothing
            //
        }
        else if (currAniStatus == TimonAniDie) {
            eyeCount++;
            if (eyeCount == 3 && finishCallback) {
                eyeCount = 0;
                finishCallback();
            }
        }
        else if (currAniStatus == TimonAniDown) {
            setIdleAniAfterCountThree();
        }
        else if (currAniStatus == TimonAniUp) {
            //
            // do nothing
            //
        }
        else if (currAniStatus == TimonAniStartSleep) {
            setAnimation(TimonAniSleepIdle);
        }
        else if (currAniStatus == TimonAniSleepIdle) {
            //
            // do nothing
            //
        }
        else if (currAniStatus == TimonAniEndSleep) {
            setIdleAni();
        }
    });
}

void BGLayer::setAnimation(TimonAnimationType aniType)
{
    if (currAniStatus == TimonAniDie) {
        return;
    }
    
    currAniStatus = aniType;

    if (aniType == TimonAniOpen) {
        setOpenAni(0.5);
    }
    else if (currAniStatus == TimonAniIdle) {
        setIdleAni();
    }
    else if (aniType == TimonAniDie) {
        for (int i=0; i<3; i++) {
            sNode[i]->setAnimation(0, "die", false);
        }
    }
    else if (aniType == TimonAniDown) {
        for (int i=0; i<3; i++) {
            sNode[i]->setAnimation(0, "scale_down", false);
        }
    }
    else if (aniType == TimonAniUp) {
        for (int i=0; i<3; i++) {
            sNode[i]->setAnimation(0, "scale_up", false);
        }
    }
    else if (currAniStatus == TimonAniStartSleep) {
        for (int i=0; i<3; i++) {
            sNode[i]->setAnimation(0, "sleep_start", false);
        }
    }
    else if (currAniStatus == TimonAniSleepIdle) {
        for (int i=0; i<3; i++) {
            sNode[i]->setAnimation(0, "sleep_idle", true);
        }
    }
    else if (currAniStatus == TimonAniEndSleep) {
        for (int i=0; i<3; i++) {
            sNode[i]->setAnimation(0, "sleep_end", false);
        }
    }
}

void BGLayer::setOpenAni(float _t)
{
    for (int i=0; i<3; i++) {
        sNode[i]->setAnimation(0, "open", false);
        sNode[i]->setTimeScale(_t);
        sNode[i]->setVisible(true);
    }
}

void BGLayer::setIdleAniAfterCountThree()
{
    eyeCount++;
    if (eyeCount >= 3) {
        eyeCount = 0;
        setAnimation(TimonAniIdle);
    }
}

void BGLayer::setIdleAni()
{
    if (currAniStatus == TimonAniIdle) {
        for (int i=0; i<3; i++) {
            sNode[i]->setTimeScale(1);
            sNode[i]->setAnimation(0, "idle0", true);
        }
    }
}

void BGLayer::setNextBg(float _angleVelocity, bool catchFreesh)
{
    angle -= _angleVelocity;
    currDegree = CC_RADIANS_TO_DEGREES(-angle);

    if (blued) {
        //
        //
        //
        if (currDegree > 360) {
            setAnimation(TimonAniDie);
            
            if (startToCloeEyeCallback) {
                startToCloeEyeCallback();
            }
        }
    }
    else {
        if (currDegree > 360) {

            //
            currDegree = currDegree - 360;
            
            //
            angle += M_PI * 2;

            circle[currCircle]->setLocalZOrder(LayerOrderTimonBodyHide);
            circle[nextCircle]->setPercentage(100);
            
            angleUpdated = false;
            laps++;
            
            //
            currCircle++;
            if (currCircle >= 3) {
                currCircle = 0;
            }
            nextCircle = currCircle + 1;
            if (nextCircle >= 3) {
                nextCircle = 0;
            }
            
            if (catchFreesh) {
                //
                // BLUED!!!
                //
                blued = true;
                nextCircle = 3;

                if (glowed > 0) {
                    baseGlowSprite->runAction(FadeOut::create(0.1));
                    movingGlowSprite->runAction(FadeOut::create(0.1));
                }
            }

            //
            baseSprite->setColor(colors[nextCircle]);
            if (baseGlowSprite) baseGlowSprite->setColor(glowColors[nextCircle]);

            movingSprite->setColor(colors[nextCircle]);
            if (movingGlowSprite) movingGlowSprite->setColor(glowColors[nextCircle]);
            circle[currCircle]->setLocalZOrder(LayerOrderTimonBodyCurrent);
            circle[nextCircle]->setLocalZOrder(LayerOrderTimonBodyNext);
            
            circle[nextCircle]->setPercentage(0);
        }
        else if (!angleUpdated && currDegree < 150) {
            angleUpdated = true;
        }
    }

    movingLayer->setRotation(currDegree);
    movingSprite->setRotation(currDegree);
    if (movingGlowSprite) movingGlowSprite->setRotation(currDegree);
    
    
    float percentage = (float)currDegree/360 * 100 + 0.2; // To prevent gaps between sprite and progressTimer, add 0.2
    circle[nextCircle]->setPercentage(percentage);
}

void BGLayer::setTimonByDistance(float diff, int freeshLap, bool catched)
{
    //
    // NOTE: To give a sound of certain amount of volume, even if timon is far enough,
    // use only indices 0~7
    //
    diff = MIN(diff, 79); // 0 ~ 79 degrees
    int idx = (int)diff/10; // 0 ~ 7
    float volumes[] = {1.000, 0.987, 0.951, 0.891, 0.809, 0.707, 0.587, 0.453, 0.309, 0.156}; // cos
    currentVolume = volumes[idx]; // scale up to 0.4
    if (timonSoundId != -1) {
        SoundManager::getInstance()->setSoundVolume(timonSoundId, currentVolume);
    }
    
    //
    if (!catched) {
        if (freeshLap == laps) {
            if (diff < 25 && isIdleState() ) {
                setTimonToGlare();
            }
            else if (diff > 25 && currAniStatus == TimonAniUp) {
                setAnimation(TimonAniDown);
            }
        }
    }
}

void BGLayer::setTimonSoundoff()
{
    if (timonSoundId != -1) {
        SoundManager::getInstance()->stopSoundEffect(timonSoundId);
    }
}
