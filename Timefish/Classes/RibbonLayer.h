//
//  RibbonLayer.h
//  Timefish
//
//  Created by midchow on 2015. 9. 10..
//
//

#ifndef __Timefish__RibbonLayer__
#define __Timefish__RibbonLayer__

#include "cocos2d.h"

#include "VideoShareX.h"
#include "MenuItemImageButton.h"

USING_NS_CC;

class RibbonLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(RibbonLayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;
    
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void onTouchCancelled(Touch *touch, Event *event);

    void showLayer()
    {
        ownsPriority = true;
    }

    void hideLayer()
    {
        ownsPriority = false;
        if (status == 3) {
            closeRibbon();
        }
    }
    
private:
    Layer *ribbonLayer;
    Sprite *ribbonHead;
    Sprite *arrow;

    bool ownsPriority;

    int status;
    
    void initMainLayer();
    void runRibbonAction();
    void initArrow();

    void openRibbon();
    void closeRibbon();
    void showEveryplay();
};

#endif /* defined(__Timefish__RibbonLayer__) */
