//
//  MenuItemImageButton.cpp
//  Timefish
//
//  Created by midchow on 2015. 9. 8..
//
//

#include "MenuItemImageButton.h"

MenuItemImageButton* MenuItemImageButton::create()
{
    MenuItemImageButton *ret = new (std::nothrow) MenuItemImageButton();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void MenuItemImageButton::selected()
{
    MenuItemImage::selected();

    //
    for (int i=0; i<StaticNumOfChild; i++) {
        if (labelChild[i]) {
            orgPosY[i] = labelChild[i]->getPositionY();
            labelChild[i]->setPositionY(orgPosY[i] - 3);
        }
    }
}

void MenuItemImageButton::unselected()
{
    MenuItemImage::unselected();

    //
    for (int i=0; i<StaticNumOfChild; i++) {
        if (labelChild[i]) {
            labelChild[i]->setPositionY(orgPosY[i]);
        }
    }
}
