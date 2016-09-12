//
//  UserInfo.h
//  Timefish
//
//  Created by midchow on 2015. 1. 23..
//
//

#ifndef __Timefish__UserInfo__
#define __Timefish__UserInfo__

#include "cocos2d.h"

#include "Global.h"
#include "SoundManager.h"
#include "GameCenterX.h"
#include "AdColonyX.h"
#include "UnityAdsX.h"
#include "SocialX.h"
#include "TapjoyX.h"

USING_NS_CC;

typedef enum
{
    // 0
    DataTypeSoundOption = 0,
    DataTypeTempAdFreeOn,
    DataTypeAdRemoveOn,
    DataTypeBestScore,
    DataTypeCurrPlayCount,
    DataTypeTotalPlayCount,
    //
    DataTypeAchievement,
    //
    DataTypeTotalScore,
    //
    DataTypeDeathCountRed,
    DataTypeDeathCountBlue,
    DataTypeDeathCountYellow,
    DataTypeDeathCountFirst,
    DataTypeDeathCountWithoutBall,
    DataTypeDeathCountJump,
    DataTypeDeathCountTotal,
    //
    DataTypeBestScoreCount,
    DataTypeRunCount,
    DataTypeTotalCapsuleCount,
    //
    DataTypeSelectedPoster,
    DataTypeNewPoster,
    //
    // New Data
    //

    // 20
    DataTypeCoins,
    DataTypeSkins1, //  1 ~ 25
    DataTypeSkins2, // 26 ~ 50
    DataTypeSkins3, // 51 ~ 75
    
    //
    DataTypeTrialSkin,
    DataTypeTrialCount,
    DataTypeRated,
    DataTypeLiked,
    DataTypeNoti,

    //
    DataTypeHiddenBackup,
    //
    DataTypeMax
} DataType;

class BannerQueue
{
public:
    void pushBanner(int idx)
    {
        bannerQueue.push(idx);
    }

    int popBanner()
    {
        if (bannerQueue.empty()) {
            return -1;
        }

        int front = bannerQueue.front();
        bannerQueue.pop();

        return front;
    }
private:
    std::queue<int> bannerQueue;
};

class UserInfo
{
public:
    static UserInfo* getInstance();

    ~UserInfo();
    
    time_t playTimeTotal;

    int key1, key2;
    int currKey;
    int checkSum;
    bool hasShowAd;
    bool getNewAchievement[NumOfAchievement];
    bool haveNewAchievement() {
        for (int i=0; i<NumOfAchievement; i++) {
            if (getNewAchievement[i]) {
                return true;
            }
        }

        return false;
    }

    bool init();

    //
    void saveUserData();
    void saveUserDataVersion106();
    void saveUserDataVersion107();
    void saveUserDataVersion110();
    void saveUserDataVersion201();
    void saveUserDataVersion203();
    //
    void readUserData();
    int getUserDataFormat();
    void readUserDataVersion106();
    void readUserDataVersion107();
    void readUserDataVersion110();
    void readUserDataVersion201();
    void readUserDataVersion203();

    //
    void setSoundOption(int value) {
        dataBackup[DataTypeSoundOption] = value;
    }
//    void setTempAdFreeOption(int value) {
//        dataBackup[DataTypeTempAdFreeOn] = value;
//    }
    void setAdRemoveOption(int value) {
        dataBackup[DataTypeAdRemoveOn] = value;
    }
    void setAdRemoveOptionWithSave(int value) {
        setAdRemoveOption(value);
        saveUserData();
    }
    void setPlayCount(int value) {
        dataBackup[DataTypeCurrPlayCount] = value;
    }
    void setTotalPlayCount(int value) {
        dataBackup[DataTypeTotalPlayCount] = value;
    }
    void incPlayCount() {
        dataBackup[DataTypeCurrPlayCount]++;
    }
    void incTotalPlayCount() {
        dataBackup[DataTypeTotalPlayCount]++;
    }
    void setTrialCount(int value) {
        dataBackup[DataTypeTrialCount] = value;
    }
    int getTrialCount() {
        return dataBackup[DataTypeTrialCount];
    }
    void setRated(int value) {
        dataBackup[DataTypeRated] = value;
    }
    int getRated() { return dataBackup[DataTypeRated]; }
    void setNoti(int value) {
        dataBackup[DataTypeNoti] = value;
    }
    int getNoti() { return dataBackup[DataTypeNoti]; }
    void setLiked(int value) {
        dataBackup[DataTypeLiked] = value;

        //
        setHiddenSkinUnlockCond(0);
    }
    int getLiked() { return dataBackup[DataTypeLiked]; }
    bool setHighScore(int value, bool increaseCount = false) {
        int tmpScore = value + baseScore;
        if (dataBackup[DataTypeBestScore] <= tmpScore) {
            dataBackup[DataTypeBestScore] = tmpScore;

            if (value > 0 && increaseCount) {
                //
                // Achieved Best Score!!!
                //
                increaseBestScoreCount(1);
                
                //
                // Check Achievements...
                //
                if (dataBackup[DataTypeBestScoreCount] >= getAchievementPassBase(AchievementType23) + baseScore) {setAchievementInfoWithType(AchievementType23);}
            }

            return true;
        }

        return false;
    }
    void addToTotalScore(int value) {
        dataBackup[DataTypeTotalScore] += value;
    }
    int getTotalScore() {
        return dataBackup[DataTypeTotalScore] - baseScore;
    }

    //
    void increaseBestScoreCount(int value = 0) {
        dataBackup[DataTypeBestScoreCount] += value;
    }
    int getBestScoreCount() {
        return dataBackup[DataTypeBestScoreCount] - baseScore;
    }
    
    //
    void increaseRunCount(int value = 1) {
        dataBackup[DataTypeRunCount] += value;

        //
        // Check Achievements...
        //
        if (dataBackup[DataTypeRunCount] >= getAchievementPassBase(AchievementType24) + baseScore) {setAchievementInfoWithType(AchievementType24);}
    }
    int getRunCount() {
        return dataBackup[DataTypeRunCount] - baseScore;
    }

    void initCoins(int amount) {
        dataBackup[DataTypeCoins] += amount;
    }
    void addToCoins(int amount) {
        dataBackup[DataTypeCoins] += amount;
        checkAchievement28();
    }
    int getCoins() {
        return dataBackup[DataTypeCoins] - baseScore;
    }
    
    void checkAchievement28() {
        //
        // Check Achievement...
        //
        if (dataBackup[DataTypeCoins] >= getAchievementPassBase(AchievementType28) + baseScore) {setAchievementInfoWithType(AchievementType28);}
    }

    //
    void setSelectedPosterType(int idx) {
        idx = MAX(0, idx);
        dataBackup[DataTypeSelectedPoster] = baseScore + idx;
    }
    int getSelectedPosterType() {
        return dataBackup[DataTypeSelectedPoster] - baseScore;
    }
    
    //
    void addToDeathCount(int idx, int value = 1) {
        dataBackup[idx] += value;

        //
        // Check Death Count Achievements...
        //
        if (dataBackup[DataTypeDeathCountRed]           >= getAchievementPassBase(AchievementType16) + baseScore) {setAchievementInfoWithType(AchievementType16);}
        if (dataBackup[DataTypeDeathCountYellow]        >= getAchievementPassBase(AchievementType17) + baseScore) {setAchievementInfoWithType(AchievementType17);}
        if (dataBackup[DataTypeDeathCountBlue]          >= getAchievementPassBase(AchievementType18) + baseScore) {setAchievementInfoWithType(AchievementType18);}
        if (dataBackup[DataTypeDeathCountFirst]         >= getAchievementPassBase(AchievementType19) + baseScore) {setAchievementInfoWithType(AchievementType19);}
        if (dataBackup[DataTypeDeathCountWithoutBall]   >= getAchievementPassBase(AchievementType20) + baseScore) {setAchievementInfoWithType(AchievementType20);}
        if (dataBackup[DataTypeDeathCountJump]          >= getAchievementPassBase(AchievementType21) + baseScore) {setAchievementInfoWithType(AchievementType21);}
        if (dataBackup[DataTypeDeathCountTotal]         >= getAchievementPassBase(AchievementType22) + baseScore) {setAchievementInfoWithType(AchievementType22);}
    }
    int getDeathCount(int idx) {
        return dataBackup[idx] - baseScore;
    }
    
    void initCapsuleCount() {
        capsuleCount = 0;
    }
    void addToCapsuleCount() {
        capsuleCount++;
        //
        // Check Achievement...
        //
        if (capsuleCount >= getAchievementPassBase(AchievementType31)) {setAchievementInfoWithType(AchievementType31);}
    }

    void initWitchCapsuleCount(int value) {
        dataBackup[DataTypeTotalCapsuleCount] += value;
    }

    void addToWitchCapsuleCount() {
        //
        if (44 == skinNum && !getHiddenSkinUnlockCond(1)) {
            dataBackup[DataTypeTotalCapsuleCount] += 1;

            //
            setHiddenSkinUnlockCond(1);
        }
    }
    int getWitchCapsuleCount() {
        return dataBackup[DataTypeTotalCapsuleCount] - baseScore;
    }

    //
    int getSoundOption() {
        return dataBackup[DataTypeSoundOption];
    }
    int getNoAdOption() {
#ifdef DEVELOPMENT_NOAD
        return 1;
#else
        return dataBackup[DataTypeTempAdFreeOn] || dataBackup[DataTypeAdRemoveOn];
#endif
    }
//    int getAdFreeOption() {
//        return dataBackup[DataTypeTempAdFreeOn];
//    }
    int getAdRemoveOption() {
        return dataBackup[DataTypeAdRemoveOn];
    }
    int getPlayCount() {
        return dataBackup[DataTypeCurrPlayCount];
    }
    int getTotalPlayCount() {
        return dataBackup[DataTypeTotalPlayCount];
    }
    
    int getAchievementPassBase(AchievementType type) {
        switch (type) {
            case AchievementType13:
                return 1; // * 10^6
                break;
            case AchievementType14:
                return 5; // * 10^6
                break;
            case AchievementType15:
                return 10; // * 10^6
                break;
            case AchievementType16:
                return 7;
                break;
            case AchievementType17:
                return 14;
                break;
            case AchievementType18:
                return 21;
                break;
            case AchievementType19:
                return 10;
                break;
            case AchievementType20:
                return 100;
                break;
            case AchievementType21:
                return 100;
                break;
            case AchievementType22:
                return 1000;
                break;
            case AchievementType23:
                return 5;
                break;
            case AchievementType24:
                return 5;
                break;
            case AchievementType25:
                return 5;
                break;
            case AchievementType26:
                return 15;
                break;
            case AchievementType27:
                return 26;
                break;
            case AchievementType28:
                return 999;
                break;
            case AchievementType29:
                return 10;
                break;
            case AchievementType30:
                return 30;
                break;
            case AchievementType31:
                return 5;
                break;
                
            default:
                break;
        }

        return 10000;
    }

    int getAchievementCurr(AchievementType type) {
        switch (type) {
            case AchievementType13:
                return (dataBackup[DataTypeTotalScore] - baseScore); // * 10^6
                break;
            case AchievementType14:
                return (dataBackup[DataTypeTotalScore] - baseScore); // * 10^6
                break;
            case AchievementType15:
                return (dataBackup[DataTypeTotalScore] - baseScore); // * 10^6
                break;
            case AchievementType16:
                return dataBackup[DataTypeDeathCountRed] - baseScore;
                break;
            case AchievementType17:
                return dataBackup[DataTypeDeathCountYellow] - baseScore;
                break;
            case AchievementType18:
                return dataBackup[DataTypeDeathCountBlue] - baseScore;
                break;
            case AchievementType19:
                return dataBackup[DataTypeDeathCountFirst] - baseScore;
                break;
            case AchievementType20:
                return dataBackup[DataTypeDeathCountWithoutBall] - baseScore;
                break;
            case AchievementType21:
                return dataBackup[DataTypeDeathCountJump] - baseScore;
                break;
            case AchievementType22:
                return dataBackup[DataTypeDeathCountTotal] - baseScore;
                break;
            case AchievementType23:
                return dataBackup[DataTypeBestScoreCount] - baseScore;
                break;
            case AchievementType24:
                return dataBackup[DataTypeRunCount] - baseScore;
                break;
            case AchievementType25:
            case AchievementType26:
            case AchievementType27:
                return getDoneAchievementNumbers();
            case AchievementType28:
                return getCoins();
            case AchievementType29:
            case AchievementType30:
                return getNumberOfUnlockedSkin();
            default:
                break;
        }
        
        return 10000;
    }
    
    //
    void addCurrScore(int deltaScore, int digits) {
        int tmp = currScore - baseScore;
        tmp += deltaScore;
        currScore = baseScore + tmp;

        //
        // One time Score-related Achievements...
        //
        int scores[] = {2,5,10,20,30,50,100};
        for (int i=0; i<7; i++) {
            for (int j=0; j<digits; j++) {
                scores[i] *= 10;
            }
        }
        if (currScore >= scores[0] + baseScore) {setAchievementInfoWithType(AchievementType01);}
        if (currScore >= scores[1] + baseScore) {setAchievementInfoWithType(AchievementType02);}
        if (currScore >= scores[2] + baseScore) {setAchievementInfoWithType(AchievementType03);}
        if (currScore >= scores[3] + baseScore) {setAchievementInfoWithType(AchievementType04);}
        if (currScore >= scores[4] + baseScore) {setAchievementInfoWithType(AchievementType05);}
        if (currScore >= scores[5] + baseScore) {setAchievementInfoWithType(AchievementType06);}
        if (currScore >= scores[6] + baseScore) {setAchievementInfoWithType(AchievementType12);}
    }
    int getCurrScore() {
        return currScore - baseScore;
    }
    int getBestScore() {
        return dataBackup[DataTypeBestScore] - baseScore;
    }

    void initCurrScore() {
        currScore = baseScore;
    }

    void setScoreAchievements() {
        
    }

    int getAchievementInfo() {return achievementInfo;}
    bool setAchievementInfoWithType(AchievementType type);
    bool getAchievementInfoWithType(AchievementType type) {
        if ((achievementInfo & type) == type) {
            return true;
        }

        return false;
    }
    int getDoneAchievementNumbers() {
        AchievementType type = AchievementType01;
        int cnt = 0;
        while (type < AchievementTypeMax) {
            if ((achievementInfo & type) == type) {
                cnt++;
            }
            type = (AchievementType)(type << 1);
        }
        
        return cnt;
    }

//    void logAchievementInfo() {
//        printf("%d\n", achievementInfo);
//        
//        return;
//        for (int i=0; i< 28; i++) {
//            AchievementType type = (AchievementType)(1 << i);
//            printf("%x\n", achievementInfo & type);
//        }
//        printf("--------------\n");
//    }
    
    //
    void processResultScore(int digits) {
        // save score
        highscored = setHighScore(getCurrScore(), true);
        addToTotalScore(getCurrScore());
        
        //
        // Accumulative Score-related Achievements...
        //
        int scores[] = {
            getAchievementPassBase(AchievementType13),
            getAchievementPassBase(AchievementType14),
            getAchievementPassBase(AchievementType15)
        }; // 1,000,000, 5,000,000, 10,000,000
        for (int i=0; i<3; i++) {
            for (int j=0; j<digits; j++) {
                scores[i] *= 10;
            }
        }
        if (dataBackup[DataTypeTotalScore] >= scores[0] + baseScore) {setAchievementInfoWithType(AchievementType13);}
        if (dataBackup[DataTypeTotalScore] >= scores[1] + baseScore) {setAchievementInfoWithType(AchievementType14);}
        if (dataBackup[DataTypeTotalScore] >= scores[2] + baseScore) {setAchievementInfoWithType(AchievementType15);}
    }

    BannerQueue *bannerQueue;

    LanguageType currLangType;

    int setGiftAmount(int& giftType) {
        int amount = baseScore;
        int delta = 14;
        
        bool firstTime = isThisFirstTimeLaunch();
        if (firstTime) {
            giftType = 1;
            amount += 86; // 100

            setFirstTimeLaunch(false);
        }
        else {
            int p = rand()%1000;
            if (p < 800) { // 80 %
                giftType = 0;
                amount += (21 + rand()%51); // 35 ~ 85 - delta
            }
            else if (p < 950) { // 15 %
                giftType = 1;
                amount += 86; // 100 - delta
            }
            else if (p < 995) { // 4.5 %
                giftType = 2;
                amount += (86 + rand()%101); // 100 ~ 200 - delta
            }
            else if (p < 1000) { // 0.5 %
                giftType = 3;
                amount += 986; // 1000 - delta
            }
        }

        addToCoins(amount - baseScore + delta);

        TapjoyX::getInstance()->logEventInEconomy(1, giftType);

        return amount - baseScore + delta;
    }

    void unlockSkinAt(int idx) {
        if (idx < 0) {
            return;
        }

        /*
         DataTypeSkins1, //  1 ~ 25
         DataTypeSkins2, // 26 ~ 50
         DataTypeSkins3, // 51 ~ 75
        */
        if(idx < 25) {
            int bit = 1;
            bit = bit << idx;
            dataBackup[DataTypeSkins1] |= bit;
        }
        else if(idx < 50) {
            int bit = 1;
            bit = bit << (idx-25);
            dataBackup[DataTypeSkins2] |= bit;
        }
        else if(idx < 75) {
            int bit = 1;
            bit = bit << (idx-50);
            dataBackup[DataTypeSkins3] |= bit;
        }

        //
        setHiddenSkinUnlockCond(6);

        //
        // set user level by skin numbers
        //
        int lvl = getNumberOfUnlockedSkin();
        TapjoyX::getInstance()->setUserLevel(lvl);
        
        //
        // Check Achievement...
        //
        if (lvl >= getAchievementPassBase(AchievementType29)) {setAchievementInfoWithType(AchievementType29);}
        if (lvl >= getAchievementPassBase(AchievementType30)) {setAchievementInfoWithType(AchievementType30);}
    }
    bool isLockedSkin(int idx) {
        if(idx < 25) {
            int bit = 1;
            bit = bit << idx;
            
            if ((dataBackup[DataTypeSkins1] & bit) == bit) {
                return false;
            }
            else {
                return true;
            }
        }
        else if(idx < 50) {
            int bit = 1;
            bit = bit << (idx-25);
            
            if ((dataBackup[DataTypeSkins2] & bit) == bit) {
                return false;
            }
            else {
                return true;
            }
        }
        else if(idx < 75) {
            int bit = 1;
            bit = bit << (idx-50);
            
            if ((dataBackup[DataTypeSkins3] & bit) == bit) {
                return false;
            }
            else {
                return true;
            }
        }
        return true;
    }
    void unlockSkinForFacebookReward() {
        int startIndexOfHiddenSkin = getHiddenSkinIndex();
        unlockSkinAt(startIndexOfHiddenSkin);
    }
    void unlockPackageSkins(int packageIdx) {
        PackageInfo pInfo = packages[packageIdx];
        for (int i=0; i<3; i++) {
            unlockSkinAt(pInfo.skinIdx[i]);
        }
    }
    int getNumberOfUnlockedSkin() {
        int cnt = 0;
        for (int i=0; i<25; i++) {
            int bit = 1;
            bit = bit << i;
            if ((dataBackup[DataTypeSkins1] & bit) == bit) {
                cnt++;
            }
            if ((dataBackup[DataTypeSkins2] & bit) == bit) {
                cnt++;
            }
            if ((dataBackup[DataTypeSkins3] & bit) == bit) {
                cnt++;
            }
        }

        return cnt;
    }
    inline int getHiddenSkinIndex() {
//        return FreeshSize - NumOfHiddenSkins;
        return FreeshSize - NumOfHiddenSkins - 1;
    }
    inline int getGoldenFishSkinIndex() {
//        return getHiddenSkinIndex() - 1;
        return FreeshSize - 1;
    }
    int getRandomSkinAmongUnlocked() {
        std::vector<int> tmpSkins;
        for (int i=0; i<FreeshSize; i++) {
            if (!isLockedSkin(i)) {
                tmpSkins.push_back(i);
            }
        }

        if (tmpSkins.size() == 0) {
            return -1;
        }

        //
        int r = rand()%tmpSkins.size();
        return tmpSkins.at(r);
    }
    int getRandomSkinAmongUnlockedWithoutPrevious() {
        std::vector<int> tmpSkins;
        for (int i=0; i<FreeshSize; i++) {
            if (!isLockedSkin(i) && i != skinNum-1) {
                tmpSkins.push_back(i);
            }
        }

        int skinIdx = 0;
        int listSize = (int)tmpSkins.size();
        if (listSize > 0) {
            //
            int r = rand()%listSize;
            skinIdx = tmpSkins.at(r);
        }
        
        int _skinNum = skinIdx + 1; // don't set skinNum here!!!
        return _skinNum;
    }
    int getRandomSkinAmongLocked() {
        //
        // NOTE: select no basic, golden fish, hidden skins which is freesh level is zero!!!
        //
        std::vector<int> uniqueSkins;
        std::vector<int> rareSkins;
        std::vector<int> normalSkins;
        int hiddenFreeshIdx = getHiddenSkinIndex();
        for (int i=0; i<hiddenFreeshIdx; i++) {
            if (isLockedSkin(i)) {
                int lvl = getFreeshLevel(i);
                if (lvl == 1) {
                    uniqueSkins.push_back(i);
                }
                else if (lvl == 2) {
                    rareSkins.push_back(i);
                }
                else if (lvl == 3) {
                    normalSkins.push_back(i);
                }
            }
        }
        
        int uniqueSize = (int)uniqueSkins.size();
        int rareSize = (int)rareSkins.size();
        int normalSize = (int)normalSkins.size();

        if ( uniqueSize == 0 && rareSize == 0 && normalSize == 0) {
            return -1;
        }

        //
        // NOTE: To pick up unique skin first, give it a high probability!
        //
        int skinIdx = -1;
        int r = rand()%100;
        if (uniqueSize > 0 && r < 70) { // 70%
            int idx = rand()%uniqueSize;
            skinIdx = uniqueSkins.at(idx);
        }
        else if (rareSize > 0 && r < 90) { // 20%
            int idx = rand()%rareSize;
            skinIdx = rareSkins.at(idx);
        }
        else if (normalSize > 0) { // 10%
            int idx = rand()%normalSize;
            skinIdx = normalSkins.at(idx);
        }

        return skinIdx;
    }
    int getRandomSkinAmongNotHiddenForDraw() {
//        //
//        // NOTE: Not to select the basic skin(idx = 0),
//        // minus one, and plus one later!
//        //
//        int hiddenFreeshIdx = getHiddenSkinIndex() - 1;
//        int r = rand()%hiddenFreeshIdx;
//        return r + 1;

        std::vector<int> uniqueSkins;
        std::vector<int> rareSkins;
        std::vector<int> normalSkins;
        int hiddenFreeshIdx = getHiddenSkinIndex();
        for (int i=0; i<hiddenFreeshIdx; i++) {
            int lvl = getFreeshLevel(i);
            if (lvl == 1) {
                uniqueSkins.push_back(i);
            }
            else if (lvl == 2) {
                rareSkins.push_back(i);
            }
            else if (lvl == 3) {
                normalSkins.push_back(i);
            }
        }

        int uniqueSize = (int)uniqueSkins.size();
        int rareSize = (int)rareSkins.size();
        int normalSize = (int)normalSkins.size();

        //
        // FOR THE FIRST-TIME USER EXPERIENCE:
        // set unique & rare probability based on skin numbers
        //
        int uniqueProbability = 25;
        int rareProbability = 60;
        int skinNums = getNumberOfUnlockedSkin();
        if (skinNums == 1) {
            uniqueProbability = 100;
        }
        else if (skinNums == 2) {
            uniqueProbability = 50;
            rareProbability = 100;
        }

        //
        // NOTE: To pick up normal skin first, give it a high probability!
        //
        int skinIdx = -1;
        int r = rand()%100;
        if (uniqueSize > 0 && r < uniqueProbability) { // 25%
            int idx = rand()%uniqueSize;
            skinIdx = uniqueSkins.at(idx);
        }
        else if (rareSize > 0 && r < rareProbability) { // 35%
            int idx = rand()%rareSize;
            skinIdx = rareSkins.at(idx);
        }
        else if (normalSize > 0) { // 40%
            int idx = rand()%normalSize;
            skinIdx = normalSkins.at(idx);
        }

        return skinIdx;
    }
    bool isThisHiddenSkin(int idx) {
        int hiddenFreeshIdx = getHiddenSkinIndex();
        int goldenfishIdx = getGoldenFishSkinIndex();
        if (idx >= hiddenFreeshIdx && idx != goldenfishIdx) {
            return true;
        }
        
        return false;
    }
    FreeshType getSkinTypeBySkinNumber(int skinNum) {
        int cnt = 0;
        for (int i=0; i<FreeshTypeMax; i++) {
            cnt += freeshTypes[i];
            if (cnt >= skinNum) {
                return (FreeshType)i;
            }
        }

        return FreeshTypeBasic;
    }

    void setTrialSkinIndex(int value) {
        dataBackup[DataTypeTrialSkin] = baseScore + value;
    }
    int getTrialSkinIndex() {
        return dataBackup[DataTypeTrialSkin] - baseScore;
    }

#ifdef ProductPurchaseCapture
    int packageIdx;
    void incPackageIdx() {
        packageIdx++;
    }
#endif
    int getRandomPackageIndex() {
#ifdef ProductPurchaseCapture
        if (packageIdx < 10) { // 0 ~ 9
            return packageIdx;
        }

        return -1;
#else
        std::vector<int> uniquePacks;
        std::vector<int> rarePacks;
        std::vector<int> normalPacks;

        for (int i=0; i<10; i++) {
            PackageInfo pInfo = packages[i];
            if (isLockedSkin(pInfo.skinIdx[0]) &&
                isLockedSkin(pInfo.skinIdx[1]) &&
                isLockedSkin(pInfo.skinIdx[2])) {
                
                if (pInfo.grade == 1) {
                    uniquePacks.push_back(i);
                }
                else if (pInfo.grade == 2) {
                    rarePacks.push_back(i);
                }
                else if (pInfo.grade == 3) {
                    normalPacks.push_back(i);
                }
            }
        }
        
        int uniqueSize = (int)uniquePacks.size();
        int rareSize = (int)rarePacks.size();
        int normalSize = (int)normalPacks.size();

        //
        int packIdx = -1;
        int r = rand()%100;
        if (uniqueSize > 0 && r < 40) { // 40%
            int idx = rand()%uniqueSize;
            packIdx = uniquePacks.at(idx);
        }
        else if (rareSize > 0 && r < 75) { // 35%
            int idx = rand()%rareSize;
            packIdx = rarePacks.at(idx);
        }
        else if (normalSize > 0) { // 25%
            int idx = rand()%normalSize;
            packIdx = normalPacks.at(idx);
        }
        
        return packIdx;
#endif
    }
    
    void initTrialSkinInfo() {
        setTrialCount(0);
        setTrialSkinIndex(-1);
    }
    void unlockTrialSkinAndGiveRewardAfterPurchase() {
        addToCoins(256);
        addToCoins(44);

        unlockTrialSkinAfterRestore();

        //
        TapjoyX::getInstance()->logEventInEconomy(3, 3);
    }
    void unlockTrialSkinAfterRestore() {
        int trialSkinIdx = getTrialSkinIndex();
        if (trialSkinIdx != -1) {
            unlockSkinAt(trialSkinIdx);
        }
        
        //
        initTrialSkinInfo();
    }
    void unlockGoldenFishAndGiveRewardAfterPurchase() {
        dataBackup[DataTypeCoins] += 512; // GoldenFishBuyBonus = 512  + 104 + 256 + 128
        dataBackup[DataTypeCoins] += 104;
        addToCoins(256);
        addToCoins(128);

        //
        int goldenFishIdx = getGoldenFishSkinIndex();
        unlockSkinAt(goldenFishIdx);
        
        //
        TapjoyX::getInstance()->logEventInEconomy(7, 4);
    }
    bool isGoldenFishUnlocked() {
        int goldenFishIdx = getGoldenFishSkinIndex();
        return !isLockedSkin(goldenFishIdx);
    }

    //
    //
    //
    void setGiftTime(int &restHour, int &restMin) {
        timeCheckIn = SocialX::getInstance()->getCurrentTime();
        secondSinceLastBoot = SocialX::getInstance()->getCurrentTimeSinceLastBootInSec();

        restHour = GiftTimeInHour;
        restMin = 0;
    }

    void getRestGiftTime(int &restHour, int &restMin) {
        //
        // First Time
        //
        if (secondSinceLastBoot == -1) {
            restHour = 0;
            restMin = 0;

            return;
        }

        float restTimeInSec = getRestTimeInSec();

        //
        if (restTimeInSec == 0) {
            restHour = 0;
            restMin = 0;
            
            return;
        }

        //
        float restTime = MAX(restTimeInSec, 0);
        restHour = restTime/3600;
        int restPartial = (restTime - restHour*3600);
        restMin = restPartial/60;
    }

    float getRestTimeInSec() {
        //
        std::time_t _timeCheckIn = SocialX::getInstance()->getCurrentTime();
        std::time_t deltaTimeCheckIn = _timeCheckIn - timeCheckIn;

        //
        float _secondSinceLastBoot = SocialX::getInstance()->getCurrentTimeSinceLastBootInSec();
        float deltaTimeInSec = _secondSinceLastBoot - secondSinceLastBoot;

        float delta = fabsf(deltaTimeCheckIn - deltaTimeInSec);

//        log("%ld, %f, %f, %d", deltaTimeCheckIn, deltaTimeInSec, delta, delta > 3600*GiftTimeInHour * DeltaGiftTimeAllowRate);

        if (deltaTimeInSec > 0) {
            //
            if (delta > 3600*GiftTimeInHour * DeltaGiftTimeAllowRate) {
                //
                // Possible cheat or Possible rebooting!!
                //
                deltaTimeInSec = deltaTimeInSec;
            }
            else {
                deltaTimeInSec = MAX(deltaTimeInSec, deltaTimeCheckIn);
            }
        }
        else {
            //
            // Assume rebooted
            //
            deltaTimeInSec = MAX(deltaTimeInSec, deltaTimeCheckIn);
        }

        float restTimeInSec = MAX(3600*GiftTimeInHour - deltaTimeInSec, 0); // 3600 * 6 = 6 hours
        return restTimeInSec;
    }

    float getSecondSinceLastBoot() {
        return secondSinceLastBoot;
    }

    void consumeCoinsForDraw() {
        dataBackup[DataTypeCoins] -= 64; // // GoldNeededToDraw = 64 + 36
        dataBackup[DataTypeCoins] -= 36;        
    }
    
    bool hasEnoughCoinsToDraw() {
        int tmp = dataBackup[DataTypeCoins] - 1;
        int base = baseScore + 99; // GoldNeededToDraw = 1 + 99
        if(tmp >= base)
            return true;
        else
            return false;
    }
    
    void addEarnedCoinsForPackage() {
        addToCoins(256);
        addToCoins(44);

        //
        TapjoyX::getInstance()->logEventInEconomy(6, 3);
    }

    void addEarnedCoinsForVideo() {
        dataBackup[DataTypeCoins] += 14; // VideoRewardGold = 14 + 11
        dataBackup[DataTypeCoins] += 11;

        //
        checkAchievement28();

        //
        TapjoyX::getInstance()->logEventInEconomy(0, 0);
    }
    void addEarnedCoinsForRemoveAds() {
        dataBackup[DataTypeCoins] += 512; // RemoveAdsRewardGold = 512  + 104 + 256 + 128
        dataBackup[DataTypeCoins] += 104;
        addToCoins(256);
        addToCoins(128);

        //
        TapjoyX::getInstance()->logEventInEconomy(4, 4);
    }

    //
    DevicePerformance devicePerformance;
    bool isRecordingEnabled() {
        return ((devicePerformance == NoRecording_NoFlowing) || (devicePerformance == NoRecording_WithFlowing))? false:true;
    }
    bool isFlowEnabled() {
        return ((devicePerformance == NoRecording_NoFlowing) || (devicePerformance == WithRecording_NoFlowing))? false:true;
    }
    bool isFullSupport() {
        return (devicePerformance == WithRecording_WithFlowing)? true:false;
    }

    //
    bool isThisFirstTimeLaunch() {
        return firstTime;
    }
    void setFirstTimeLaunch(bool flag) {
        firstTime = flag;
    }
    
    bool isVideoAvailable() {
        return UnityAdsX::getInstance()->isVideoAvailable() ||
                AdColonyX::getInstance()->isVideoAvailable() ||
                TapjoyX::getInstance()->isVideoReady();
    }

    void initHighScored() {
        highscored = false;
    }
    bool userHighscored() {
        return highscored;
    }
    
    void setPrevResultType(ResultType _type) {
        prevResultType = _type;
    }
    ResultType getPrevResultType() {
        return prevResultType;
    }
    
    const char *getFontPath() {
        if (UserInfo::getInstance()->currLangType == LanguageType::KOREAN) {
            return KoreanFontPath;
        }
        else if (UserInfo::getInstance()->currLangType == LanguageType::CHINESE) {
            return ChineseFontPath;
        }
        else if (UserInfo::getInstance()->currLangType == LanguageType::JAPANESE) {
            return JapaneseFontPath;
        }
        else {
            return EnglishFontPath;
        }
    }

    void setHiddenSkinUnlockCond(int idx) {
        int bit = 1;

        //
        bit = bit << 0;
        if (idx == 0) { // retro_03
            if (getLiked()) {
                dataBackup[DataTypeHiddenBackup] = dataBackup[DataTypeHiddenBackup] | bit;
            }
        }

        //
        bit = bit << 1;
        if (idx == 1) { // parody_04
            if (44 == skinNum && getWitchCapsuleCount() >= 10) {
                dataBackup[DataTypeHiddenBackup] = dataBackup[DataTypeHiddenBackup] | bit;
            }
        }
        
        //
        bit = bit << 1;
        if (idx == 2) { // parody_06
            if (lapInfo[4] == 16 && lapInfo[3] == 8) {
                dataBackup[DataTypeHiddenBackup] = dataBackup[DataTypeHiddenBackup] | bit;
            }
        }
        
        //
        bit = bit << 1;
        if (idx == 3) { // parody_09
            if (getCoins() == 999) {
                dataBackup[DataTypeHiddenBackup] = dataBackup[DataTypeHiddenBackup] | bit;
            }
        }
        
        //
        bit = bit << 1;
        if (idx == 4) { // parody_10
//            if (20 == skinNum && lapInfo[4] == 6 && lapInfo[3] == 1 && lapInfo[2] == 9 && lapInfo[1] == 1) {
            if (20 == skinNum && getCurrScore() > 1915) {
                dataBackup[DataTypeHiddenBackup] = dataBackup[DataTypeHiddenBackup] | bit;
            }
        }
        
        //
        bit = bit << 1;
        if (idx == 5) { // special_01
            if (39 == skinNum && getCurrScore() > 9999) {
                dataBackup[DataTypeHiddenBackup] = dataBackup[DataTypeHiddenBackup] | bit;
            }
        }
        
        //
        bit = bit << 1;
        if (idx == 6) { // special_02
            if (getNumberOfUnlockedSkin() >= 49) {
                dataBackup[DataTypeHiddenBackup] = dataBackup[DataTypeHiddenBackup] | bit;
            }
        }
        
        //
        bit = bit << 1;
        if (idx == 7) { // history_01
            if (51 == skinNum && jumpCount == 28) {
                dataBackup[DataTypeHiddenBackup] = dataBackup[DataTypeHiddenBackup] | bit;
            }
        }
    }

    void initJumpCount() {
        jumpCount = 0;
    }
    void increaseJumpCount()
    {
        jumpCount++;
        if (51 == skinNum && !getHiddenSkinUnlockCond(7)) {
            //
            setHiddenSkinUnlockCond(7);
        }
    }

    bool getHiddenSkinUnlockCond(int idx) {
        int bit = 1;
        bit = bit << idx;
        if ((dataBackup[DataTypeHiddenBackup] & bit) == bit) {
            return true;
        }
        else {
            return false;
        }
    }

    void saveLapInfo(int _curr) {
        lapInfo[0] = lapInfo[1];
        lapInfo[1] = lapInfo[2];
        lapInfo[2] = lapInfo[3];
        lapInfo[3] = lapInfo[4];
        lapInfo[4] = _curr;

        //
        setHiddenSkinUnlockCond(2);
        setHiddenSkinUnlockCond(3);
        setHiddenSkinUnlockCond(4);
        setHiddenSkinUnlockCond(5);
    }
    
    bool isFarEnoughFromLastSeenVideo() {
//        log("haveSeenVideo: %d", haveSeenVideo);
        if (haveSeenVideo >= 1) {
            return true;
        }

        return false;
    }
    void setHaveSeenVideo(bool flag) {
        if (flag) {
            haveSeenVideo = 0;
        }
        else {
            haveSeenVideo++;
        }
    }
    
    float screenScale;
    bool haveSeenTutorial;
    
    int getSkinNum() {
        return skinNum;
    }
    void setSkinNumWithTypeSet(int _num) {
        skinNum = _num;
    }
    void setRandomSkin(bool flag) {
        randomSkin = flag;
    }
    bool getRandomSkin() {
        return randomSkin;
    }
    int getFirstTimeResultTypeIndex() {
        return firstTimeResultTypeIndex;
    }
    void increaseFirstTimeResultTypeIndex() {
        firstTimeResultTypeIndex++;
    }
    int getResultGroupTypeIndex() {
        return resultGroupTypeIndex;
    }
    void increaseResultGroupTypeIndex() {
        resultGroupTypeIndex++;
        if (resultGroupTypeIndex >= ResultTypeGroupIndexMax) {
            resultGroupTypeIndex = 0;
        }
    }

private:
    bool randomSkin;

    int skinNum;
    int jumpCount;
    int capsuleCount;
    int dataBackup[DataTypeMax];
    bool firstTime;
    bool highscored;
    ResultType prevResultType;
    int firstTimeResultTypeIndex; // refer to FirstTimeResultTypeIndexMax
    int resultGroupTypeIndex;
    int lapInfo[5];

    //
    std::time_t timeCheckIn;
    float secondSinceLastBoot;

    int currScore;
    int baseScore;
    int achievementInfo;
    int currPlayCount;
    int haveSeenVideo;

    unsigned char* getHashValue(const unsigned char* data, int size);
    unsigned int digitsCount(int targetNum) {
        unsigned int test = 1;
        unsigned int digits = 0;
        while (targetNum >= test) {
            ++digits;
            test *= 10;
            if (digits == 10) {
                //
                // NOTE: 10 digits is maximum, test will overflow!
                //
                break;
            }
        }
        return digits;
    }
    
    void convertOldSkinInfoTo203(int skinInfo2) {
        //
        // Note:
        // At version 110 and 201 has empty skinInfo3, and
        // the hidden skin info is located in skinInfo2
        //
        // But version 203 has hidden skin info at skinInfo3.
        // So, we need to split skinInfo2 !!!
        //
        int _skinInfo2 = skinInfo2;
        int _skinInfo3 = skinInfo2;
        //
        _skinInfo2 &= 0b00000111111111111111111110000000;
        //
        _skinInfo3 &= 0b00000000000000000000000001111111; // grab last 7 bits for hidden skin
        _skinInfo3 = _skinInfo3 << 15; // move 15 bits to left
        _skinInfo3 |= 0b00001110000000000000000000000000;

        //
        dataBackup[DataTypeSkins2] = _skinInfo2;
        dataBackup[DataTypeSkins3] = _skinInfo3;
    }

    std::string cutByDigits(int originalNumber, int validDigits);
};

#endif /* defined(__Timefish__UserInfo__) */
