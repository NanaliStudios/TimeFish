//
//  Global.h
//  Timefish
//
//  Created by midchow on 2015. 1. 13..
//
//

#ifndef Timefish_Global_h
#define Timefish_Global_h

#define CurrentVersion 101

#define BaseScreenWidth      640.0f // iphone 5s
#define BaseScreenHeight    1136.0f // iphone 5s

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#define IAP_ID_RemoveAds "remove_ads"
#else
#define IAP_ID_RemoveAds "RemoveAds"
#endif

//////////
//#define ProductPurchaseCapture
//#define DEVELOPMENT_NOAD
//#define AUTOJUMP_TEST
//#define DEVELOPMENT
//#define LEVELDESIGN

#define RGBValue(__C__) (float)__C__/255
//#define DefaultFontPath "fonts/sesquipe_timefish_number.fnt"


///////////
#define BoostProbability        700
#define BoostProbabilityForFirstUser 900
#define GoldNeededToDraw        100
#define VideoRewardGold          25
#define RemoveAdsRewardGold    1000
#define GoldenFishBuyBonus     1000
#define TrialSkinBuyBonus       300
#define SkinPackageBuyBonus     300
#define TrialAllowNum             3
#define HiddenSkinStartIndexForTapJoy   1000
#define GiftTimeInHour            6
#define DeltaGiftTimeAllowRate  0.1

//////////
#define MaxComboCount 5
#define PointBoostBonus 1
#define FirstTimeResultTypeIndexMax 1
#define ResultTypeGroupIndexMax     6

#define NumberFontPath "fonts/kimberley bl.ttf"
#define EnglishFontPath "fonts/kimberley bl.ttf"
#define KoreanFontPath "fonts/BMHANNA_11yrs_ttf.ttf"
#define JapaneseFontPath "fonts/LightNovelPOP_FONT.ttf"
#define ChineseFontPath "fonts/DFPHaiBaoW12-GB_sma.ttf"

#include "cocos2d.h"

#include <iostream>
#include <iomanip>
#include <string>

USING_NS_CC;

#define NumOfAchievement 31

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
namespace std
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream ss;
        ss << n ;
        return ss.str() ;
    }
}
#endif

typedef enum {
    StatusNone,
    StatusJump,
    StatusIntro,
    StatusReady,
    StatusPause,
    StatusGameOver,
    StatusAdTime
} Status;

typedef enum {
    ResultTypeGift1 = 0,
    ResultTypeGiftZPlay,
    ResultTypeDraw1,
    ResultTypeDrawZPlay,
    //
    ResultTypeMax
} ResultType;

static Color3B posterBgColor[] = {
    Color3B(239,165, 40), // 1
    Color3B(255,  1,  2), // 2
    Color3B( 22,126,251), // 3
    Color3B( 12, 19, 37), // 4
    Color3B( 12, 19, 37), // 5
    Color3B( 12, 19, 37), // 6
    Color3B( 12, 19, 37), // 7
    Color3B(184,236, 62), // 8
    Color3B( 22,126,251), // 9
    Color3B(255,255,255), // 10
    Color3B(255,255,255), // 11
    Color3B(255,255,255), // 12
    Color3B(239,165, 40), // 13
    Color3B(255,  1,  2), // 14
    Color3B( 22,126,251), // 15
    Color3B(255,  1,  2), // 16
    Color3B(239,165, 40), // 17
    Color3B( 22,126,251), // 18
    Color3B(255,168, 24), // 19
    Color3B(  0, 14, 36), // 20
    Color3B(255,180, 58), // 21
    Color3B( 12, 19, 37), // 22
    Color3B( 61, 41, 74), // 23
    Color3B(255,255,255), // 24
    Color3B(239,165, 40), // 25
    Color3B(255,112,138), // 26
    Color3B( 77,  0, 32), // 27
    //
    Color3B( 14, 28, 62), // 28
    Color3B(255,255,255), // 29
    Color3B(255,255,255), // 30
    Color3B(255,255,255), // 31
};

static Vec2 posterIconPosDelta[] = {
    Vec2(   0, -20), // 1
    Vec2(   0, -20), // 2
    Vec2(   0, -20), // 3
    Vec2(   0, -20), // 4
    Vec2(   0, -20), // 5
    Vec2(   0, -20), // 6
    Vec2(   2, -2), // 7
    Vec2(   0,  0), // 8
    Vec2(   0,  0), // 9
    Vec2(  20, 85), // 10
    Vec2(   0, 32), // 11
    Vec2(   0,  0), // 12
    Vec2(  25,  0), // 13
    Vec2(  25,  0), // 14
    Vec2(  25,  0), // 15
    Vec2( -10,  5), // 16
    Vec2( -10,  5), // 17
    Vec2( -10,  5), // 18
    Vec2(   0, -5), // 19
    Vec2(   0,  2), // 20
    Vec2(   0, -2), // 21
    Vec2(   0,  0), // 22
    Vec2(   3,  0), // 23
    Vec2(   0,  0), // 24
    Vec2(   0,  0), // 25
    Vec2(   5,  0), // 26
    Vec2(   0,  0), // 27
    //
    Vec2(   0,  0), // 28
    Vec2(   0,  0), // 29
    Vec2(   0,  0), // 30
    Vec2(   0,  0), // 31
};

typedef enum
{
    SoundAmbience,
    SoundGameStart,
    SoundFreesh,
    SoundLanding1,
    SoundLanding2,
    SoundLanding3,
    SoundCrash,
    SoundCapsule,
    SoundGone,
    SoundGameOver,
    SoundButton,
    SoundDisabledButton,
    SoundDrawStart,
    SoundDrawZoomin,
    SoundDrawPop,
    SoundScroll,
    SoundSkinNew,
    SoundSkinNotNew,
    SoundGift1,
    SoundGift2,
    SoundGift3,
    SoundGiftTap,
    SoundGiftOpen,
    SoundCoinRain1,
    SoundCoinRain2,
    SoundCoinRain3,
    //
    SoundFeverBoost,
    SoundCoinBoost,
    SoundSlowBoost,
    SoundPointBoost,
    SoundShieldBoost,
    //
    SoundFeverCrash1,
    SoundFeverCrash2,
    SoundFeverCrash3,
    //
    SoundCoinGet1,
    SoundCoinGet2,
    SoundCoinGet3,
    SoundCoinGet4,
    //
    SoundShieldBreak,
    SoundPointBreak,
    //
    SoundHatNormal,
    SoundHatIron,
    //
    SoundBestScore,
    SoundButtonAppear,
    SoundGoldenFishCoin,
    SoundEffectMax
} SoundEffect;

typedef enum
{
    AchievementType01 = 1 << 0,
    AchievementType02 = 1 << 1,
    AchievementType03 = 1 << 2,
    AchievementType04 = 1 << 3,
    AchievementType05 = 1 << 4,
    AchievementType06 = 1 << 5,
    AchievementType07 = 1 << 6,
    AchievementType08 = 1 << 7,
    AchievementType09 = 1 << 8,
    AchievementType10 = 1 << 9,
    AchievementType11 = 1 << 10,
    AchievementType12 = 1 << 11,
    AchievementType13 = 1 << 12,
    AchievementType14 = 1 << 13,
    AchievementType15 = 1 << 14,
    AchievementType16 = 1 << 15,
    AchievementType17 = 1 << 16,
    AchievementType18 = 1 << 17,
    AchievementType19 = 1 << 18,
    AchievementType20 = 1 << 19,
    AchievementType21 = 1 << 20,
    AchievementType22 = 1 << 21,
    AchievementType23 = 1 << 22,
    AchievementType24 = 1 << 23,
    AchievementType25 = 1 << 24,
    AchievementType26 = 1 << 25,
    AchievementType27 = 1 << 26,
    //
    // newly added
    //
    AchievementType28 = 1 << 27,
    AchievementType29 = 1 << 28,
    AchievementType30 = 1 << 29,
    AchievementType31 = 1 << 30,
    //
    AchievementTypeMax = 1 << 30,
} AchievementType;

typedef enum {
    AniNone,
    AniStart,
    AniCrash,
    AniDie1,
    AniDie2,
    AniEat,
    AniIdle,
    AniJump1,
    AniJump2,
    AniMax
} AnimationType;

typedef enum {
    TimonAniNone,
    TimonAniDie,
    TimonAniIdle,
//    TimonAniIdle1,
//    TimonAniIdle2,
//    TimonAniIdle3,
    TimonAniOpen,
    TimonAniDown,
    TimonAniUp,
    TimonAniStartSleep,
    TimonAniSleepIdle,
    TimonAniEndSleep,
    TimonAniMax
} TimonAnimationType;

typedef enum {
    ObjectTypeNone,
    ObjectTypeObstacle,
    ObjectTypeCapsule1, // Fever
    ObjectTypeCapsule2, // Coin
    ObjectTypeCapsule3, // Slow
    ObjectTypeCapsule4, // Point
    ObjectTypeCapsule5, // Shield
    ObjectTypeCoin,
    ObjectTypeColorBall,
} ObjectType;

typedef enum {
    NormalStatus        = -1,
    FeverBoosterStatus  = 0,
    CoinBoosterStatus   = 1,
    SlowBoosterStatus   = 2,
    PointBoosterStatus  = 3,
    ShieldBoosterStatus = 4,
} FreeshStatus;

struct SoundInfo
{
    const char* filePath;
    float volume;
    
    SoundInfo(const char* path, float v)
    {
        filePath = path;
        volume = v;
    }
};

static const SoundInfo soundInfo[] = {
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    SoundInfo("sounds/Background_water.caf", 1),
    SoundInfo("sounds/gameStart.caf", 1),
    SoundInfo("sounds/freesh.caf", 1),
    SoundInfo("sounds/landing1.caf", 1),
    SoundInfo("sounds/landing2.caf", 1),
    SoundInfo("sounds/landing3.caf", 1),
    SoundInfo("sounds/crash.caf", 1),
    SoundInfo("sounds/capsule.caf", 1),
    SoundInfo("sounds/gone.caf", 1),
    SoundInfo("sounds/gameOver.caf", 1),
    SoundInfo("sounds/button.caf", 1),
    SoundInfo("sounds/button_notwork.caf", 1),
    SoundInfo("sounds/draw_start.caf", 1),
    SoundInfo("sounds/draw_zoomin.caf", 1),
    SoundInfo("sounds/draw_ballopen.caf", 1),
    SoundInfo("sounds/scroll.caf", 1),
    SoundInfo("sounds/Fanfare_new.caf", 1),
    SoundInfo("sounds/Fanfare_normal.caf", 1),
    SoundInfo("sounds/gift_idle_01.caf", 1),
    SoundInfo("sounds/gift_idle_02.caf", 1),
    SoundInfo("sounds/gift_idle_03.caf", 1),
    SoundInfo("sounds/gift_tap.caf", 1),
    SoundInfo("sounds/gift_open.caf", 1),
    SoundInfo("sounds/coin_raining_short.caf", 1),
    SoundInfo("sounds/coin_raining_normal.caf", 1),
    SoundInfo("sounds/coin_raining.caf", 1),
    SoundInfo("sounds/eat_boost_fever.caf", 1),
    SoundInfo("sounds/eat_boost_coin.caf", 1),
    SoundInfo("sounds/eat_boost_slow.caf", 1),
    SoundInfo("sounds/eat_boost_score.caf", 1),
    SoundInfo("sounds/eat_boost_shiled.caf", 1),
    SoundInfo("sounds/spike_crash_01.caf", 1),
    SoundInfo("sounds/spike_crash_02.caf", 1),
    SoundInfo("sounds/spike_crash_03.caf", 1),
    SoundInfo("sounds/get_coin_01.caf", 1),
    SoundInfo("sounds/get_coin_02.caf", 1),
    SoundInfo("sounds/get_coin_03.caf", 1),
    SoundInfo("sounds/get_coin_04.caf", 1),
    SoundInfo("sounds/shiled_crash.caf", 1),
    SoundInfo("sounds/score_reset.caf", 1),
    SoundInfo("sounds/death_hat_leather.caf", 1),
    SoundInfo("sounds/death_hat_iron.caf", 1),
    SoundInfo("sounds/best_score.caf", 1),
    SoundInfo("sounds/button_appear.caf", 1),
    SoundInfo("sounds/goldenfish_getcoin.caf", 1),
#else
    SoundInfo("soundsAndroid/Background_water.ogg", 1),
    SoundInfo("soundsAndroid/gameStart.ogg", 1),
    SoundInfo("soundsAndroid/freesh.ogg", 1),
    SoundInfo("soundsAndroid/landing1.ogg", 1),
    SoundInfo("soundsAndroid/landing2.ogg", 1),
    SoundInfo("soundsAndroid/landing3.ogg", 1),
    SoundInfo("soundsAndroid/crash.ogg", 1),
    SoundInfo("soundsAndroid/capsule.ogg", 1),
    SoundInfo("soundsAndroid/gone.ogg", 1),
    SoundInfo("soundsAndroid/gameOver.ogg", 1),
    SoundInfo("soundsAndroid/button.ogg", 1),
    SoundInfo("soundsAndroid/button_notwork.ogg", 1),
    SoundInfo("soundsAndroid/draw_start.ogg", 1),
    SoundInfo("soundsAndroid/draw_zoomin.ogg", 1),
    SoundInfo("soundsAndroid/draw_ballopen.ogg", 1),
    SoundInfo("soundsAndroid/scroll.ogg", 1),
    SoundInfo("soundsAndroid/Fanfare_new.ogg", 1),
    SoundInfo("soundsAndroid/Fanfare_normal.ogg", 1),
    SoundInfo("soundsAndroid/gift_idle_01.ogg", 1),
    SoundInfo("soundsAndroid/gift_idle_02.ogg", 1),
    SoundInfo("soundsAndroid/gift_idle_03.ogg", 1),
    SoundInfo("soundsAndroid/gift_tap.ogg", 1),
    SoundInfo("soundsAndroid/gift_open.ogg", 1),
    SoundInfo("soundsAndroid/coin_raining_short.ogg", 1),
    SoundInfo("soundsAndroid/coin_raining_normal.ogg", 1),
    SoundInfo("soundsAndroid/coin_raining.ogg", 1),
    SoundInfo("soundsAndroid/eat_boost_fever.ogg", 1),
    SoundInfo("soundsAndroid/eat_boost_coin.ogg", 1),
    SoundInfo("soundsAndroid/eat_boost_slow.ogg", 1),
    SoundInfo("soundsAndroid/eat_boost_score.ogg", 1),
    SoundInfo("soundsAndroid/eat_boost_shiled.ogg", 1),
    SoundInfo("soundsAndroid/spike_crash_01.ogg", 1),
    SoundInfo("soundsAndroid/spike_crash_02.ogg", 1),
    SoundInfo("soundsAndroid/spike_crash_03.ogg", 1),
    SoundInfo("soundsAndroid/get_coin_01.ogg", 1),
    SoundInfo("soundsAndroid/get_coin_02.ogg", 1),
    SoundInfo("soundsAndroid/get_coin_03.ogg", 1),
    SoundInfo("soundsAndroid/get_coin_04.ogg", 1),
    SoundInfo("soundsAndroid/shiled_crash.ogg", 1),
    SoundInfo("soundsAndroid/score_reset.ogg", 1),
    SoundInfo("soundsAndroid/death_hat_leather.ogg", 1),
    SoundInfo("soundsAndroid/death_hat_iron.ogg", 1),
    SoundInfo("soundsAndroid/best_score.ogg", 1),
    SoundInfo("soundsAndroid/button_appear.ogg", 1),
    SoundInfo("sounds/goldenfish_getcoin.ogg", 1),
#endif
};

static const SoundInfo getSoundFileName(int idx)
{
    //
    // Assume idx is in array range
    //
    return soundInfo[idx];
}

#define FreeshSize          62
#define NumOfHiddenSkins    8

typedef enum {
    FreeshTypeRandom = -1,
    //
    // Normal skins
    //
    FreeshTypeBasic = 0,
    FreeshTypeSnapBack,
    FreeshTypeJob,
    FreeshTypeRetro,
    FreeshTypeEtc,
    FreeshTypeParody,
    FreeshTypeSpecial,
    FreeshTypeRetroHistory,
    FreeshTypeHistory,

    //
    // Hidden skins
    //
    FreeshTypeRetroHidden,
    FreeshTypeParodyHidden,
    FreeshTypeSpecialHidden,
    FreeshTypeHistoryHidden,

    //
    // Golden fish
    //
    FreeshTypeGoldfish,

    //
    FreeshTypeMax
} FreeshType;

static int freeshTypes[] = {
    //
    // Normal skins
    //
    1,
    10,
    10,
    8,
    7,
    6,
    1,
    1,
    9,

    //
    // Hidden skins
    //
    1,
    4,
    2,
    1,

    //
    // Golden fish
    //
    1,
};

static const char* skinNames[] = {
    //
    // Normal skins
    //
    "basic_01",
    "snapback_01",
    "snapback_02",
    "snapback_03",
    "snapback_04",
    "snapback_05",
    "snapback_06",
    "snapback_07",
    "snapback_08",
    "snapback_09",
    "snapback_10", // idx = 10
    "job_01",
    "job_02",
    "job_03",
    "job_04",
    "job_05",
    "job_06",
    "job_07",
    "job_08",
    "job_09",
    "job_10",       // idx = 20
    "retro_01",
    "retro_02",
    "retro_04",
    "retro_05",
    "retro_06",
    "retro_07",
    "retro_08",
    "retro_09",
    "etc_01",
    "etc_02",       // idx = 30
    "etc_03",
    "etc_04",
    "etc_05",
    "etc_06",
    "etc_07",
    "parody_01",
    "parody_02",
    "parody_03",
    "parody_05",
    "parody_07",    // idx = 40
    "parody_08",
    "special_02",
    "retro_10",
    "history_02",
    "history_03",
    "history_04",
    "history_05",
    "history_06",
    "history_07",
    "history_08",
    "history_09",
    "history_10",

    //
    // Hidden skins
    //
    "retro_03",
    "parody_04",
    "parody_06",
    "parody_09",
    "parody_10",
    "special_01",
    "special_03",
    "history_01",
    
    //
    // Golden fish
    //
    "Goldfish",
};

static int freeshSoundTypes[] = {
    //
    // Normal skins
    //
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1, // idx = 10
    1,
    2,
    2,
    2,
    1,
    2,
    1,
    1,
    1,
    1,       // idx = 20
    2,
    1,
    1,
    2,
    1,
    1,
    1,
    1,
    1,
    1,       // idx = 30
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    1,
    1,    // idx = 40
    1,
    0,
    // Update
    0, // update
    0, // update
    0, // update
    0, // update
    0, // update
    0, // update
    0, // update
    0, // update
    0, // update
    0, // update
    
    //
    // Hidden skins
    //
    1,
    0,
    0,
    1,
    0,
    0,
    0,
    0, // update

    //
    // Golden fish
    //
    0,
};

static const char* specialSkinNames[] = {
    "special_01_Deepsea01",
    "special_01_Deepsea02",
    "special_01_Deepsea03",
    "special_02_Ghost01",
    "special_02_Ghost02",
    "special_02_Ghost03",
    "special_03_Red",
    "special_03_Blue",
    "special_03_Yellow",
    "Goldfish_01",
    "Goldfish_02",
    "Goldfish_03"
};

static const char* getNormalSkinName(int idx)
{
    return skinNames[idx];
}
static const char* getSpecialSkinName(int idx)
{
    if (!strcmp(skinNames[idx], "special_02")) {
        return specialSkinNames[3];
    }
    else if (!strcmp(skinNames[idx], "special_01")) {
        return specialSkinNames[0];
    }
    else if (!strcmp(skinNames[idx], "special_03")) {
        return specialSkinNames[6];
    }
    else if (!strcmp(skinNames[idx], "Goldfish")) {
        return specialSkinNames[9];
    }

    return "";
}
static const int getSkinIndexBySkinName(const char *skinName) {
    for (int i=0; i<FreeshSize; i++) {

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        //
        // TODO: fix this ugly codes!!!
        //
        if (!strcmp(skinName, "goldfish")) {
            if (!strcmp("Goldfish", skinNames[i])) {
                return i;
            }
        }
        else {
            if (!strcmp(skinName, skinNames[i])) {
                return i;
            }
        }
#else
        if (!strcmp(skinName, skinNames[i])) {
            return i;
        }
#endif
    }
    
    return  -1;
}
static const char* getSpecialSkinNameWithColor(int _idx, int currColor)
{
    int idx = 0;
    if (!strcmp(skinNames[_idx], "special_02")) {
        idx = 3 + currColor;
    }
    else if (!strcmp(skinNames[_idx], "special_01")) {
        idx = currColor;
    }
    else if (!strcmp(skinNames[_idx], "special_03")) {
        idx = 6 + currColor;
    }
    else if (!strcmp(skinNames[_idx], "Goldfish")) {
        idx = 9 + currColor;
    }

    return specialSkinNames[idx];
}

static const char* getFreeshKeyCode(int idx)
{
    return skinNames[idx];
}

//
// 1 : unique
// 2 : rare
// 3 : normal
// 0 : basic, hidden
//
static int freeshLevels[] = {
    //
    // Normal skins
    //
    0,
    3,
    3,
    2,
    2,
    3,
    3,
    2,
    2,
    2,
    3,
    1, // job_01
    2,
    2,
    1,
    2,
    1,
    2,
    2,
    2,
    2,
    2, // retro_01
    1,
    2,
    1,
    1,
    1,
    1,
    2,
    2, // etc_01
    2,
    2,
    2,
    3,
    1,
    2,
    2, // parody_01
    2,
    2,
    1,
    1,
    1,
    1, // special_02
    1, // retro_10
    2, // history_02
    2,
    2,
    2,
    1,
    2,
    1,
    2,
    2,

    //
    // Hidden skins
    //
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,

    //
    // Golden fish
    //
    0,
};

static const int getFreeshLevel(int idx)
{
    return freeshLevels[idx];
}

//
typedef enum {
    ColorSet_PurpleSet,
    ColorSet_SkyBlueSet,
    ColorSet_RedSet,
    ColorSet_GreenSet,
    ColorSet_YellowSet,
    ColorSet_WBSet,
    ColorSet_YBSet,
    ColorSet_WSSet,
    ColorSet_WBlueSet,
    ColorSet_DarkBGSet,
    ColorSet_DeepSeaSet,
    //
    ColorSet_ArmySet,
    ColorSet_WheatSet,
    //
    ColorSet_GoldSet,
    // newly Added
    ColorSet_Restro10Set,
    ColorSet_History01Set,
    ColorSet_History02Set,
    ColorSet_History03Set,
    ColorSet_History04Set,
    ColorSet_History05Set,
    ColorSet_History06Set,
    ColorSet_History07Set,
    ColorSet_History08Set,
    ColorSet_History09Set,
    ColorSet_History10Set,
    //
    ColorSet_Max,
} ColorSet;

typedef enum {
    InColor     = 0,
    OutColor    = 1,
    DotColor    = 2
} ColorIndex;

//
// for each ColorSet
// in color
// out color
// dot color
//
static const Color3B skinColorTable[] = {
    // ColorSet_PurpleSet
    Color3B(151,58,186),
    Color3B(115,37,191),
    Color3B(198,96,205),
    // ColorSet_SkyBlueSet
    Color3B(73,208,228),
    Color3B(38,103,127),
    Color3B(141,151,255),
    // ColorSet_RedSet
    Color3B(210,12,11),
    Color3B(155,10,9),
    Color3B(250,64,63),
    // ColorSet_GreenSet
    Color3B(62,198,123),
    Color3B(27,119,68),
    Color3B(115,248,175),
    // ColorSet_YellowSet
    Color3B(255,170,36),
    Color3B(169,95,15),
    Color3B(255,209,84),
    // ColorSet_WBSet
    Color3B(255,255,255),
    Color3B(11,23,45),
    Color3B(255,255,255),
    // ColorSet_YBSet
    Color3B(253,186,25),
    Color3B(11,23,45),
    Color3B(255,231,70),
    // ColorSet_WSSet
    Color3B(255,255,255),
    Color3B(200,200,200),
    Color3B(255,255,255),
    // ColorSet_WBlueSet
    Color3B(255,255,255),
    Color3B(7,100,255),
    Color3B(7,100,255),
    // ColorSet_DarkBGSet
    Color3B(13,13,13),
    Color3B(34,34,34),
    Color3B(253,186,2),
    // ColorSet_DeepSeaSet
    Color3B(112,255,205),
    Color3B(10,26,54),
    Color3B(255,255,255),
    // ColorSet_ArmySet
    Color3B(155,149,102),
    Color3B(35,39,31),
    Color3B(241,228,176),
    // ColorSet_WheatSet
    Color3B(232,211,179),
    Color3B(197,179,148),
    Color3B(255,242,221),
    // ColorSet_GoldSet
    Color3B(254,223,10),
    Color3B(254,247,45),
    Color3B(255,255,255),
    //ColorSet_Restro10Set,
    Color3B(243,158,2),
    Color3B(255,205,52),
    Color3B(0,0,0),
    //ColorSet_History01Set,
    Color3B(1,1,1),
    Color3B(143,20,1),
    Color3B(255,181,2),
    //ColorSet_History02Set,
    Color3B(44,25,4),
    Color3B(60,34,3),
    Color3B(255,181,2),
    //ColorSet_History03Set,
    Color3B(1,1,1),
    Color3B(26,26,26),
    Color3B(168,16,16),
    //ColorSet_History04Set,
    Color3B(1,1,1),
    Color3B(39,39,39),
    Color3B(27,227,135),
    //ColorSet_History05Set,
    Color3B(54,35,11),
    Color3B(83,54,18),
    Color3B(44,74,4),
    //ColorSet_History06Set,
    Color3B(117,117,117),
    Color3B(169,169,169),
    Color3B(207,23,23),
    //ColorSet_History07Set,
    Color3B(35,58,4),
    Color3B(44,74,4),
    Color3B(171,184,72),
    //ColorSet_History08Set,
    Color3B(46,50,53),
    Color3B(68,71,76),
    Color3B(171,171,171),
    //ColorSet_History09Set,
    Color3B(54,38,26),
    Color3B(71,48,30),
    Color3B(1,1,1),
    //ColorSet_History10Set,
    Color3B(13,73,171),
    Color3B(15,83,224),
    Color3B(207,23,23),
};

static const ColorSet skinColorSets[] = {
    //
    // Normal skins
    //
    ColorSet_PurpleSet,
    ColorSet_PurpleSet,
    ColorSet_PurpleSet,
    ColorSet_PurpleSet,
    ColorSet_WBlueSet,
    ColorSet_WSSet,
    ColorSet_WheatSet,
    ColorSet_GreenSet,
    ColorSet_ArmySet,
    ColorSet_PurpleSet,
    ColorSet_GreenSet,
    ColorSet_SkyBlueSet,
    ColorSet_PurpleSet,
    ColorSet_SkyBlueSet,
    ColorSet_ArmySet,
    ColorSet_RedSet,
    ColorSet_RedSet,
    ColorSet_GreenSet,
    ColorSet_SkyBlueSet,
    ColorSet_GreenSet,
    ColorSet_GreenSet,
    ColorSet_PurpleSet,
    ColorSet_YellowSet,
    ColorSet_WBSet,
    ColorSet_GreenSet,
    ColorSet_GreenSet,
    ColorSet_PurpleSet,
    ColorSet_PurpleSet,
    ColorSet_GreenSet,
    ColorSet_GreenSet,
    ColorSet_GreenSet,
    ColorSet_YBSet,
    ColorSet_WSSet,
    ColorSet_GreenSet,
    ColorSet_PurpleSet,
    ColorSet_WBSet,
    ColorSet_WBSet,
    ColorSet_WBSet,
    ColorSet_WBlueSet,
    ColorSet_DarkBGSet,
    ColorSet_DarkBGSet,
    ColorSet_WBSet,
    ColorSet_WSSet,
    // Update
    ColorSet_Restro10Set,
    ColorSet_History02Set,
    ColorSet_History03Set,
    ColorSet_History04Set,
    ColorSet_History05Set,
    ColorSet_History06Set,
    ColorSet_History07Set,
    ColorSet_History08Set,
    ColorSet_History09Set,
    ColorSet_History10Set,

    //
    // Hidden skins
    //
    ColorSet_PurpleSet,
    ColorSet_PurpleSet,
    ColorSet_WBSet,
    ColorSet_DarkBGSet,
    ColorSet_DarkBGSet,
    ColorSet_DeepSeaSet,
    ColorSet_PurpleSet,
    // Update
    ColorSet_History01Set,

    //
    // Golden fish
    //
    ColorSet_GoldSet,
};

static const Color3B getSkinColorSet(int idx, ColorIndex c)
{
    ColorSet currSet = skinColorSets[idx];

    return skinColorTable[currSet*3 + c];
}


typedef enum {
    BGColorSet_DeepBlue = 0,
    BGColorSet_White,
    BGColorSet_DeepRed,
    BGColorSet_Yellow,
    BGColorSet_Purple,
    BGColorSet_EGreen,
    BGColorSet_Pink,
    BGColorSet_Silver,
    BGColorSet_Blue,
    BGColorSet_Black,
    BGColorSet_ArmyGreen,
    BGColorSet_SkyBlue,
    BGColorSet_LimeGreen,
    // newly added
    BGColorSet_History02,
    BGColorSet_History07,
    BGColorSet_History09,
    //
    BGColorSet_Max,
} BGColorSet;

static const Color3B bgColorTable[] = {
    Color3B(11,24,47),
    Color3B(255,255,255),
    Color3B(81,3,3),
    Color3B(253,186,25),
    Color3B(148,25,253),
    Color3B(3,191,165),
    Color3B(255,112,138),
    Color3B(200,200,200),
    Color3B(7,100,255),
    Color3B(0,0,0),
    //
    Color3B(76,95,45),
    Color3B(51,158,230),
    Color3B(197,221,44),
    // newly added
    Color3B(81,49,9),
    Color3B(171,184,28),
    Color3B(52,48,44),
};

#define RGBValueByPercentage(__C__) (float)__C__/100 * 255
static const float flowBGOpacityTable[BGColorSet_Max][2] = {
    {RGBValueByPercentage(10), RGBValueByPercentage(10)},
    {RGBValueByPercentage(100), RGBValueByPercentage(100)},
    {RGBValueByPercentage(60), RGBValueByPercentage(60)},
    {RGBValueByPercentage(60), RGBValueByPercentage(60)},
    {RGBValueByPercentage(60), RGBValueByPercentage(60)},
    {RGBValueByPercentage(60), RGBValueByPercentage(60)},
    {RGBValueByPercentage(60), RGBValueByPercentage(60)},
    {RGBValueByPercentage(60), RGBValueByPercentage(60)},
    {RGBValueByPercentage(34), RGBValueByPercentage(34)},
    {RGBValueByPercentage(10), RGBValueByPercentage(30)},
    //
    {RGBValueByPercentage(34), RGBValueByPercentage(34)},
    {RGBValueByPercentage(34), RGBValueByPercentage(34)},
    {RGBValueByPercentage(60), RGBValueByPercentage(60)},
    // newly added
    {RGBValueByPercentage(34), RGBValueByPercentage(34)},
    {RGBValueByPercentage(34), RGBValueByPercentage(34)},
    {RGBValueByPercentage(10), RGBValueByPercentage(10)},
};

static const BGColorSet bgColorSets[] = {
    //
    // Normal skins
    //
    BGColorSet_DeepBlue,
    BGColorSet_DeepBlue,
    BGColorSet_DeepBlue,
    BGColorSet_DeepBlue,
    BGColorSet_SkyBlue,
    BGColorSet_Black,
    BGColorSet_DeepBlue,
    BGColorSet_DeepBlue,
    BGColorSet_ArmyGreen,
    BGColorSet_LimeGreen,
    BGColorSet_LimeGreen,
    BGColorSet_DeepBlue,
    BGColorSet_DeepBlue,
    BGColorSet_DeepBlue,
    BGColorSet_ArmyGreen,
    BGColorSet_Silver, // white -> silver
    BGColorSet_DeepBlue,
    BGColorSet_DeepBlue,
    BGColorSet_DeepBlue,
    BGColorSet_Yellow,
    BGColorSet_Yellow,
    BGColorSet_DeepBlue,
    BGColorSet_DeepBlue,
    BGColorSet_DeepBlue,
    BGColorSet_DeepBlue,
    BGColorSet_Black,
    BGColorSet_DeepBlue,
    BGColorSet_DeepBlue,
    BGColorSet_DeepBlue,
    BGColorSet_EGreen,
    BGColorSet_Pink,
    BGColorSet_Yellow,
    BGColorSet_Purple,
    BGColorSet_Silver,
    BGColorSet_DeepBlue,
    BGColorSet_Purple,
    BGColorSet_DeepBlue,
    BGColorSet_Blue,
    BGColorSet_Blue,
    BGColorSet_Silver,
    BGColorSet_DeepBlue,
    BGColorSet_Silver, // white -> silver
    BGColorSet_Black,
    // Update
    BGColorSet_Yellow,
    BGColorSet_History02,
    BGColorSet_Black,
    BGColorSet_Black,
    BGColorSet_ArmyGreen,
    BGColorSet_Silver,
    BGColorSet_History07,
    BGColorSet_Silver,
    BGColorSet_History09,
    BGColorSet_EGreen,
    
    //
    // Hidden skins
    //
    BGColorSet_Purple,
    BGColorSet_DeepBlue,
    BGColorSet_DeepBlue,
    BGColorSet_DeepBlue,
    BGColorSet_EGreen,
    BGColorSet_Black,
    BGColorSet_DeepBlue,
    // Update
    BGColorSet_Silver,

    //
    // Golden fish
    //
    BGColorSet_Black,
};

static const Color3B getBGColorSet(int idx)
{
    BGColorSet currSet = bgColorSets[idx];
    return bgColorTable[currSet];
}

typedef enum {
    FreeshColorSet_Red = 0,
    FreeshColorSet_Blue,
    FreeshColorSet_Yellow,
    FreeshColorSet_EGreen,
    FreeshColorSet_Pink,
    FreeshColorSet_YGreen,
    //
    FreeshColorSet_DeepSea1,
    FreeshColorSet_DeepSea2,
    FreeshColorSet_DeepSea3,
    //
    FreeshColorSet_Ghost1,
    FreeshColorSet_Ghost2,
    FreeshColorSet_Ghost3,
    //
    FreeshColorSet_Gold1,
    FreeshColorSet_Gold2,
    FreeshColorSet_Gold3,
    // newly added
    FreeshColorSet_Black,
    FreeshColorSet_SGray,
    FreeshColorSet_YGray,
    FreeshColorSet_Brown,
    FreeshColorSet_DBlue,
    FreeshColorSet_DGree,
    FreeshColorSet_YKhaki,
    FreeshColorSet_DKhaki,
} FreeshColorSet;

static const Color3B freeshColorTable[] = {
    Color3B(255,60,33),
    Color3B(7,100,255),
    Color3B(255,181,2),
    Color3B(3,191,165),
    Color3B(255,112,138),
    Color3B(183,236,61),
    //
    Color3B(142,254,215),
    Color3B(245,131,255),
    Color3B(10,26,54),
    //
    Color3B(219,219,219),
    Color3B(0,0,0),
    Color3B(32,107,120),
    //
    Color3B(255,228,71),
    Color3B(239,165,40),
    Color3B(208,214,216),
    // newly added
    Color3B(34,34,34),
    Color3B(217,217,217),
    Color3B(72,72,72),
    Color3B(83,33,11),
    Color3B(22,38,82),
    Color3B(44,74,4),
    Color3B(219,195,113),
    Color3B(170,154,129),
};

static const char *aniNameColorPrefix[] = {
    "R_",
    "B_",
    "Y_",
    "EG_",
    "P_",
    "YG_",
    //
    "Deepsea01_",
    "Deepsea02_",
    "Deepsea03_",
    //
    "Ghost01_",
    "Ghost02_",
    "Ghost03_",
    //
    "goldfish01_",
    "goldfish02_",
    "goldfish03_",
    // newly added
    "Black_",
    "S-Gray_",
    "Y-Gray_",
    "Brown_",
    "D-Blue_",
    "D-Green_",
    "Y-Khaki_",
    "D-Khaki_",
};

static const FreeshColorSet freeshColorSets[FreeshSize][3] = {
    // basic
    {FreeshColorSet_Red, FreeshColorSet_Blue, FreeshColorSet_Yellow},
    
    // snapback
    {FreeshColorSet_Red, FreeshColorSet_Blue, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_Yellow, FreeshColorSet_EGreen},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    // job
    {FreeshColorSet_Red, FreeshColorSet_Blue, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_Blue, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_Blue, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Pink},
    {FreeshColorSet_Red, FreeshColorSet_Blue, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Pink},
    {FreeshColorSet_Red, FreeshColorSet_Blue, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    // retro
    {FreeshColorSet_Red, FreeshColorSet_Blue, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_YGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Pink},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_Yellow, FreeshColorSet_EGreen},
    // etc
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Pink},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Pink},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Pink},
    {FreeshColorSet_Red, FreeshColorSet_Yellow, FreeshColorSet_YGreen},
    {FreeshColorSet_Red, FreeshColorSet_Pink, FreeshColorSet_EGreen},
    {FreeshColorSet_Red, FreeshColorSet_Pink, FreeshColorSet_EGreen},
    // parody
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Blue},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Blue},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_Blue, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    // special
    {FreeshColorSet_Ghost1, FreeshColorSet_Ghost2, FreeshColorSet_Ghost3},
    // Update
    {FreeshColorSet_Red, FreeshColorSet_Black, FreeshColorSet_SGray},
    {FreeshColorSet_Red, FreeshColorSet_Yellow, FreeshColorSet_Brown},
    {FreeshColorSet_Red, FreeshColorSet_YGray, FreeshColorSet_Black},
    {FreeshColorSet_Red, FreeshColorSet_DBlue, FreeshColorSet_EGreen},
    {FreeshColorSet_Red, FreeshColorSet_DGree, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_SGray, FreeshColorSet_DBlue},
    {FreeshColorSet_Red, FreeshColorSet_YKhaki, FreeshColorSet_DGree},
    {FreeshColorSet_Red, FreeshColorSet_SGray, FreeshColorSet_EGreen},
    {FreeshColorSet_Red, FreeshColorSet_DKhaki, FreeshColorSet_YGray},
    {FreeshColorSet_Red, FreeshColorSet_Yellow, FreeshColorSet_EGreen},

    //
    // hidden
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Pink},
    {FreeshColorSet_Red, FreeshColorSet_Blue, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_Red, FreeshColorSet_EGreen, FreeshColorSet_Yellow},
    {FreeshColorSet_DeepSea1, FreeshColorSet_DeepSea2, FreeshColorSet_DeepSea3},
    {FreeshColorSet_Red, FreeshColorSet_Blue, FreeshColorSet_Yellow},
    // Update
    {FreeshColorSet_Red, FreeshColorSet_Yellow, FreeshColorSet_YGray},

    //
    // Golden fish
    //
    {FreeshColorSet_Gold1, FreeshColorSet_Gold2, FreeshColorSet_Gold3},
};

typedef enum {
    ColorOrderFirst = 0,
    ColorOrderSecond,
    ColorOrderThird
} ColorOrder;
static const FreeshColorSet getFreeshColorSet(int idx, ColorOrder order)
{
    return freeshColorSets[idx][order];
}
static const Color3B getFreeshColor(int idx, ColorOrder order)
{
    FreeshColorSet currSet = freeshColorSets[idx][order];
    return freeshColorTable[currSet];
}

//
// returns degree which start from 12 oclock and run clockwise
// from radian which start from 3 oclock and run counterclockwise
//
static float get12OclockDegreeFrom3OclockRadian(float radian)
{
    float degree = 0;
    if (radian <= 0) {
        degree = CC_RADIANS_TO_DEGREES(-radian);
        degree += 90;
    }
    else {
        degree = CC_RADIANS_TO_DEGREES(radian);
        degree = 90 - degree;
    }
    return degree;
}

static const char* iconFileNames[] = {
    "icon_gift.png",
    "icon_draw.png",
    "icon_share.png",
};

inline const char* getIconFileName(int idx)
{
    return iconFileNames[idx];
}

static const char* iconKeyStrings[] = {
    "ResultGiftBtn",
    "ResultDraw",
    "ResultShareBtn",
};

inline const char* getIconKeyStrings(int idx)
{
    return iconKeyStrings[idx];
}

static const char* btnStringKeys[] = {
    "ResultGiftBtn",
    "ResultDrawBtn",
    "ResultShareBtn",
};

//
// Background Music
//
static const int backgroundMusicIdx[] = {
    //
    // Normal skins
    //
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    9,
    0,
    0,
    9,
    0,
    9,
    0,
    0,
    0,
    0,
    0,
    9,
    0,
    9,
    0,
    9,
    9,
    2,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    3,
    9,
    4,
    2,
    5,
    1,
    6,
    5,
    7,
    2,

    //
    // Hidden skins
    //
    0,
    0,
    0,
    9,
    0,
    0,
    9,
    5,
    //
    // Golden fish
    //
    8,
};

static const char* backgroundMusicFile[] = {
    "clock",
    "MiddleAge", // 1
    "80s",
    "indian_01",
    "BanjoWind", // 4
    "korean",
    "nonla",
    "QuestionMark", // 7
    "goldenfish",
    "clockwisemarch", // 9
};
static bool hasSongWriter(int songIdx)
{
    if (songIdx == 1 || songIdx == 4 || songIdx == 7) {
        return true;
    }
    
    return false;
}
static std::string getBackgroundMusicFileName(int skinIdx)
{
    int idx = backgroundMusicIdx[skinIdx];
    std::stringstream stream;
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    stream << "sounds/music/music_" << backgroundMusicFile[idx] << ".caf";
#else
    stream << "soundsAndroid/music/music_" << backgroundMusicFile[idx] << ".ogg";
#endif
    std::string fileName = stream.str();
    
    return fileName;
}

//
// Timon Background Sound Info
//
typedef enum {
    TimonDefault = 0,
    TimonHipHop,
    TimonWar,
    TimonHorse,
    TimonPirate,
    TimonSlapstick,
    TimonSleep,
    TimonGhost,
    TimonElvis,
    Timon999,
    TimonDeepsea,
    TimonKiller,
} TimonBGType;
static const int timonBGSoundIdx[] = {
    //
    // Normal skins
    //
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    -1,
    0,
    0,
    2,
    0,
    -1,
    0,
    0,
    0,
    0,
    0,
    3,
    0,
    -1,
    4,
    -1,
    -1,
    -1,
    5,
    5,
    5,
    5,
    5,
    6,
    5,
    5,
    0,
    10,
    0,
    11,
    5,
    7,
    -1,
    13,
    12,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,

    //
    // Hidden skins
    //
    0,
    0,
    8,
    9,
    0,
    10,
    10,
    -1,
    
    //
    // Golden fish
    //
    -1,
};

static const char* timonBGSound[] = {
    "default",
    "hiphop",
    "war",
    "horse",
    "pirate", // 4
    "slapstick",
    "sleep",
    "ghost",
    "elvis", // 8
    "999",
    "deepsea",
    "killer",
    // update
    "ninja", // 12
    "middleage", // 13
};
static std::string getTimonSoundFileName(int skinIdx)
{
    int idx = timonBGSoundIdx[skinIdx];
    std::stringstream stream;
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    stream << "sounds/timon/timon_" << timonBGSound[idx] << ".caf";
#else
    stream << "soundsAndroid/timon/timon_" << timonBGSound[idx] << ".ogg";
#endif
    std::string fileName = stream.str();

    return fileName;
}

//
// Freesh Jump Sound Info
//
typedef enum {
    FreeshJumpDefault = 0,
    FreeshJumpHipHop,
    FreeshJumpSlapstick,
    FreeshJumpGhost,
    FreeshJumpDeepsea,
} FreeshJumpType;

static const int freeshJumpSoundIdx[] = {
    //
    // Normal skins
    //
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    2,
    2,
    2,
    2,
    2,
    0,
    2,
    2,
    0,
    0,
    0,
    0,
    2,
    3,
    // Update
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,

    //
    // Hidden skins
    //
    0,
    0,
    0,
    0,
    0,
    4,
    0,
    // Update
    0,
    
    //
    // Golden fish
    //
    5,
};

static const char* freeshJumpSound[] = {
    "default",
    "hiphop",
    "slapstick",
    "ghost",
    "deepsea",
    "goldenfish"
};

static const int freeshJumpSoundList[6][5] = {
    {3, 1,2,3,0},
    {4, 1,2,3,4},
    {4, 1,2,3,4},
    {3, 1,2,3,0},
    {3, 1,2,3,0},
    {3, 1,2,3,0},
};

static std::string getFreeshJumpSoundFileName(int skinIdx)
{
    int idx = freeshJumpSoundIdx[skinIdx];

    int variations = freeshJumpSoundList[idx][0];
    int r = rand()%variations + 1;
    int variation = freeshJumpSoundList[idx][r];

    // jump_ghost_01.caf
    std::stringstream stream;
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    stream << "sounds/jump/jump_" << freeshJumpSound[idx] << "_";
    stream << std::setfill('0') << std::setw(2) << variation << ".caf";
#else
    stream << "soundsAndroid/jump/jump_" << freeshJumpSound[idx] << "_";
    stream << std::setfill('0') << std::setw(2) << variation << ".ogg";
#endif
    std::string fileName = stream.str();

    return fileName;
}

//
// Freesh Voice Sound Info
//
typedef enum {
    FreeshVoiceNone = -1,
    FreeshVoiceMan = 0,
    FreeshVoiceWar,
    FreeshVoiceWoman,
    FreeshVoiceHorse,
    FreeshVoicePirate,
    FreeshVoicePig,
    FreeshVoiceSleep,
    FreeshVoiceMagic,
    FreeshVoiceMan2,
} FreeshVoiceType;

static const int freeshVoiceSoundIdx[] = {
    //
    // Normal skins
    //
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    0 ,
    -1,
    -1,
    1 ,
    2 ,
    8 ,
    -1,
    -1,
    -1,
    8 ,
    -1,
    3 ,
    0 ,
    8 ,
    4 ,
    2 ,
    -1,
    -1,
    -1,
    5 ,
    -1,
    -1,
    -1,
    6 ,
    -1,
    -1,
    -1,
    -1,
    2 ,
    -1,
    -1,
    -1,
    // Update
    15,
    12,
    11,
    0 ,
    13,
    12,
    -1,
    14,
    -1,
    2 ,

    //
    // Hidden skins
    //
    7 ,
    7 ,
    -1,
    -1,
    9, // albert
    -1,
    -1,
    // Update
    10,

    //
    // Golden fish
    //
    -1,
};

static const char* freeshVoiceSound[] = {
    "man",
    "war",
    "woman",
    "horse",
    "pirate",
    "pig",  // 5
    "sleep",
    "magic",
    "man",
    "albert",   // 9
    // update
    "koreancrown",
    "ninja",
    "pirate",
    "lumberjack", // 13
    "seonbi",   // 14
    "indian",   // 15
};

static const int freeshVoiceSoundList[16][5] = {
    {4, 2,3,6,7},
    {3, 1,2,3,0},
    {4, 1,2,3,4},
    {3, 1,2,3,0},
    {3, 1,2,3,0},
    {3, 1,2,3,0},
    {2, 1,2,0,0},
    {3, 1,2,3,0},
    {2, 4,5,0,0},
    {4, 1,2,3,4},
    // update
    {4, 1,2,3,4},
    {3, 1,2,3,0},
    {3, 1,2,6,0},
    {3, 1,2,3,0},
    {4, 1,2,3,4},
    {4, 1,2,3,4},
};

static std::string getFreeshVoiceSoundFileName(int skinIdx)
{
    int idx = freeshVoiceSoundIdx[skinIdx];
    int variations = freeshVoiceSoundList[idx][0];
    int r = rand()%variations + 1;
    int variation = freeshVoiceSoundList[idx][r];
    
    // jump_ghost_01.caf
    std::stringstream stream;
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    stream << "sounds/voice/voice_" << freeshVoiceSound[idx] << "_";
    stream << std::setfill('0') << std::setw(2) << variation << ".caf";
#else
    stream << "soundsAndroid/voice/voice_" << freeshVoiceSound[idx] << "_";
    stream << std::setfill('0') << std::setw(2) << variation << ".ogg";
#endif

    std::string fileName = stream.str();
    
    return fileName;
}

//
// Freesh Boost Sound Info
//
typedef enum {
    FreeshBoostNone = -1,
    FreeshBoostMan = 0,
    FreeshBoostPirate,
} FreeshBoostType;

static const int freeshBoostSoundIdx[] = {
    //
    // Normal skins
    //
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    0, // job_01
    -1,
    -1,
    -1,
    1, // job_05
    0, // job_06
    -1,
    -1,
    -1,
    0, // job_10
    -1,
    -1,
    0, // retro_04
    0, // retro_05
    2, // retro_06
    1, // retro_07
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    1, // parody_05
    -1,
    -1,
    -1,
    // Update
    6,
    -1,
    5 , // history_03
    0 , // history_04
    -1,
    -1,
    -1,
    -1,
    -1,
    1 , // history_10

    //
    // Hidden skins
    //
    -1,
    -1,
    -1,
    -1,
    3, // parody_10
    -1,
    -1,
    // Update
    4 ,

    //
    // Golden fish
    //
    -1,
};

static const char* freeshBoostSound[] = {
    "man",
    "woman",
    "pirate",
    "albert",
    // update
    "koreancrown",
    "ninja",
    "indian",
};

static const int freeshBoostSoundList[7][3] = {
    {1, 1,0},
    {1, 5,0},
    {2, 4,5},
    {1, 5,0},
    // update
    {1, 5,0},
    {1, 3,0},
    {1, 4,0},
};

static std::string getFreeshBoostSoundFileName(int skinIdx)
{
    int idx = freeshBoostSoundIdx[skinIdx];
    int variations = freeshBoostSoundList[idx][0];
    int r = rand()%variations + 1;
    int variation = freeshBoostSoundList[idx][r];

    //
    std::stringstream stream;
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    stream << "sounds/voice/voice_" << freeshBoostSound[idx] << "_";
    stream << std::setfill('0') << std::setw(2) << variation << ".caf";
#else
    stream << "soundsAndroid/voice/voice_" << freeshBoostSound[idx] << "_";
    stream << std::setfill('0') << std::setw(2) << variation << ".ogg";
#endif
    std::string fileName = stream.str();
    
    return fileName;
}

struct PackageInfo {
    int skinIdx[3];
    int grade;
    
    PackageInfo(int t, int l, int r, int g) {
        skinIdx[0] = t;
        skinIdx[1] = l;
        skinIdx[2] = r;
        grade = g;
    }
};

static const PackageInfo packages[10] =
{
    PackageInfo(14,11,27,1),
    PackageInfo(39,40,41,1),
    PackageInfo(16,44,48,1),
    PackageInfo(13,15,12,2),
    PackageInfo(31,30,32,3),
    PackageInfo(25,37,24,1),
    PackageInfo(23,43,22,1),
    PackageInfo(45,49,50,1),
    PackageInfo(10,7,4,3),
    PackageInfo(46,28,52,2),
};

#endif
