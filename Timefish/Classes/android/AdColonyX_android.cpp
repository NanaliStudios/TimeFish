//
//  AdColonyX_android.cpp
//  Timefish
//
//  Created by midchow on 2015. 8. 19..
//
//

#include "AdColonyX.h"
#include "C2DXAdColonyJni.h"

static AdColonyX* s_pAdColonyX = nullptr;

AdColonyX* AdColonyX::getInstance()
{
    if (s_pAdColonyX == nullptr)
    {
        s_pAdColonyX = new AdColonyX();
    }
    return s_pAdColonyX;
}

void AdColonyX::startSession()
{
    // do nothing here
}

void AdColonyX::showVideo()
{
}

bool AdColonyX::isVideoAvailable()
{
    return false;
}
