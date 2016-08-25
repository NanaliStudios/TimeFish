//
//  LoadingScene.h
//  Timefish
//
//  Created by midchow on 2015. 3. 1..
//
//

#ifndef __Timefish__LoadingScene__
#define __Timefish__LoadingScene__

#include "cocos2d.h"
#include "PlayScene.h"

USING_NS_CC;

class LoadingScene : public Layer
{
public:
    static Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(LoadingScene);
    
    cocos2d::Size visibleSize;
    Vec2 origin;
    
    void changeScene(float dt);
};

#endif /* defined(__Timefish__LoadingScene__) */
