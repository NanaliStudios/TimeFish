//
//  UserInfo.cpp
//  Timefish
//
//  Created by midchow on 2015. 1. 23..
//
//

#include "UserInfo.h"
#include "sha2.h"
#include "PlayScene.h"

#include <iostream>
#include <iomanip>
#include <string>

static UserInfo *_sharedUserInfo = nullptr;

USING_NS_CC;

#define HashSize 2 * SHA256_DIGEST_SIZE + 1

using namespace std;

UserInfo* UserInfo::getInstance()
{
    if (!_sharedUserInfo)
    {
        _sharedUserInfo = new (nothrow) UserInfo();
        _sharedUserInfo->init();
    }
    
    return _sharedUserInfo;
}

bool UserInfo::init(void)
{
    srand((unsigned)time(NULL));

    hasShowAd = false;
    for (int i=0; i<NumOfAchievement; i++) {
        getNewAchievement[i] = false;
    }
    playTimeTotal = 0;
    achievementInfo = 0;

    firstTime = false;
    haveSeenTutorial = false;

    currPlayCount = 0;
    haveSeenVideo = 0;
    
    for (int i=0; i<5; i++) {
        lapInfo[i] = 0;
    }

    devicePerformance = NoRecording_NoFlowing;

    //
    baseScore = 0x3A4B5 + rand()%1000;
    //
    // Initialization : This is IMPORTANT due to baseScore!
    //
    setPlayCount(0);
    setHighScore(0);
    addToTotalScore(baseScore);
    addToDeathCount(DataTypeDeathCountRed, baseScore);
    addToDeathCount(DataTypeDeathCountBlue, baseScore);
    addToDeathCount(DataTypeDeathCountYellow, baseScore);
    addToDeathCount(DataTypeDeathCountFirst, baseScore);
    addToDeathCount(DataTypeDeathCountWithoutBall, baseScore);
    addToDeathCount(DataTypeDeathCountJump, baseScore);
    addToDeathCount(DataTypeDeathCountTotal, baseScore);
    increaseBestScoreCount(baseScore);
    increaseRunCount(baseScore);
    setSelectedPosterType(10); // 10 is default poster.
    initCoins(baseScore);
    dataBackup[DataTypeTotalCapsuleCount] = baseScore;
    setTrialSkinIndex(-1); // -1 means no trial
    setTrialCount(0);
    setRated(0);
    setLiked(0);
    setNoti(1);

    // init skins
    dataBackup[DataTypeSkins1] = 0b00000010000000000000000000000000;
    dataBackup[DataTypeSkins2] = 0b00000110000000000000000000000000;
    dataBackup[DataTypeSkins3] = 0b00001110000000000000000000000000;
    dataBackup[DataTypeHiddenBackup] = 0b00101010000000000000000000000000;

    // initially unlock first freesh skin
    unlockSkinAt(0);

    //
    initJumpCount();
    initCapsuleCount();

    //
    bannerQueue = new BannerQueue();

    //
    currLangType = Application::getInstance()->getCurrentLanguage();

    //
    // TEMP
    //
//    skinType = FreeshTypeBasic;
    randomSkin = false;
    skinNum = 1;
    firstTimeResultTypeIndex = 0;
    resultGroupTypeIndex = 0;

    //
    timeCheckIn = -1;//std::time(0); // Current Time(sec)
    secondSinceLastBoot = -1;

#ifdef ProductPurchaseCapture
    packageIdx = 0;
#endif

    return true;
}

UserInfo::~UserInfo()
{
    delete bannerQueue;
}

//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// version 106:
// [2][2][1][1][1][5][6] =
// [play count digits][best score digits][sound option][ad option][play counts + fake padding][best score]
//
// version 107: [2][2][1][1][1][5][5][10][10][5][5][5][5][5][5][5][3][4][2] =
// [play count digits][best score digits][sound option][ad option][play counts + fake padding][best score][achievement info][total score][death count1][death count2][death count3][death count4][death count5][death count6][death count7][best scores count][run count][selected poster]
//
// version110: [2][2][1][1][1][5][5][10][10][5][5][5][5][5][5][5][3][4][2] + [2][10][10][10][10][10][1][1][1][1][2][2][10]=
//[play count digits][best score digits][sound option][ad option][play counts + fake padding][best score][achievement info][total score][death count1][death count2][death count3][death count4][death count5][death count6][death count7][best scores count][run count][selected poster] +
//[coin digits][coins + fake padding][skin1][skin2][skin3][hidden unlock cond][like option][review option][noti option][capsule count][giftTime digits][giftTime]
//

//
// version201: [2][2][1][1][1][5][5][10][10][5][5][5][5][5][5][5][3][4][2] + [2][10][10][10][10][10][1][1][1][1][2][2][10] + [2][10][1][3] =
//[play count digits][best score digits][sound option][ad option][play counts + fake padding][best score][achievement info][total score][death count1][death count2][death count3][death count4][death count5][death count6][death count7][best scores count][run count][selected poster] +
//[coin digits][coins + fake padding][skin1][skin2][skin3][hidden unlock cond][like option][review option][noti option][capsule count][giftTime digits][giftTime] +
//[soft time digits][soft time + fake padding][skin digits][skin num + fake padding]
//

//
// version201: [2][2][1][1][1][5][5][10][10][5][5][5][5][5][5][5][3][4][2] + [2][10][10][10][10][10][1][1][1][1][2][2][10] + [2][10][1][3] + [1] =
//[play count digits][best score digits][sound option][ad option][play counts + fake padding][best score][achievement info][total score][death count1][death count2][death count3][death count4][death count5][death count6][death count7][best scores count][run count][selected poster] +
//[coin digits][coins + fake padding][skin1][skin2][skin3][hidden unlock cond][like option][review option][noti option][capsule count][giftTime digits][giftTime] +
//[soft time digits][soft time + fake padding][skin digits][skin num + fake padding][firstTimeResultTypeIndex][resultGroupTypeIndex]
//

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
#pragma mark -
#pragma mark SAVE
void UserInfo::saveUserData()
{
    //
    // Recent version is 203
    //
    saveUserDataVersion203();
}

void UserInfo::saveUserDataVersion106()
{
    int tmp1 = getSoundOption();
    int tmp4 = getAdRemoveOption();
    int tmp2 = getTotalPlayCount();
    int tmp3 = getBestScore();
    
    // count digits for play count
    unsigned int digits1 = digitsCount(tmp2) + 1; // +1 is for for fake
    
    // count digits for best score
    unsigned int digits2 = digitsCount(tmp3);

    srand((unsigned)time(NULL));
    char randChar1 = (char)(rand()%10) + 48;
    char randChar2 = (char)(rand()%10) + 48;
    
    // assume digits is less than 10
    stringstream stream;
    stream << setfill('0') << setw(2) << digits1 << setfill('0') << setw(2) << digits2 << tmp1 << tmp4 << setfill(randChar1) << setw(digits1) <<tmp2 << setfill(randChar2) << setw(6) <<tmp3;
    string dataStr = stream.str();
    
    int size = (int)dataStr.size();
    
    //
    // DATA : ::<9><#!%!
    // HASH : 9650b35d6bb36a4e840e8503ad846b3400c01a1051cec7b631dee846ed547d71
    //
    {
        unsigned char* data = (unsigned char*)malloc(size + 1); // +1 is for NULL
        memcpy(data, dataStr.c_str(), size + 1); // +1 is for NULL
        
        //
        // encoding
        //
        for (int i=0; i<size; i++) {
            data[i] =  (0b1010+i) ^ data[i];
        }
        
        stringstream stream;
        stream << data;
        string dataStr = stream.str();
        UserDefault::getInstance()->setStringForKey("DATA", dataStr); // save encoded value
        
        memset(data, '0', size + 1);
        free(data);
    }
    {
        unsigned char* hash = getHashValue((const unsigned char*)dataStr.c_str(), size);
        
        stringstream stream;
        stream << hash;
        string hashStr = stream.str();
        UserDefault::getInstance()->setStringForKey("HASH", hashStr); // hash value for original value
        
        memset(hash, '0', HashSize);
        free(hash);
    }

    //
    // Manual invoke
    //
    UserDefault::getInstance()->flush();
}

void UserInfo::saveUserDataVersion107()
{
    int tmp1 = getSoundOption();
    int tmp4 = getAdRemoveOption();
    int tmp2 = getTotalPlayCount();
    int tmp3 = getBestScore();
    int tmp5 = getTotalScore();

    // count digits for play count
    unsigned int digits1 = digitsCount(tmp2) + 1; // +1 is for for fake

    // count digits for best score
    unsigned int digits2 = digitsCount(tmp3);

    srand((unsigned)time(NULL));
    char randChar1 = (char)(rand()%10) + 48;
    char randChar2 = (char)(rand()%10) + 48;

    // assume digits is less than 10
    stringstream stream;
    stream << setfill('0') << setw(2) << digits1 << setfill('0') << setw(2) << digits2;
    stream << tmp1 << tmp4;
    stream << setfill(randChar1) << setw(digits1) <<tmp2;
    stream << setfill(randChar2) << setw(6) <<tmp3;

    //
    // Achievement
    //
    stream << setfill('0') << setw(10) << achievementInfo;

    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    //
    // NOTE: Just ignore overflowed digits...
    //

    //
    // Total Score
    //
    stream << setfill('0') << setw(10) << cutByDigits(tmp5, 10);
    
    //
    // Death Count
    //
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountRed), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountBlue), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountYellow), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountFirst), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountWithoutBall), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountJump), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountTotal), 5);
    
    // Best Score Count
    stream << setfill('0') << setw(3) << cutByDigits(getBestScoreCount(), 3);

    // Run Count
    stream << setfill('0') << setw(4) << cutByDigits(getRunCount(), 4);

    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////

    // Poster Index
    stream << setfill('0') << setw(2) << getSelectedPosterType();

    //
    string dataStr = stream.str();

//    log("%s", dataStr.c_str());

    int size = (int)dataStr.size();
    
    //
    // DATA : ::<9><#!%!
    // HASH : 9650b35d6bb36a4e840e8503ad846b3400c01a1051cec7b631dee846ed547d71
    //
    {
        unsigned char* data = (unsigned char*)malloc(size + 1); // +1 is for NULL
        memcpy(data, dataStr.c_str(), size + 1); // +1 is for NULL
        
        //
        // encoding
        //
        int baseFirst = 0b1010;
        for (int i=0; i<size; i++) {
            int salt = 1 << (i%5);
            int base = baseFirst - (i%baseFirst);
            if (base <= 0) {
                base = baseFirst;
            }
            data[i] =  (base+salt) ^ data[i];
        }

        //
        stringstream stream;
        stream << data;
        string dataStr = stream.str();
        UserDefault::getInstance()->setStringForKey("DATA", dataStr); // save encoded value
        
        memset(data, '0', size + 1);
        free(data);
    }
    {
        unsigned char* hash = getHashValue((const unsigned char*)dataStr.c_str(), size);
        
        stringstream stream;
        stream << hash;
        string hashStr = stream.str();
        UserDefault::getInstance()->setStringForKey("HASH", hashStr); // hash value for original value
        
        UserDefault::getInstance()->flush();

        memset(hash, '0', HashSize);
        free(hash);
    }

    //
    // Manual invoke
    //
    UserDefault::getInstance()->flush();
}

void UserInfo::saveUserDataVersion110()
{
    int tmp1 = getSoundOption();
    int tmp4 = getAdRemoveOption();
    int tmp2 = getTotalPlayCount();
    int tmp3 = getBestScore();
    int tmp5 = getTotalScore();
    
    // count digits for play count
    unsigned int digits1 = digitsCount(tmp2) + 1; // +1 is for for fake
    
    // count digits for best score
    unsigned int digits2 = digitsCount(tmp3);
    
    srand((unsigned)time(NULL));
    char randChar1 = (char)(rand()%10) + 48;
    char randChar2 = (char)(rand()%10) + 48;
    
    // assume digits is less than 10
    stringstream stream;
    stream << setfill('0') << setw(2) << digits1 << setfill('0') << setw(2) << digits2;
    stream << tmp1 << tmp4;
    stream << setfill(randChar1) << setw(digits1) <<tmp2;
    stream << setfill(randChar2) << setw(6) <<tmp3;
    
    //
    // Achievement
    //
    stream << setfill('0') << setw(10) << achievementInfo;
    
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    //
    // NOTE: Just ignore overflowed digits...
    //
    
    //
    // Total Score
    //
    stream << setfill('0') << setw(10) << cutByDigits(tmp5, 10);
    
    //
    // Death Count
    //
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountRed), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountBlue), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountYellow), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountFirst), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountWithoutBall), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountJump), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountTotal), 5);
    
    // Best Score Count
    stream << setfill('0') << setw(3) << cutByDigits(getBestScoreCount(), 3);
    
    // Run Count
    stream << setfill('0') << setw(4) << cutByDigits(getRunCount(), 4);
    
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    
    // Poster Index
    stream << setfill('0') << setw(2) << getSelectedPosterType();

    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////// newly added data
    

    // 020500338138900016778759000001656300000000000000100000000020000100003001000410
    
//    {string dataStr = stream.str();log("%s", dataStr.c_str());}
    //
    // Coin digits and Coins
    //
    int tmpCoin = getCoins();
    // count digits for best coins
    unsigned int digits3 = (tmpCoin==0)? 1: digitsCount(tmpCoin);
    stream << setfill('0') << setw(2) << digits3;
    for (int i=0; i<10-digits3; i++) { // random padding
        char randChar3 = (char)(rand()%10) + 48;
        stream << randChar3;
    }
    stream << tmpCoin;
//    {string dataStr = stream.str();log("%s", dataStr.c_str());}

    //
    // Skin info
    //
    stream << setfill('0') << setw(10) << dataBackup[DataTypeSkins1];
    stream << setfill('0') << setw(10) << dataBackup[DataTypeSkins2];
    stream << setfill('0') << setw(10) << dataBackup[DataTypeSkins3];
//    {string dataStr = stream.str();log("%s", dataStr.c_str());}

    //
    // hidden skin unlock condition
    //
    stream << setfill('0') << setw(10) << dataBackup[DataTypeHiddenBackup];
//    {string dataStr = stream.str();log("%s", dataStr.c_str());}

    //
    // Facebook Like Option/ Review Option
    //
    stream << getLiked() << getRated() << getNoti();

    //
    // Capsule count
    //
    stream << setfill('0') << setw(2) << getWitchCapsuleCount();

    //
    // Gift Time digits and Gift Time
    //
    int tmpTime = (int)secondSinceLastBoot;
    unsigned int digits4 = 0;
    if (tmpTime < 0) {
        stream << 99;
    }
    else {
        digits4 = digitsCount(tmpTime);
        stream << setfill('0') << setw(2) << digits4;
    }
    for (int i=0; i<10-digits4; i++) { // random padding
        char randChar4 = (char)(rand()%10) + 48;
        stream << randChar4;
    }
    if (digits4 > 0) {
        stream << tmpTime;
    }

    //
    string dataStr = stream.str();
    
//    log("%s", dataStr.c_str());
    
    int size = (int)dataStr.size();
    
    //
    // DATA : ::<9><#!%!
    // HASH : 9650b35d6bb36a4e840e8503ad846b3400c01a1051cec7b631dee846ed547d71
    //
    {
        unsigned char* data = (unsigned char*)malloc(size + 1); // +1 is for NULL
        memcpy(data, dataStr.c_str(), size + 1); // +1 is for NULL
        
        //
        // encoding
        //
        int baseFirst = 0b1001;
        for (int i=0; i<size; i++) {
            int salt = 1 << (i%5);
            int base = baseFirst - (i%baseFirst);
            if (base <= 0) {
                base = baseFirst;
            }
            data[i] =  (base+salt) ^ data[i];
        }
        
        //
        stringstream stream;
        stream << data;
        string dataStr = stream.str();

        UserDefault::getInstance()->setStringForKey("DATA", dataStr); // save encoded value
        
        memset(data, '0', size + 1);
        free(data);
    }
    {
        unsigned char* hash = getHashValue((const unsigned char*)dataStr.c_str(), size);
        
        stringstream stream;
        stream << hash;
        string hashStr = stream.str();
        UserDefault::getInstance()->setStringForKey("HASH", hashStr); // hash value for original value
        
        memset(hash, '0', HashSize);
        free(hash);
    }

    //
    // Manual invoke
    //
    UserDefault::getInstance()->flush();
}

void UserInfo::saveUserDataVersion201()
{
    int tmp1 = getSoundOption();
    int tmp4 = getAdRemoveOption();
    int tmp2 = getTotalPlayCount();
    int tmp3 = getBestScore();
    int tmp5 = getTotalScore();
    
    // count digits for play count
    unsigned int digits1 = digitsCount(tmp2) + 1; // +1 is for for fake
    
    // count digits for best score
    unsigned int digits2 = digitsCount(tmp3);
    
    srand((unsigned)time(NULL));
    char randChar1 = (char)(rand()%10) + 48;
    char randChar2 = (char)(rand()%10) + 48;
    
    // assume digits is less than 10
    stringstream stream;
    stream << setfill('0') << setw(2) << digits1 << setfill('0') << setw(2) << digits2;
    stream << tmp1 << tmp4;
    stream << setfill(randChar1) << setw(digits1) <<tmp2;
    stream << setfill(randChar2) << setw(6) <<tmp3;
    
    //
    // Achievement
    //
    stream << setfill('0') << setw(10) << achievementInfo;
    
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    //
    // NOTE: Just ignore overflowed digits...
    //
    
    //
    // Total Score
    //
    stream << setfill('0') << setw(10) << cutByDigits(tmp5, 10);
    
    //
    // Death Count
    //
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountRed), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountBlue), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountYellow), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountFirst), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountWithoutBall), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountJump), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountTotal), 5);
    
    // Best Score Count
    stream << setfill('0') << setw(3) << cutByDigits(getBestScoreCount(), 3);
    
    // Run Count
    stream << setfill('0') << setw(4) << cutByDigits(getRunCount(), 4);
    
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    
    // Poster Index
    stream << setfill('0') << setw(2) << getSelectedPosterType();
    
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////// newly added data
    
    
    // 020500338138900016778759000001656300000000000000100000000020000100003001000410
    
//    {string dataStr = stream.str();log("%s", dataStr.c_str());}
    //
    // Coin digits and Coins
    //
    int tmpCoin = getCoins();
    // count digits for best coins
    unsigned int digits3 = (tmpCoin==0)? 1: digitsCount(tmpCoin);
    stream << setfill('0') << setw(2) << digits3;
    for (int i=0; i<10-digits3; i++) { // random padding
        char randChar3 = (char)(rand()%10) + 48;
        stream << randChar3;
    }
    stream << tmpCoin;
//    {string dataStr = stream.str();log("%s", dataStr.c_str());}
    
    //
    // Skin info
    //
    stream << setfill('0') << setw(10) << dataBackup[DataTypeSkins1];
    stream << setfill('0') << setw(10) << dataBackup[DataTypeSkins2];
    stream << setfill('0') << setw(10) << dataBackup[DataTypeSkins3];
//    {string dataStr = stream.str();log("%s", dataStr.c_str());}
    
    //
    // hidden skin unlock condition
    //
    stream << setfill('0') << setw(10) << dataBackup[DataTypeHiddenBackup];
//    {string dataStr = stream.str();log("%s", dataStr.c_str());}
    
    //
    // Facebook Like Option/ Review Option
    //
    stream << getLiked() << getRated() << getNoti();
    
    //
    // Capsule count
    //
    stream << setfill('0') << setw(2) << getWitchCapsuleCount();

    //
    // Gift Time digits and Gift Time(Hardware ABS Time)
    //
    int hTime = (int)secondSinceLastBoot;
    unsigned int digits4 = 0;
    if (hTime < 0) {
        stream << 99;
    }
    else {
        digits4 = digitsCount(hTime);
        stream << setfill('0') << setw(2) << digits4;
    }
    for (int i=0; i<10-digits4; i++) { // random padding
        char randChar4 = (char)(rand()%10) + 48;
        stream << randChar4;
    }
    if (digits4 > 0) {
        stream << hTime;
    }
    
    //
    // Gift Time digits and Gift Time(Software Time)
    //
    int sTime = (int)timeCheckIn;
    unsigned int digits5 = 0;
    if (sTime < 0) {
        stream << 99;
    }
    else {
        digits5 = digitsCount(sTime);
        stream << setfill('0') << setw(2) << digits5;
    }
    for (int i=0; i<10-digits5; i++) { // random padding
        char randChar5 = (char)(rand()%10) + 48;
        stream << randChar5;
    }
    if (digits5 > 0) {
        stream << sTime;
    }
    
    //
    // Selected Skin Number
    //
    if (randomSkin) {
        skinNum = 0;
    }
    unsigned int digits6 = (skinNum==0)? 1: digitsCount(skinNum);
//    stream << setfill('0') << setw(3) << digits6;
    stream << digits6;
    for (int i=0; i<3-digits6; i++) { // random padding
        char randChar3 = (char)(rand()%10) + 48;
        stream << randChar3;
    }
    stream << skinNum;
    
    //
    string dataStr = stream.str();
    
//    log("%s", dataStr.c_str());
    
    int size = (int)dataStr.size();
    
    //
    // DATA : ::<9><#!%!
    // HASH : 9650b35d6bb36a4e840e8503ad846b3400c01a1051cec7b631dee846ed547d71
    //
    {
        unsigned char* data = (unsigned char*)malloc(size + 1); // +1 is for NULL
        memcpy(data, dataStr.c_str(), size + 1); // +1 is for NULL
        
        //
        // encoding
        //
        int baseFirst = 0b0101;
        for (int i=0; i<size; i++) {
            int salt = 1 << (i%5);
            int base = baseFirst - (i%baseFirst);
            if (base <= 0) {
                base = baseFirst;
            }
            data[i] =  (base+salt) ^ data[i];
        }
        
        //
        stringstream stream;
        stream << data;
        string dataStr = stream.str();
        
        UserDefault::getInstance()->setStringForKey("DATA", dataStr); // save encoded value
        
        memset(data, '0', size + 1);
        free(data);
    }
    {
        unsigned char* hash = getHashValue((const unsigned char*)dataStr.c_str(), size);
        
        stringstream stream;
        stream << hash;
        string hashStr = stream.str();
        UserDefault::getInstance()->setStringForKey("HASH", hashStr); // hash value for original value
        
        memset(hash, '0', HashSize);
        free(hash);
    }

    //
    // Manual invoke
    //
    UserDefault::getInstance()->flush();
}

void UserInfo::saveUserDataVersion203()
{
    int tmp1 = getSoundOption();
    int tmp4 = getAdRemoveOption();
    int tmp2 = getTotalPlayCount();
    int tmp3 = getBestScore();
    int tmp5 = getTotalScore();
    
    // count digits for play count
    unsigned int digits1 = digitsCount(tmp2) + 1; // +1 is for for fake
    
    // count digits for best score
    unsigned int digits2 = digitsCount(tmp3);
    
    srand((unsigned)time(NULL));
    char randChar1 = (char)(rand()%10) + 48;
    char randChar2 = (char)(rand()%10) + 48;
    
    // assume digits is less than 10
    stringstream stream;
    stream << setfill('0') << setw(2) << digits1 << setfill('0') << setw(2) << digits2;
    stream << tmp1 << tmp4;
    stream << setfill(randChar1) << setw(digits1) <<tmp2;
    stream << setfill(randChar2) << setw(6) <<tmp3;
    
    //
    // Achievement
    //
    stream << setfill('0') << setw(10) << achievementInfo;
    
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    //
    // NOTE: Just ignore overflowed digits...
    //
    
    //
    // Total Score
    //
    stream << setfill('0') << setw(10) << cutByDigits(tmp5, 10);
    
    //
    // Death Count
    //
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountRed), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountBlue), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountYellow), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountFirst), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountWithoutBall), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountJump), 5);
    stream << setfill('0') << setw(5) << cutByDigits(getDeathCount(DataTypeDeathCountTotal), 5);
    
    // Best Score Count
    stream << setfill('0') << setw(3) << cutByDigits(getBestScoreCount(), 3);
    
    // Run Count
    stream << setfill('0') << setw(4) << cutByDigits(getRunCount(), 4);
    
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    
    // Poster Index
    stream << setfill('0') << setw(2) << getSelectedPosterType();
    
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////// newly added data
    
    
    // 020500338138900016778759000001656300000000000000100000000020000100003001000410
    
    //    {string dataStr = stream.str();log("%s", dataStr.c_str());}
    //
    // Coin digits and Coins
    //
    int tmpCoin = getCoins();
    // count digits for best coins
    unsigned int digits3 = (tmpCoin==0)? 1: digitsCount(tmpCoin);
    stream << setfill('0') << setw(2) << digits3;
    for (int i=0; i<10-digits3; i++) { // random padding
        char randChar3 = (char)(rand()%10) + 48;
        stream << randChar3;
    }
    stream << tmpCoin;
    //    {string dataStr = stream.str();log("%s", dataStr.c_str());}
    
    //
    // Skin info
    //
    stream << setfill('0') << setw(10) << dataBackup[DataTypeSkins1];
    stream << setfill('0') << setw(10) << dataBackup[DataTypeSkins2];
    stream << setfill('0') << setw(10) << dataBackup[DataTypeSkins3];
    //    {string dataStr = stream.str();log("%s", dataStr.c_str());}
    
    //
    // hidden skin unlock condition
    //
    stream << setfill('0') << setw(10) << dataBackup[DataTypeHiddenBackup];
    //    {string dataStr = stream.str();log("%s", dataStr.c_str());}
    
    //
    // Facebook Like Option/ Review Option
    //
    stream << getLiked() << getRated() << getNoti();
    
    //
    // Capsule count
    //
    stream << setfill('0') << setw(2) << getWitchCapsuleCount();
    
    //
    // Gift Time digits and Gift Time(Hardware ABS Time)
    //
    int hTime = (int)secondSinceLastBoot;
    unsigned int digits4 = 0;
    if (hTime < 0) {
        stream << 99;
    }
    else {
        digits4 = digitsCount(hTime);
        stream << setfill('0') << setw(2) << digits4;
    }
    for (int i=0; i<10-digits4; i++) { // random padding
        char randChar4 = (char)(rand()%10) + 48;
        stream << randChar4;
    }
    if (digits4 > 0) {
        stream << hTime;
    }
    
    //
    // Gift Time digits and Gift Time(Software Time)
    //
    int sTime = (int)timeCheckIn;
    unsigned int digits5 = 0;
    if (sTime < 0) {
        stream << 99;
    }
    else {
        digits5 = digitsCount(sTime);
        stream << setfill('0') << setw(2) << digits5;
    }
    for (int i=0; i<10-digits5; i++) { // random padding
        char randChar5 = (char)(rand()%10) + 48;
        stream << randChar5;
    }
    if (digits5 > 0) {
        stream << sTime;
    }
    
    //
    // Selected Skin Number
    //
    if (randomSkin) {
        skinNum = 0;
    }
    unsigned int digits6 = (skinNum==0)? 1: digitsCount(skinNum);
    //    stream << setfill('0') << setw(3) << digits6;
    stream << digits6;
    for (int i=0; i<3-digits6; i++) { // random padding
        char randChar3 = (char)(rand()%10) + 48;
        stream << randChar3;
    }
    stream << skinNum;
    
    //
    //
    //
    stream << MIN(firstTimeResultTypeIndex, FirstTimeResultTypeIndexMax);
    
    //
    //
    //
    stream << resultGroupTypeIndex;

    //
    string dataStr = stream.str();
    
    //    log("%s", dataStr.c_str());
    
    int size = (int)dataStr.size();
    
    //
    // DATA : ::<9><#!%!
    // HASH : 9650b35d6bb36a4e840e8503ad846b3400c01a1051cec7b631dee846ed547d71
    //
    {
        unsigned char* data = (unsigned char*)malloc(size + 1); // +1 is for NULL
        memcpy(data, dataStr.c_str(), size + 1); // +1 is for NULL
        
        //
        // encoding
        //
        int baseFirst = 0b0110;
        for (int i=0; i<size; i++) {
            int salt = 1 << (i%5);
            int base = baseFirst - (i%baseFirst);
            if (base <= 0) {
                base = baseFirst;
            }
            data[i] =  (base+salt) ^ data[i];
        }
        
        //
        stringstream stream;
        stream << data;
        string dataStr = stream.str();
        
        UserDefault::getInstance()->setStringForKey("DATA", dataStr); // save encoded value
        
        memset(data, '0', size + 1);
        free(data);
    }
    {
        unsigned char* hash = getHashValue((const unsigned char*)dataStr.c_str(), size);
        
        stringstream stream;
        stream << hash;
        string hashStr = stream.str();
        UserDefault::getInstance()->setStringForKey("HASH", hashStr); // hash value for original value
        
        memset(hash, '0', HashSize);
        free(hash);
    }
    
    //
    // Manual invoke
    //
    UserDefault::getInstance()->flush();
}

#pragma mark -
#pragma mark READ
void UserInfo::readUserData()
{
    string dataStr = UserDefault::getInstance()->getStringForKey("DATA");
    int size = (int)dataStr.size();

    if (size == 0) {
        //
        // first time
        //
        TapjoyX::getInstance()->setUserLevel(1);
        firstTime = true;
    }
    else {
        int dataVersion = getUserDataFormat();
        if (dataVersion == 106) {
            //
            // assume old version user is first time
            //
            TapjoyX::getInstance()->setUserLevel(1);
            firstTime = true;

            //
            // version 1.06
            //
            readUserDataVersion106();
            
            //
            // version up!
            //
            saveUserDataVersion110();
        }
        else if (dataVersion == 107) {
            //
            // assume old version user is first time
            //
            TapjoyX::getInstance()->setUserLevel(1);
            firstTime = true;

            //
            // version 1.07
            //
            readUserDataVersion107();
            
            //
            //
            //
            saveUserDataVersion110();
        }
        else if (dataVersion == 110) {
            //
            // assume old version user is first time
            //
            TapjoyX::getInstance()->setUserLevel(1);
            firstTime = true;

            //
            // version 2.00
            //
            readUserDataVersion110();
        }
        else if (dataVersion == 201) {
            haveSeenTutorial = true;
            
            //
            // version 2.01
            //
            readUserDataVersion201();
        }
        else if (dataVersion == 203) {
            haveSeenTutorial = true;
            
            //
            // version 2.03
            //
            readUserDataVersion203();
        }
        else {
            // wrong hash: not supported version of hacked???
            return;
        }
    }

//    unlockSkinAt(0);
//    unlockSkinAt(20);
//    unlockSkinAt(26);
//    unlockSkinAt(34);
//    for (int i=0; i<FreeshSize - NumOfHiddenSkins; i++) {
//    for (int i=0; i<FreeshSize; i++) {
//        unlockSkinAt(i);
//    }

#ifdef AUTOJUMP_TEST
    for (int i=0; i<FreeshSize; i++) {
        unlockSkinAt(i);
    }
#endif

}

int UserInfo::getUserDataFormat()
{
    string dataStr = UserDefault::getInstance()->getStringForKey("DATA");
    int size = (int)dataStr.size();

    int version = -1;

    string keyHash = UserDefault::getInstance()->getStringForKey("HASH");

    unsigned char* data = (unsigned char*)malloc(size + 1); // +1 is for NULL
    memcpy(data, dataStr.c_str(), size + 1); // +1 is for NULL

    //
    // check 106
    //
    if (version == -1) {
        // encoding
        for (int i=0; i<size; i++) {
            data[i] =  (0b1010+i) ^ data[i];
        }

        stringstream stream;
        stream << data;
        string dataStr = stream.str();

        unsigned char* hash = getHashValue((const unsigned char*)dataStr.c_str(), size);
        // compare two hash values(calculated, saved)
        if (!strcmp((const char*)hash, keyHash.c_str())) {
            version = 106;
        }
        else {
            // reverse data
            for (int i=0; i<size; i++) {
                data[i] =  (0b1010+i) ^ data[i];
            }
        }

        memset(hash, '0', HashSize);
        free(hash);
    }

    //
    // check 107
    //
    if (version == -1) {
        // encoding
        int baseFirst = 0b1010;
        for (int i=0; i<size; i++) {
            int salt = 1 << (i%5);
            int base = baseFirst - (i%baseFirst);
            if (base <= 0) {
                base = baseFirst;
            }
            data[i] =  (base+salt) ^ data[i];
        }
        
        stringstream stream;
        stream << data;
        string dataStr = stream.str();
        
        unsigned char* hash = getHashValue((const unsigned char*)dataStr.c_str(), size);
        // compare two hash values(calculated, saved)
        if (!strcmp((const char*)hash, keyHash.c_str())) {
            version = 107;
        }
        else {
            // reverse data
            int baseFirst = 0b1010;
            for (int i=0; i<size; i++) {
                int salt = 1 << (i%5);
                int base = baseFirst - (i%baseFirst);
                if (base <= 0) {
                    base = baseFirst;
                }
                data[i] =  (base+salt) ^ data[i];
            }
        }
        
        memset(hash, '0', HashSize);
        free(hash);
    }
    
    //
    // check 110
    //
    if (version == -1) {
        // encoding
        int baseFirst = 0b1001;
        for (int i=0; i<size; i++) {
            int salt = 1 << (i%5);
            int base = baseFirst - (i%baseFirst);
            if (base <= 0) {
                base = baseFirst;
            }
            data[i] =  (base+salt) ^ data[i];
        }
        
        stringstream stream;
        stream << data;
        string dataStr = stream.str();
        
        unsigned char* hash = getHashValue((const unsigned char*)dataStr.c_str(), size);
        // compare two hash values(calculated, saved)
        if (!strcmp((const char*)hash, keyHash.c_str())) {
            version = 110;
        }
        else {
            // reverse data
            int baseFirst = 0b1001;
            for (int i=0; i<size; i++) {
                int salt = 1 << (i%5);
                int base = baseFirst - (i%baseFirst);
                if (base <= 0) {
                    base = baseFirst;
                }
                data[i] =  (base+salt) ^ data[i];
            }
        }
        
        memset(hash, '0', HashSize);
        free(hash);
    }
    
    //
    // check 201
    //
    if (version == -1) {
        // encoding
        int baseFirst = 0b0101;
        for (int i=0; i<size; i++) {
            int salt = 1 << (i%5);
            int base = baseFirst - (i%baseFirst);
            if (base <= 0) {
                base = baseFirst;
            }
            data[i] =  (base+salt) ^ data[i];
        }
        
        stringstream stream;
        stream << data;
        string dataStr = stream.str();
        
        unsigned char* hash = getHashValue((const unsigned char*)dataStr.c_str(), size);
        // compare two hash values(calculated, saved)
        if (!strcmp((const char*)hash, keyHash.c_str())) {
            version = 201;
        }
        else {
            // reverse data
            int baseFirst = 0b0101;
            for (int i=0; i<size; i++) {
                int salt = 1 << (i%5);
                int base = baseFirst - (i%baseFirst);
                if (base <= 0) {
                    base = baseFirst;
                }
                data[i] =  (base+salt) ^ data[i];
            }
        }
        
        memset(hash, '0', HashSize);
        free(hash);
    }
    
    //
    // check 203
    //
    if (version == -1) {
        // encoding
        int baseFirst = 0b0110;
        for (int i=0; i<size; i++) {
            int salt = 1 << (i%5);
            int base = baseFirst - (i%baseFirst);
            if (base <= 0) {
                base = baseFirst;
            }
            data[i] =  (base+salt) ^ data[i];
        }
        
        stringstream stream;
        stream << data;
        string dataStr = stream.str();
        
        unsigned char* hash = getHashValue((const unsigned char*)dataStr.c_str(), size);
        // compare two hash values(calculated, saved)
        if (!strcmp((const char*)hash, keyHash.c_str())) {
            version = 203;
        }
        else {
            // reverse data
            int baseFirst = 0b0110;
            for (int i=0; i<size; i++) {
                int salt = 1 << (i%5);
                int base = baseFirst - (i%baseFirst);
                if (base <= 0) {
                    base = baseFirst;
                }
                data[i] =  (base+salt) ^ data[i];
            }
        }
        
        memset(hash, '0', HashSize);
        free(hash);
    }

    memset(data, '0', size+1);
    free(data);

    return version;
}

void UserInfo::readUserDataVersion106()
{
    string dataStr = UserDefault::getInstance()->getStringForKey("DATA");
    int size = (int)dataStr.size();
    bool valid = false;

    //
    // decoding
    //
    unsigned char* data = (unsigned char*)malloc(size + 1); // +1 is for NULL
    memcpy(data, dataStr.c_str(), size + 1); // +1 is for NULL
    
    for (int i=0; i<size; i++) {
        data[i] =  (0b1010+i) ^ data[i];
    }
    
    {
        stringstream stream;
        stream << data;
        string dataStr = stream.str();
        
        unsigned char* hash = getHashValue((const unsigned char*)dataStr.c_str(), size);
        string keyHash = UserDefault::getInstance()->getStringForKey("HASH");
        
//        log("%s", keyHash.c_str());

        if (!strcmp((const char*)hash, keyHash.c_str())) {
            valid = true;
        }
        
        memset(hash, '0', HashSize);
        free(hash);
    }
    
    //
    // set data only if it's valid!
    //
    if (valid) {

//        for (int i=0; i<size; i++) {
//            log("%c, %d", data[i], (int)data[i]);
//        }
        
        //
        // character 0 is 48 when ascii code(decimal)
        // we use only numbers, so just subtracting will be ok
        //
        int digit1 = (int)(data[0] - 48) * 10 + (int)(data[1] - 48);
        int digit2 = (int)(data[2] - 48) * 10 + (int)(data[3] - 48);
        
        // sound Option
        int soundOption = (int)(data[4] - 48);
        
        // ad status
        int adStatus = (int)(data[5] - 48);
        
        // playCount
        int playCount = 0;
        {
            int endIdx = 5 + digit1;
            int decimalDigit = 1;
            for (int i=endIdx; i > endIdx - digit1 + 1; i--) { // // +1 is for fake
                playCount += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        
        // highScore
        int highScore = 0;
        {
            int endIdx = size-1;
            int decimalDigit = 1;
            // start from end!!!
            for (int i=endIdx; i > endIdx - digit2; i--) {
                highScore += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        
//        log("%d, %d, %d, %d", soundOption, adStatus, playCount, highScore);
        
        setSoundOption(soundOption);
        SoundManager::getInstance()->setMuteFromFile(soundOption);
        setAdRemoveOption(adStatus);
        setTotalPlayCount(playCount);
        setHighScore(highScore);
    }
    
    memset(data, '0', size+1);
    free(data);
}

void UserInfo::readUserDataVersion107()
{
    string dataStr = UserDefault::getInstance()->getStringForKey("DATA");
    int size = (int)dataStr.size();
    bool valid = false;
    
    //
    // decoding
    //
    unsigned char* data = (unsigned char*)malloc(size + 1); // +1 is for NULL
    memcpy(data, dataStr.c_str(), size + 1); // +1 is for NULL

    int baseFirst = 0b1010;
    for (int i=0; i<size; i++) {
        int salt = 1 << (i%5);
        int base = baseFirst - (i%baseFirst);
        if (base <= 0) {
            base = baseFirst;
        }
        data[i] =  (base+salt) ^ data[i];
    }
    
    {
        stringstream stream;
        stream << data;
        string dataStr = stream.str();
        
        unsigned char* hash = getHashValue((const unsigned char*)dataStr.c_str(), size);
        string keyHash = UserDefault::getInstance()->getStringForKey("HASH");
        
//        log("%s", keyHash.c_str());

        if (!strcmp((const char*)hash, keyHash.c_str())) {
            valid = true;
        }
        
        memset(hash, '0', HashSize);
        free(hash);
    }
    
    //
    // set data only if it's valid!
    //
    if (valid) {

//        for (int i=0; i<size; i++) {
//            log("%c, %d", data[i], (int)data[i]);
//        }
        
        //
        // character 0 is 48 when ascii code(decimal)
        // we use only numbers, so just subtracting will be ok
        //
        int digit1 = (int)(data[0] - 48) * 10 + (int)(data[1] - 48);
        int digit2 = (int)(data[2] - 48) * 10 + (int)(data[3] - 48);
        
        // sound Option
        int soundOption = (int)(data[4] - 48);
        
        // ad status
        int adStatus = (int)(data[5] - 48);
        
        // playCount
        int fromIdx = 5 + digit1;
        int toIdx = fromIdx - digit1 + 1; // +1 is for fake
        int playCount = 0;
        {
            int decimalDigit = 1;
            for (int i=fromIdx; i > toIdx; i--) {
                playCount += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }

        // highScore
        fromIdx = fromIdx + 6;
        toIdx = fromIdx - digit2;
        int highScore = 0;
        {
            int decimalDigit = 1;
            // start from end!!!
            for (int i=fromIdx; i > toIdx; i--) {
                highScore += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        
        // achievement
        fromIdx = fromIdx + 1;
        toIdx = fromIdx + 10;
        int achievement = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            achievement = atoi(dataStr.c_str());

            //
            achievementInfo = achievement;
        }
        
        // total score
        fromIdx = toIdx;
        toIdx = fromIdx + 10;
        int totalScore = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            totalScore = atoi(dataStr.c_str());
        }

        // death count
        int deathCount = 0;
        fromIdx = toIdx;
        for (int i=0; i<7; i++) { // 7 death counts
            stringstream stream;
            toIdx = fromIdx+5; // 5 digits each
            for (int j=fromIdx; j<toIdx; j++) {
                stream << data[j];
            }
            string dataStr = stream.str();
            deathCount = atoi(dataStr.c_str());

            fromIdx = toIdx;

            addToDeathCount(DataTypeDeathCountRed + i, deathCount);
        }

        // best score count
        fromIdx = toIdx;
        toIdx = fromIdx + 3;
        int bestScoreCount = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            bestScoreCount = atoi(dataStr.c_str());
        }
        
        // run count
        fromIdx = toIdx;
        toIdx = fromIdx + 4;
        int runCount = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            runCount = atoi(dataStr.c_str());
        }
        
        // Poster Index
        fromIdx = toIdx;
        toIdx = fromIdx + 2;
        int posterType = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            posterType = atoi(dataStr.c_str());
        }

        //
        setSoundOption(soundOption);
        SoundManager::getInstance()->setMuteFromFile(soundOption);
        setAdRemoveOption(adStatus);
        setTotalPlayCount(playCount);
        setHighScore(highScore);
        addToTotalScore(totalScore);
        increaseBestScoreCount(bestScoreCount);
        increaseRunCount(runCount);
        setSelectedPosterType(posterType);
    }
    
    memset(data, '0', size+1);
    free(data);
}

void UserInfo::readUserDataVersion110()
{
    string dataStr = UserDefault::getInstance()->getStringForKey("DATA");
    int size = (int)dataStr.size();
    bool valid = false;
    
    //
    // decoding
    //
    unsigned char* data = (unsigned char*)malloc(size + 1); // +1 is for NULL
    memcpy(data, dataStr.c_str(), size + 1); // +1 is for NULL
    
    int baseFirst = 0b1001;
    for (int i=0; i<size; i++) {
        int salt = 1 << (i%5);
        int base = baseFirst - (i%baseFirst);
        if (base <= 0) {
            base = baseFirst;
        }
        data[i] =  (base+salt) ^ data[i];
    }
    
    {
        stringstream stream;
        stream << data;
        string dataStr = stream.str();
        
        unsigned char* hash = getHashValue((const unsigned char*)dataStr.c_str(), size);
        string keyHash = UserDefault::getInstance()->getStringForKey("HASH");
        
//        log("%s", keyHash.c_str());

        if (!strcmp((const char*)hash, keyHash.c_str())) {
            valid = true;
        }
        
        memset(hash, '0', HashSize);
        free(hash);
    }
    
    //
    // set data only if it's valid!
    //
    if (valid) {
        
//        for (int i=0; i<size; i++) {
//            std::cout << data[i];
////            log("%c, %d", data[i], (int)data[i]);
//        }
//        std::cout << std::endl;
        
        //
        // character 0 is 48 when ascii code(decimal)
        // we use only numbers, so just subtracting will be ok
        //
        int digit1 = (int)(data[0] - 48) * 10 + (int)(data[1] - 48);
        int digit2 = (int)(data[2] - 48) * 10 + (int)(data[3] - 48);
        
        // sound Option
        int soundOption = (int)(data[4] - 48);
        
        // ad status
        int adStatus = (int)(data[5] - 48);
        
        // playCount
        int fromIdx = 5 + digit1;
        int toIdx = fromIdx - digit1 + 1; // +1 is for fake
        int playCount = 0;
        {
            int decimalDigit = 1;
            for (int i=fromIdx; i > toIdx; i--) {
                playCount += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        
        // highScore
        fromIdx = fromIdx + 6;
        toIdx = fromIdx - digit2;
        int highScore = 0;
        {
            int decimalDigit = 1;
            // start from end!!!
            for (int i=fromIdx; i > toIdx; i--) {
                highScore += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        
        // achievement
        fromIdx = fromIdx + 1;
        toIdx = fromIdx + 10;
        int achievement = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            achievement = atoi(dataStr.c_str());
            
            //
            achievementInfo = achievement;
        }
        
        // total score
        fromIdx = toIdx;
        toIdx = fromIdx + 10;
        int totalScore = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            totalScore = atoi(dataStr.c_str());
        }
        
        // death count
        int deathCount = 0;
        fromIdx = toIdx;
        for (int i=0; i<7; i++) { // 7 death counts
            stringstream stream;
            toIdx = fromIdx+5; // 5 digits each
            for (int j=fromIdx; j<toIdx; j++) {
                stream << data[j];
            }
            string dataStr = stream.str();
            deathCount = atoi(dataStr.c_str());
            
            fromIdx = toIdx;
            
            addToDeathCount(DataTypeDeathCountRed + i, deathCount);
        }
        
        // best score count
        fromIdx = toIdx;
        toIdx = fromIdx + 3;
        int bestScoreCount = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            bestScoreCount = atoi(dataStr.c_str());
        }
        
        // run count
        fromIdx = toIdx;
        toIdx = fromIdx + 4;
        int runCount = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            runCount = atoi(dataStr.c_str());
        }
        
        // Poster Index
        fromIdx = toIdx;
        toIdx = fromIdx + 2;
        int posterType = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            posterType = atoi(dataStr.c_str());
        }

        //
        // coin digits
        //
        fromIdx = toIdx;
        toIdx = fromIdx + 2;
        int coinDigits = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            coinDigits = atoi(dataStr.c_str());
        }

        // coins
        fromIdx = toIdx + 10 - 1;
        toIdx = fromIdx - coinDigits;
        int coins = 0;
        {
            int decimalDigit = 1;
            // start from end!!!
            for (int i=fromIdx; i > toIdx; i--) {
                coins += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        
        // skin info1
        fromIdx = fromIdx + 1;
        toIdx = fromIdx + 10;
        int skinInfo1 = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            skinInfo1 = atoi(dataStr.c_str());
        }
        
        // skin info2
        fromIdx = toIdx;
        toIdx = fromIdx + 10;
        int skinInfo2 = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            skinInfo2 = atoi(dataStr.c_str());
        }
        
        // skin info3
        fromIdx = toIdx;
        toIdx = fromIdx + 10;
        int skinInfo3 = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            skinInfo3 = atoi(dataStr.c_str());
        }
        
        // hidden skin Unlock info3
        fromIdx = toIdx;
        toIdx = fromIdx + 10;
        int hiddenUnlock = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            hiddenUnlock = atoi(dataStr.c_str());
        }

        // like
        int liked = (int)(data[toIdx] - 48);
        toIdx++;
        
        // review
        int review = (int)(data[toIdx] - 48);
        toIdx++;
        
        // review
        int noti = (int)(data[toIdx] - 48);
        toIdx++;
        
        // capsuleCount
        fromIdx = toIdx;
        toIdx = fromIdx + 2;
        int capsuleCount = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            capsuleCount = atoi(dataStr.c_str());
        }
        
        //
        // gift time digits
        //
        fromIdx = toIdx;
        toIdx = fromIdx + 2;
        int giftTimeDigits = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            giftTimeDigits = atoi(dataStr.c_str());
        }
        
        // gift time
        fromIdx = toIdx + 10 - 1;
        toIdx = fromIdx - giftTimeDigits;
        int giftTime = 0;
        if (giftTimeDigits != 99)
        {
            int decimalDigit = 1;
            // start from end!!!
            for (int i=fromIdx; i > toIdx; i--) {
                giftTime += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        else {
            giftTime = -1;
        }

        //
        setSoundOption(soundOption);
        SoundManager::getInstance()->setMuteFromFile(soundOption);
        setAdRemoveOption(adStatus);
        setTotalPlayCount(playCount);
        setHighScore(highScore);
        addToTotalScore(totalScore);
        increaseBestScoreCount(bestScoreCount);
        increaseRunCount(runCount);
        setSelectedPosterType(posterType);
        initCoins(coins);
        dataBackup[DataTypeSkins1] = skinInfo1;
//        dataBackup[DataTypeSkins2] = skinInfo2;
//        dataBackup[DataTypeSkins3] = skinInfo3;
        convertOldSkinInfoTo203(skinInfo2);
        dataBackup[DataTypeHiddenBackup] = hiddenUnlock;
        setLiked(liked);
        setRated(review);
        setNoti(noti);
        initWitchCapsuleCount(capsuleCount);
        secondSinceLastBoot = giftTime;
    }
    
    memset(data, '0', size+1);
    free(data);
}

void UserInfo::readUserDataVersion201()
{
    string dataStr = UserDefault::getInstance()->getStringForKey("DATA");
    int size = (int)dataStr.size();
    bool valid = false;
    
    //
    // decoding
    //
    unsigned char* data = (unsigned char*)malloc(size + 1); // +1 is for NULL
    memcpy(data, dataStr.c_str(), size + 1); // +1 is for NULL
    
    int baseFirst = 0b0101;
    for (int i=0; i<size; i++) {
        int salt = 1 << (i%5);
        int base = baseFirst - (i%baseFirst);
        if (base <= 0) {
            base = baseFirst;
        }
        data[i] =  (base+salt) ^ data[i];
    }
    
    {
        stringstream stream;
        stream << data;
        string dataStr = stream.str();
        
        unsigned char* hash = getHashValue((const unsigned char*)dataStr.c_str(), size);
        string keyHash = UserDefault::getInstance()->getStringForKey("HASH");
        
//        log("%s", keyHash.c_str());

        if (!strcmp((const char*)hash, keyHash.c_str())) {
            valid = true;
        }
        
        memset(hash, '0', HashSize);
        free(hash);
    }
    
    //
    // set data only if it's valid!
    //
    if (valid) {
        
//        for (int i=0; i<size; i++) {
//            std::cout << data[i];
////            log("%c, %d", data[i], (int)data[i]);
//        }
//        std::cout << std::endl;
        
        //
        // character 0 is 48 when ascii code(decimal)
        // we use only numbers, so just subtracting will be ok
        //
        int digit1 = (int)(data[0] - 48) * 10 + (int)(data[1] - 48);
        int digit2 = (int)(data[2] - 48) * 10 + (int)(data[3] - 48);
        
        // sound Option
        int soundOption = (int)(data[4] - 48);
        
        // ad status
        int adStatus = (int)(data[5] - 48);
        
        // playCount
        int fromIdx = 5 + digit1;
        int toIdx = fromIdx - digit1 + 1; // +1 is for fake
        int playCount = 0;
        {
            int decimalDigit = 1;
            for (int i=fromIdx; i > toIdx; i--) {
                playCount += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        
        // highScore
        fromIdx = fromIdx + 6;
        toIdx = fromIdx - digit2;
        int highScore = 0;
        {
            int decimalDigit = 1;
            // start from end!!!
            for (int i=fromIdx; i > toIdx; i--) {
                highScore += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        
        // achievement
        fromIdx = fromIdx + 1;
        toIdx = fromIdx + 10;
        int achievement = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            achievement = atoi(dataStr.c_str());
            
            //
            achievementInfo = achievement;
        }
        
        // total score
        fromIdx = toIdx;
        toIdx = fromIdx + 10;
        int totalScore = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            totalScore = atoi(dataStr.c_str());
        }
        
        // death count
        int deathCount = 0;
        fromIdx = toIdx;
        for (int i=0; i<7; i++) { // 7 death counts
            stringstream stream;
            toIdx = fromIdx+5; // 5 digits each
            for (int j=fromIdx; j<toIdx; j++) {
                stream << data[j];
            }
            string dataStr = stream.str();
            deathCount = atoi(dataStr.c_str());
            
            fromIdx = toIdx;
            
            addToDeathCount(DataTypeDeathCountRed + i, deathCount);
        }
        
        // best score count
        fromIdx = toIdx;
        toIdx = fromIdx + 3;
        int bestScoreCount = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            bestScoreCount = atoi(dataStr.c_str());
        }
        
        // run count
        fromIdx = toIdx;
        toIdx = fromIdx + 4;
        int runCount = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            runCount = atoi(dataStr.c_str());
        }
        
        // Poster Index
        fromIdx = toIdx;
        toIdx = fromIdx + 2;
        int posterType = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            posterType = atoi(dataStr.c_str());
        }
        
        //
        // coin digits
        //
        fromIdx = toIdx;
        toIdx = fromIdx + 2;
        int coinDigits = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            coinDigits = atoi(dataStr.c_str());
        }
        
        // coins
        fromIdx = toIdx + 10 - 1;
        toIdx = fromIdx - coinDigits;
        int coins = 0;
        {
            int decimalDigit = 1;
            // start from end!!!
            for (int i=fromIdx; i > toIdx; i--) {
                coins += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        
        // skin info1
        fromIdx = fromIdx + 1;
        toIdx = fromIdx + 10;
        int skinInfo1 = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            skinInfo1 = atoi(dataStr.c_str());
        }
        
        // skin info2
        fromIdx = toIdx;
        toIdx = fromIdx + 10;
        int skinInfo2 = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            skinInfo2 = atoi(dataStr.c_str());
        }
        
        // skin info3
        fromIdx = toIdx;
        toIdx = fromIdx + 10;
        int skinInfo3 = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            skinInfo3 = atoi(dataStr.c_str());
        }
        
        // hidden skin Unlock info3
        fromIdx = toIdx;
        toIdx = fromIdx + 10;
        int hiddenUnlock = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            hiddenUnlock = atoi(dataStr.c_str());
        }
        
        // like
        int liked = (int)(data[toIdx] - 48);
        toIdx++;
        
        // review
        int review = (int)(data[toIdx] - 48);
        toIdx++;
        
        // review
        int noti = (int)(data[toIdx] - 48);
        toIdx++;
        
        // capsuleCount
        fromIdx = toIdx;
        toIdx = fromIdx + 2;
        int capsuleCount = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            capsuleCount = atoi(dataStr.c_str());
        }
        
        //
        // gift time digits (Hardware ABS Time)
        //
        fromIdx = toIdx;
        toIdx = fromIdx + 2;
        int giftTimeDigits = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            giftTimeDigits = atoi(dataStr.c_str());
        }
        
        // gift time
        fromIdx = toIdx + 10 - 1;
        toIdx = fromIdx - giftTimeDigits;
        int giftTime = 0;
        if (giftTimeDigits != 99)
        {
            int decimalDigit = 1;
            // start from end!!!
            for (int i=fromIdx; i > toIdx; i--) {
                giftTime += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        else {
            giftTime = -1;
        }
        
        //
        // gift time digits (Software Time)
        //
        fromIdx = fromIdx + 1;
        toIdx = fromIdx + 2;
        int sGiftTimeDigits = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            sGiftTimeDigits = atoi(dataStr.c_str());
        }
        
        // gift time
        fromIdx = toIdx + 10 - 1;
        toIdx = fromIdx - sGiftTimeDigits;
        int sGiftTime = 0;
        if (sGiftTimeDigits != 99)
        {
            int decimalDigit = 1;
            // start from end!!!
            for (int i=fromIdx; i > toIdx; i--) {
                sGiftTime += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        else {
            sGiftTime = -1;
        }

        //
        // selected skin number digits
        //
        fromIdx = fromIdx + 1;
        toIdx = fromIdx + 1;
        int skinNumDigits = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            skinNumDigits = atoi(dataStr.c_str());
        }

        // selected skin number
        fromIdx = toIdx + 3 - 1;
        toIdx = fromIdx - skinNumDigits;
        int _skinNum = 0;
        {
            int decimalDigit = 1;
            // start from end!!!
            for (int i=fromIdx; i > toIdx; i--) {
                _skinNum += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }

        //
        setSoundOption(soundOption);
        SoundManager::getInstance()->setMuteFromFile(soundOption);
        setAdRemoveOption(adStatus);
        setTotalPlayCount(playCount);
        setHighScore(highScore);
        addToTotalScore(totalScore);
        increaseBestScoreCount(bestScoreCount);
        increaseRunCount(runCount);
        setSelectedPosterType(posterType);
        initCoins(coins);
        dataBackup[DataTypeSkins1] = skinInfo1;
//        dataBackup[DataTypeSkins2] = skinInfo2;
//        dataBackup[DataTypeSkins3] = skinInfo3;
        convertOldSkinInfoTo203(skinInfo2);
        dataBackup[DataTypeHiddenBackup] = hiddenUnlock;
        setLiked(liked);
        setRated(review);
        setNoti(noti);
        initWitchCapsuleCount(capsuleCount);
        secondSinceLastBoot = giftTime;
        timeCheckIn = sGiftTime;

        //
        if (_skinNum != 0) {
            int skinIdx = _skinNum - 1;
            if (isLockedSkin(skinIdx)) {
                //
                // NOTE: if it is locked skin, there's something wrong...maybe hacked?
                //
                _skinNum = 0;
                randomSkin = true;
            }
        }
        else if (_skinNum == 0) {
            randomSkin = true;
        }
        skinNum = _skinNum;
    }
    
    memset(data, '0', size+1);
    free(data);
}

void UserInfo::readUserDataVersion203()
{
    string dataStr = UserDefault::getInstance()->getStringForKey("DATA");
    int size = (int)dataStr.size();
    bool valid = false;
    
    //
    // decoding
    //
    unsigned char* data = (unsigned char*)malloc(size + 1); // +1 is for NULL
    memcpy(data, dataStr.c_str(), size + 1); // +1 is for NULL
    
    int baseFirst = 0b0110;
    for (int i=0; i<size; i++) {
        int salt = 1 << (i%5);
        int base = baseFirst - (i%baseFirst);
        if (base <= 0) {
            base = baseFirst;
        }
        data[i] =  (base+salt) ^ data[i];
    }
    
    {
        stringstream stream;
        stream << data;
        string dataStr = stream.str();
        
        unsigned char* hash = getHashValue((const unsigned char*)dataStr.c_str(), size);
        string keyHash = UserDefault::getInstance()->getStringForKey("HASH");
        
        //        log("%s", keyHash.c_str());
        
        if (!strcmp((const char*)hash, keyHash.c_str())) {
            valid = true;
        }
        
        memset(hash, '0', HashSize);
        free(hash);
    }
    
    //
    // set data only if it's valid!
    //
    if (valid) {
        
        //        for (int i=0; i<size; i++) {
        //            std::cout << data[i];
        ////            log("%c, %d", data[i], (int)data[i]);
        //        }
        //        std::cout << std::endl;
        
        //
        // character 0 is 48 when ascii code(decimal)
        // we use only numbers, so just subtracting will be ok
        //
        int digit1 = (int)(data[0] - 48) * 10 + (int)(data[1] - 48);
        int digit2 = (int)(data[2] - 48) * 10 + (int)(data[3] - 48);
        
        // sound Option
        int soundOption = (int)(data[4] - 48);
        
        // ad status
        int adStatus = (int)(data[5] - 48);
        
        // playCount
        int fromIdx = 5 + digit1;
        int toIdx = fromIdx - digit1 + 1; // +1 is for fake
        int playCount = 0;
        {
            int decimalDigit = 1;
            for (int i=fromIdx; i > toIdx; i--) {
                playCount += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        
        // highScore
        fromIdx = fromIdx + 6;
        toIdx = fromIdx - digit2;
        int highScore = 0;
        {
            int decimalDigit = 1;
            // start from end!!!
            for (int i=fromIdx; i > toIdx; i--) {
                highScore += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        
        // achievement
        fromIdx = fromIdx + 1;
        toIdx = fromIdx + 10;
        int achievement = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            achievement = atoi(dataStr.c_str());
            
            //
            achievementInfo = achievement;
        }
        
        // total score
        fromIdx = toIdx;
        toIdx = fromIdx + 10;
        int totalScore = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            totalScore = atoi(dataStr.c_str());
        }
        
        // death count
        int deathCount = 0;
        fromIdx = toIdx;
        for (int i=0; i<7; i++) { // 7 death counts
            stringstream stream;
            toIdx = fromIdx+5; // 5 digits each
            for (int j=fromIdx; j<toIdx; j++) {
                stream << data[j];
            }
            string dataStr = stream.str();
            deathCount = atoi(dataStr.c_str());
            
            fromIdx = toIdx;
            
            addToDeathCount(DataTypeDeathCountRed + i, deathCount);
        }
        
        // best score count
        fromIdx = toIdx;
        toIdx = fromIdx + 3;
        int bestScoreCount = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            bestScoreCount = atoi(dataStr.c_str());
        }
        
        // run count
        fromIdx = toIdx;
        toIdx = fromIdx + 4;
        int runCount = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            runCount = atoi(dataStr.c_str());
        }
        
        // Poster Index
        fromIdx = toIdx;
        toIdx = fromIdx + 2;
        int posterType = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            posterType = atoi(dataStr.c_str());
        }
        
        //
        // coin digits
        //
        fromIdx = toIdx;
        toIdx = fromIdx + 2;
        int coinDigits = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            coinDigits = atoi(dataStr.c_str());
        }
        
        // coins
        fromIdx = toIdx + 10 - 1;
        toIdx = fromIdx - coinDigits;
        int coins = 0;
        {
            int decimalDigit = 1;
            // start from end!!!
            for (int i=fromIdx; i > toIdx; i--) {
                coins += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        
        // skin info1
        fromIdx = fromIdx + 1;
        toIdx = fromIdx + 10;
        int skinInfo1 = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            skinInfo1 = atoi(dataStr.c_str());
        }
        
        // skin info2
        fromIdx = toIdx;
        toIdx = fromIdx + 10;
        int skinInfo2 = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            skinInfo2 = atoi(dataStr.c_str());
        }
        
        // skin info3
        fromIdx = toIdx;
        toIdx = fromIdx + 10;
        int skinInfo3 = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            skinInfo3 = atoi(dataStr.c_str());
        }
        
        // hidden skin Unlock info3
        fromIdx = toIdx;
        toIdx = fromIdx + 10;
        int hiddenUnlock = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            hiddenUnlock = atoi(dataStr.c_str());
        }
        
        // like
        int liked = (int)(data[toIdx] - 48);
        toIdx++;
        
        // review
        int review = (int)(data[toIdx] - 48);
        toIdx++;
        
        // review
        int noti = (int)(data[toIdx] - 48);
        toIdx++;
        
        // capsuleCount
        fromIdx = toIdx;
        toIdx = fromIdx + 2;
        int capsuleCount = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            capsuleCount = atoi(dataStr.c_str());
        }
        
        //
        // gift time digits (Hardware ABS Time)
        //
        fromIdx = toIdx;
        toIdx = fromIdx + 2;
        int giftTimeDigits = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            giftTimeDigits = atoi(dataStr.c_str());
        }
        
        // gift time
        fromIdx = toIdx + 10 - 1;
        toIdx = fromIdx - giftTimeDigits;
        int giftTime = 0;
        if (giftTimeDigits != 99)
        {
            int decimalDigit = 1;
            // start from end!!!
            for (int i=fromIdx; i > toIdx; i--) {
                giftTime += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        else {
            giftTime = -1;
        }
        
        //
        // gift time digits (Software Time)
        //
        fromIdx = fromIdx + 1;
        toIdx = fromIdx + 2;
        int sGiftTimeDigits = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            sGiftTimeDigits = atoi(dataStr.c_str());
        }
        
        // gift time
        fromIdx = toIdx + 10 - 1;
        toIdx = fromIdx - sGiftTimeDigits;
        int sGiftTime = 0;
        if (sGiftTimeDigits != 99)
        {
            int decimalDigit = 1;
            // start from end!!!
            for (int i=fromIdx; i > toIdx; i--) {
                sGiftTime += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        else {
            sGiftTime = -1;
        }
        
        //
        // selected skin number digits
        //
        fromIdx = fromIdx + 1;
        toIdx = fromIdx + 1;
        int skinNumDigits = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            skinNumDigits = atoi(dataStr.c_str());
        }
        
        // selected skin number
        fromIdx = toIdx + 3 - 1;
        toIdx = fromIdx - skinNumDigits;
        int _skinNum = 0;
        {
            int decimalDigit = 1;
            // start from end!!!
            for (int i=fromIdx; i > toIdx; i--) {
                _skinNum += (int)(data[i] - 48) * decimalDigit;
                decimalDigit *= 10;
            }
        }
        
        // firstTimeResultTypeIndex
        fromIdx = fromIdx + 1;
        toIdx = fromIdx + 1;
        int _firstTimeResultTypeIndex = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            _firstTimeResultTypeIndex = atoi(dataStr.c_str());
        }
        
        // resultGroupTypeIndex
        fromIdx = fromIdx + 1;
        toIdx = fromIdx + 1;
        int _resultGroupTypeIndex = 0;
        {
            stringstream stream;
            for (int i=fromIdx; i<toIdx; i++) {
                stream << data[i];
            }
            string dataStr = stream.str();
            _resultGroupTypeIndex = atoi(dataStr.c_str());
        }

        //
        setSoundOption(soundOption);
        SoundManager::getInstance()->setMuteFromFile(soundOption);
        setAdRemoveOption(adStatus);
        setTotalPlayCount(playCount);
        setHighScore(highScore);
        addToTotalScore(totalScore);
        increaseBestScoreCount(bestScoreCount);
        increaseRunCount(runCount);
        setSelectedPosterType(posterType);
        initCoins(coins);
        dataBackup[DataTypeSkins1] = skinInfo1;
        dataBackup[DataTypeSkins2] = skinInfo2;
        dataBackup[DataTypeSkins3] = skinInfo3;
        dataBackup[DataTypeHiddenBackup] = hiddenUnlock;
        setLiked(liked);
        setRated(review);
        setNoti(noti);
        initWitchCapsuleCount(capsuleCount);
        secondSinceLastBoot = giftTime;
        timeCheckIn = sGiftTime;
        firstTimeResultTypeIndex = _firstTimeResultTypeIndex;
        resultGroupTypeIndex = _resultGroupTypeIndex;
        
        //
        if (_skinNum != 0) {
            int skinIdx = _skinNum - 1;
            if (isLockedSkin(skinIdx)) {
                //
                // NOTE: if it is locked skin, there's something wrong...maybe hacked?
                //
                _skinNum = 0;
                randomSkin = true;
            }
        }
        else if (_skinNum == 0) {
            randomSkin = true;
        }
        skinNum = _skinNum;
    }
    
    memset(data, '0', size+1);
    free(data);
}

#pragma mark -
#pragma mark HELPER
unsigned char* UserInfo::getHashValue(const unsigned char* data, int size)
{
    unsigned char digest[SHA256_DIGEST_SIZE];
    sha256(data, size, digest);

    unsigned char *output = (unsigned char*)malloc(HashSize);
    int i;

    output[2 * SHA256_DIGEST_SIZE] = '\0';

    for (i = 0; i < (int) SHA256_DIGEST_SIZE ; i++) {
        sprintf((char*)output + 2 * i, "%02x", digest[i]);
    }

    return output;
}

bool UserInfo::setAchievementInfoWithType(AchievementType type) {
    if ((achievementInfo & type) == type) {
        return false;
    }

    //
    // Set Achievement flag
    //
    achievementInfo = achievementInfo | type;

    //
    // Find Achievement index
    //
    int achievementIdx = 0;
    while (type > 1) {
        type = (AchievementType)(type >> 1);
        achievementIdx++;
    }
    
    getNewAchievement[achievementIdx] = true;
    
    //
    // If this is first achievement, set it as poster index
    //
    if (getSelectedPosterType() == 0) {
        setSelectedPosterType(achievementIdx + 1);
    }
//
//    //
//    // If authenticated, Report to Apple Server
//    //
//#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
//    if (GameCenterX::getInstance()->isAuthenticated(false)) {
//        GameCenterX::getInstance()->reportAchievementIdentifier(achievementIdx+1, 100);
//    }
//#else
//    if (NativeUtils::isSignedIn()) {
//        NativeUtils::unlockAchievement(achievementIdx);
//    }
//#endif

    //
    //
    //
    bannerQueue->pushBanner(achievementIdx);

    //
    // Only PlayScene has showAchievementsBanners
    //
    Scene *scene = Director::getInstance()->getRunningScene();
    if (scene) {
        PlayScene *cScene = dynamic_cast<PlayScene*>(scene->getChildByTag(1));
        if(cScene != NULL) {
            cScene->showAchievementsBanners(0);
        }
    }
    
    //
    // Achievement for Number of Achievements
    //
    if (getDoneAchievementNumbers() == getAchievementPassBase(AchievementType25)) {setAchievementInfoWithType(AchievementType25);}
    if (getDoneAchievementNumbers() == getAchievementPassBase(AchievementType26)) {setAchievementInfoWithType(AchievementType26);}
    if (getDoneAchievementNumbers() == getAchievementPassBase(AchievementType27)) {setAchievementInfoWithType(AchievementType27);}
    
    return true;
}

string UserInfo::cutByDigits(int originalNumber, int validDigits)
{
    stringstream stream;
    stream << originalNumber;
    string dataStr = stream.str();
    
    size_t leng = dataStr.length();
    if (leng <= validDigits) {
        return dataStr;
    }
    
    return dataStr.substr(leng - validDigits);
}
