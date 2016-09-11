package es.Nanali.C2DXAdColony;

import org.cocos2dx.lib.Cocos2dxActivity;

import com.jirbo.adcolony.*;

import android.util.Log;

public class C2DXAdColonyBridge {
    private static final String TAG = "C2DXAdColony";
    private static Cocos2dxActivity s_activity;
    private static String zoneID;
    
    private static native void onAdColonyAdStartedInZone(String zoneID);
    private static native void onAdColonyAdAttemptFinished(boolean shown, String zoneID);
//    private static native void onAdColonyAdAvailabilityChange(boolean available, String zoneID);

    // the method must be called in main thread, before any other method
    public static void initC2DXAdColonyBridge(Cocos2dxActivity activity){
    	C2DXAdColonyBridge.s_activity = activity;
    }

    public static void configureWith(String addID, String _zoneID, String versionName) {
//        Log.v(TAG, "configureWith is called...");

        // Init AdColony
        zoneID = _zoneID;
        AdColony.configure(s_activity, "version:"+ versionName, addID, _zoneID);
    }
    
    public static boolean isVideoAvailable() {
//        Log.v(TAG, "isVideoAvailable is called...");

        /*
         "invalid" - You've entered an incorrect zone id, or have not included this zone id in your configure call.
         "unknown" - You have not configured AdColony.
         "off" - The zone is disabled.
         "loading" - The zone is enabled, but does not have any ads available to be played at this point.
         "active" - The zone is enabled and has ads ready to be played.
         */
        String status = AdColony.statusForZone(zoneID);
        if (status.equals("active")) {
//            Log.d(TAG, "isVideoReady: Active");
            return true;
        }

//        Log.d(TAG, "isVideoReady: inActive");
        return false;
    }

    static private AdColonyAdListener listener = new AdColonyAdListener() {
    // Ad Started Callback - called only when an ad successfully starts playing
    public void onAdColonyAdStarted( AdColonyAd ad )
    {
//        Log.d(TAG, "onAdColonyAdStarted");

        C2DXAdColonyBridge.s_activity.runOnGLThread(new Runnable() {
            public void run() {
                C2DXAdColonyBridge.onAdColonyAdStartedInZone("zoneID");
            }
        });

    }

    //Ad Attempt Finished Callback - called at the end of any ad attempt - successful or not.
    public void onAdColonyAdAttemptFinished( AdColonyAd ad )
    {
    // You can ping the AdColonyAd object here for more information:
    // ad.shown() - returns true if the ad was successfully shown.
    // ad.notShown() - returns true if the ad was not shown at all (i.e. if onAdColonyAdStarted was never triggered)
    // ad.skipped() - returns true if the ad was skipped due to an interval play setting
    // ad.canceled() - returns true if the ad was cancelled (either programmatically or by the user)
    // ad.noFill() - returns true if the ad was not shown due to no ad fill.

//        Log.d(TAG, "onAdColonyAdAttemptFinished");

        C2DXAdColonyBridge.s_activity.runOnGLThread(new Runnable() {
            public void run() {
                C2DXAdColonyBridge.onAdColonyAdAttemptFinished(true, "zoneID");
            }
        });

    }

    // Ad Availability Change Callback - update button text
    public void onAdColonyAdAvailabilityChange(boolean available, String zone_id)
    {
//        Log.d(TAG, "onAdColonyAdAvailabilityChange:" + available);
    }
    };

    static private AdColonyV4VCListener v4vcListener = new AdColonyV4VCListener() {
    // Reward Callback
    public void onAdColonyV4VCReward( AdColonyV4VCReward reward )
    {
        if (reward.success())
        {
//            Log.d(TAG, "reward.success");

//            C2DXAdColonyBridge.s_activity.runOnGLThread(new Runnable() {
//            public void run() {
//                C2DXAdColonyBridge.onAdColonyAdAttemptFinished(true, "zoneID");
//            }
//            });
        }
        else
        {
//            Log.d(TAG, "reward.fail");
//            C2DXAdColonyBridge.s_activity.runOnGLThread(new Runnable() {
//                public void run() {
//                    C2DXAdColonyBridge.onAdColonyAdAttemptFinished(false, "zoneID");
//                }
//            });

        }
    }
    };

    public static void showVideo() {
//        Log.d(TAG, "showVideo");
        AdColony.addV4VCListener(C2DXAdColonyBridge.v4vcListener);

        AdColonyV4VCAd ad = new AdColonyV4VCAd(zoneID).withListener(C2DXAdColonyBridge.listener);
        ad.show();
    }
}
