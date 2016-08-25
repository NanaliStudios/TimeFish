//
//  BonusTooltipLayer.h
//  Timefish
//
//  Created by midchow on 2015. 9. 11..
//
//

#ifndef __Timefish__BonusTooltipLayer__
#define __Timefish__BonusTooltipLayer__

#include "cocos2d.h"

#include "Global.h"
#include "UserInfo.h"
#include "SocialX.h"
#include "LocalizationManager.h"

#include "MenuItemSameImage.h"

USING_NS_CC;

class BonusTooltipLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(BonusTooltipLayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;

    void initWithAmount(int posterIdx);
private:
    Layer *mainLayer;

    Vec2 initBackground(float length);
};

#endif /* defined(__Timefish__BonusTooltipLayer__) */
