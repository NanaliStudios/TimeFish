//
//  GameCenterX.h
//  Timefish
//
//  Created by midchow on 2015. 1. 22..
//
//

#ifndef Timefish_GameCenterX_h
#define Timefish_GameCenterX_h

#include "UserInfo.h"

class GameCenterXDelegate
{
public:
    virtual ~GameCenterXDelegate() {}
};

class GameCenterX {
    
public:
    static GameCenterX* getInstance();
    static void purgeGameCenterX();

    void authenticateLocalPlayer();
    bool isAuthenticated(bool withAlert);
    void showLeaderboard();
    void showAchievementList();
    void reportScore(int _score);
    void reportAchievementIdentifier(int achievementType, float percentage);
    void resetAchievements();

    void setDelegate(GameCenterXDelegate* delegate) { m_delegate = delegate; }
    GameCenterXDelegate* getDelegate() { return m_delegate; }

private:
    GameCenterX()
    : m_delegate(nullptr)
    {
    }
    
    GameCenterXDelegate* m_delegate;
};

#endif
