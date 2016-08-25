//
//  MenuItemAutoGray.h
//  Timefish
//
//  Created by midchow on 2015. 7. 26..
//
//

#ifndef __Timefish__MenuItemAutoGray__
#define __Timefish__MenuItemAutoGray__

#include "cocos2d.h"

#include "MenuItemImageButton.h"
#include "GraySprite.h"

USING_NS_CC;

class CC_DLL MenuItemAutoGray : public MenuItemImageButton
{
public:
    static MenuItemAutoGray* create();
    static MenuItemAutoGray* create(const std::string& normalImage);
    static MenuItemAutoGray* create(const std::string& normalImage, const std::string& selectedImage);
    static MenuItemAutoGray* create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage);
    
    static MenuItemAutoGray* create(const std::string&normalImage, const ccMenuCallback& callback);
    
    static MenuItemAutoGray* create(const std::string&normalImage, const std::string&selectedImage, const ccMenuCallback& callback);
    
    /** creates a menu item with a normal,selected  and disabled image with a callable object */
    static MenuItemAutoGray* create(const std::string&normalImage, const std::string&selectedImage, const std::string&disabledImage, const ccMenuCallback& callback);

    bool getGrayed() { return grayed; }
    void setGrayed();
    void setColored();
    
CC_CONSTRUCTOR_ACCESS:
    /**
     * @js ctor
     */
    MenuItemAutoGray()
    :grayed(true)
    {}
    /**
     * @js NA
     * @lua NA
     */
    virtual ~MenuItemAutoGray() {};
    
    bool init();
    
    /** initializes a menu item with a normal, selected  and disabled image with a callable object */
    bool initWithNormalImage(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback);
    
private:
    bool grayed;

    CC_DISALLOW_COPY_AND_ASSIGN(MenuItemAutoGray);
};

#endif /* defined(__Timefish__MenuItemAutoGray__) */
