LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
					../../../Classes/AchievementBannerLayer.cpp \
					../../../Classes/AchievementSelectionLayer.cpp \
					../../../Classes/AppDelegate.cpp \
					../../../Classes/BGLayer.cpp \
					../../../Classes/BallLayer.cpp \
					../../../Classes/BonusTooltipLayer.cpp \
					../../../Classes/CILogoScene.cpp \
					../../../Classes/CreditLayer.cpp \
					../../../Classes/FishLayer.cpp \
					../../../Classes/FreeGiftLayer.cpp \
					../../../Classes/FreeshMakerLayer.cpp \
					../../../Classes/FreeshSelectionLayer.cpp \
					../../../Classes/FreeshTrailLayer.cpp \
					../../../Classes/GraySprite.cpp \
					../../../Classes/IdleFreeshLayer.cpp \
					../../../Classes/JumpCoin.cpp \
					../../../Classes/LoadingScene.cpp \
					../../../Classes/LocalizationManager.cpp \
					../../../Classes/MenuItemAutoGray.cpp \
					../../../Classes/MenuItemImageButton.cpp \
					../../../Classes/MenuItemSameImage.cpp \
					../../../Classes/Obstacle.cpp \
					../../../Classes/ObstacleLayer.cpp \
					../../../Classes/OptionLayer.cpp \
					../../../Classes/PageViewlayer.cpp \
					../../../Classes/PlayScene.cpp \
					../../../Classes/Popup.cpp \
					../../../Classes/PopupManager.cpp \
					../../../Classes/PosterLayer.cpp \
					../../../Classes/ResultMainLayer.cpp \
					../../../Classes/ResultUIButtonLayer.cpp \
					../../../Classes/ScoreLayer.cpp \
					../../../Classes/ScrollLayerBase.cpp \
					../../../Classes/ShakeAction.cpp \
					../../../Classes/SkeletonHolder.cpp \
					../../../Classes/SleepyTimon.cpp \
					../../../Classes/SoundManager.cpp \
					../../../Classes/SpriteFlow.cpp \
					../../../Classes/SpriteHole.cpp \
					../../../Classes/TestScene2.cpp \
					../../../Classes/TitleScene.cpp \
					../../../Classes/TooltipLayer.cpp \
					../../../Classes/TutorialLayer.cpp \
					../../../Classes/UILayer.cpp \
					../../../Classes/UserInfo.cpp \
					../../../Classes/sha2.c \
                   C2DXAdColonyJni.cpp \
                   C2DXChartboostJni.cpp \
                   C2DXFlurryJni.cpp \
                   C2DXNotificationJni.cpp \
                   C2DXSocialJni.cpp \
                   C2DXStoreJni.cpp \
                   C2DXTapjoyJni.cpp \
                   C2DXUnityAdsJni.cpp \
					../../../Classes/android/AdColonyX_android.cpp \
					../../../Classes/android/ChartboostX_android.cpp \
					../../../Classes/android/FlurryX_android.cpp \
					../../../Classes/android/NotificationX_android.cpp \
					../../../Classes/android/SocialX_android.cpp \
					../../../Classes/android/StoreKitX_android.cpp \
					../../../Classes/android/TapjoyX_android.cpp \
					../../../Classes/android/UnityAdsX_android.cpp \
					../../../Classes/android/VideoShareX_android.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END

