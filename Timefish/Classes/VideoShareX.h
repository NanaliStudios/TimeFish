//
//  VideoShareX.h
//  Timefish
//
//  Created by midchow on 2015. 8. 4..
//
//

#ifndef __Timefish__VideoShareX__
#define __Timefish__VideoShareX__

class VideoShareXDelegate
{
public:
    virtual ~VideoShareXDelegate() {}
};

class VideoShareX {
    
public:
    static VideoShareX* getInstance();
    
    void initVideoShare();

    //
    void startRecording();
    void stopRecording();
    void pauseRecording();
    void resumeRecording();
    void showView();
    void showEveryplay();

    //
    void setDelegate(VideoShareXDelegate* delegate) { m_delegate = delegate; }
    VideoShareXDelegate* getDelegate() { return m_delegate; }
    
private:
    VideoShareX()
    : m_delegate(nullptr)
    {
    }
    
    VideoShareXDelegate* m_delegate;
};


#endif /* defined(__Timefish__VideoShareX__) */
