//
//  ScoreLayer.h
//  Timefish
//
//  Created by midchow on 2015. 7. 18..
//
//

#ifndef __Timefish__ScoreLayer__
#define __Timefish__ScoreLayer__

#include "cocos2d.h"
#include "Global.h"
#include "UserInfo.h"
#include "LocalizationManager.h"

USING_NS_CC;

#define ScoreLabelNum 7
class ScoreLayer : public Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(ScoreLayer);
    
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;

    cocos2d::Size getLabelSize() {
        return scoreLabel[0]->getContentSize();
    }
    void setLabelPositionY(float _inGameUIPositionY, float _resultUIPositionY) {
        inGameUIPositionY = _inGameUIPositionY;
        resultUIPositionY = _resultUIPositionY;
    }
    void updateScore(int _currScore);

    //
    void setInGameUIPosition();
    void setResultUIPosition(int _currScore, int _bestScore);

private:
    Layer *currScoreLayer;
    
    float scaleFactor;

    Label *scoreTitleLabel;
    Label *scoreLabel[ScoreLabelNum];
    Label *bestLabel;
    Label *bestScoreLabel;
    
    float inGameUIPositionY;
    float resultUIPositionY;
    void initMainLayer();
};

#endif /* defined(__Timefish__ScoreLayer__) */
