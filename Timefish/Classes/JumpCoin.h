//
//  JumpCoin.h
//  Timefish
//
//  Created by midchow on 2015. 9. 15..
//
//

#ifndef __Timefish__JumpCoin__
#define __Timefish__JumpCoin__

#include "cocos2d.h"

#include <spine/spine-cocos2dx.h>

#include "Global.h"
#include "SkeletonHolder.h"

USING_NS_CC;

using namespace spine;

class JumpCoin : public Sprite
{
public:
    virtual bool init() override;
    
    CREATE_FUNC(JumpCoin);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;
    
    void initMainLayer();
    void showJumpCoin() {
        sNode->setVisible(true);
        sNode->setAnimation(0, "coin_goldfish", false);
    }
    void setCoinRotation(float r) {
        sNode->setRotation(r);
    }

private:

    SkeletonAnimation *sNode;

};

#endif /* defined(__Timefish__JumpCoin__) */
