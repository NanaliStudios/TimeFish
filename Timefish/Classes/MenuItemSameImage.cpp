    //
//  MenuItemSameImage.cpp
//  Sanji
//
//  Created by midchow on 2014. 11. 17..
//
//

#include "MenuItemSameImage.h"

///
/// MenuItemSameImage
///

MenuItemSameImage* MenuItemSameImage::create()
{
    MenuItemSameImage *ret = new (std::nothrow) MenuItemSameImage();
    if (ret && ret->init())
    {
        ret->_originalScale = 1.0f;
        ret->touchAreaScale = 1.0f;

        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MenuItemSameImage::init(void)
{
    return initWithNormalImage("", "", "", (const ccMenuCallback&)nullptr);
}

MenuItemSameImage * MenuItemSameImage::create(const std::string& normalImage)
{
    return MenuItemSameImage::create(normalImage, "", "", (const ccMenuCallback&)nullptr);
}

MenuItemSameImage * MenuItemSameImage::create(const std::string& normalImage, const std::string& selectedImage)
{
    return MenuItemSameImage::create(normalImage, selectedImage, "", (const ccMenuCallback&)nullptr);
}

MenuItemSameImage * MenuItemSameImage::create(const std::string& normalImage, const ccMenuCallback& callback)
{
    return MenuItemSameImage::create(normalImage, "", "", callback);
}

MenuItemSameImage * MenuItemSameImage::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
    MenuItemSameImage *ret = new (std::nothrow) MenuItemSameImage();
    if (ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, callback))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

MenuItemSameImage * MenuItemSameImage::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage)
{
    MenuItemSameImage *ret = new (std::nothrow) MenuItemSameImage();
    if (ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, (const ccMenuCallback&)nullptr))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MenuItemSameImage::initWithNormalImage(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
    Node *normalSprite = nullptr;
    Node *selectedSprite = nullptr;
    Node *disabledSprite = nullptr;
    
    if (normalImage.size() >0)
    {
        normalSprite = Sprite::createWithSpriteFrameName(normalImage);
        if (!normalSprite) {
            normalSprite = Sprite::create(normalImage);
        }
    }
    
    if (selectedImage.size() >0)
    {
        selectedSprite = Sprite::createWithSpriteFrameName(selectedImage);
    }
    
    if(disabledImage.size() >0)
    {
        disabledSprite = Sprite::createWithSpriteFrameName(disabledImage);
    }
    return initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, callback);
}

void MenuItemSameImage::selected()
{
    // subclass to change the default action
    if(_enabled)
    {
        MenuItem::selected();
        
        Action *action = getActionByTag(kZoomActionTag);
        if (action)
        {
            this->stopAction(action);
        }
        else
        {
            _originalScale = this->getScale();
        }
        
        Action *zoomAction = ScaleTo::create(0.1f, _originalScale * 1.2f);
        zoomAction->setTag(kZoomActionTag);
        this->runAction(zoomAction);
    }
}

void MenuItemSameImage::unselected()
{
    // subclass to change the default action
    if(_enabled)
    {
        MenuItem::unselected();
        this->stopActionByTag(kZoomActionTag);
        Action *zoomAction = ScaleTo::create(0.1f, _originalScale);
        zoomAction->setTag(kZoomActionTag);
        this->runAction(zoomAction);
    }
}

Rect MenuItemSameImage::rect() const
{
    return Rect( _position.x - _contentSize.width * _anchorPoint.x * touchAreaScale,
                _position.y - _contentSize.height * _anchorPoint.y * touchAreaScale,
                _contentSize.width * touchAreaScale, _contentSize.height * touchAreaScale);
}
