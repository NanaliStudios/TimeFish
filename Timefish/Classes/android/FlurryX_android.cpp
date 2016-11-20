//
//  FlurryX_android.cpp
//  Timefish
//
//  Created by midchow on 2015. 2. 4..
//
//

#include "FlurryX.h"
#include "C2DXFlurryJni.h"

static FlurryX* s_pFlurryX = nullptr;

FlurryX* FlurryX::getInstance()
{
    if (s_pFlurryX == nullptr)
    {
        s_pFlurryX = new FlurryX();
    }
    return s_pFlurryX;
}

void FlurryX::startSession()
{
}

void FlurryX::logEvent(const char *eventName)
{
}

void FlurryX::logBackgroundEventWithInt(const char *eventName, int playCount)
{
}

void FlurryX::logPosterSelection(const char *eventName, int type)
{
}

void FlurryX::logResultEvent(int score, int bestScore, int playCount, int totalCount)
{
}

