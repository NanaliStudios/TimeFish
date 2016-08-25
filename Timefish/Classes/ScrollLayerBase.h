//
//  ScrollLayerBase.h
//  GodPunch
//
//  Created by midchow on 2015. 5. 12..
//
//

#ifndef __GodPunch__ScrollLayerBase__
#define __GodPunch__ScrollLayerBase__

#include "cocos2d.h"
#include "Global.h"
#include "SoundManager.h"

USING_NS_CC;

class ScrollMoveTo : public MoveTo
{
public:
    static ScrollMoveTo* create(float duration, const Vec3& position);

    virtual void update(float time) override;
    void setUpdaterCallback(const std::function<void()>& _updaterCallback) {
        updaterCallback = _updaterCallback;
    }

private:
    std::function<void()> updaterCallback;
};

class ScrollLayerBase : public Layer
{
public:
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;
    
    virtual bool init();
    
    CREATE_FUNC(ScrollLayerBase);
    
    void setEnableLayer(bool flag);
    void setUIUpdater(const std::function<void()>& uiUpdater) {
        _uiUpdater = uiUpdater;
    }

    void directMoveScrollToIndex(int idx);
    
protected:
    // scroll
    Vec2 prevPos, touchPos;

    Vec2 accelStartPos;
    Vec2 accelPos[3];

    bool moved;
    float basePosX, basePosY, gapX;
    float scrollMinX, scrollMaxX;
    int colMax;
    cocos2d::Size iconSize;
    float scrollLength;

    int currTouchID;
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    
    Layer *scrollLayer;

    int itemSize;

    int prevSelectedIdx;
    int selectedIdx;

    std::function<void(bool)> _selectionCallback;
    std::function<void()> _uiUpdater;
    
//    void updateSelectionInfo();
    
    void selectionUpdater();
};

#endif /* defined(__GodPunch__ScrollLayerBase__) */
