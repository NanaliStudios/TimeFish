//
//  LocalizationManager.cpp
//  Timefish
//
//  Created by midchow on 2015. 7. 20..
//
//

#include "LocalizationManager.h"

#include "Global.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <istream>

USING_NS_CC;

using namespace std;
using std::cout;
using std::endl;

static LocalizationManager *_sharedLocalizationManager = nullptr;

LocalizationManager* LocalizationManager::getInstance()
{
    if (!_sharedLocalizationManager)
    {
        _sharedLocalizationManager = new (std::nothrow) LocalizationManager();
    }
    
    return _sharedLocalizationManager;
}

int LocalizationManager::readTables()
{
    int ret = 0;
    
    ret = readLocalizationTable();
    
    return ret;
}


std::string LocalizationManager::getPriceTagByProductName(const char* productName)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //
    // NOTE: Current fonts don't have Won mark.
    // So, replace it with Korean won letter.
    //
    std::string priceOrg = StoreKitX::getInstance()->getPriceByProductName(productName);
    std::stringstream stream;
    stream << LocalizationManager::getInstance()->getPriceStringWithRemoveWon(priceOrg);
    if (UserInfo::getInstance()->currLangType == LanguageType::KOREAN) {
        stream << "원";
    }
    std::string price = stream.str();
#else
    std::string price = StoreKitX::getInstance()->getPriceByProductName(productName);
#endif

    return price;
}

#define LOGDOTS for(int aaa=0;aaa<50;aaa++)log(".");
std::string LocalizationManager::getTablePathForFilename(const std::string &filename)
{
    return FileUtils::getInstance()->fullPathForFilename(filename);
}

//
// http://www.zedwood.com/article/cpp-csv-parser
//
std::vector<std::string> csv_read_row(std::istream &in, char delimiter);

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
int LocalizationManager::readLocalizationTable()
{
    std::string fullpath = getTablePathForFilename("Tables/Localization.csv");

    std::ifstream in(fullpath.c_str());
    if (in.fail()) return (cout << "File not found" << endl) && 0;
    int lineCount = -1;
    while(in.good())
    {
        lineCount++;
        
        // read line
        std::vector<std::string> row = csv_read_row(in, '|');
        
        // ignore first line
        if (lineCount == 0) {
            continue;
        }
        
        // process lines...
        int leng=(int)row.size();
        
        if (leng < 6) {
            break;
        }
        
        //
        LocalizationStat *stat = new LocalizationStat();
        
        int idx = 0;
        stat->keyCode   = row[idx]; idx++;
        stat->localizationString1   = row[idx]; idx++;
        stat->localizationString2   = row[idx]; idx++;
        stat->localizationString3   = row[idx]; idx++;
        stat->localizationString4   = row[idx]; idx++;
        stat->localizationString5   = row[idx]; idx++;
        
//        log("%s, %s, %s, %s, %s", stat->keyCode.c_str(),
//            stat->localizationString1.c_str(),
//            stat->localizationString2.c_str(),
//            stat->localizationString3.c_str(),
//            stat->localizationString4.c_str());
//            stat->localizationString5.c_str());
        
        localizationStats.push_back(stat);
    }
    in.close();
    
    localizationStatSize = localizationStats.size();
    
    return 1;
}
#else
int LocalizationManager::readLocalizationTable()
{
    std::string fullpath = getTablePathForFilename("Tables/Localization.csv");
    
    Data data = FileUtils::getInstance()->getDataFromFile(fullpath);
    int length = (int)data.getSize();
    char* bytes = (char*)malloc(sizeof(char)*length);
    memcpy(bytes, data.getBytes(), length);
    
    //
    //
    int lineCount = -1;
    int cnt = 0;
    while (cnt < length) {
        int start = cnt;
        int leng = 0;
        while (bytes[cnt] != '\n') {
            leng++;
            cnt++;
        }
        
        std::vector<std::string> row;//relying on RVO
        std::stringstream ss;
        int finish = start+leng+1;
        for (int i=start; i<finish; i++) {
            bool inquotes = false;
            char c = bytes[i];
            if (!inquotes && c=='"') //beginquotechar
            {
                inquotes=true;
            }
            else if (inquotes && c=='"') //quotechar
            {
                if ( bytes[i+1] == '"')//2 consecutive quotes resolve to 1
                {
                    ss << bytes[i];
                }
                else //endquotechar
                {
                    inquotes=false;
                }
            }
            else if (!inquotes && c=='|') //end of field
            {
                row.push_back( ss.str() );
                ss.str("");
            }
            else if (!inquotes && (i == finish - 1 || c=='\r' || c=='\n') )
            {
                row.push_back( ss.str() );
                break;
            }
            else
            {
                ss << c;
            }
        }
        
        lineCount++;
        
        cnt++;
        
        // ignore first line
        if (lineCount == 0) {
            continue;
        }
        
        // process lines...
        int items=(int)row.size();
        
        if (items < 6) {
            break;
        }
        
        //
        LocalizationStat *stat = new LocalizationStat();
        
        int idx = 0;
        stat->keyCode   = row[idx]; idx++;
        stat->localizationString1   = row[idx]; idx++;
        stat->localizationString2   = row[idx]; idx++;
        stat->localizationString3   = row[idx]; idx++;
        stat->localizationString4   = row[idx]; idx++;
        stat->localizationString5   = row[idx]; idx++;
        
//        log("Localization: %s, %s, %s, %s, %s", stat->keyCode.c_str(),
//            stat->localizationString1.c_str(),
//            stat->localizationString2.c_str(),
//            stat->localizationString3.c_str(),
//            stat->localizationString4.c_str());
//            stat->localizationString5.c_str());
        
        localizationStats.push_back(stat);
    }
    localizationStatSize = localizationStats.size();
    
    return 1;
    
    free(bytes);
}
#endif

std::vector<std::string> csv_read_row(std::istream &in, char delimiter)
{
    std::stringstream ss;
    bool inquotes = false;
    std::vector<std::string> row;//relying on RVO
    while(in.good())
    {
        char c = in.get();
        if (!inquotes && c=='"') //beginquotechar
        {
            inquotes=true;
        }
        else if (inquotes && c=='"') //quotechar
        {
            if ( in.peek() == '"')//2 consecutive quotes resolve to 1
            {
                ss << in.get();
            }
            else //endquotechar
            {
                inquotes=false;
            }
        }
        else if (!inquotes && c==delimiter) //end of field
        {
            row.push_back( ss.str() );
            ss.str("");
        }
        else if (!inquotes && (c=='\r' || c=='\n') )
        {
            row.push_back( ss.str() );
            return row;
        }
        else
        {
            ss << c;
        }
    }
    
    return row;
}
