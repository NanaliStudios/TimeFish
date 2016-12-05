package es.Nanali.C2DXFirebase;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.InterstitialAd;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.AdRequest;

import android.graphics.Color;
import android.util.Log;
import android.view.Gravity;
import android.view.SurfaceView;
import android.view.View;
import android.provider.Settings;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

import org.cocos2dx.cpp.AppActivity;

public class C2DXFirebaseBridge {
    
    private static final String TAG = "C2DXFirebaseBridge";
    private static AppActivity s_activity;
    
    private static AdView adView;
    private static InterstitialAd mInterstitialAd;

    private static final String AdMob_BannerAdUnitID = "ca-app-pub-6269735295695961/5486843334";
    private static final String AdMob_InterstitialAdUnitID = "ca-app-pub-6269735295695961/6963576530";

    private static native void admobInterstitialReady(boolean flag);
    private static native void admobInterstitialClosed();
    private static native void onAdmobInterstitialFailed();
    private static native void onAdmobInterstitialLeft();
    
    public static void initC2DXFirebaseBridge(AppActivity activity) {
        C2DXFirebaseBridge.s_activity = activity;
        
        MobileAds.initialize(s_activity.getApplicationContext(), "ca-app-pub-2888609101881425~6676712596");
        
        //
        // Banner Ad
        //
        adView = new AdView(s_activity);
        
        adView.setAdUnitId(AdMob_BannerAdUnitID);
        adView.setAdSize(AdSize.SMART_BANNER);
        
        AdRequest adRequest = new AdRequest.Builder()
//            .addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
//            .addTestDevice("00ADE0F1152C716E544531DA9BC7D1DA")
//            .addTestDevice("HASH_DEVICE_ID")
            .build();
        adView.loadAd(adRequest);
        adView.setBackgroundColor(Color.BLACK);
        adView.setBackgroundColor(0);
        adView.setEnabled(true);
        adView.setVisibility(View.VISIBLE);
        
        //
        // Interstitial Ad
        //
        mInterstitialAd = new InterstitialAd(s_activity);
        mInterstitialAd.setAdUnitId(AdMob_InterstitialAdUnitID);
        mInterstitialAd.setAdListener(new AdListener() {
            @Override
            public void onAdClosed() {
                onAdmobInterstitialClosed();
            }
            @Override
            public void onAdFailedToLoad(int var1) {
                onAdmobInterstitialFailed();
            }
            @Override
            public void onAdLeftApplication() {
                onAdmobInterstitialLeft();
            }
            @Override
            public void onAdOpened() {
            }
            @Override
            public void onAdLoaded() {
            }
        });
        requestNewInterstitial();
    }
    
    private static void onAdmobInterstitialClosed() {
        requestNewInterstitial();
        s_activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                C2DXFirebaseBridge.admobInterstitialClosed();
            }
        });
    }
    
    private static void requestNewInterstitial() {
        AdRequest adRequest = new AdRequest.Builder()
//            .addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
//            .addTestDevice("00ADE0F1152C716E544531DA9BC7D1DA")
            .build();
        
        mInterstitialAd.loadAd(adRequest);
    }
    
    public static void onResume() {
        if (adView != null) {
            adView.resume();
        }
    }
    public static void onPause() {
        if (adView != null) {
            adView.pause();
        }
    }
    public static void onDestroy() {
        if (adView != null) {
            adView.destroy();
        }
    }
    
    public static void initBanner()
    {
        s_activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            }
        });
    }
    
    public static void initInterstitial()
    {
    }
    
    public static void showBanner()
    {
        s_activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                adView.setVisibility(View.VISIBLE);
                
                //
                // NOTE: 최초에만...
                //
                if (s_activity.linearLayout  == null) {
                    s_activity.linearLayout = new LinearLayout( s_activity );
                    LinearLayout.LayoutParams adParams = new LinearLayout.LayoutParams(
                                                                                       LinearLayout.LayoutParams.WRAP_CONTENT,
                                                                                       LinearLayout.LayoutParams.WRAP_CONTENT);
                    adParams.gravity = Gravity.BOTTOM;
                    s_activity.linearLayout.addView( adView, adParams );
                    
                    //
                    s_activity.getResizeLayout().addView( s_activity.linearLayout );
                }
                
            }
        });
    }
    
    public static void hideBanner() {
        s_activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                adView.setVisibility(View.GONE);
            }
        });
    }
    public static void checkInterstitialReady()
    {
        s_activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (mInterstitialAd.isLoaded()) {
                    C2DXFirebaseBridge.admobInterstitialReady(true);
                }
                else {
                    C2DXFirebaseBridge.admobInterstitialReady(false);
                }
            }
        });
    }
    public static void showInterstitial()
    {
        s_activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (mInterstitialAd.isLoaded()) {
                    mInterstitialAd.show();
                }
                else {
                    onAdmobInterstitialClosed();
                }
            }
        });
    }
}
