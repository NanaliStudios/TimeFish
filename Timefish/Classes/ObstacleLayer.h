//
//  ObstacleLayer.h
//  Timefish
//
//  Created by midchow on 2015. 1. 14..
//
//

#ifndef __Timefish__ObstacleLayer__
#define __Timefish__ObstacleLayer__

#include "cocos2d.h"

#include "Global.h"
#include "Obstacle.h"

USING_NS_CC;

#define ObstaclePoolSize 20

class ObstacleLayer : public Layer
{
public:
    virtual bool init();

    CREATE_FUNC(ObstacleLayer);

    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;

    void initObstaclesWithRadius(float watchRadius, int _colorIdx);
    void reinitObstacles();

    void makeObstaclesToCoin(int amount);

    void setObstacleAt(float degree, int nextColor, bool doubleObs, bool asCoin = false);
    void _setObstacleAt(float degree, int nextColor, bool asCoin = false);
    void setObstacleDisappear(float currDegree);
    
    bool testGetClosestObstacle(float currDegree);

    ObjectType checkCollisions(Vec2 characterPos, bool blinking, bool isThisFeverCollision);

    int getCollidedObstacleIdx() {
        return collidedObstacleIdx;
    }
    
    int getRestCoins() {
        return restNumToMakeCoin;
    }

private:
    Vec2 basePos;
    float obsRadius, coinRadius;

    int collidedObstacleIdx;
    
    int restNumToMakeCoin;

    Vector<Obstacle*> obstacles;

    //
    // Pool pointer : represents last using obstacle's index
    //
    // init value is -1.
    // make sure +1 before using.
    //
    int poolPtr;
    void initPoolPointer() {
        poolPtr = -1;
        nextObsIndex = -1;
    }
    
    int nextObsIndex;
};

#endif /* defined(__Timefish__ObstacleLayer__) */
