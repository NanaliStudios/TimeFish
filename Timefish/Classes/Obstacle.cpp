//
//  Obstacle.cpp
//  Timefish
//
//  Created by midchow on 2015. 6. 4..
//
//

#include "Obstacle.h"

#include <iostream>
#include <iomanip>
#include <string>

using namespace spine;

bool Obstacle::init()
{
    if ( !Sprite::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    collided = false;

    return true;
}

void Obstacle::initMainLayer(int type, int _colorIdx)
{
    currType = type;
    colorIdx = _colorIdx;

    //
    // Initialize colors
    //
    colors[0] = getFreeshColor(colorIdx, ColorOrderFirst);
    colors[1] = getFreeshColor(colorIdx, ColorOrderSecond);
    colors[2] = getFreeshColor(colorIdx, ColorOrderThird);
    
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

    /*
     Animations:
             coin_die
             coin_idle
             skin_BackLight
             trap_die_boost
             trap_die_nomal
             trap_idle
     */

    sNode = SkeletonAnimation::createWithData(SkeletonHolder::getInstance()->obstacleSkeleton);
    sNode->setDebugSlotsEnabled(false);
    sNode->setAnchorPoint(Vec2(0.5, 0.5));
    addChild(sNode, 1);

    sNode->setAnimation(0, "trap_idle", true);
    sNode->setColor(colors[currType]);

    obsType = ObjectTypeObstacle;
    
    // glow
    if (glowed > 0) {
        obsGlow = Sprite::createWithSpriteFrameName("TF_trap_glow.png");
        obsGlow->setPosition(Vec2(0, 2));
        addChild(obsGlow, 2);
    }
}

void Obstacle::makeToCoin()
{
    obsType = ObjectTypeCoin;
    sNode->setAnimation(0, "coin_idle", true);
    sNode->setColor(Color3B(255, 255, 255));
    sNode->setScale(0.9);
    if (obsGlow) {
        obsGlow->setVisible(false);
    }
}

#define CollisionSize 50
bool Obstacle::checkCollision(Vec2 characterPos)
{
    Vec2 pos = getPosition();
    
//    log("%f,%f,%f,%f, %f", pos.x, pos.y, characterPos.x, characterPos.y, (characterPos - pos).length());
    
    //
    // TODO: For now, do simple distance check.
    // Does it need to be accurate???
    //
    if ((characterPos - pos).length() < 80) {
        return true;
    }
    
//    //
//    Vec2 _one = Vec2::ONE;
//    _one.normalize();
//    float degree = -getRotation();
//
//    Vec2 btmLeftPos = (pos - _one * s.width/2 * 1.4142136);
//    btmLeftPos.rotate(pos, CC_DEGREES_TO_RADIANS(degree));
//
//    Vec2 btmRightPos = btmLeftPos;
//    btmRightPos.rotate(pos, CC_DEGREES_TO_RADIANS(90));
//
//    Vec2 topRightPos = btmLeftPos;
//    topRightPos.rotate(pos, CC_DEGREES_TO_RADIANS(180));
//
//    Vec2 topLeftPos = btmLeftPos;
//    topLeftPos.rotate(pos, CC_DEGREES_TO_RADIANS(-90));
//
//    bool c = false;
//    // collision check with top-right corner
//    Vec2 diff = topRightPos - characterPos;
//    if (diff.length() < CollisionSize) {
//        c = true;
//    }
//
//    // collision check with center-right corner
//    Vec2 centerRightPos = btmRightPos + (topRightPos - btmRightPos)*0.5;
//    diff = centerRightPos - characterPos;
//    if (diff.length() < CollisionSize) {
//        c = true;
//    }
//
//    // collision check with top-left corner
//    diff = topLeftPos - characterPos;
//    if (diff.length() < CollisionSize) {
//        c = true;
//    }
//
//    // collision check with top-center corner
//    Vec2 topCenterPos = topRightPos + (topLeftPos - topRightPos)*0.5;
//    diff = topCenterPos - characterPos;
//    if (diff.length() < CollisionSize) {
//        c = true;
//    }
//
//    // collision check with bottom-left corner
//    diff = btmLeftPos - characterPos;
//    if (diff.length() < CollisionSize) {
//        c = true;
//    }
//
//    if (c) {
//        setCollided();
//        return true;
//    }
    
    return false;
}

void Obstacle::setDisappear()
{
//    sNode->runAction(FadeOut::create(0.25));

    if (obsType == ObjectTypeCoin) {
        sNode->runAction(FadeOut::create(0.01));
    }
    else {
        sNode->setColor(Color3B(3, 191, 255));
    }
    
    if (glowed > 0 && obsGlow) {
        obsGlow->setVisible(false);
    }
    
    obsType = ObjectTypeNone;
}

ObjectType Obstacle::setCollidedWith(bool blinking, bool isThisFeverCollision)
{
    collided = true;

    //
    // NOTE: when freesh is blinking, ignore the collision with obstacle!
    // but need to check the collision with coin!
    //
    if (!blinking && obsType == ObjectTypeObstacle) {
        
        if (isThisFeverCollision) {
            sNode->setAnimation(0, "trap_die_boost", false);
        }
        else {
            sNode->setAnimation(0, "trap_die_nomal", false);
        }
        
        if (obsGlow) {
            obsGlow->setVisible(false);
        }

        return ObjectTypeObstacle;
    }
    else if (obsType == ObjectTypeCoin) {
        sNode->setAnimation(0, "coin_die", false);
        
        return ObjectTypeCoin;
    }
    
    return ObjectTypeNone;
}

void Obstacle::reinitSprite()
{
    collided = false;

    obsType = ObjectTypeObstacle;
    
    sNode->setAnimation(0, "trap_idle", true);
    sNode->setScale(1.0);
    
    if (glowed > 0 && obsGlow) {
        obsGlow->setVisible(true);
    }
}
