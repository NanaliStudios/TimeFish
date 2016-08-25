//
//  VideoShareX_android.cpp
//  Timefish
//
//  Created by midchow on 2015. 8. 19..
//
//

#include "VideoShareX.h"

static VideoShareX* s_pVideoShareX = nullptr;

VideoShareX* VideoShareX::getInstance()
{
    if (s_pVideoShareX == nullptr)
    {
        s_pVideoShareX = new VideoShareX();
    }
    return s_pVideoShareX;
}

void VideoShareX::initVideoShare()
{
    
}
void VideoShareX::startRecording()
{
    
}
void VideoShareX::stopRecording()
{
    
}
void VideoShareX::pauseRecording()
{
    
}
void VideoShareX::resumeRecording()
{
    
}
void VideoShareX::showView()
{
    
}

void VideoShareX::showEveryplay()
{
    
}