//
//  SleepyTimon.cpp
//  Timefish
//
//  Created by midchow on 2015. 8. 8..
//
//

#include "SleepyTimon.h"

#include <iostream>
#include <iomanip>
#include <string>

bool SleepyTimon::init()
{
    if ( !BGLayer::init() )
    {
        return false;
    }
    
    return true;
}

void SleepyTimon::setTimonAnimationCompleteListers(int idx)
{
    sNode[idx]->setCompleteListener( [this] (int trackIndex, int loopCount) {
        if (currAniStatus == TimonAniOpen) {
            setIdleAniAfterCountThree();
        }
        else if (currAniStatus == TimonAniDie) {
            eyeCount++;
            if (eyeCount == 3 && finishCallback) {
                eyeCount = 0;
                finishCallback();
            }
        }
        else if (currAniStatus == TimonAniDown) {
            setIdleAniAfterCountThree();
        }
        else if (currAniStatus == TimonAniUp) {
            //
            // do nothing
            //
        }
        else if (currAniStatus == TimonAniStartSleep) {
            setAnimation(TimonAniSleepIdle);
        }
        else if (currAniStatus == TimonAniSleepIdle) {
            //
            // do nothing
            //
        }
        else if (currAniStatus == TimonAniEndSleep) {
            setAnimation(TimonAniUp);
        }
    });
}

void SleepyTimon::setAnimation(TimonAnimationType aniType)
{
    if (currAniStatus == TimonAniDie) {
        return;
    }
    
    currAniStatus = aniType;
    
    if (aniType == TimonAniOpen) {
        setOpenAni(0.5);
    }
    else if (aniType == TimonAniDie) {
        for (int i=0; i<3; i++) {
            sNode[i]->setAnimation(0, "die", false);
        }
    }
    else if (aniType == TimonAniDown) {
        for (int i=0; i<3; i++) {
            sNode[i]->setAnimation(0, "scale_down", false);
        }
    }
    else if (aniType == TimonAniUp) {
        for (int i=0; i<3; i++) {
            sNode[i]->setAnimation(0, "scale_up", false);
        }
    }
    else if (currAniStatus == TimonAniStartSleep) {
        for (int i=0; i<3; i++) {
            sNode[i]->setAnimation(0, "sleep_start", false);
        }
    }
    else if (currAniStatus == TimonAniSleepIdle) {
        for (int i=0; i<3; i++) {
            sNode[i]->setAnimation(0, "sleep_idle", true);
        }
    }
    else if (currAniStatus == TimonAniEndSleep) {
        for (int i=0; i<3; i++) {
            sNode[i]->setAnimation(0, "sleep_end", false);
        }
    }
}

void SleepyTimon::setIdleAniAfterCountThree()
{
    eyeCount++;
    if (eyeCount >= 3) {
        eyeCount = 0;
        setAnimation(TimonAniStartSleep);
    }
}

