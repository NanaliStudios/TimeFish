//
//  BallLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 1. 14..
//
//

#include "BallLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

using namespace spine;

bool BallLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    //
    boosted = -1;

    return true;
}

//void BallLayer::initMainLayer(int type)
void BallLayer::initMainLayer(int _colorIdx, int type)
{
    colorIdx = _colorIdx;
    currType = type;
    
    collided = false;

    capsuleStatus = CapsuleIdle;
    
    for (int i=0; i<3; i++) {
        FreeshColorSet colorSet = getFreeshColorSet(colorIdx, (ColorOrder)i);
        
        {
            std::stringstream stream;
            stream << aniNameColorPrefix[colorSet] << "idle";
            idleAniNames[i] = stream.str();
        }
        
        {
            std::stringstream stream;
            stream << aniNameColorPrefix[colorSet] << "die";
            dieAniNames[i] = stream.str();
        }
    }

    //
    sNode = SkeletonAnimation::createWithData(SkeletonHolder::getInstance()->capsuleSkeleton);
    sNode->setDebugSlotsEnabled(false);
    sNode->setAnchorPoint(Vec2(0.5, 0.5));
    addChild(sNode);

    //
    sNode->setCompleteListener( [this] (int trackIndex, int loopCount) {
        if (capsuleStatus == CapsuleIdle) {
            // do nothing
        }
//        else if (capsuleStatus == CapsuleDied) {
//            this->setVisible(false);
//        }
    });


    /*
     Animations:
            B_die
            B_idle
            EG_die
            EG_idle
            P_die
            P_idle
            R_die
            R_idle
            YG_die
            YG_idle
            Y_die
            Y_idle
            skin_BackLight
     
     Skins:
             boost
             coin
             life
             point
             slow
    */

    //
    setIdleAnimation();

    //
    light = Sprite::createWithSpriteFrameName("TF_Tuto_light.png");
    light->setAnchorPoint(Vec2(0.5, 0));
    sNode->addChild(light, 10);

    setVisible(false);
}

void BallLayer::setAsBooster(int boostType)
{
    // return if same
    if (boosted == boostType) {
        return;
    }

//    log("setAsBooster from %d to %d", boosted, boostType);

    boosted = boostType;

    //
    // Default capsule
    //
    if (boostType == -1) {
        //
        // skin no icon
        //
        sNode->setSkin("empty");
    }
    //
    // Boster capsule
    //
    else if (boostType == 0) {
        // fever
        sNode->setSkin("boost");
    }
    else if (boostType == 1) {
        // coin
        sNode->setSkin("coin");
    }
    else if (boostType == 2) {
        // slow
        sNode->setSkin("slow");
    }
    else if (boostType == 3) {
        // point
        sNode->setSkin("point");
    }
    else if (boostType == 4) {
        // point
        sNode->setSkin("life");
    }
}

void BallLayer::setEffectRotation(float rotationDegree, float positionRadian)
{
    light->setRotation(rotationDegree); // degree starts from 12 o'clock and run clockwise
    positionRadian = positionRadian - M_PI; // degree starts from 3 o'clock and run counterclockwise
    light->setPosition(Vec2(cos(positionRadian),  sin(positionRadian)) * 35);
}

void BallLayer::setIdleAnimation()
{
    capsuleStatus = CapsuleIdle;

//    FreeshColorSet colorSet = getFreeshColorSet(colorIdx, (ColorOrder)currType);
//    std::stringstream stream;
//    stream << aniNameColorPrefix[colorSet] << "idle";
//    std::string aniName = stream.str();
//
//    sNode->setAnimation(0, aniName.c_str(), false);
    
    sNode->setAnimation(0, idleAniNames[currType], false);
}

#define CollisionSize 100
ObjectType BallLayer::checkCollision(Vec2 characterPos)
{
    if (collided) {
        return ObjectTypeNone;
    }

    Vec2 pos = getPosition();
    
//    log("%f,%f,%f,%f, %f", pos.x, pos.y, characterPos.x, characterPos.y, (characterPos - pos).length());
    //
    // TODO: For now, do simple distance check. Later, need to be accurate!
    //
    if ((characterPos - pos).length() < CollisionSize) {
        collided = true;
        setCollided();

        if (boosted != -1) {            
            //
            return (ObjectType)(ObjectTypeCapsule1 + boosted);
        }

        return ObjectTypeColorBall;
    }

    return ObjectTypeNone;
}

void BallLayer::setCollided()
{
    //
    stopLightBlinking();

    capsuleStatus = CapsuleDied;

//    FreeshColorSet colorSet = getFreeshColorSet(colorIdx, (ColorOrder)currType);
//    std::stringstream stream;
//    stream << aniNameColorPrefix[colorSet] << "die";
//    std::string aniName = stream.str();
//    sNode->setAnimation(0, aniName.c_str(), false);
    
    sNode->setAnimation(0, dieAniNames[currType], false);
}

void BallLayer::stopLightBlinking()
{
    //
    light->stopAllActions();
    light->setVisible(false);
}

void BallLayer::setFinish()
{
    setCollided();
}
