//
//  PopupManager.cpp
//  Timefish
//
//  Created by midchow on 2015. 8. 3..
//
//

#include "PopupManager.h"

#include <iostream>
#include <iomanip>
#include <string>

bool PopupManager::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();

    currentIdx = 0;
    popups = 0;
    
    popups = Node::create();
    addChild(popups, 100);

    //
    lNode = SkeletonAnimation::createWithData(SkeletonHolder::getInstance()->lightSkeleton);
    lNode->setDebugSlotsEnabled(false);
    lNode->setPosition(Vec2(visibleSizeHalf.width, visibleSizeHalf.height) + origin);
    lNode->setVisible(false);
    addChild(lNode, 100);

    return true;
}

void PopupManager::pushPopup(HiddenskinPopup *child)
{
    int cnt = (int)popups->getChildrenCount();

    if (cnt == 0) {
        //
        blackScreen = LayerColor::create(Color4B(0,0,0,150));
        addChild(blackScreen, 10);
    }

    child->setVisible(false);
    child->setTag(cnt); // start from 0
    popups->addChild(child);
}

void PopupManager::showNextPopup()
{
    int cnt = (int)popups->getChildrenCount();
    if (currentIdx < cnt && currentIdx == 0) {
        //
        // initial popup
        //
        showCurrentPopup();
    }
    else if (cnt > 1) {
        //
        removePrevPopup();

        //
        showCurrentPopup();
    }
    else if (cnt == 1) {
        //
        // end of popups
        //
        removePrevPopup(); // remove last

        blackScreen->removeFromParent();

        //
        if (endOfPopupCallback) {
            endOfPopupCallback();
        }
    }
}

void PopupManager::showCurrentPopup()
{
    //
    // show current popup
    //
    HiddenskinPopup *popup = (HiddenskinPopup*)popups->getChildByTag(currentIdx);
    if (popup) {
        popup->startIntro();
        
        //
        lNode->setVisible(true);
        lNode->setAnimation(0, "animation", false);

        Color4F colors[3] = {
            Color4F(RGBValue(0), RGBValue(138), RGBValue(255), 1),
            Color4F(RGBValue(251), RGBValue(217), RGBValue(60), 1),
            Color4F(RGBValue(255), RGBValue(27), RGBValue(0), 1),
        };
        for (int i=0; i<3; i++) {
            ParticleSystemQuad *partyPtcl = ParticleSystemQuad::create("particle/VendingMachinePopper_particle/VendingMachinePopper_particle.plist");
            partyPtcl->setPosition(Vec2(visibleSizeHalf.width, visibleSize.height * 1.2) + origin);
            partyPtcl->setStartColor(colors[i]);
            partyPtcl->setEndColor(colors[i]);
            addChild(partyPtcl, 100);
        }

        
        //
        SoundManager::getInstance()->playSoundEffect(SoundDrawPop, false);
        
        //
        SoundManager::getInstance()->playSoundEffect(SoundSkinNew, false);
    }

    // set to next
    currentIdx++;
}

void PopupManager::removePrevPopup()
{
    int prevIdx = currentIdx - 1;
    if (prevIdx >= 0) {
        // remove prev
        HiddenskinPopup *popup = (HiddenskinPopup*)popups->getChildByTag(prevIdx);
        if (popup) {
            popups->removeChild(popup);
        }
    }
}
