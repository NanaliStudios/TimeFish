#include "AppDelegate.h"
#include "CILogoScene.h"
#include "PlayScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

void AppDelegate::setResolutionInfo()
{
    //
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    auto screenSize = glview->getFrameSize();
    
//    CCLOG("Screen Size : %f,%f", screenSize.width, screenSize.height);

    std::vector<std::string> resourceDirOrder;
    
    //
    if (screenSize.width == 2048 || screenSize.height == 2048) { // iPad Retina/ iPad Air / iPad3
        resourceDirOrder.push_back("ipadhd");
        resourceDirOrder.push_back("ipad");
        resourceDirOrder.push_back("iphonehd5");
        resourceDirOrder.push_back("iphonehd");
        resourceDirOrder.push_back("iPhone");

        glview->setDesignResolutionSize(BaseScreenWidth, BaseScreenHeight, ResolutionPolicy::NO_BORDER); // 1.775
    }
    else if (screenSize.width == 1024 || screenSize.height == 1024) { // iPad 2
        resourceDirOrder.push_back("ipad");
        resourceDirOrder.push_back("iphonehd5");
        resourceDirOrder.push_back("iphonehd");
        resourceDirOrder.push_back("iPhone");

        glview->setDesignResolutionSize(BaseScreenWidth, BaseScreenHeight, ResolutionPolicy::NO_BORDER); // 1.775
    }
    else if (screenSize.width == 2208 || screenSize.height == 2208) { // iPhone 6 Plus
        resourceDirOrder.push_back("iphonehd");
        resourceDirOrder.push_back("iphonehd5");

        glview->setDesignResolutionSize(BaseScreenWidth, BaseScreenHeight, ResolutionPolicy::NO_BORDER); // 1.775
    }
    else if (screenSize.width == 1334 || screenSize.height == 1334) { // iPhone 6
        resourceDirOrder.push_back("iphonehd");
        resourceDirOrder.push_back("iphonehd5");
        resourceDirOrder.push_back("mac");

        glview->setDesignResolutionSize(BaseScreenWidth, BaseScreenHeight, ResolutionPolicy::NO_BORDER); // 1.775
    }
    else if (screenSize.width == 1136 || screenSize.height == 1136) { // iPhone 5/ iPhone 5S
        resourceDirOrder.push_back("iphonehd5");
        resourceDirOrder.push_back("iphonehd");
        resourceDirOrder.push_back("iphone");
        
        glview->setDesignResolutionSize(BaseScreenWidth, BaseScreenHeight, ResolutionPolicy::NO_BORDER); // 1.775
    }
    else if (screenSize.width == 960 || screenSize.height == 960) { // iPhone 4/ iPhone 4S
        resourceDirOrder.push_back("iphonehd");
        resourceDirOrder.push_back("iphonehd5");
        resourceDirOrder.push_back("mac");

        glview->setDesignResolutionSize(BaseScreenWidth, BaseScreenHeight, ResolutionPolicy::NO_BORDER); // 1.775
    }
    else {
        resourceDirOrder.push_back("iphonehd5");

        glview->setDesignResolutionSize(BaseScreenWidth, BaseScreenHeight, ResolutionPolicy::NO_BORDER); // 1.775
    }
    
    float ratio1 = screenSize.height/ screenSize.width;
    float ratio2 = (float)(BaseScreenHeight)/ (float)BaseScreenWidth;

    UserInfo::getInstance()->screenScale = ratio1 / ratio2;
    
//    log("%f", UserInfo::getInstance()->screenScale);
    
    UserInfo::getInstance()->devicePerformance = SocialX::getInstance()->getDevicePerformance();

    //
    auto fileUtils = FileUtils::getInstance();
    fileUtils->setSearchPaths(resourceDirOrder);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Timefish");
        director->setOpenGLView(glview);
    }

    //
    // Read User Data
    //
    UserInfo::getInstance()->readUserData();

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    //
    GameCenterX::getInstance()->authenticateLocalPlayer();
#endif
    
    //
    FirebaseX::getInstance()->startSession();

    //
    UnityAdsX::getInstance()->startSession();

    //
    AdColonyX::getInstance()->startSession();

    //
    FlurryX::getInstance()->startSession();

    //
    ChartboostX::getInstance()->startSession();

    //
    UserInfo::getInstance()->increaseRunCount();

    //
    StoreKitX::getInstance()->requestProductData();

    //
    VideoShareX::getInstance()->initVideoShare();
    
    //
    TapjoyX::getInstance()->startSession();

    //
    NotificationX::getInstance()->enableNotification();
    NotificationX::getInstance()->cancelAllNotifications();

    // set resolution
    setResolutionInfo();

    // turn on display FPS
    director->setDisplayStats(false);

    director->setAnimationInterval(1.0 / 60);

    //
    runWithScene();
    

    return true;
}

void AppDelegate::runWithScene()
{
    //
    LocalizationManager::getInstance()->readTables();

    //
    SkeletonHolder::getInstance()->initSkeletons();

    //
    auto director = Director::getInstance();
    auto scene = CILogoScene::createScene();
//    auto scene = TestScene2::createScene();
    director->runWithScene(scene);
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
    NotificationX::getInstance()->registerAllLocalNotifications();
    UserInfo::getInstance()->saveUserData();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //
    SoundManager::getInstance()->pauseAllSoundEffect();
    Scene *scene = Director::getInstance()->getRunningScene();
    if (scene) {
        PlayScene *pScene = dynamic_cast<PlayScene*>(scene->getChildByTag(1));
        if(pScene != NULL) {
            pScene->enterBackgroundPause();
        }
    }
#endif

    //
    FlurryX::getInstance()->logBackgroundEventWithInt("Background", UserInfo::getInstance()->getPlayCount());
    TapjoyX::getInstance()->logEventInUIFlow("Background", UserInfo::getInstance()->getPlayCount());
    TapjoyX::getInstance()->endSession();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

    NotificationX::getInstance()->cancelAllNotifications();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //
    SoundManager::getInstance()->resumeAllSoundEffect();
    Scene *scene = Director::getInstance()->getRunningScene();
    if (scene) {
        PlayScene *pScene = dynamic_cast<PlayScene*>(scene->getChildByTag(1));
        if(pScene != NULL) {
            pScene->returnToForeground();
        }
    }
#endif
    //
    FlurryX::getInstance()->logEvent("Foreground");
    TapjoyX::getInstance()->logEventInUIFlow("Foreground");
    TapjoyX::getInstance()->startSession();
}
