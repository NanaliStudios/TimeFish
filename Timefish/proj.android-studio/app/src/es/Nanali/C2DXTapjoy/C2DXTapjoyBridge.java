package es.Nanali.C2DXTapjoy;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.*;
import android.os.Bundle;

import java.util.Hashtable;

import com.tapjoy.Tapjoy;
import com.tapjoy.TJPlacement;
import com.tapjoy.TJPlacementListener;
import com.tapjoy.TJError;
import com.tapjoy.TJActionRequest;
import com.tapjoy.TapjoyConnectFlag;
import com.tapjoy.TJConnectListener;

import android.util.Log;

public class C2DXTapjoyBridge {
    private static final String TAG = "C2DXTapjoy";
    private static Cocos2dxActivity s_activity;
    
    private static native void onRequestSuccess(String placement);
    private static native void onRequestFailure(String placement);
    private static native void onContentReady(String placement);
    private static native void onContentShow(String placement);
    private static native void onContentDismiss(String placement);

    //
    private static TJPlacement launchPlacement;
    private static TJPlacement videoPlacement;
    
    /*
     * TJPlacement callbacks
     */
    static private TJPlacementListener listener = new TJPlacementListener() {
        @Override
        public void onRequestSuccess(final TJPlacement placement) {
        // If content is not available you can note it here and act accordingly as best suited for your app
//            Log.i(TAG, "Tapjoy on request success, contentAvailable: " + placement.isContentAvailable());
            C2DXTapjoyBridge.s_activity.runOnGLThread(new Runnable() {
                public void run() {
                    C2DXTapjoyBridge.onRequestSuccess(placement.getName());
                }
            });
        }

        @Override
        public void onRequestFailure(final TJPlacement placement, TJError error) {
//            Log.v(TAG, "Request failed..." + error.message);
//            Log.i(TAG, "Tapjoy send event " + placement.getName() + " failed with error: " + error.message);
            C2DXTapjoyBridge.s_activity.runOnGLThread(new Runnable() {
                public void run() {
                    C2DXTapjoyBridge.onRequestFailure(placement.getName());
                }
            });
        }

        @Override
        public void onContentReady(final TJPlacement placement) {
//            Log.i(TAG, "Tapjoy onContentReady: " + placement.getName());
            C2DXTapjoyBridge.s_activity.runOnGLThread(new Runnable() {
                public void run() {
                    C2DXTapjoyBridge.onContentReady(placement.getName());
                }
            });
        }

        @Override
        public void onContentShow(final TJPlacement placement) {
            C2DXTapjoyBridge.s_activity.runOnGLThread(new Runnable() {
                public void run() {
                    C2DXTapjoyBridge.onContentShow(placement.getName());
                }
            });
        }

        @Override
        public void onContentDismiss(final TJPlacement placement) {
            // Begin preloading the next placement after the previous one is dismissed
            requestPlacement("VideoRewardPlacement");

            C2DXTapjoyBridge.s_activity.runOnGLThread(new Runnable() {
                public void run() {
                    C2DXTapjoyBridge.onContentDismiss(placement.getName());
                }
            });
        }
        @Override
        public void onPurchaseRequest(TJPlacement placement, TJActionRequest request, String productId) {
        }
        @Override
        public void onRewardRequest(TJPlacement placement, TJActionRequest request, String itemId, int quantity) {
        }
    };

    // the method must be called in main thread, before any other method
    public static void initC2DXTapjoyBridge(Cocos2dxActivity activity){
    	C2DXTapjoyBridge.s_activity = activity;
    }

    public static void startWithSDKKey(Context context, String sdkKey) {
//        Log.v(TAG, "startWithSDKKey is called...");

        // Init Tapjoy
        Hashtable<String, Object> connectFlags = new Hashtable<String, Object>();
        connectFlags.put(TapjoyConnectFlag.ENABLE_LOGGING, "true");

        Tapjoy.connect(context, sdkKey, connectFlags, new TJConnectListener() {
            @Override
            public void onConnectSuccess() {
//                Log.v(TAG, "Connection success...");
//                TapjoyEasyApp.this.onConnectSuccess();
            }
            
            @Override
            public void onConnectFailure() {
//                Log.v(TAG, "Connection fail...");
//                TapjoyEasyApp.this.onConnectFail();
            }
        });
        Tapjoy.setDebugEnabled(true);

        //
        launchPlacement  = new TJPlacement(context, "App Launch", listener);
        videoPlacement= new TJPlacement(context, "VideoRewardPlacement", listener);
    }
    
    public static void startTapjoySession() {
        // The session is controlled in AppActivity! Do nothing here
    }
    public static void endTapjoySession() {
        // The session is controlled in AppActivity! Do nothing here
    }

    public static boolean isVideoReady() {
        return videoPlacement.isContentReady();
    }
    public static boolean isPlacementReady(final String placement) {
        if (placement.equals("VideoRewardPlacement")) {
            if (videoPlacement != null) {
                return videoPlacement.isContentReady();
            }
        }
        else if (placement.equals("App Launch")) {
            return launchPlacement.isContentReady();
        }

        
        // else
        TJPlacement p = new TJPlacement(C2DXTapjoyBridge.s_activity, placement, listener);
        return p.isContentReady();
    }
    
    public static void requestPlacement(final String placement) {
        if (placement.equals("VideoRewardPlacement")) {
            if (videoPlacement != null) {
//                Log.v(TAG, "requestPlacement..." + placement);
                videoPlacement.requestContent();
                return;
            }
        }
        else if (placement.equals("App Launch")) {
            if (launchPlacement != null) {
//                Log.v(TAG, "requestPlacement..." + placement);
                launchPlacement.requestContent();
                return;
            }
        }

        TJPlacement p = new TJPlacement(C2DXTapjoyBridge.s_activity, placement, listener);
        p.requestContent();
    }

    public static void showPlacement(final String placement) {
//        Log.v(TAG, "showPlacement..." + placement);
        if (placement.equals("VideoRewardPlacement")) {
            if (videoPlacement != null) {
                if (videoPlacement.isContentReady()) {
                    videoPlacement.showContent();
                }
                
                return;
            }
        }
        else if (placement.equals("App Launch")) {
            if (launchPlacement != null) {
                if (launchPlacement.isContentReady()) {
                    launchPlacement.showContent();
                }
                
                return;
            }
        }

        TJPlacement p = new TJPlacement(C2DXTapjoyBridge.s_activity, placement, listener);
        if (p.isContentReady()) {
            p.showContent();
        }
        else {
             //handle situation where there is no content to show, or it has not yet downloaded.
        }
    }

    public static void setUserLevel(int lvl) {
        Tapjoy.setUserLevel(lvl);
    }

    public static void logEventInEconomy(int eventTypeIdx, int param) {
        final String eventNames[] = {
            "VideoReward",
            "FreeGift",
            "EarnInGame",
            "TrialReward",
            "RemoveAdsReward",
            "UseGold",
            "PackageReward",
            "GoldenFish",
        };
        String eventName = String.format("%s", eventNames[eventTypeIdx]);
        Tapjoy.trackEvent("Economy", eventName, param + 1);
    }

    public static void logEventInUIFlow(final String eventName, int param) {
        if (param > -1) {
            String parameter = String.format("Tier%d", param);
            Tapjoy.trackEvent("UIFlow", eventName, parameter, "");
        }
        else {
            Tapjoy.trackEvent("UIFlow", eventName, "", "");
        }
    }

    public static void logEventInUIFlowResult(final String eventName, int resultTypeIdx) {
        final String resultTypes[] = {
            "ResultTypeGift1",
            "ResultTypeGift2",
            "ResultTypeDraw1",
            "ResultTypeDraw2",
            "ResultTypeVideo1",
            "ResultTypeVideo2",
            "ResultTypeBuy1",
            "ResultTypeBuy2",
            "ResultTypeRemoveAD1",
            "ResultTypeRemoveAD2",
            "ResultTypeTrial",
            "ResultTypeRate",
            "ResultTypeFacebook",
            "ResultTypeShare",
            "ResultTypeArtwork",
            "ResultTypeBuyPackage",
            "ResultTypeGoldenFish",
        };
        String parameter = String.format("%s", resultTypes[resultTypeIdx]);
        Tapjoy.trackEvent("UIFlow", eventName, parameter, "");
    }

    public static void logEventInUIFlowOption(int btnIdx) {
        if (btnIdx > 5) {
            return;
        }
        
        final String eventNames[] = {
            "Sound",
            "Notification",
            "Restore",
            "Tutorial",
            "Facebook",
            "Credit",
        };
        String eventName = String.format("%s", eventNames[btnIdx]);
        Tapjoy.trackEvent("UIFlow", eventName, "", "");
    }

    public static void logEventInResultFlowResultButton(int btnIdx, int skinNum) {
        final String eventNames[] = {
            "GiftButton",
            "DrawButton",
            "VideoButton",
            "BuySkinButton",
            "RemoveAdsButton",
            "TrialButton",
            "RateButton",
            "LikeButton",
            "VideoShareButton",
            "ArtButton",
            "PackageButton"
        };
        String eventName = String.format("%s", eventNames[btnIdx]);
        if (skinNum > -1) {
            String parameter = String.format("%d", skinNum);
            Tapjoy.trackEvent("ResultFlow", eventName, parameter, "");
        }
        else {
            Tapjoy.trackEvent("ResultFlow", eventName, "", "");
        }
    }

    public static void logEventInIAP(final String eventName, int skinIdx) {
        if (skinIdx > -1) {
            String parameter = String.format("%d", skinIdx + 1);
            Tapjoy.trackEvent("IAP", eventName, parameter, "");
        }
        else {
            Tapjoy.trackEvent("IAP", eventName, "", "");
        }
    }
}
