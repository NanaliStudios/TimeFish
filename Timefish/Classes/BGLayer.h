//
//  BGLayer.h
//  Timefish
//
//  Created by midchow on 2015. 1. 13..
//
//

#ifndef __Timefish__BGLayer__
#define __Timefish__BGLayer__

#include "cocos2d.h"

#include <spine/spine-cocos2dx.h>

#include "Global.h"

#include "SkeletonHolder.h"

#include "SoundManager.h"

USING_NS_CC;

class BGLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(BGLayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;

    //
    //
    //
    void initMainLayerWithColor(int _colorIdx);
    void setFinishCallback(const std::function<void()>& _finishCallback) {
        finishCallback = _finishCallback;
    }
    void setStartToCloeEyeCallback(const std::function<void()>& _callback) {
        startToCloeEyeCallback = _callback;
    }

    //
    //
    //
    void setTimonStart() {
        setAnimation(TimonAniOpen);
    }
    void setTimonRunMore() {
        setAnimation(TimonAniDown);
    }
    void setTimonToSlow(bool flag) {
        if (flag) {
            setAnimation(TimonAniStartSleep);
        }
        else {
            setAnimation(TimonAniEndSleep);
        }
    }

    void setTimonByDistance(float diff, int freeshLap, bool catched);

    void setNextBg(float keepAngle, bool catchFreesh = false);
    void setTimonSoundoff();

    //
    //
    //
    float getCurrDegree() {
        return currDegree;
    }
    int getCurrCircle() {
        return currCircle;
    }
    int getNextCircle() {
        return nextCircle;
    }
    
    int getCurrLap() {
        return laps;
    }
    FreeshColorSet getCurrColorSet() {
        ColorOrder next = (ColorOrder)nextCircle;
        FreeshColorSet colorSet = getFreeshColorSet(colorIdx, next);
        /*
         FreeshColorSet_Red = 0,
         FreeshColorSet_Blue,
         FreeshColorSet_Yellow,
         FreeshColorSet_EGreen,
         FreeshColorSet_Pink,
         FreeshColorSet_YGreen,
         //
         FreeshColorSet_DeepSea1,
         FreeshColorSet_DeepSea2,
         FreeshColorSet_DeepSea3,
         //
         FreeshColorSet_Ghost1,
         FreeshColorSet_Ghost2,
         FreeshColorSet_Ghost3,
         */
        /*
         switch (colorSet) {
         case FreeshColorSet_Red:
         log("color: FreeshColorSet_Red , next: %d", next);
         break;
         case FreeshColorSet_Yellow:
         log("color: FreeshColorSet_Yellow , next: %d", next);
         break;
         case FreeshColorSet_Blue:
         log("color: FreeshColorSet_Blue , next: %d", next);
         break;
         case FreeshColorSet_EGreen:
         log("color: FreeshColorSet_EGreen , next: %d", next);
         break;
         case FreeshColorSet_Pink:
         log("color: FreeshColorSet_Pink , next: %d", next);
         break;
         case FreeshColorSet_YGreen:
         log("color: FreeshColorSet_YGreen , next: %d", next);
         break;
         
         default:
         break;
         }
         */
        return colorSet;
    }

    //
    //
    //
    virtual bool isIdleState() {
        return (currAniStatus == TimonAniIdle)? true: false;
    }
    bool isMoveable() {
        return !(currAniStatus == TimonAniOpen || currAniStatus == TimonAniDie);
    }
    bool isTimonBlue() {
        return blued;
    }

    //
    //
    //
    void pauseAnimation() {
        sNode[0]->pause();
        sNode[1]->pause();
        sNode[2]->pause();
    }
    void resumeAnimation() {
        sNode[0]->resume();
        sNode[1]->resume();
        sNode[2]->resume();
    }

private:
    int colorIdx;

    bool blued;
    
    int currCircle, nextCircle;
    float currDegree;

    ProgressTimer *circle[4];

    Color3B colors[4], glowColors[4];

    Layer *movingLayer;
    Sprite *baseSprite, *baseGlowSprite;
    Sprite *movingSprite, *movingGlowSprite;

    int glowed;

    bool angleUpdated;
    
    float angle;
    float baseDelta;
    float delta;
    
    void setIdleAni();

    virtual void setAnimation(TimonAnimationType aniType);
    virtual void setTimonAnimationCompleteListers(int idx);
    virtual void setIdleAniAfterCountThree();
    virtual void setTimonToGlare() {
        setAnimation(TimonAniUp);
    }

protected:
    TimonAnimationType currAniStatus;
    spine::SkeletonAnimation *sNode[3];

    int eyeCount;
    int laps;
    
    int timonSoundId;
    float currentVolume;

    std::function<void()> finishCallback;
    std::function<void()> startToCloeEyeCallback;

    //
    void addEyes();
    void setOpenAni(float _t);
};

#endif /* defined(__Timefish__BGLayer__) */
