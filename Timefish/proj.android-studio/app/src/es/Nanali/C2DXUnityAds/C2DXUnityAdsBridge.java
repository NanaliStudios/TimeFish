package es.Nanali.C2DXUnityAds;

/**
 * Created by midchow on 16. 9. 13..
 */

import com.unity3d.ads.IUnityAdsListener;
import com.unity3d.ads.UnityAds;

import org.cocos2dx.lib.Cocos2dxActivity;

public class C2DXUnityAdsBridge {
    //
    private static Cocos2dxActivity s_activity;

    private static UnityAdsListener unityAdsListener;

    private static native void onUnityAdsStart();
    private static native void onUnityAdsFinish(boolean shown);

    public static void initC2DXUnityAdsBridge(Cocos2dxActivity activity){
        s_activity = activity;

        unityAdsListener = new UnityAdsListener();

        UnityAds.initialize(s_activity, "1139029", unityAdsListener);
        UnityAds.setDebugMode(false);
    }

    public static boolean isVideoAvailable() {
        return UnityAds.isReady();
    }

    public static void showVideo() {
        UnityAds.show(s_activity, "rewardedVideo");
    }

    private static class UnityAdsListener implements IUnityAdsListener {

        @Override
        public void onUnityAdsReady(String s) {

        }

        @Override
        public void onUnityAdsStart(final String s) {
            C2DXUnityAdsBridge.s_activity.runOnGLThread(new Runnable() {
                public void run() {
                    C2DXUnityAdsBridge.onUnityAdsStart();
                }
            });
        }

        @Override
        public void onUnityAdsFinish(String s, UnityAds.FinishState finishState) {
            if (finishState == UnityAds.FinishState.COMPLETED) {
                C2DXUnityAdsBridge.s_activity.runOnGLThread(new Runnable() {
                    public void run() {
                        C2DXUnityAdsBridge.onUnityAdsFinish(true);
                    }
                });
            }
            else {
                C2DXUnityAdsBridge.s_activity.runOnGLThread(new Runnable() {
                    public void run() {
                        C2DXUnityAdsBridge.onUnityAdsFinish(false);
                    }
                });
            }
        }

        @Override
        public void onUnityAdsError(UnityAds.UnityAdsError unityAdsError, String s) {

        }
    }
}
