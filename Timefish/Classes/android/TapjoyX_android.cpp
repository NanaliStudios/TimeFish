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
    return isVideoReadyJNI();
}
bool TapjoyX::isPlacementReady(const char *placementName)
{
    return isPlacementReadyJNI(placementName);
}
void TapjoyX::startSession()
{
    startTapjoySessionJNI();
}
void TapjoyX::endSession()
{
    endTapjoySessionJNI();
}
void TapjoyX::requestPlacement(const char *placementName)
{
    requestPlacementJNI(placementName);
}
void TapjoyX::showPlacement(const char *placementName)
{
    showPlacementJNI(placementName);
}
void TapjoyX::setUserLevel(int lvl)
{
    setUserLevelJNI(lvl);
}
void TapjoyX::logEventInEconomy(int eventTypeIdx, int param)
{
    logEventInEconomyJNI(eventTypeIdx, param);
}
void TapjoyX::logEventInUIFlow(const char *eventName, int param)
{
    logEventInUIFlowJNI(eventName, param);
}
void TapjoyX::logEventInUIFlowResult(const char *eventName, int resultTypeIdx)
{
    logEventInUIFlowResultJNI(eventName, resultTypeIdx);
}
void TapjoyX::logEventInUIFlowOption(int btnIdx)
{
    logEventInUIFlowOptionJNI(btnIdx);
}
void TapjoyX::logEventInResultFlowResultButton(int btnIdx, int skinNum)
{
    logEventInResultFlowResultButtonJNI(btnIdx, skinNum);
}
void TapjoyX::logEventInIAP(const char *eventName, int skinIdx)
{
    logEventInIAPJNI(eventName, skinIdx);
}
