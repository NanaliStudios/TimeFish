//
//  IdleFreeshLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 7. 30..
//
//

#include "IdleFreeshLayer.h"

bool IdleFreeshLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

IdleFreeshLayer* IdleFreeshLayer::getIdleFreeshSkin(int skinNum)
{
    IdleFreeshLayer *freeshSkin = IdleFreeshLayer::create();

    FreeshType skinType = UserInfo::getInstance()->getSkinTypeBySkinNumber(skinNum);
    spSkeletonData *currSkeleton = SkeletonHolder::getInstance()->getIdelSkeletonBySkinType(skinType);
    freeshSkin->initFreeshWithSkeleton(skinType, skinNum, currSkeleton);
//    freeshSkin->setSelected(true);

    freeshSkin->retain();
    
    return freeshSkin;
}

void IdleFreeshLayer::initFreeshWithSkeleton(FreeshType type, int skinNum, spSkeletonData *fishSkeleton)
{
    currSkinType = type;
    currSkinNumber = skinNum;

    //
    sNode = SkeletonAnimation::createWithData(fishSkeleton);
    sNode->setDebugSlotsEnabled(false);
    sNode->setPosition(Vec2::ZERO);
    addChild(sNode, 10);

    //
    if (isThisSpecialTypeFish()) {
        sNode->setAnimation(0, "idle", true);

        //
        int idx = currSkinNumber -1;
        sNode->setSkin(getSpecialSkinName(idx));
    }
    else {
        FreeshColorSet colorSet = getFreeshColorSet(currSkinNumber - 1, ColorOrderFirst);
        std::stringstream stream;
        stream << aniNameColorPrefix[colorSet] << "idle";
        std::string aniName = stream.str();
        sNode->setAnimation(0, aniName.c_str(), true);

        //
        int idx = currSkinNumber -1;
        sNode->setSkin(getNormalSkinName(idx));
    }
    
    //
    initParticles();
}

void IdleFreeshLayer::initRandomSelectionFreesh()
{
    currSkinType = FreeshTypeRandom;
    currSkinNumber = 0;
    
    //
    spSkeletonData *currSkeleton = SkeletonHolder::getInstance()->randomFreeshSkeleton;
    sNode = SkeletonAnimation::createWithData(currSkeleton);
    sNode->setDebugSlotsEnabled(false);
    addChild(sNode, 10);

    //
    sNode->setAnimation(0, "R_idle", true);
    sNode->setSkin("basic_01");
}

void IdleFreeshLayer::initParticles()
{
    if (currSkinType == FreeshTypeGoldfish) {
        emitter = ParticleSystemQuad::create("particle/GoldFish_particle/goldfish_twinkle.plist");
        emitter->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
        emitter->setPositionType(ParticleSystem::PositionType::RELATIVE);
        emitter->setPosition(Vec2::ZERO);
        emitter->stopSystem();
        addChild(emitter, 50);
    }
}

void IdleFreeshLayer::setLocked()
{
    sNode->setColor(Color3B(100, 100, 100));
}

void IdleFreeshLayer::setHidden()
{
    sNode->setColor(Color3B::BLACK);
}

void IdleFreeshLayer::setSelected(bool flag)
{
    float _scale = (flag)? 1.3:0.6;
    sNode->stopAllActions();
    sNode->runAction(ScaleTo::create(0.1, _scale));
//    sNode->setScale(_scale);

    showParticle(flag);
}

void IdleFreeshLayer::showParticle(bool flag)
{
    if (emitter) {
        if (flag) {
            emitter->resetSystem();
        }
        else {
            emitter->stopSystem();
        }
    }
}
