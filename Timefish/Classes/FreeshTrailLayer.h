//
//  FreeshTrailLayer.h
//  Timefish
//
//  Created by midchow on 2015. 7. 29..
//
//

#ifndef __Timefish__FreeshTrailLayer__
#define __Timefish__FreeshTrailLayer__

#include "cocos2d.h"

USING_NS_CC;

class FreeshTrailLayer : public MotionStreak
{
public:
    static FreeshTrailLayer* create(float fade, float minSeg, float stroke, const Color3B& color, const std::string& path);
    virtual void update(float delta) override;

private:
    float startOpacity;
};

#endif /* defined(__Timefish__FreeshTrailLayer__) */
