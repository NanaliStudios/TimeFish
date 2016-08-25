//
//  BallLayer.h
//  Timefish
//
//  Created by midchow on 2015. 1. 14..
//
//

#ifndef __Timefish__BallLayer__
#define __Timefish__BallLayer__

#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>

#include "Global.h"
#include "SkeletonHolder.h"

USING_NS_CC;

#define CapsuleIdle 0
#define CapsuleDied 1

class BallLayer : public Layer
{
public:
    virtual bool init();

    CREATE_FUNC(BallLayer);

    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;

//    void initMainLayer(int type);
    void initMainLayer(int _colorIdx, int type);

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
    
    ObjectType checkCollision(Vec2 characterPos);
    
    int getCurrType() {
        return currType;
    }

    void setCurrType(int type) {
        collided = false;
        currType = type;
        setVisible(true);
        setIdleAnimation();

        light->setVisible(true);
        light->runAction(RepeatForever::create(Blink::create(2, 10)));
    }
    
    void setEffectRotation(float rotationDegree, float positionRadian);

    void setFinish();

    void setAsBooster(int boostType);

private:
    float angle;
    int currType;
    bool collided;

    int colorIdx;

    int capsuleStatus;
    
    std::string idleAniNames[3];
    std::string dieAniNames[3];

    spine::SkeletonAnimation *sNode;
    Sprite *light;

    int boosted;

    void setCollided();
    void setIdleAnimation();
    
    void stopLightBlinking();
};

#endif /* defined(__Timefish__BallLayer__) */
