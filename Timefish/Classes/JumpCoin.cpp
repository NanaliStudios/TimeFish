//
//  JumpCoin.cpp
//  Timefish
//
//  Created by midchow on 2015. 9. 15..
//
//

#include "JumpCoin.h"

#include <iostream>
#include <iomanip>
#include <string>

using namespace spine;

bool JumpCoin::init()
{
    if ( !Sprite::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    //
    initMainLayer();

    return true;
}

void JumpCoin::initMainLayer()
{
    sNode = SkeletonAnimation::createWithData(SkeletonHolder::getInstance()->obstacleSkeleton);
    sNode->setDebugSlotsEnabled(false);
    sNode->setVisible(false);
//    sNode->setAnimation(0, "coin_goldfish", false);
    addChild(sNode, 12);
}
