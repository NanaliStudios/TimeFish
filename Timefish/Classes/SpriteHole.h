//
//  SpriteHole.h
//  Sanji
//
//  Created by midchow on 2014. 12. 18..
//
//

#ifndef __SpaceCleaner__SpriteHole__
#define __SpaceCleaner__SpriteHole__

#include "cocos2d.h"

USING_NS_CC;

class SpriteHole : public Sprite
{
public:
    ~SpriteHole();
    bool initWithTexture(Texture2D* texture, const Rect&  rect);
    void initGLProgram();
    
    static SpriteHole* create(const char *pszFileName);

    void setHoleAlpha(float a);

protected:
    Size size;
    
    float _holeAlpha;
};

#endif /* defined(__SpaceCleaner__SpriteHole__) */
