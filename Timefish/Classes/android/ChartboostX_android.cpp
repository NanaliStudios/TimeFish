//
//  ChartboostX_android.cpp
//  Timefish
//
//  Created by midchow on 2015. 2. 4..
//
//

#include "ChartboostX.h"
#include "C2DXChartboostJni.h"

static ChartboostX* s_pChartboostX = nullptr;

ChartboostX* ChartboostX::getInstance()
{
    if (s_pChartboostX == nullptr)
    {
        s_pChartboostX = new ChartboostX();
    }
    return s_pChartboostX;
}

void ChartboostX::startSession()
{
    // do nothing here
}

void ChartboostX::purgeChartboostX()
{
}

void ChartboostX::cacheInterstitial(const char* location)
{
    cacheInterstitialJNI(location);
}

void ChartboostX::showInterstitial(const char* location)
{
    showInterstitialJNI(location);
}

bool ChartboostX::hasCachedInterstitial(const char* location)
{
    return hasInterstitialJNI(location);
}
