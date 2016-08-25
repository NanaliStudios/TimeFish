//
//  TitleScene.h
//  Sanji
//
//  Created by midchow on 2014. 11. 24..
//
//

#ifndef __SpaceCleaner__TitleScene__
#define __SpaceCleaner__TitleScene__

#include "cocos2d.h"

#include "PlayScene.h"
#include "PosterLayer.h"

USING_NS_CC;

class TitleScene : public Layer
{
public:
    static Scene* createScene();

    virtual bool init();

    CREATE_FUNC(TitleScene);
    
    Size visibleSize;
    Size visibleSizeHalf;
    Vec2 origin;

private:
    int userSelectedPoster;
    PosterLayer *poster;

    void changeScene(float dt);
    void setCILogo(float dt);
    void setPoster(float dt);
};

#endif /* defined(__SpaceCleaner__TitleScene__) */
