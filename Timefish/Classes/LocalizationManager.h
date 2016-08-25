//
//  LocalizationManager.h
//  Timefish
//
//  Created by midchow on 2015. 7. 20..
//
//

#ifndef __Timefish__LocalizationManager__
#define __Timefish__LocalizationManager__

#include "cocos2d.h"

#include "UserInfo.h"
#include "StoreKitX.h"

struct LocalizationStat
{
    std::string keyCode;
    std::string localizationString1; // korean
    std::string localizationString2; // english
    std::string localizationString3; // chinese
    std::string localizationString4; // japanese
};

USING_NS_CC;

class LocalizationManager
{
public:
    static LocalizationManager* getInstance();
    //
    // TODO: NEED vector cleanup!
    //
    
    int readTables();
    
    void replaceStringInPlace(std::string& subject, const std::string& search,
                              const std::string& replace) {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
    }
    
    std::string getLocalizationString(std::string _keyCode) {
        
        for (int i=0; i<localizationStatSize; i++) {
            LocalizationStat *item = localizationStats.at(i);
            if (item->keyCode == _keyCode) {
                //
                std::string str;
                if (UserInfo::getInstance()->currLangType == LanguageType::KOREAN) {
                    str = std::string(item->localizationString1);
                }
                else if (UserInfo::getInstance()->currLangType == LanguageType::CHINESE) {
                    str = std::string(item->localizationString3);
                }
                else if (UserInfo::getInstance()->currLangType == LanguageType::JAPANESE) {
                    str = std::string(item->localizationString4);
                }
                else {
                    str = std::string(item->localizationString2);
                }

                // line breaks 'string's
                replaceStringInPlace(str, "\\n", "\n");

                return str;
            }
        }
        
        return "";
    }
    
    std::string getLocalizationStringWithEnglish(std::string _keyCode) {
        
        for (int i=0; i<localizationStatSize; i++) {
            LocalizationStat *item = localizationStats.at(i);
            if (item->keyCode == _keyCode) {
                //
                std::string str = std::string(item->localizationString2);
                
                // line breaks 'string's
                replaceStringInPlace(str, "\\n", "\n");
                
                return str;
            }
        }
        
        return "";
    }
    
    std::string getLocalizationStringWithReplace(std::string _keyCode, int replaceWith) {
        return getLocalizationStringWithReplace(_keyCode, std::to_string(replaceWith));
    }

    std::string getLocalizationStringWithReplace(std::string _keyCode, std::string replaceWith) {
        for (int i=0; i<localizationStatSize; i++) {
            LocalizationStat *item = localizationStats.at(i);
            if (item->keyCode == _keyCode) {
                
                std::string str;
                
                if (UserInfo::getInstance()->currLangType == LanguageType::KOREAN) {
                    str = std::string(item->localizationString1);
                }
                else if (UserInfo::getInstance()->currLangType == LanguageType::CHINESE) {
                    str = std::string(item->localizationString3);
                }
                else if (UserInfo::getInstance()->currLangType == LanguageType::JAPANESE) {
                    str = std::string(item->localizationString4);
                }
                else {
                    str = std::string(item->localizationString2);
                }
                
                // line breaks 'string's
                replaceStringInPlace(str, "\\n", "\n");
                
                // replace string
                size_t f = str.find("{0}");
                str.replace(f, std::string("{0}").length(), replaceWith);
                
                return str;
            }
        }
        
        return "";
    }

    std::string getPriceStringWithRemoveWon(std::string orgStr) {
        replaceStringInPlace(orgStr, "₩", "");

        return orgStr;
    }
    
    std::string getPriceTagByProductName(const char* productName);

    std::string getLocalizationStringWithReplaceTwo(std::string _keyCode, std::string replaceWith1, std::string replaceWith2) {
        
        for (int i=0; i<localizationStatSize; i++) {
            LocalizationStat *item = localizationStats.at(i);
            if (item->keyCode == _keyCode) {
                
                std::string str;
                
                if (UserInfo::getInstance()->currLangType == LanguageType::KOREAN) {
                    str = std::string(item->localizationString1);
                }
                else if (UserInfo::getInstance()->currLangType == LanguageType::CHINESE) {
                    str = std::string(item->localizationString3);
                }
                else if (UserInfo::getInstance()->currLangType == LanguageType::JAPANESE) {
                    str = std::string(item->localizationString4);
                }
                else {
                    str = std::string(item->localizationString2);
                }
                
                // line breaks 'string's
                replaceStringInPlace(str, "\\n", "\n");
                
                // replace string1
                size_t f1 = str.find("{0}");
                str.replace(f1, std::string("{0}").length(), replaceWith1);

                // replace string2
                size_t f2 = str.find("{00}");
                str.replace(f2, std::string("{00}").length(), replaceWith2);

                return str;
            }
        }
        
        return "";
    }
    
private:
    std::vector<LocalizationStat*> localizationStats;
    ssize_t localizationStatSize;
    
    std::string getTablePathForFilename(const std::string &filename);
    int readLocalizationTable();
};

#endif /* defined(__Timefish__LocalizationManager__) */
