//
//  MenuItemImageButton.h
//  Timefish
//
//  Created by midchow on 2015. 9. 8..
//
//

#ifndef __Timefish__MenuItemImageButton__
#define __Timefish__MenuItemImageButton__

#include "cocos2d.h"

USING_NS_CC;

#define StaticNumOfChild    3
class CC_DLL MenuItemImageButton : public MenuItemImage
{
public:
    static MenuItemImageButton* create();

    //
    void setLabelChild(Node *child1 = NULL, Node *child2 = NULL, Node *child3 = NULL) {
        labelChild[0] = child1;
        labelChild[1] = child2;
        labelChild[2] = child3;
    };

CC_CONSTRUCTOR_ACCESS:
    /**
     * @js ctor
     */
    MenuItemImageButton()
    {
        setLabelChild();
    }

    virtual ~MenuItemImageButton(){}
    
    bool init() {
        return  MenuItemImage::init();
    }

    //
    virtual void selected() override;
    virtual void unselected() override;
private:
    Node *labelChild[StaticNumOfChild];
    float orgPosY[StaticNumOfChild];
};

#endif /* defined(__Timefish__MenuItemImageButton__) */
