//
//  TestScene2.h
//  GodPunch
//
//  Created by midchow on 2015. 4. 12..
//
//

#ifndef __GodPunch__TestScene2__
#define __GodPunch__TestScene2__

#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>

#include "FishLayer.h"

USING_NS_CC;

class TestScene2 : public Layer
{
public:
    static Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(TestScene2);

    //
    void onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event);
    void onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event);
    void onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event);
    void onTouchesCancelled(const std::vector<Touch*>& touches, cocos2d::Event  *event);

private:
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;
};

#endif /* defined(__GodPunch__TestScene2__) */
