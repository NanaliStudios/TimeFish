//
//  SkeletonHolder.h
//  Timefish
//
//  Created by midchow on 2015. 7. 30..
//
//

#ifndef __Timefish__SkeletonHolder__
#define __Timefish__SkeletonHolder__

#include "cocos2d.h"

#include <spine/spine-cocos2dx.h>

#include "Global.h"

USING_NS_CC;

class SkeletonHolder
{
public:
    static SkeletonHolder* getInstance();
    
    ~SkeletonHolder();

    // Freesh Skeleton for run
    spSkeletonData *basicFreeshSkeleton;
    spSkeletonData *etcFreeshSkeleton;
    spSkeletonData *historyFreeshSkeleton;
    spSkeletonData *jobFreeshSkeleton;
    spSkeletonData *parodyFreeshSkeleton;
    spSkeletonData *retroFreeshSkeleton;
    spSkeletonData *snapFreeshSkeleton;
    spSkeletonData *specialFreeshSkeleton;
    spSkeletonData *goldFreeshSkeleton;

    //
    spSkeletonData *capsuleSkeleton;
    spSkeletonData *obstacleSkeleton;
    spSkeletonData *fingerSkeleton;
    spSkeletonData *shieldSkeleton;
    spSkeletonData *lightSkeleton;
    spSkeletonData* timonSkeleton;
    
    //
    spSkeletonData *giftSkeleton;
    spSkeletonData *drawSkeleton;

    // Freesh Skeleton for idle
    spSkeletonData *randomFreeshSkeleton;
    spSkeletonData *basicFreeshIdleSkeleton;
    spSkeletonData *etcFreeshIdleSkeleton;
    spSkeletonData *historyFreeshIdleSkeleton;
    spSkeletonData *jobFreeshIdleSkeleton;
    spSkeletonData *parodyFreeshIdleSkeleton;
    spSkeletonData *retroFreeshIdleSkeleton;
    spSkeletonData *snapFreeshIdleSkeleton;
    spSkeletonData *specialFreeshIdleSkeleton;
    spSkeletonData *goldFreeshIdleSkeleton;

    void initSkeletons();
    
    spSkeletonData* getSkeletonBySkinType(FreeshType skinType);
    spSkeletonData* getIdelSkeletonBySkinType(FreeshType skinType);
};

#endif /* defined(__Timefish__SkeletonHolder__) */
