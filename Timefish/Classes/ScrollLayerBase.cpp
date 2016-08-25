//
//  ScrollLayerBase.cpp
//  GodPunch
//
//  Created by midchow on 2015. 5. 12..
//
//

#include "ScrollLayerBase.h"

ScrollMoveTo* ScrollMoveTo::create(float duration, const Vec3& position)
{
    ScrollMoveTo *ret = new (std::nothrow) ScrollMoveTo();
    
    if (ret)
    {
        if (ret->initWithDuration(duration, position))
        {
            ret->autorelease();
        }
        else
        {
            delete ret;
            ret = nullptr;
        }
    }
    
    return ret;
}

void ScrollMoveTo::update(float t)
{
    if (_target)
    {
#if CC_ENABLE_STACKABLE_ACTIONS
        Vec3 currentPos = _target->getPosition3D();
        Vec3 diff = currentPos - _previousPosition;
        _startPosition = _startPosition + diff;
        Vec3 newPos =  _startPosition + (_positionDelta * t);
        _target->setPosition3D(newPos);
        _previousPosition = newPos;
#else
        _target->setPosition3D(_startPosition + _positionDelta * t);
#endif // CC_ENABLE_STACKABLE_ACTIONS
        
        if (updaterCallback) {
            updaterCallback();
        }
    }
}

bool ScrollLayerBase::init()
{
    if ( !Layer::init() ) {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    prevSelectedIdx = -1;
    selectedIdx = -1;

    scrollLayer = Layer::create();
    addChild(scrollLayer, 3);
    
    return true;
}

void ScrollLayerBase::setEnableLayer(bool flag)
{
    this->setVisible(flag);
    if (flag) {
        // setTouchEnabled
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true);
        touchListener->onTouchBegan = CC_CALLBACK_2(ScrollLayerBase::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(ScrollLayerBase::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(ScrollLayerBase::onTouchEnded, this);
        touchListener->onTouchCancelled = CC_CALLBACK_2(ScrollLayerBase::onTouchCancelled, this);
        dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    }
    else {
        // setTouchDisabled
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->removeEventListenersForTarget(this);
    }
}

bool ScrollLayerBase::onTouchBegan(Touch *touch, Event *event)
{
    Vec2 pos = touch->getLocation();

    //
    // if user inputs another touch while scrolling, ignore it.
    //
    int _currTouchID = touch->getID();
    if (currTouchID != -1 && currTouchID != _currTouchID) {
        return false;
    }
    currTouchID = _currTouchID;
    
    scrollLayer->stopAllActions();
    
    prevPos = pos;
    moved = false;

    //
    accelStartPos = prevPos;
    accelPos[0] = prevPos;
    accelPos[1] = prevPos;
    accelPos[2] = prevPos;

    return true;
}

void ScrollLayerBase::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 pos = touch->getLocation();
    float diffX = pos.x - prevPos.x;
    
    //
    // ignore tiny move
    //
    if (fabsf(diffX) < 10) {
        return;
    }
    
    //
    // move scroll
    //
    float currX = scrollLayer->getPositionX();
    moved = true;
    
    if (currX > scrollMinX || currX <= -scrollMaxX) {
        diffX *= 0.2;
    }
    
    scrollLayer->setPositionX(currX + diffX);
    selectionUpdater();
    prevPos = pos;

    //
    // 가속도 관련
    //
    accelPos[0] = accelPos[1];
    accelPos[1] = accelPos[2];
    accelPos[2] = prevPos;
}

#define AccelDeltaMin 20
#define AccelDeltaMin2 30

void ScrollLayerBase::onTouchEnded(Touch *touch, Event *event)
{
    float currX = scrollLayer->getPositionX();
    if (currX > scrollMinX) {
        //
        float moveToX = scrollMinX;
        auto moveTo = ScrollMoveTo::create(0.3, Vec3(moveToX, scrollLayer->getPositionY(), 0));
        moveTo->setUpdaterCallback(CC_CALLBACK_0(ScrollLayerBase::selectionUpdater, this));

        //
        auto callback = CallFunc::create( [this]() {
            selectionUpdater();
        });

        //
        auto seq = Sequence::create(moveTo, callback, NULL);
        scrollLayer->runAction(seq);
    }
    else if (currX <= -scrollMaxX) {
        //
        float moveToX = -scrollMaxX;
        auto moveTo = ScrollMoveTo::create(0.3, Vec3(moveToX, scrollLayer->getPositionY(), 0));
        moveTo->setUpdaterCallback(CC_CALLBACK_0(ScrollLayerBase::selectionUpdater, this));

        //
        auto callback = CallFunc::create( [this]() {
            selectionUpdater();
        });

        //
        auto seq = Sequence::create(moveTo, callback, NULL);
        scrollLayer->runAction(seq);
    }
    else {
        if (moved) {
            //
            // velocity is up to the diff of distance
            //
            float velocity1 = (accelPos[1].x-accelPos[0].x);
            float velocity2 = (accelPos[2].x-accelPos[1].x);
            
            //
            // set MoveTo by acceleration
            //
            float absDelta = fabsf(velocity1 - velocity2);
            if (fabsf(velocity2) > AccelDeltaMin) {
                int accelDirection = (velocity2>0)? 1:-1;

                float deviceMult = 0.03;
                float multiplier = absDelta * deviceMult;
                
                Vec2 currScrollPos = scrollLayer->getPosition();
                currScrollPos.x = currScrollPos.x + accelDirection * (scrollLength*0.1*multiplier);
                if (currScrollPos.x > 0) {
                    currScrollPos.x = scrollMinX;
                }
                else if (currScrollPos.x <= -scrollMaxX) {
                    currScrollPos.x = -scrollMaxX;
                }

                //
                auto moveToAction = ScrollMoveTo::create(0.3, Vec3(currScrollPos.x, currScrollPos.y, 0));
                moveToAction->setUpdaterCallback(CC_CALLBACK_0(ScrollLayerBase::selectionUpdater, this));
                auto scrollAction = EaseOut::create(moveToAction, 2.5); // faster -> slower

                //
                auto callback = CallFunc::create( [this]() {
                    selectionUpdater();
                });

                //
                auto seq = Sequence::create(scrollAction, callback, NULL);
                scrollLayer->runAction(seq);
            }
        }
    }
    
    selectionUpdater();

    // initialize!
    moved = false;
    currTouchID = -1;
}

void ScrollLayerBase::onTouchCancelled(Touch *touch, Event *event)
{
}


void ScrollLayerBase::directMoveScrollToIndex(int idx)
{
    Vec2 currScrollPos = scrollLayer->getPosition();
    currScrollPos.x = (gapX * (idx - 1 - 1) + scrollMinX);
//    currScrollPos.x = -scrollMaxX;

    //
    auto moveToAction = ScrollMoveTo::create(0.3, Vec3(-currScrollPos.x, currScrollPos.y, 0));
    moveToAction->setUpdaterCallback(CC_CALLBACK_0(ScrollLayerBase::selectionUpdater, this));
    auto scrollAction = EaseOut::create(moveToAction, 2.5); // faster -> slower
    
    //
    auto callback = CallFunc::create( [this]() {
        selectionUpdater();
    });
    
    //
    auto seq = Sequence::create(scrollAction, callback, NULL);
    scrollLayer->runAction(seq);
}


void ScrollLayerBase::selectionUpdater()
{
    float currPosX = scrollLayer->getPositionX() - gapX * 0.5;
    float deltaPosX = scrollMinX - currPosX;
    float idx = deltaPosX/gapX;

    selectedIdx = MIN(MAX((int)idx, 0), itemSize-1);

    //
    //
    //
    float velocity1 = (accelPos[1].x-accelPos[0].x);
    int visibleStart = selectedIdx;
    int visibleEnd   = selectedIdx;
    if (velocity1 < 0) {
        visibleStart -= 3;
        visibleEnd += 6;
    }
    else if (velocity1 > 0) {
        visibleStart -= 6;
        visibleEnd += 3;
    }
    else {
        visibleStart -= 3;
        visibleEnd += 3;
    }
//    int visibleStart = selectedIdx - 4;
//    int visibleEnd   = selectedIdx + 4;

    //
    // set visibility by in-screen/out-of-screen
    //
    for (const auto& child : scrollLayer->getChildren()) {
        if(child) {
            int tag = child->getTag();
            if (tag >= visibleStart && tag <= visibleEnd) {
                child->setVisible(true);
            }
            else {
                child->setVisible(false);
            }
        }
    }

    if (_selectionCallback) {
        _selectionCallback(false);
    }
}
