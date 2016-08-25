//
//  FishLayer.h
//  Timefish
//
//  Created by midchow on 2015. 1. 13..
//
//

#ifndef __Timefish__FishLayer__
#define __Timefish__FishLayer__

#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>

#include "SkeletonHolder.h"
#include "SoundManager.h"

#include "Global.h"

#define BubbleParticles   5

USING_NS_CC;

using namespace spine;

class mySkeletonAnimation : public SkeletonAnimation
{
public:
    static mySkeletonAnimation* createWithData (spSkeletonData* skeletonData, bool ownsSkeletonData = false);

    void setLocked();

private:
    GLProgramState *_glprogramstate;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    std::string _fragSource;
    EventListenerCustom* _backgroundListener;
#endif
    
protected:
    mySkeletonAnimation(spSkeletonData *skeletonData, bool ownsSkeletonData);
};

class FishLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(FishLayer);

//    void initMainLayer(int type);
    void initFreeshWithSkinType(FreeshType skinType, int _currSkin);
    
    void setFishOpacity(float _opacity) {
        sNode->setOpacity(_opacity);
    }

    void runFishFadeOut() {
        auto seq = Sequence::create(
                                    FadeOut::create(0.1),
                                    CallFunc::create( CC_CALLBACK_0(FishLayer::removeFromParent,this)),
                                    NULL);
        sNode->runAction(seq);
    }

    void setFishRotation(float r) {
        sNode->setRotation(r);
    }

    int getCurrType() {
        return currType;
    }
    
//    void setNextType();
    void setType(int type);
    
    void setAnglePosition(float radian) {
        //
        // radian starts from 3 o'clock and move counterclock wise
        // angle starts from 12 o'clock and move clock wise
        //
        angle = 90 - CC_RADIANS_TO_DEGREES(radian);
        
        if (angle > 360) {
            angle -= 360;
        }
        else if (angle < 0) {
            angle += 360;
        }
    }
    float getAnglePosition() {
        return angle;
    }

    int getCurrLap() {
        return currLap;
    }

    void increaseCurrLap() {
        currLap++;
    }
    
    void setShield(bool flag) {
        if (flag) {
            shieldMark->setVisible(true);
            shieldMark->setAnimation(0, "idle", true);
        }
        else {
            shieldMark->setAnimation(0, "die2", false);
        }
    }

    void setAnimation(AnimationType aniType);
    AnimationType getCurrAniStatus() {
        return currAniStatus;
    }
    
    void setOpenAniCallback(const std::function<void(Ref*)>& _startCallback) {
        startCallback = _startCallback;
    }
    void setFinishStartAniCallback(const std::function<void()>& _callback) {
        finishStartAniCallback = _callback;
    }
//    void setDieAniCallback(const std::function<void(Ref*)>& _finishCallback) {
//        finishCallback = _finishCallback;
//    }
    
    void pauseAnimation() {
        sNode->pause();
        emitter[bubbleParticlePtr]->pause();
    }
    void resumeAnimation() {
        sNode->resume();
        emitter[bubbleParticlePtr]->resume();
    }

    void setPlayableScale() {
        sNode->setScale(0.81);
    }

    FreeshType getCurrSkinType() { return currSkinType; }
    int getCurrSkinNumber() { return currSkinNumber; }

private:
    int currType;
    int currLap;

    FreeshType currSkinType;
    int currSkinNumber;
    int colorIdx;

    AnimationType currAniStatus;
    
    float angle;
    float watchRadius;

    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;

    Color4F colors[3];
    
    std::function<void(Ref*)> startCallback;
    std::function<void()> finishStartAniCallback;
//    std::function<void(Ref*)> finishCallback;

    SkeletonAnimation *sNode;

    SkeletonAnimation *shieldMark;

    void hideParticle();
    void showParticle();
    
    ParticleSystemQuad *emitter[BubbleParticles];
    int bubbleParticlePtr;
    void initParticles();

    void setParticleColor();
    
    void callStartCallback(int trackIndex, int loopCount);
//    void callFinishCallback(int trackIndex, int loopCount);
    
    bool isThisSpecialTypeFish() {
        if (currSkinType == FreeshTypeSpecial ||
            currSkinType == FreeshTypeSpecialHidden ||
            currSkinType == FreeshTypeGoldfish) {
            return true;
        }

        return false;
    }
};

#endif /* defined(__Timefish__FishLayer__) */
