package es.Nanali.C2DXFlurry;

import java.util.HashMap;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.util.Log;
import android.net.Uri;
import android.content.*;

import com.flurry.android.Constants;
import com.flurry.android.FlurryAgent;

import com.Nanali.Timefish.R;

//import android.content.DialogInterface;
//import android.content.Intent;
import android.app.Activity;
import android.app.AlertDialog;

public class C2DXFlurryBridge {
    private static final String TAG = "C2DXFlurry";
    private static Cocos2dxActivity s_activity;

    // the method must be called in main thread, before any other method
    public static void initC2DXFlurryBridge(Cocos2dxActivity activity){
        C2DXFlurryBridge.s_activity = activity;
    }
    
    public static void startWithAppVersion(String versionName) {
//        Log.v(TAG, "startSession(\""  + versionName + "\") is called...");
        
        FlurryAgent.setLogEnabled(false);
        FlurryAgent.setVersionName(versionName);
        FlurryAgent.init(s_activity.getContext(), "Z6YWGZRRZ54RJSR4DXJT");
    }
    
    public static void startSession() {
    }
    
    public static void logEvent(final String eventID) {
//        Log.v(TAG, "logEvent(\"" + eventID + "\") is called...");
        
        C2DXFlurryBridge.s_activity.runOnUiThread(new Runnable() {
            public void run() {
                FlurryAgent.logEvent(eventID);
            }
        });
    }
    
    public static void logBackgroundEvent(final String eventID, final int playCount) {
//        Log.v(TAG, "logEvent(\"" + eventID + "\") is called...");
        
        C2DXFlurryBridge.s_activity.runOnUiThread(new Runnable() {
            public void run() {
                
                HashMap<String, String> paramMap = new HashMap<String, String>();
                paramMap.put("Plays", String.valueOf(playCount));
                FlurryAgent.logEvent(eventID, paramMap);
            }
        });
    }
    
    public static void logPosterSelection(final String eventID, final int posterType) {
//        Log.v(TAG, "logEvent(\"" + eventID + "\") is called...");
        
        C2DXFlurryBridge.s_activity.runOnUiThread(new Runnable() {
            public void run() {
                
                HashMap<String, String> paramMap = new HashMap<String, String>();
                paramMap.put("Poster", String.valueOf(posterType));
                FlurryAgent.logEvent(eventID, paramMap);
            }
        });
    }
    
    public static void logResultEvent(final int score, final int bestScore, final int playCount, final int totalCount) {
//        Log.v(TAG, "logResultEvent(\"" + "Result" + "\") is called...");
        
        C2DXFlurryBridge.s_activity.runOnUiThread(new Runnable() {
            public void run() {
                
                HashMap<String, String> paramMap = new HashMap<String, String>();
                paramMap.put("Score", String.valueOf(score));
                paramMap.put("Best", String.valueOf(bestScore));
                paramMap.put("Plays", String.valueOf(playCount));
                paramMap.put("TotalPlays", String.valueOf(totalCount));
                FlurryAgent.logEvent("Result", paramMap);
            }
        });
    }
}
