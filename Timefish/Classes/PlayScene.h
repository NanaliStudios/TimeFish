//
//  PlayScene.h
//  Timefish
//
//  Created by midchow on 2015. 1. 13..
//
//

#ifndef __Timefish__PlayScene__
#define __Timefish__PlayScene__

#include "cocos2d.h"
#include "ChartboostX.h"
#include "FirebaseX.h"
#include "TapjoyX.h"

#include "Global.h"
#include "SoundManager.h"
#include "UserInfo.h"

#include "LoadingScene.h"

#include "UILayer.h"

#include "SpriteFlow.h"

#include "FreeshTrailLayer.h"

//
#include <spine/spine-cocos2dx.h>

#include "BGLayer.h"
#include "SleepyTimon.h"
#include "FishLayer.h"
#include "JumpCoin.h"

#include "ObstacleLayer.h"
#include "BallLayer.h"

#include "AchievementBannerLayer.h"

#include "FlurryX.h"
#include "VideoShareX.h"

#define JumpParticles   5

USING_NS_CC;

class PlayScene : public cocos2d::Layer, public ChartboostXDelegate, public FirebaseXDelegate
{
public:
    cocos2d::Size visibleSize, visibleSizeHalf;
    Vec2 origin;
    
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(PlayScene);
    
//    ~PlayScene();
    
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);

    BGLayer *bgLayer;
    FishLayer *character;
    JumpCoin *jCoin;

    UILayer *uiLayer;

    Layer *worldLayer;
    ObstacleLayer *obsLayer; // obstacle layer
    Layer *itemLayer;
    Vec2 center;
    
    FreeshDeathType deathType;

    float circleAlpha;
    
    float angle;
    float watchRadius, fishRadius;
    
    float jumpTime;
    float currTime;

    FreeshDeathType checkEndingConditions();
    
    void startGameByEnablingTouchEvent();
    void setBackgroundSoundVolume(float dt);
    void resetWorldLayerPositionAndScale();

    void startIntro(float dt);
    void startOutro();
    void startOutroLoop(float dt);
    void showScoreUI();

    void startEscaping();
    void startChasing(Ref* pSender);
    void startMainLoop();
    void moveCharacter(float dt);
    
    Status status;
    Status prevStatus;
    
    void setGameStatus(Status _s) {
        status = _s;
        uiLayer->setGameStatus(status);
    }

    Vector<BallLayer*> balls;

    float runVelocity;
    float maxVelocity;
    float timonMaxVelocity;
    float feverVelocity;
    float maxJumpTime;
    bool crashed, freeshDied;
    
    //
    void setBackgroundLayer();
    void setBackgroundLayerTo(int stat);
    void setWorldLayer();
    void updateUILayer(float delta);
    void initObstacles();
    void setObstacles();
    
    //
    int getNextBallIndex();
    void initBalls();
    void setCapsule();
    void setNextCapsuleLap();

    void getOneCoin();

    void checkObstacles();
    
    void checkCapsule();
    
    void setCrashed();
    void setFreeshBoosted(ObjectType obsType);
    void setFreeshToNormalStatus();
    void setFreeshBlink();

    void setDied();
    void askToContinue();
    void setNotContinue();
    void setContinue();
    
    void addNewBall(float dt);

    int ballLap;
    int currLap;
    int nextCapsuleLap;
    int currRound;

    Vec2 setWorldLayerPos(Vec2 newPos);

    //
    // ChartboostXDelegate methods
    //
    void continueGameLogic();
    bool shouldDisplayInterstitial(const char* location);
    void didCacheInterstitial(const char* location);
    void didFailToLoadInterstitial(const char* location, int errorCode);
    void didCloseInterstitial(const char* location);
    void didClickInterstitial(const char* location);

    void showPostInterstitial(Ref* pSender);
    
    //
    // FirebaseXDelegate methods
    //
    void admobInterstitialReady(bool success);
    void admobInterstitialClosed();
    void interstitialDidFailToPresentScreen();
    void interstitialWillLeaveApplication();
    void doAfterAdmobInterstitialCheck(bool isAdMobReady);

    //
    void pauseScene(Ref* pSender);
    void resumeScene(Ref* pSender);
    
    void enterBackgroundPause();
    void returnToForeground();

    void showJumpParticle();
    void showCrashParticle();

    void showAchievementsBanners(float dt);
private:
    bool gameContinuedWithVideo;

    time_t sTime, eTime;
    float bgVolume;
    
    int currCombo, maxComboCount;
    
    Sprite *boostIcon[5];

    //
    // NOTE: freeshStat and freeshShieldStat are different!!!
    //
    FreeshStatus freeshStat;
    FreeshStatus freeshShieldStat;
    float boostDuration; // in degree!!!

    Color4F ptclColors[3];

    //
    bool bannerIsOn;
    AchievementBannerLayer *banner;
    void showNextAchievementsBanner();

    //
    ParticleSystemQuad *jumpEmitter[JumpParticles];
    int jumpParticlePtr;
    
    ParticleSystemQuad *crashEmitter;
    void initParticles();

    //
    int setFreeshSkin();

    FreeshTrailLayer *freeshTrail;
    
    void setToMaxCombo(int bonus = 0);

    //
    void reloadPlayScene();

    LayerColor *background;
    SpriteFlow *flowTop, *flowBottom;
//    Sprite *stayTop, *stayBottom;

    //
    int colorIdx;
    
    float freeshHeight;
    
    int jumpCount, goldenJumpCount;
    float prevDiffLength;
    bool goingUp;

    Layer *centerCircle;
    
    void setToJump();
    
    int testBoostType;
    
    float outroTime;
    int coinsEarned;

    bool getByMonster(float dist) {
        float characterPos = character->getAnglePosition();
        float timonPos = bgLayer->getCurrDegree();
        float posDiff = characterPos - timonPos;
        
        int freeshLap = character->getCurrLap();
        int timonLap = bgLayer->getCurrLap();
        
        bool getByMonster = (freeshLap == timonLap &&
                             posDiff < dist);
        return getByMonster;
    }
};

#endif /* defined(__Timefish__PlayScene__) */
