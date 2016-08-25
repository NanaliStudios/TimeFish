//
//  GraySprite.h
//  Timefish
//
//  Created by midchow on 2015. 7. 26..
//
//

#ifndef __Timefish__GraySprite__
#define __Timefish__GraySprite__

#include "cocos2d.h"

USING_NS_CC;

class GraySprite : public Sprite
{
public:
    static GraySprite* create(const std::string& filename);
    static GraySprite* createWithSpriteFrame(SpriteFrame *spriteFrame);
    static GraySprite* createWithSpriteFrameName(const std::string& spriteFrameName);

    GraySprite();
    ~GraySprite();

    void setGrayed();
    void setColored();
    bool getGrayed() {
        return grayed;
    }

    void setColorRate(float mult = 1.3); // default value 1.3
private:
    GLProgramState *_glprogramstate;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    std::string _fragSource;
    EventListenerCustom* _backgroundListener;
#endif

    bool grayed;
    void initGLProgram();
    
    virtual bool initWithSpriteFrame(SpriteFrame *spriteFrame);
};

#endif /* defined(__Timefish__GraySprite__) */
