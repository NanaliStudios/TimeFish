//
//  LoadingScene.cpp
//  Timefish
//
//  Created by midchow on 2015. 3. 1..
//
//

#include "LoadingScene.h"

USING_NS_CC;

Scene* LoadingScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LoadingScene::create();
    scene->addChild(layer);
    return scene;
}

void LoadingScene::changeScene(float dt)
{
    auto scene = PlayScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0,0,0)));
}

bool LoadingScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("timefish0.plist");
    cache->addSpriteFramesWithFile("timefish1.plist");
    cache->addSpriteFramesWithFile("timefish2.plist");
    cache->addSpriteFramesWithFile("timefish3.plist");
    cache->addSpriteFramesWithFile("timefish4.plist");

    scheduleOnce(CC_SCHEDULE_SELECTOR(LoadingScene::changeScene), 0.3);
    
    return true;
}
