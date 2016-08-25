//
//  ObstacleLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 1. 14..
//
//

#include "ObstacleLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

bool ObstacleLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    collidedObstacleIdx = -1;
    restNumToMakeCoin = 0;

    initPoolPointer();

    return true;
}

void ObstacleLayer::initObstaclesWithRadius(float _watchRadius, int _colorIdx)
{
    //
    // init variables...
    //
    basePos =  Vec2::ZERO;

    obsRadius = _watchRadius - 53;
    coinRadius = obsRadius - 30;

    //
    // generate obstacle pool
    //
    float degreeDelta = 360.0f / ObstaclePoolSize;
    for (int i=0; i<ObstaclePoolSize; i++) {
        float degree = degreeDelta * i;

        //
        // Create One Obstacle
        //
        Obstacle *obs = Obstacle::create();
        
        obs->initMainLayer(0, _colorIdx);
        
        float radian = CC_DEGREES_TO_RADIANS(degree);
        Vec2 pos = Vec2(cosf(radian), sinf(radian))* obsRadius + basePos;
        obs->setPosition(pos);
        obs->setAnglePosition(degree);
        obs->setVisible(false);
        obs->setValid(false);
        addChild(obs, 0);

        obstacles.pushBack(obs);
        
        //
        // set object rotation
        //
        {
            Vec2 diff = basePos - pos;
            float radian = diff.getAngle();
            float degree = get12OclockDegreeFrom3OclockRadian(radian);
            obs->setRotation(degree);
        }
    }
}

void ObstacleLayer::makeObstaclesToCoin(int amount)
{
    restNumToMakeCoin = amount;
}

//void ObstacleLayer::setObstacleAt(float degree, int nextColor, bool asCoin)
//{
//    poolPtr++;
//    if (poolPtr >= ObstaclePoolSize) {
//        //
//        // Note: Assume we have enough obstacles,
//        // so that we don't create obstacle at main loop.
//        //
//        poolPtr = 0;
//    }
//
//    //
//    // get next obstacle available
//    //
//    Obstacle *obs = obstacles.at(poolPtr);
//
//    //
//    // initialize!
//    //
//    obs->reinitSprite();
//    obs->setVisible(true);
//    obs->setValid(true);
//    obs->setSpriteType(nextColor + 1); // next-next color
//
//    //
//    // make coin or obstacle
//    //
//    Vec2 pos = Vec2::ZERO;
//    float radian = CC_DEGREES_TO_RADIANS(degree);
//    obs->setAnglePosition(degree);
//    if (asCoin || restNumToMakeCoin > 0) {
//        //
//        // coin
//        //
//        pos = Vec2(cosf(radian), sinf(radian))* coinRadius + basePos;
//        obs->makeToCoin();
//        restNumToMakeCoin--;
//    }
//    else {
//        //
//        // obstacle
//        //
//        pos = Vec2(cosf(radian), sinf(radian))* obsRadius + basePos;
//    }
//    obs->setPosition(pos);
//
//    //
//    // set object rotation
//    //
//    {
//        Vec2 diff = basePos - pos;
//        float radian = diff.getAngle();
//        float degree = get12OclockDegreeFrom3OclockRadian(radian);
//        obs->setRotation(degree);
//    }
//}

#define DegreeVariation 2.5f
void ObstacleLayer::setObstacleAt(float degree, int nextColor, bool doubleObs, bool asCoin)
{
    bool makeAsCoin = (asCoin || restNumToMakeCoin > 0);
    if (doubleObs) {
        _setObstacleAt(degree - DegreeVariation, nextColor, makeAsCoin);
        _setObstacleAt(degree + DegreeVariation, nextColor, makeAsCoin);
        
        if (makeAsCoin) {
            restNumToMakeCoin++;
        }
    }
    else {
        _setObstacleAt(degree, nextColor, makeAsCoin);
    }
}

void ObstacleLayer::_setObstacleAt(float degree, int nextColor, bool asCoin)
{
    poolPtr++;
    if (poolPtr >= ObstaclePoolSize) {
        //
        // Note: Assume we have enough obstacles,
        // so that we don't create obstacle at main loop.
        //
        poolPtr = 0;
    }

    //
    // get next obstacle available
    //
    Obstacle *obs = obstacles.at(poolPtr);
    //
    // initialize!
    //
    obs->reinitSprite();
    obs->setVisible(true);
    obs->setValid(true);
    obs->setSpriteType(nextColor + 1); // next-next color
    Vec2 pos = Vec2::ZERO;
    float radian = CC_DEGREES_TO_RADIANS(degree);
    obs->setAnglePosition(degree);
    if (asCoin) {
        //
        // coin
        //
        pos = Vec2(cosf(radian), sinf(radian))* coinRadius + basePos;
        obs->makeToCoin();
        restNumToMakeCoin--;
    }
    else {
        //
        // obstacle
        //
        pos = Vec2(cosf(radian), sinf(radian))* obsRadius + basePos;
    }
    obs->setPosition(pos);

    //
    // set object rotation
    //
    {
        Vec2 diff = basePos - pos;
        float radian = diff.getAngle();
        float degree = get12OclockDegreeFrom3OclockRadian(radian);
        obs->setRotation(degree);
    }
}


void ObstacleLayer::reinitObstacles()
{
    for (int i=0; i<ObstaclePoolSize; i++) {
        Obstacle *obs = obstacles.at(i);
        obs->setVisible(false);
        obs->setValid(false);
    }

    //
    initPoolPointer();
}

void ObstacleLayer::setObstacleDisappear(float currDegree)
{
    //
    // NOTE: loop till poolPtr!!!
    //
    for (int i=0; i<=poolPtr; i++) {
        Obstacle *obs = obstacles.at(i);
        float degree = obs->getAnglePosition();
        if (degree <= currDegree) {
            obs->setDisappear();
        }
    }
}

bool ObstacleLayer::testGetClosestObstacle(float currDegree)
{
    for (int i=poolPtr; i>=0; i--) {
        Obstacle *obs = obstacles.at(i);
        float degree = obs->getAnglePosition();
        float delta = degree - currDegree;
        if (obs->getObjectType() == ObjectTypeObstacle && delta > 0  && delta < 7) {
            return true;
        }
    }
    
    return false;
}

ObjectType ObstacleLayer::checkCollisions(Vec2 characterPos, bool blinking, bool isThisFeverCollision)
{
    //
    // loop till poolPtr!!!
    //
    // TODO: We don't need to check all the obstacles.
    // check only two obstacles within characterPos +1 and -1.
    //
    for (int i=0; i<=poolPtr; i++) {
        Obstacle *obs = obstacles.at(i);
        if (obs->isValid() && !obs->getCollided() && obs->checkCollision(characterPos)) {
            //
            ObjectType obsType = obs->setCollidedWith(blinking, isThisFeverCollision);
            
            collidedObstacleIdx = i;

            return obsType;
        };
    }
    return ObjectTypeNone;
}
