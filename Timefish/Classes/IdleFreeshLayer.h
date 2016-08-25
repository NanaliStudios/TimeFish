//
//  IdleFreeshLayer.h
//  Timefish
//
//  Created by midchow on 2015. 7. 30..
//
//

#ifndef __Timefish__IdleFreeshLayer__
#define __Timefish__IdleFreeshLayer__

#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>

#include <iostream>
#include <iomanip>
#include <string>

#include "SoundManager.h"

#include "Global.h"
#include "SkeletonHolder.h"

USING_NS_CC;

using namespace spine;

class IdleFreeshLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(IdleFreeshLayer);

    static IdleFreeshLayer* getIdleFreeshSkin(int skinNum);

    void initFreeshWithSkeleton(FreeshType type, int skinNum, spSkeletonData *fishSkeleton);
    void initRandomSelectionFreesh();

    void pauseAnimation() {
        this->setVisible(false);
        sNode->setToSetupPose();
        sNode->pause();
        if (emitter) {
            emitter->pause();
        }
    }
    void resumeAnimation() {
        this->setVisible(true);
        sNode->resume();
        if (emitter) {
            emitter->resume();
        }
    }

    void setLocked();
    void setHidden();
    void setSelected(bool flag);
    void setScaleForPackage() {
        sNode->setScale(0.85);
    }

    FreeshType getCurrSkinType() { return currSkinType; }
    int getCurrSkinNumber() { return currSkinNumber; }
    void showParticle(bool flag);
    
private:
    FreeshType currSkinType;
    int currSkinNumber;

    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;

    SkeletonAnimation *sNode;
    
    ParticleSystemQuad *emitter;

    void initParticles();

    bool isThisSpecialTypeFish() {
        if (currSkinType == FreeshTypeSpecial ||
            currSkinType == FreeshTypeSpecialHidden ||
            currSkinType == FreeshTypeGoldfish) {
            return true;
        }
        
        return false;
    }
};

#endif /* defined(__Timefish__IdleFreeshLayer__) */
