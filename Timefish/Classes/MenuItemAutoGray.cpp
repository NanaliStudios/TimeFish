//
//  MenuItemAutoGray.cpp
//  Timefish
//
//  Created by midchow on 2015. 7. 26..
//
//

#include "MenuItemAutoGray.h"

MenuItemAutoGray* MenuItemAutoGray::create()
{
    MenuItemAutoGray *ret = new (std::nothrow) MenuItemAutoGray();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MenuItemAutoGray::init(void)
{
    grayed = false;

    return initWithNormalImage("", "", "", (const ccMenuCallback&)nullptr);
}

MenuItemAutoGray * MenuItemAutoGray::create(const std::string& normalImage)
{
    return MenuItemAutoGray::create(normalImage, "", "", (const ccMenuCallback&)nullptr);
}

MenuItemAutoGray * MenuItemAutoGray::create(const std::string& normalImage, const std::string& selectedImage)
{
    return MenuItemAutoGray::create(normalImage, selectedImage, "", (const ccMenuCallback&)nullptr);
}

MenuItemAutoGray * MenuItemAutoGray::create(const std::string& normalImage, const ccMenuCallback& callback)
{
    return MenuItemAutoGray::create(normalImage, "", "", callback);
}

MenuItemAutoGray * MenuItemAutoGray::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
    MenuItemAutoGray *ret = new (std::nothrow) MenuItemAutoGray();
    if (ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, callback))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

MenuItemAutoGray * MenuItemAutoGray::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage)
{
    MenuItemAutoGray *ret = new (std::nothrow) MenuItemAutoGray();
    if (ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, (const ccMenuCallback&)nullptr))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

MenuItemAutoGray * MenuItemAutoGray::create(const std::string& normalImage, const std::string& selectedImage, const ccMenuCallback& callback)
{
    MenuItemAutoGray *ret = new (std::nothrow) MenuItemAutoGray();
    if (ret && ret->initWithNormalImage(normalImage, selectedImage, "", callback))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MenuItemAutoGray::initWithNormalImage(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
    Node *normalSprite = nullptr;
    Node *selectedSprite = nullptr;
    Node *disabledSprite = nullptr;
    
    if (normalImage.size() >0)
    {
        normalSprite = GraySprite::createWithSpriteFrameName(normalImage);
    }
    
    if (selectedImage.size() >0)
    {
        selectedSprite = GraySprite::createWithSpriteFrameName(selectedImage);
    }
    
    if(disabledImage.size() >0)
    {
        disabledSprite = GraySprite::createWithSpriteFrameName(disabledImage);
    }
    return initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, callback);
}

void MenuItemAutoGray::setGrayed()
{
    grayed = true;

    GraySprite *nNode = (GraySprite*)getNormalImage();
    nNode->setGrayed();
    
    GraySprite *sNode = (GraySprite*)getSelectedImage();
    sNode->setGrayed();
}

void MenuItemAutoGray::setColored()
{
    grayed = false;

    GraySprite *nNode = (GraySprite*)getNormalImage();
    nNode->setColored();
    
    GraySprite *sNode = (GraySprite*)getSelectedImage();
    sNode->setColored();
}
