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
#define ResultImageShareBtnTag      2
#define ResultVideoShareBtnTag      3

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
                            const std::function<void()>& _shareCallback,
                            const std::function<void()>& _shareVideoCallback,
                            const std::function<void()>& _finishCallback);
    
    void showButtons();
    void hideVideoShareButton() {
        //
        alreadyShared = true;

        // revert video button settings
        MenuItemImageButton *btn = (MenuItemImageButton*)mainBtn[3]->getChildByTag(ResultImageShareBtnTag);
        if (btn) {
            btn->setTag(ResultBtnTag);
            videoShareBtn->setVisible(false);
        }
    }

    float getPosY() { return posY; }
    
    void setButtonVisibleForCapture() {
        videoShareBtn->setVisible(false);
        MenuItemImageButton *btn = (MenuItemImageButton*)mainBtn[3]->getChildByTag(ResultImageShareBtnTag);
        if (btn) {
            btn->setTag(ResultBtnTag);
        }
    }

private:
    Menu *mainBtn[4];
    
    Menu *videoShareBtn;

    float posY;
    bool alreadyShared;
    int btnCnt;

    std::function<void()> shareCallback;
    std::function<void()> shareVideoCallback;
    std::function<void()> finishCallback;

    void btnCallback(Ref *pSender);
};

#endif /* defined(__Timefish__ResultUIButtonLayer__) */
