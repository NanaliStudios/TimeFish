//
//  ResultUIButtonLayer.h
//  Timefish
//
//  Created by midchow on 2015. 7. 19..
//
//

#ifndef __Timefish__ResultUIButtonLayer__
#define __Timefish__ResultUIButtonLayer__

#include "cocos2d.h"
#include "Global.h"

#include "UserInfo.h"

#include "TapjoyX.h"

#include "MenuItemImageButton.h"

USING_NS_CC;

#define ResultBtnTag                0

class ResultUIButtonLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(ResultUIButtonLayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;
    
    void initMainLayerWithCallbacks(const ccMenuCallback& callback1,
                            const ccMenuCallback& callback2,
                            const ccMenuCallback& callback3,
                            const std::function<void()>& _finishCallback);
    
    void showButtons();

    float getPosY() { return posY; }

private:
    Menu *mainBtn[2];

    float posY;
    int btnCnt;

    std::function<void()> finishCallback;
};

#endif /* defined(__Timefish__ResultUIButtonLayer__) */
