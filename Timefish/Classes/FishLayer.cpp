//
//  FishLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 1. 13..
//
//

#include "FishLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

using namespace spine;

mySkeletonAnimation* mySkeletonAnimation::createWithData (spSkeletonData* skeletonData, bool ownsSkeletonData) {
    mySkeletonAnimation* node = new mySkeletonAnimation(skeletonData, ownsSkeletonData);
    node->autorelease();
    return node;
}

mySkeletonAnimation::mySkeletonAnimation (spSkeletonData *skeletonData, bool ownsSkeletonData)
: SkeletonAnimation(skeletonData, ownsSkeletonData) {
    initialize();
}

void mySkeletonAnimation::setLocked()
{
    auto fileUtiles = FileUtils::getInstance();
    auto fragmentFullPath = fileUtiles->fullPathForFilename("Shaders/example_greyScale.fsh");
    auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
    auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    _fragSource = fragSource;
    
    _backgroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
                                                      [this](EventCustom*)
                                                      {
                                                          auto glProgram = _glprogramstate->getGLProgram();
                                                          glProgram->reset();
                                                          glProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, _fragSource.c_str());
                                                          glProgram->link();
                                                          glProgram->updateUniforms();
                                                      }
                                                      );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif
    
    _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    _glprogramstate->retain();
    setGLProgramState(_glprogramstate);
    getGLProgramState()->setUniformVec3("colorRate", Vec3(0.2126, 0.7152, 0.0722));
//    getGLProgramState()->setUniformVec3("colorRate", Vec3(0.2126, 0.7152, 0.0722)*0.3);

    getGLProgramState()->setUniformFloat("u_BlendValue", 0.5);
}

bool FishLayer::init()
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

void FishLayer::initFreeshWithSkinType(FreeshType skinType, int _currSkin)
{
    currSkinType = skinType;
    currSkinNumber = _currSkin;

    spSkeletonData *currSkeleton = SkeletonHolder::getInstance()->getSkeletonBySkinType(currSkinType);

    currLap = 0;
    currAniStatus = AniNone;

    startCallback = NULL;
//    finishCallback = NULL;

    //
    sNode = SkeletonAnimation::createWithData(currSkeleton);
    sNode->setDebugSlotsEnabled(false);
    addChild(sNode, 10);

    //
    // NOTE: Setting or changing completeListner at runtime is dangerous.
    // it may cause crash when destroying old CompleteListener.
    // So initialize it here!
    //
    sNode->setCompleteListener( [this] (int trackIndex, int loopCount) {
        if (currAniStatus == AniJump1) {
            sNode->setTimeScale(1);
        }
        else if (currAniStatus == AniIdle) {
            //
            // do nothing
            //
        }
        else if (currAniStatus == AniDie1) {
            setAnimation(AniDie2);
//            callFinishCallback(trackIndex, loopCount);
            
            sNode->runAction(FadeOut::create(0.3));
            
            int idx = currSkinNumber - 1;
            int soundType = freeshSoundTypes[idx];
            if (soundType == 1) {
                //
                SoundManager::getInstance()->playSoundEffect(SoundHatNormal, false);
            }
            else if (soundType == 2) {
                //
                SoundManager::getInstance()->playSoundEffect(SoundHatIron, false);
            }
        }
        else if (currAniStatus == AniDie2) {
            //
            // do nothing
            //
        }
        else if (currAniStatus == AniStart) {
            callStartCallback(trackIndex, loopCount);
        }
        else {
            this->setAnimation(AniIdle);
        }
    });
    
    //
    // initialize colors
    //
    int colorIndex = currSkinNumber - 1;
    Color3B _c1 = getFreeshColor(colorIndex, ColorOrderFirst);
    Color3B _c2 = getFreeshColor(colorIndex, ColorOrderSecond);
    Color3B _c3 = getFreeshColor(colorIndex, ColorOrderThird);
    colors[0] = Color4F(RGBValue(_c1.r), RGBValue(_c1.g), RGBValue(_c1.b), 1);
    colors[1] = Color4F(RGBValue(_c2.r), RGBValue(_c2.g), RGBValue(_c2.b), 1);
    colors[2] = Color4F(RGBValue(_c3.r), RGBValue(_c3.g), RGBValue(_c3.b), 1);

    //
    shieldMark = SkeletonAnimation::createWithData(SkeletonHolder::getInstance()->shieldSkeleton);
    shieldMark->setDebugSlotsEnabled(false);
    shieldMark->setPosition(Vec2(-120, 0));
    shieldMark->setAnchorPoint(Vec2(1, 0.5));
    shieldMark->setVisible(false);
    shieldMark->setAnimation(0, "idle", true);
    sNode->addChild(shieldMark, 10);

    //
    initParticles();

    //
    // set skin
    //
    if (!isThisSpecialTypeFish()) {
        
        currType = 0; // initially 0, which is red
        
        int idx = currSkinNumber -1;
        sNode->setSkin(getNormalSkinName(idx));
    }
    else {
        currType = -1; // initially -1
        setType(0);
    }
}

void FishLayer::setAnimation(AnimationType aniType)
{
    // if same, return!
    if (currAniStatus == aniType) {
        return;
    }

    currAniStatus = aniType;

    //
    const char *typeName[] = {"start", "crash", "die1", "die2", "eat", "idle", "jump1", "jump2"};

    //
    std::stringstream stream;
    
    if (isThisSpecialTypeFish()) {
        stream << typeName[aniType-1];
    }
    else {
        FreeshColorSet colorSet = getFreeshColorSet(currSkinNumber - 1, (ColorOrder)currType);
        stream << aniNameColorPrefix[colorSet] << typeName[aniType-1];
    }
    std::string aniName = stream.str();

    if (aniType == AniJump1) {
        sNode->setAnimation(0, aniName, false);
    }
    else if (aniType == AniIdle) {
        sNode->setAnimation(0, aniName.c_str(), true);
    }
    else if (aniType == AniDie1) {
        hideParticle();

//        sNode->setTimeScale(0.6);
        sNode->setAnimation(0, aniName.c_str(), false);
    }
    else if (aniType == AniDie2) {
//        sNode->setTimeScale(0.6);
        sNode->setAnimation(0, aniName.c_str(), false);
    }
    else if (aniType == AniStart) {
        hideParticle();

        sNode->setAnimation(0, aniName.c_str(), false);
        sNode->setEventListener( [this] (int trackIndex, spEvent* event) {
//            log("%d event: %s, %d, %f, %s", trackIndex, event->data->name, event->intValue, event->floatValue, event->stringValue);
            //
            // Set Event Name
            //
            if (!strcmp(event->data->name, "start")) {
                SoundManager::getInstance()->playSoundEffect(SoundFreesh, false);
            }
            else if (!strcmp(event->data->name, "landing")) {
                SoundManager::getInstance()->playSoundEffect(SoundLanding3, false);

                if (startCallback) {
                    startCallback(this);
                }
            }
        });
    }
    else {
        sNode->setAnimation(0, aniName.c_str(), false);
    }
}

void FishLayer::callStartCallback(int trackIndex, int loopCount)
{
    this->setAnimation(AniIdle);

    showParticle();
    
    if (finishStartAniCallback) {
        finishStartAniCallback();
    }
}
//void FishLayer::callFinishCallback(int trackIndex, int loopCount)
//{
//    setAnimation(AniDie2);
//    if (finishCallback) {
//        finishCallback(this);
//    }
//}

void FishLayer::initParticles()
{
    //
    bubbleParticlePtr = 0;

    //
    for (int i=0; i<BubbleParticles; i++) {
        int zOrder = -1;
        if (currSkinType == FreeshTypeGoldfish) {
            emitter[i] = ParticleSystemQuad::create("particle/GoldFish_particle/goldfish_particle.plist");
            zOrder = 1;
            emitter[i]->setPosition(Vec2(120, 0));
            emitter[i]->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
        }
        else {
            emitter[i] = ParticleSystemQuad::create("particle/idle/particle_texture.plist");
            emitter[i]->setPosition(Vec2(60, 0));
        }
        emitter[i]->setAnchorPoint(Vec2(0, 0.5));
        emitter[i]->stopSystem();
        sNode->addChild(emitter[i], zOrder);
    }
}

void FishLayer::hideParticle()
{
    emitter[bubbleParticlePtr]->stopSystem();
}

void FishLayer::showParticle()
{
    bubbleParticlePtr++;
    if (bubbleParticlePtr >= BubbleParticles) {
        bubbleParticlePtr = 0;
    }

    setParticleColor();

    emitter[bubbleParticlePtr]->setVisible(true);
    emitter[bubbleParticlePtr]->resetSystem();
}

//void FishLayer::setNextType()
//{
//    currType++;
//    if (currType >= 3) {
//        currType = 0;
//    }
//
//    setAnimation(AniIdle);
//}

void FishLayer::setType(int type)
{
    if (currType == type) {
        return;
    }

    currType = type;

    //
    if (isThisSpecialTypeFish()) {
        int idx = currSkinNumber -1;

        sNode->setSkin(getSpecialSkinNameWithColor(idx, currType));
    }

    //
    setParticleColor();
}

void FishLayer::setParticleColor()
{
    if (currSkinType != FreeshTypeGoldfish) {
        int idx = currType;
        emitter[bubbleParticlePtr]->setStartColor(colors[idx]);
        emitter[bubbleParticlePtr]->setEndColor(colors[idx]);
    }
}
