//
//  TitleScene.cpp
//  Sanji
//
//  Created by midchow on 2014. 11. 24..
//
//

#include "TitleScene.h"

#include <iostream>
#include <iomanip>
#include <string>

USING_NS_CC;

Scene* TitleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = TitleScene::create();
    scene->addChild(layer);
    return scene;
}

void TitleScene::changeScene(float dt)
{    
    auto scene = PlayScene::createScene();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    Director::getInstance()->replaceScene(TransitionFade::create(2.0, scene, Color3B(0,0,0)));
#else
    Director::getInstance()->replaceScene(TransitionFade::create(3.0, scene, Color3B(0,0,0)));
#endif
}

bool TitleScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();

//    srand((unsigned)time(NULL));
//    userSelectedPoster = rand()%NumOfPosters + 1;

//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    auto touchListener = EventListenerTouchOneByOne::create();
//    touchListener->setSwallowTouches(true);
//    touchListener->onTouchBegan = CC_CALLBACK_2(TitleScene::onTouchBegan, this);
//    touchListener->onTouchMoved = CC_CALLBACK_2(TitleScene::onTouchMoved, this);
//    touchListener->onTouchEnded = CC_CALLBACK_2(TitleScene::onTouchEnded, this);
//    touchListener->onTouchCancelled = CC_CALLBACK_2(TitleScene::onTouchCancelled, this);
//    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //
    setCILogo(0);
#else
    //
    setPoster(0);
#endif

    return true;
}

void TitleScene::setCILogo(float dt)
{
    float scaleFactor = 1/UserInfo::getInstance()->screenScale;

    auto logo = Sprite::create("soundsAndroid/Default-568h@2x.png");
    logo->setScale(scaleFactor);
    logo->setPosition(Vec2(visibleSizeHalf) + origin);
    addChild(logo);

    //
    auto callback = CallFunc::create( [this]() {
        setPoster(0);
    });
    logo->runAction(Sequence::create(DelayTime::create(2),
                                     FadeOut::create(2),
                                     callback,
                                     NULL));
    
//    scheduleOnce(CC_SCHEDULE_SELECTOR(TitleScene::setPoster), 3);
}

void TitleScene::setPoster(float dt)
{
    userSelectedPoster = UserInfo::getInstance()->getSelectedPosterType();

    poster = PosterLayer::create();
    poster->initMainLayerWithType(userSelectedPoster);
    addChild(poster);
    
    scheduleOnce(CC_SCHEDULE_SELECTOR(TitleScene::changeScene), 3);
}

//bool TitleScene::onTouchBegan(Touch *touch, Event *event)
//{
//    return true;
//}
//
//void TitleScene::onTouchMoved(Touch *touch, Event *event)
//{
//}
//void TitleScene::onTouchEnded(Touch *touch, Event *event)
//{
//    userSelectedPoster++;
//    if (userSelectedPoster >= NumOfPosters + 1) {
//        userSelectedPoster = 1;
//    }
//
//    setPoster();
//}
//void TitleScene::onTouchCancelled(Touch *touch, Event *event)
//{
//}
