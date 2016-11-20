//
//  TapjoyX_android.cpp
//  Timefish
//
//  Created by midchow on 2015. 8. 19..
//
//

#include "TapjoyX.h"
#include "C2DXTapjoyJni.h"

static TapjoyX* s_pTapjoyX = nullptr;

TapjoyX* TapjoyX::getInstance()
{
    if (s_pTapjoyX == nullptr)
    {
        s_pTapjoyX = new TapjoyX();
    }
    return s_pTapjoyX;
}

bool TapjoyX::isVideoReady()
{
    return false;
}
bool TapjoyX::isPlacementReady(const char *placementName)
{
    return false;
}
void TapjoyX::startSession()
{
}
void TapjoyX::endSession()
{
}
void TapjoyX::requestPlacement(const char *placementName)
{
}
void TapjoyX::showPlacement(const char *placementName)
{
}
void TapjoyX::setUserLevel(int lvl)
{
}
void TapjoyX::logEventInEconomy(int eventTypeIdx, int param)
{
}
void TapjoyX::logEventInUIFlow(const char *eventName, int param)
{
}
void TapjoyX::logEventInUIFlowResult(const char *eventName, int resultTypeIdx)
{
}
void TapjoyX::logEventInUIFlowOption(int btnIdx)
{
}
void TapjoyX::logEventInResultFlowResultButton(int btnIdx, int skinNum)
{
}
void TapjoyX::logEventInIAP(const char *eventName, int skinIdx)
{
}
