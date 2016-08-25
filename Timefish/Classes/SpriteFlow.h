//
//  SpriteFlow.h
//  Sanji
//
//  Created by midchow on 2014. 11. 6..
//
//

#ifndef __SpaceCleaner__SpriteFlow__
#define __SpaceCleaner__SpriteFlow__

#include "cocos2d.h"

USING_NS_CC;

class SpriteFlow : public cocos2d::Sprite
{
public:
    ~SpriteFlow();
    bool initWithTexture(cocos2d::Texture2D* texture, const cocos2d::Rect&  rect);
    void initGLProgram();
    
    static SpriteFlow* create(const char *pszFileName);
    void setDelta(Vec2 delta);
    Vec2 getFlowCount() {
        return Vec2(_flowCountX, _flowCountY);
    }
    void setFlowCount(Vec2 flowCount) {
        _flowCountX = flowCount.x;
        _flowCountY = flowCount.y;
    }
    
    void tick(float dt);
    
    
protected:
    float _flowCountX, _flowCountY;
    float deltaX, deltaY;

    void setFlowCountXY(float numX, float numY);
};


#endif /* defined(__SpaceCleaner__SpriteFlow__) */
