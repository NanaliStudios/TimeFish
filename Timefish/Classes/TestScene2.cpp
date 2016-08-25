//
//  TestScene2.cpp
//  GodPunch
//
//  Created by midchow on 2015. 4. 12..
//
//

#include "TestScene2.h"

Scene* TestScene2::createScene()
{
    auto scene = Scene::create();
    auto layer = TestScene2::create();
    scene->addChild(layer, 1, 1);
    
    return scene;
}

bool TestScene2::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();

    //
    // setMultiTouchEnabled
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(TestScene2::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(TestScene2::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(TestScene2::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    

    return true;
}


void TestScene2::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{

}

void TestScene2::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
    
}

void TestScene2::onTouchesEnded(const std::vector<Touch*>& touches, Event  *event)
{
    auto jumpEmitter = ParticleSystemQuad::create("particle/jump/particle_jump.plist");
    jumpEmitter->setPosition(Vec2(visibleSizeHalf));
    addChild(jumpEmitter);
}

void TestScene2::onTouchesCancelled(const std::vector<Touch*>& touches, Event  *event)
{
}
