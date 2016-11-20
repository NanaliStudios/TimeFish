//
//  PageViewlayer.h
//  Timefish
//
//  Created by midchow on 2015. 3. 19..
//
//

#ifndef __Timefish__PageViewlayer__
#define __Timefish__PageViewlayer__

#include "cocos2d.h"
#include "Global.h"
#include "UserInfo.h"

#include "TooltipLayer.h"
#include "PosterLayer.h"
#include "MenuItemSameImage.h"
#include "MenuItemImageButton.h"

USING_NS_CC;

class PageViewlayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(PageViewlayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;

    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);

    void enableLayerTouch();
    void disableLayerTouch();
    void initLayersWithPosY(float posY);
    void updatePages();
    void unlockAchievement(Sprite *outBox, Sprite *inBox, Vec2 pos, AchievementType posterType);
    void setUpdateCallback(const std::function<void(Ref*)>& _updateCallback) {
        updateCallback = _updateCallback;
    }
    
    bool ownsBackButtonPriority;

private:
    Layer *mainLayer;
    Layer *movingLayer;
    
    Label *countLabel;

    float basePosY;
    int colsPage, rowsPage;
    int currPage, pageNumbers;
    int itemsPerPage;

    void initMainLayer();
    void initMovingLayer(float posY);

    std::function<void(Ref*)> updateCallback;

    //
    Vec2 beginPos, prevPos;
    bool movingLeft;
    bool isMoving;
    float pageWidth;
    void setCurrPage(int newPage);
    void finishPageMoving();

    bool isThisFirstPage() {
        if (currPage - 1 <= 0) {
            return true;
        }
        
        return false;
    }
    bool isThisLastPage() {
        if (currPage + 1 >= pageNumbers) {
            return true;
        }
        
        return false;
    }
    
    //
    Sprite *selectionMarker;
    void setSelectionMarker(Vec2 pos);
    Vec2 getAchievemenetIconPos(int posterIdx);

    //
    TooltipLayer *tooltip;
    void tooltipCloseCallback();
    void posterCloseCallback(Ref *pSender);
    
    //
    void showGameCenterAchievement();
    
    //
    bool btnSelected;
};

#endif /* defined(__Timefish__PageViewlayer__) */
