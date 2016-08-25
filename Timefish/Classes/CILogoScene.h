//
//  CILogoScene.h
//  Sanji
//
//  Created by midchow on 2014. 11. 21..
//
//

#ifndef __SpaceCleaner__CILogoScene__
#define __SpaceCleaner__CILogoScene__

#include "cocos2d.h"

#include "TitleScene.h"
#include "PlayScene.h"

USING_NS_CC;

class CILogoScene : public Layer
{
public:
    static Scene* createScene();

    virtual bool init();

    CREATE_FUNC(CILogoScene);

    Size visibleSize;
    Vec2 origin;
    
    void changeScene(float dt);
};

#endif /* defined(__SpaceCleaner__CILogoScene__) */
