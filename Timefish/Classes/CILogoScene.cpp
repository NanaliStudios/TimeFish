//
//  CILogoScene.cpp
//  Sanji
//
//  Created by midchow on 2014. 11. 21..
//
//

#include "CILogoScene.h"

Scene* CILogoScene::createScene()
{
    auto scene = Scene::create();
    auto layer = CILogoScene::create();
    scene->addChild(layer);
    return scene;
}

void CILogoScene::changeScene(float dt)
{
    auto scene = TitleScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene, Color3B(0,0,0)));
}

bool CILogoScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
//    auto cache = SpriteFrameCache::getInstance();
//    cache->addSpriteFramesWithFile("timefish.plist");

    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("timefish0.plist");
    cache->addSpriteFramesWithFile("timefish1.plist");
    cache->addSpriteFramesWithFile("timefish2.plist");
    cache->addSpriteFramesWithFile("timefish3.plist");
    cache->addSpriteFramesWithFile("timefish4.plist");

//
//    auto director = Director::getInstance();
//    auto glview = director->getOpenGLView();
//    auto screenSize = glview->getFrameSize();
//
//    Sprite *logo = NULL;
//#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
//    if (screenSize.width == 2048 || screenSize.height == 2048) { // iPad Retina/ iPad Air
//        logo = Sprite::create("TF_CI.png");
//    }
//    else if (screenSize.width == 1024 || screenSize.height == 1024) { // iPad 2
//        logo = Sprite::create("Default-568h@2x.png");
//    }
//    else if (screenSize.width == 2208 || screenSize.height == 2208) { // iPhone 6 Plus
//        logo = Sprite::create("Default-736h@3x.png");
//    }
//    else if (screenSize.width == 1334 || screenSize.height == 1334) { // iPhone 6
//        logo = Sprite::create("Default-667h@2x.png");
//    }
//    else if (screenSize.width == 1136 || screenSize.height == 1136) { // iPhone 5/ iPhone 5S
//        logo = Sprite::create("Default-568h@2x.png");
//    }
//    else if (screenSize.width == 960 || screenSize.height == 960) { // iPhone 4/ iPhone 4S
//        logo = Sprite::create("Default@2x.png");
//    }
//    else if (screenSize.width == 480 || screenSize.height == 480) { // iPhone 3GS
//        logo = Sprite::create("Default.png");
//    }
//    else {
//        logo = Sprite::create("TF_CI.png");
//    }
//#else 
//    logo = Sprite::create("TF_CI.png");
//#endif
//
//    logo->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
//    addChild(logo);

    scheduleOnce(CC_SCHEDULE_SELECTOR(CILogoScene::changeScene), 0.1);

    return true;
}
