//
//  SleepyTimon.h
//  Timefish
//
//  Created by midchow on 2015. 8. 8..
//
//

#ifndef __Timefish__SleepyTimon__
#define __Timefish__SleepyTimon__

#include "cocos2d.h"

#include <spine/spine-cocos2dx.h>

#include "Global.h"

#include "SkeletonHolder.h"

#include "BGLayer.h"

USING_NS_CC;

class SleepyTimon : public BGLayer
{
public:
    virtual bool init();
    
    CREATE_FUNC(SleepyTimon);

    virtual bool isIdleState() {
        return (currAniStatus == TimonAniStartSleep ||
                currAniStatus == TimonAniSleepIdle)? true: false;
    }

private:
    virtual void setAnimation(TimonAnimationType aniType);
    virtual void setTimonAnimationCompleteListers(int idx);
    virtual void setIdleAniAfterCountThree();
    virtual void setTimonToGlare() {
        setAnimation(TimonAniEndSleep);
    }
};

#endif /* defined(__Timefish__SleepyTimon__) */
