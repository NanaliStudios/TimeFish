//
//  MenuItemSameImage.h
//  Sanji
//
//  Created by midchow on 2014. 11. 17..
//
//

#ifndef __SpaceCleaner__MenuItemSameImage__
#define __SpaceCleaner__MenuItemSameImage__

#include "cocos2d.h"

USING_NS_CC;

const unsigned int    kZoomActionTag = 0xc0c05002;

class CC_DLL MenuItemSameImage : public MenuItemSprite
{
public:
    static MenuItemSameImage* create();
    static MenuItemSameImage* create(const std::string& normalImage);
    static MenuItemSameImage* create(const std::string& normalImage, const std::string& selectedImage);
    static MenuItemSameImage* create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage);

    static MenuItemSameImage* create(const std::string&normalImage, const ccMenuCallback& callback);
    

    /** creates a menu item with a normal,selected  and disabled image with a callable object */
    static MenuItemSameImage* create(const std::string&normalImage, const std::string&selectedImage, const std::string&disabledImage, const ccMenuCallback& callback);

    virtual cocos2d::Rect rect() const;

    virtual void selected() override;
    virtual void unselected() override;

    void setTouchAreaScale(float scale) {
        touchAreaScale = scale;
    }

CC_CONSTRUCTOR_ACCESS:
    /**
     * @js ctor
     */
    MenuItemSameImage()
    : touchAreaScale(1.0),
    _originalScale(1.0)
    {}
    /**
     * @js NA
     * @lua NA
     */
    virtual ~MenuItemSameImage(){}
    
    bool init();
    
    /** initializes a menu item with a normal, selected  and disabled image with a callable object */
    bool initWithNormalImage(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback);

private:
    CC_DISALLOW_COPY_AND_ASSIGN(MenuItemSameImage);
    
    float      _originalScale;
    float       touchAreaScale;
};


#endif /* defined(__SpaceCleaner__MenuItemSameImage__) */
