//
//  PopupManager.h
//  Timefish
//
//  Created by midchow on 2015. 8. 3..
//
//

#ifndef __Timefish__PopupManager__
#define __Timefish__PopupManager__

#include "cocos2d.h"
#include "Global.h"

#include "FlurryX.h"
#include "GameCenterX.h"

#include "LocalizationManager.h"

#include "MenuItemSameImage.h"

#include "Popup.h"

USING_NS_CC;

class PopupManager : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(PopupManager);

    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;
    
    void pushPopup(HiddenskinPopup *child);
    void showNextPopup();

    void setEndOfPopupCallback(const std::function<void()>& _endOfPopupCallback) {
        endOfPopupCallback = _endOfPopupCallback;
    }
    
private:
    int currentIdx;
    
    int numOfPopups;

    LayerColor *blackScreen;
    Node *popups;
    spine::SkeletonAnimation *lNode;

    std::function<void()> endOfPopupCallback;

    void showCurrentPopup();
    void removePrevPopup();
};

#endif /* defined(__Timefish__PopupManager__) */
