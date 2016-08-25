//
//  ScoreLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 7. 18..
//
//

#include "ScoreLayer.h"

#include <iostream>
#include <iomanip>
#include <string>

bool ScoreLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSizeHalf = visibleSize * 0.5;
    origin = Director::getInstance()->getVisibleOrigin();

    scaleFactor = UserInfo::getInstance()->screenScale;
    
    //
    initMainLayer();

    return true;
}

void ScoreLayer::initMainLayer()
{
    //
    currScoreLayer = Layer::create();
    currScoreLayer->setPosition(Vec2::ZERO);
    currScoreLayer->setAnchorPoint(Vec2(0,0));
    addChild(currScoreLayer);

    float centerX = 0;
    for (int i=0; i<ScoreLabelNum; i++) {
        scoreLabel[i] = Label::createWithTTF("0", NumberFontPath, 65);
        scoreLabel[i]->setPosition(Vec2(centerX, 0));
        scoreLabel[i]->setVisible(false);
        currScoreLayer->addChild(scoreLabel[i], 1);
    }
    
    //
    {
        std::stringstream stream;
        stream << "ResultScore";
        std::string keyCode = stream.str();
        
        //
        stream.str("");
        stream.clear();
        stream << LocalizationManager::getInstance()->getLocalizationStringWithEnglish(keyCode);
        std::string uiInfo = stream.str();
        
        scoreTitleLabel = Label::createWithTTF(uiInfo, EnglishFontPath, 42);
        scoreTitleLabel->setPosition(Vec2(centerX, 60*scaleFactor));
        scoreTitleLabel->setOpacity(0);
        scoreTitleLabel->setScale(scaleFactor);
        addChild(scoreTitleLabel, 1);
    }

    //
    {
        std::stringstream stream;
        stream << "ResultBest";
        std::string keyCode = stream.str();
        
        //
        stream.str("");
        stream.clear();
        stream << LocalizationManager::getInstance()->getLocalizationStringWithEnglish(keyCode);
        std::string uiInfo = stream.str();

        bestLabel = Label::createWithTTF(uiInfo, EnglishFontPath, 40);
        bestLabel->setTextColor(Color4B(255, 61, 1, 255));
        bestLabel->setPosition(Vec2(centerX, -60*scaleFactor));
        bestLabel->setOpacity(0);
        bestLabel->setScale(scaleFactor);
        addChild(bestLabel, 1);
    }
    
    //
    {
        bestScoreLabel = Label::createWithTTF("0", NumberFontPath, 40);
        bestScoreLabel->setTextColor(Color4B(255, 61, 1, 255));
        bestScoreLabel->setPosition(Vec2(centerX, -60*scaleFactor));
        bestScoreLabel->setOpacity(0);
        bestScoreLabel->setScale(scaleFactor);
        addChild(bestScoreLabel, 1);
    }
}

void ScoreLayer::updateScore(int _currScore)
{
    int currScore = _currScore;
    int zeroCount = 0;
    if (currScore == 0) {
        zeroCount = ScoreLabelNum - 1;
        scoreLabel[zeroCount]->setString("0");
        scoreLabel[zeroCount]->setVisible(true);
    }
    else {
        int digits = 1000000;
        bool zero = true;
        for (int i=0; i<ScoreLabelNum; i++) {
            int digit = (int)(currScore/digits);
            currScore = currScore - digit * digits;
            
            std::stringstream stream;
            stream << digit;
            std::string scoreInfo = stream.str();
            
            scoreLabel[i]->setString(scoreInfo.c_str());
            if (zero && digit == 0) {
                scoreLabel[i]->setVisible(false);
                zeroCount++;
            }
            else {
                zero = false;
                scoreLabel[i]->setVisible(true);
            }
            
            digits = (int)(digits * 0.1);
        }
    }

    //
    // re-position for center alignment
    //
    float centerX = 0;
    float deltaX = 38;
    int labelNum = ScoreLabelNum - zeroCount;
    float startPosX = centerX - 0.5 * (labelNum - 1) * deltaX;
    for (int i=0; i<labelNum; i++) {
        scoreLabel[zeroCount + i]->setPosition(Vec2(startPosX + deltaX * i, 0));
    }
}

void ScoreLayer::setInGameUIPosition()
{
    this->setPosition(Vec2(visibleSizeHalf.width, inGameUIPositionY));
    this->setScale(1.0);

    scoreTitleLabel->setOpacity(0);
    for (int i=0; i<7; i++) {
        scoreLabel[i]->setScale(1.0);
    }
    bestLabel->setOpacity(0);
    bestScoreLabel->setOpacity(0);
}

void ScoreLayer::setResultUIPosition(int _currScore, int _bestScore)
{
    float _duration = 1.5;

    this->runAction(MoveTo::create(_duration, Vec2(visibleSizeHalf.width, resultUIPositionY) + origin));

    //
    // Current Score Info
    //
    scoreTitleLabel->runAction(FadeIn::create(_duration));
    currScoreLayer->runAction(ScaleTo::create(_duration, 1.4 * scaleFactor));
    
    //
    // Best Score Info
    {
        int bestScore = _bestScore;
        //
        std::stringstream stream;
//        stream << std::setfill('0') << std::setw(6) << bestScore;
        stream << bestScore;
        std::string scoreInfo = stream.str();

        // update label
        bestScoreLabel->setString(scoreInfo);

        // reposition
        float width = bestLabel->getContentSize().width*scaleFactor +
                                bestScoreLabel->getContentSize().width*scaleFactor + 10;
        float halfWidth = width * 0.5;
        bestLabel->setPositionX(-halfWidth * 0.5);
        bestScoreLabel->setPositionX(halfWidth * 0.5);

        // fade-in
        bestLabel->runAction(FadeIn::create(_duration));
        bestScoreLabel->runAction(FadeIn::create(_duration));
    }
    
    //
    // It's the best score, if the current and the best are same!
    //
    if (_currScore == _bestScore) {
        //
        auto crown = Sprite::createWithSpriteFrameName("TF_Font_(New).png");
        Size s = crown->getContentSize()*scaleFactor;
        crown->setScale(scaleFactor);
        float posY = scoreTitleLabel->getPositionY() + scoreTitleLabel->getContentSize().height * 0.5 + s.height * 0.5 + 1;

        crown->setPosition(Vec2(scoreTitleLabel->getPositionX(), posY));
        addChild(crown);

        //
        SoundManager::getInstance()->playSoundEffect(SoundBestScore, false);

        //
        {
            auto seq = Sequence::create(ScaleTo::create(0.25, 1.2*scaleFactor),
                                        ScaleTo::create(0.25, 1.0*scaleFactor),
                                        NULL);
            crown->runAction(RepeatForever::create(seq));
        }
    }
}
