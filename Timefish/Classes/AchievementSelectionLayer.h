//
//  AchievementSelectionLayer.h
//  Timefish
//
//  Created by midchow on 2015. 7. 17..
//
//

#ifndef __Timefish__AchievementSelectionLayer__
#define __Timefish__AchievementSelectionLayer__

#include "cocos2d.h"
#include "Global.h"

#include "FlurryX.h"
#include "GameCenterX.h"

#include "PageViewlayer.h"
#include "MenuItemSameImage.h"

USING_NS_CC;

class AchievementSelectionLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(AchievementSelectionLayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    virtual void onKeyReleased( EventKeyboard::KeyCode keycode, Event *event );
#endif

    void setCloseCallback(const std::function<void(Ref*)>& _closeCallback) {
        closeCallback = _closeCallback;
    }
    
    void showNewMark(bool flag) {
//        if (newMark) {
//            newMark->setVisible(flag);
//        }
    }
    PageViewlayer* getPageLayer() {
        return pageLayer;
    }


    void showThis();

    virtual void setVisible(bool visible);

    bool ownsTouchPriority() {
        return ownsPriority;
    }
private:
    int status;
    bool ownsPriority;

    std::function<void(Ref*)> closeCallback;
    
    void btnCallback(Ref *pSender);
    void hideThis();
    
//    LayerColor *mainLayer;
    Layer *mainLayer;
    PageViewlayer *pageLayer;
    
    MenuItemSameImage *mainBtnItem;
    Menu *mainBtn;
//    Sprite *newMark;
    
    float layerHeight;

    void initMainLayer();
    
    void updateLayer(Ref *pSender);
};

#endif /* defined(__Timefish__AchievementSelectionLayer__) */
