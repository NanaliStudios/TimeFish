//
//  Obstacle.h
//  Timefish
//
//  Created by midchow on 2015. 6. 4..
//
//

#ifndef __Timefish__Obstacle__
#define __Timefish__Obstacle__

#include "cocos2d.h"

#include <spine/spine-cocos2dx.h>

#include "Global.h"
#include "SkeletonHolder.h"

USING_NS_CC;

class Obstacle : public Sprite
{
public:
    virtual bool init() override;
    
    CREATE_FUNC(Obstacle);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;
    
    void initMainLayer(int type, int _colorIdx);
    
    void setAnglePosition(float degree) {
        //
        // degree starts from 3 o'clock and move counterclock wise
        // angle starts from 12 o'clock and move clock wise
        //
        angle = 90 - degree;
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
    
    void setSpriteType(int type) {
        if (collided) {
            return;
        }
        
        if (type >= 3) {
            type = 0;
        }
        
        currType = type;

        //
        sNode->setColor(colors[currType]);

        if (glowed > 0 && obsGlow) {
            obsGlow->setColor(glowColors[currType]);
        }
    }
    
    bool getCollided() {
        return collided;
    }
    
    bool checkCollision(Vec2 characterPos);
    
    ObjectType setCollidedWith(bool blinking, bool isThisFeverCollision);
    
    void reinitSprite();
    void setValid(bool flag) {
        valid = flag;
    }
    bool isValid() {
        return valid;
    }

    void makeToCoin();
    
//    bool isThisVisibleObstacle() {
//        return (obstacle->getOpacity() == 255 && obstacle->isVisible());
//    }
    int getCurrType() {
        return currType;
    }
    
    ObjectType getObjectType() {
        return obsType;
    }
    
    void setDisappear();

private:
    float watchRadius;
    float angle;
    
    int currType;
    int colorIdx;

    ObjectType obsType;
    
    bool collided;
    bool valid;

    spine::SkeletonAnimation *sNode;

    Color3B colors[3];

    int glowed;
    Color3B glowColors[3];
    Sprite *obsGlow;
};

#endif /* defined(__Timefish__Obstacle__) */
