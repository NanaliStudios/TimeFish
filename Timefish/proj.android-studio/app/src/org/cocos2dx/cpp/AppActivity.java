/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

//import org.cocos2dx.lib.Cocos2dxActivity;

import com.carlospinan.utils.UtilActivity;

import com.chartboost.sdk.Chartboost;
import com.flurry.android.FlurryAgent;
import com.tapjoy.Tapjoy;
import com.jirbo.adcolony.*;

import es.jesanchezgo.C2DXChartboost.C2DXChartboostBridge;
import es.Nanali.C2DXTapjoy.C2DXTapjoyBridge;
import es.Nanali.C2DXAdColony.C2DXAdColonyBridge;
import es.Nanali.C2DXFlurry.C2DXFlurryBridge;
import es.Nanali.C2DXSocial.C2DXSocialBridge;

//
import android.content.Context;
import java.util.Calendar;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.os.SystemClock;
import android.app.PendingIntent;
import android.app.AlarmManager;

import android.util.Log;
import android.app.Activity;

//
import android.os.Bundle;

import android.content.pm.PackageManager.NameNotFoundException;
import android.content.*;

//
//
//
import android.app.AlertDialog;
import com.android.util.IabHelper;
import com.android.util.IabResult;
import com.android.util.Inventory;
import com.android.util.Purchase;
import com.android.util.SkuDetails;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.json.JSONArray;
import org.json.JSONException;

//
// Import this code, to check Android Device ID
//
//import android.util.Log;
//import android.provider.Settings.Secure;


public class AppActivity extends UtilActivity {//Cocos2dxActivity {
    private static final String TAG = "C2DX";
    private static final String base64EncodedPublicKey3 = "x93HyZoQ1bSVpcO3Xb4HoX74Q6dDQCCHziATwyH6jKw2+7RxSly5l+So2eR1acvlM";
    private static final String base64EncodedPublicKey5 = "CmHrtwNo{6wYm89Mplbm8wFdw7{ner{+KM06Lp[t3J5vtosV[Skz9YdVMPNErwu7e"; // original code is BmGrswMoz6vYl88Molam7wEdv7zndrz+JM/6KpZt2J4vsorVZSjz8YcVLPMEqwt7d

    private static AppActivity sActivity;

    // (arbitrary) request code for the purchase flow
    static final int RC_REQUEST = 10101;
    private static native void onRequestProduct(String productId, String price, String currency, boolean purchased);
    private static native void onFinishPurchase(String transactionID, String productId, boolean restored, boolean success);
    private static native void onFinishRestore(boolean success);
//    private static native void onFinishRestoreAll(boolean success);

    //
    //
    // The helper object
    IabHelper mHelper;
    private String currSKU;
    // SKUs for our products
    private static JSONArray skuArray;

    //
    /*
    @Override
    public void sessionLoginError(FuseError error)
    {
        //Log.d(TAG, "sessionLoginError.");

    }
    @Override
    public void sessionStartReceived()
    {
        //Log.d(TAG, "sessionStartReceived.");
    }
    @Override
    public void didRecieveGCMRegistrationToken(String token) {
        
    }
    @Override
    public void timeUpdated(Date timestamp) {
        
    }
    @Override
    public void virtualGoodsOfferAcceptedWithObject (VGOfferInfo offer) {
        
    }
    public void IAPOfferAcceptedWithObject (IAPOfferInfo offer) {
        
    }
    public void rewardedAdCompleteWithObject (RewardedInfo rewardInfo) {
        
    }
    public void adDidShow(int a, int b) {
        
    }
    public void adFailedToDisplay() {
        
    }
    public void adWillClose () {
        
    }
    public void adAvailabilityResponse (boolean available, int error) {
        
    }
    public void purchaseVerification (int verified, String transaction_id, String originalTransactionID) {
        //Log.d(TAG, "purchaseVerification: " + transaction_id + "," + originalTransactionID);
//     1: The purchase is suspected to be valid.
//     0: The purchase is suspected to be invalid.
//     -1: We did not attempt verification. This can be due to a network error or lack of connectivity.
        if (verified == 1) {
            //
            // Success
            //
            //Log.d(TAG, "purchaseVerification: Success!");
        }
        else {
            //
            // Fail
            //
            //Log.d(TAG, "purchaseVerification: Fail!" + Integer.toString(verified));
        }
        
    }
    public void friendsListError(FuseError error) {
        
    }
    public void friendsListUpdated(ArrayList<Player> list) {
        
    }
    public void friendsMigrated(String a, FuseError error) {
        
    }
    public void friendRejected(String a, FuseError error) {
        
    }
    public void friendAccepted(String a, FuseError error) {
        
    }
    public void friendRemoved(String a, FuseError error) {
        
    }
    public void friendAdded(String a, FuseError error) {
        
    }
    public void notificationWillClose() {
        
    }
    public void notificationAction (String action) {
        
    }
    public void accountLoginError(String a, FuseError error) {
        
    }
    public void accountLoginComplete(int type, String accountId) {
        
    }
    public void gameConfigurationReceived () {
        
    }
     */

    //
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // version
        String versionName = "1.0";
        try {
            Context context = getApplicationContext(); // or activity.getApplicationContext()
            versionName = context.getPackageManager().getPackageInfo(context.getPackageName(), 0).versionName;
        } catch (NameNotFoundException e) {
            // TODO Auto-generated catch block
        }

        // Init Chartboost
        C2DXChartboostBridge.initC2DXChartboostBridge(this);
        C2DXChartboostBridge.startWithAppId("54d2016b43150f18526612a9","c66728d9f730f2c90ca0db8f5c3acc3125d992db");

        // Init Tapjoy
        C2DXTapjoyBridge.initC2DXTapjoyBridge(this);
        C2DXTapjoyBridge.startWithSDKKey(getApplicationContext(), "pOKf3eTQQeuJZOoQpIhd4wEC2JIm0K3yxYyrcVlALWavXQ7qMHo4yQxQvLgj");
        
        // Init AdColony
        C2DXAdColonyBridge.initC2DXAdColonyBridge(this);
        C2DXAdColonyBridge.configureWith("appd3fd7205779a4872a1", "vz080cafe7cd854f0eac", versionName);

        // init Flurry
        C2DXFlurryBridge.initC2DXFlurryBridge(this);
        C2DXFlurryBridge.startWithAppVersion(versionName);

        //
        C2DXSocialBridge.initC2DXSocialBridge(this);

        sActivity = this;
        
        //
//        FuseSDK.startSession("70cc5de9-e0c8-4e10-a490-bc680ad2167b", this, this, null);

        /* base64EncodedPublicKey should be YOUR APPLICATION'S PUBLIC KEY
         * (that you got from the Google Play developer console). This is not your
         * developer public key, it's the *app-specific* public key.
         *
         * Instead of just storing the entire literal string here embedded in the
         * program,  construct the key at runtime from pieces or
         * use bit manipulation (for example, XOR with some other string) to hide
         * the actual key.  The key itself is not secret information, but we don't
         * want to make it easy for an attacker to replace the public key with one
         * of their own and then fake messages from the server.
         */
//        String base64EncodedPublicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAoiC2nfpfvQC1UKUCiwwjFKT0l8Sd+GGOFXvlN4NHhiEEX+vFpZY5jYKIAMldIS39Ar4kqD+h2XlgEzLC1QFFWhx93HyZoQ1bSVpcO3Xb4HoX74Q6dDQCCHziATwyH6jKw2+7RxSly5l+So2eR1acvlMGPvakx0zS6SQ2K58ReA/q5F2bWdwcxxdr037f+n28vMLxmOyHE54xdJEJRMlZ1tSjBmGrswMoz6vYl88Molam7wEdv7zndrz+JM/6KpZt2J4vsorVZSjz8YcVLPMEqwt7dbAXXyICniVlIgxagPIuOEBV/Af5diRKQfNWAFsY5NaomIwUeIQEeJgjbjRfpQIDAQAB";
        String base64EncodedPublicKey6 = "bAXXyICniVlIgxagPIuOEBV/Af5diRKQfNWAFsY5NaomIwUeIQEeJgjbjRfpQIDAQ";
        String base64EncodedPublicKey4 = "GPvakx0zS6SQ2K58ReA/q5F2bWdwcxxdr037f+n28vMLxmOyHE54xdJEJRMlZ1tSj";

        //
        StringBuilder sb = new StringBuilder();
        int salt = 0;
        for(int i=0; i<base64EncodedPublicKey5.length() ;i++){
            salt = 1 - salt;
            sb.append((char)(base64EncodedPublicKey5.charAt(i) - salt));
        }

        // Create the helper, passing it our context and the public key to verify signatures with
        mHelper = new IabHelper(this, base64EncodedPublicKey3+
                                        base64EncodedPublicKey4+
                                        sb.toString()+
                                        base64EncodedPublicKey6);

        // enable debug logging (for a production application, you should set this to false).
        mHelper.enableDebugLogging(false);
        
        // Start setup. This is asynchronous and the specified listener
        // will be called once setup completes.
        //Log.d(TAG, "Starting setup.");
        mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
            public void onIabSetupFinished(IabResult result) {
                //Log.d(TAG, "Setup finished.");
                
                if (!result.isSuccess()) {
                    // Oh noes, there was a problem.
//                    complain("Problem setting up in-app billing: " + result);
                    return;
                }
                
                // Have we been disposed of in the meantime? If so, quit.
                if (mHelper == null) return;
                
                // IAB is fully set up. Now, let's get an inventory of stuff we own.
                //Log.d(TAG, "Setup successful. Querying inventory.");
                
                
                ArrayList<String> skuList = new ArrayList<String> ();
                skuList.add("timefish.etc_01");
                skuList.add("timefish.etc_02");
                skuList.add("timefish.etc_03");
                skuList.add("timefish.etc_04");
                skuList.add("timefish.etc_05");
                skuList.add("timefish.etc_06");
                skuList.add("timefish.etc_07");
                skuList.add("timefish.job_01");
                skuList.add("timefish.job_02");
                skuList.add("timefish.job_03");
                skuList.add("timefish.job_04");
                skuList.add("timefish.job_05");
                skuList.add("timefish.job_06");
                skuList.add("timefish.job_07");
                skuList.add("timefish.job_08");
                skuList.add("timefish.job_09");
                skuList.add("timefish.job_10");
                skuList.add("timefish.parody_01");
                skuList.add("timefish.parody_02");
                skuList.add("timefish.parody_03");
                skuList.add("timefish.parody_05");
                skuList.add("timefish.parody_07");
                skuList.add("timefish.parody_08");
                skuList.add("timefish.retro_01");
                skuList.add("timefish.retro_02");
                skuList.add("timefish.retro_04");
                skuList.add("timefish.retro_05");
                skuList.add("timefish.retro_06");
                skuList.add("timefish.retro_07");
                skuList.add("timefish.retro_08");
                skuList.add("timefish.retro_09");
                //
                skuList.add("timefish.retro_10");
                skuList.add("timefish.history_02");
                skuList.add("timefish.history_03");
                skuList.add("timefish.history_04");
                skuList.add("timefish.history_05");
                skuList.add("timefish.history_06");
                skuList.add("timefish.history_07");
                skuList.add("timefish.history_08");
                skuList.add("timefish.history_09");
                skuList.add("timefish.history_10");
                skuList.add("timefish.goldfish");
                skuList.add("timefish.package01");
                skuList.add("timefish.package02");
                skuList.add("timefish.package03");
                skuList.add("timefish.package04");
                skuList.add("timefish.package05");
                skuList.add("timefish.package06");
                skuList.add("timefish.package07");
                skuList.add("timefish.package08");
                skuList.add("timefish.package09");
                skuList.add("timefish.package10");
                skuList.add("timefish.piggybank");
                //
                skuList.add("timefish.snapback_01");
                skuList.add("timefish.snapback_02");
                skuList.add("timefish.snapback_03");
                skuList.add("timefish.snapback_04");
                skuList.add("timefish.snapback_05");
                skuList.add("timefish.snapback_06");
                skuList.add("timefish.snapback_07");
                skuList.add("timefish.snapback_08");
                skuList.add("timefish.snapback_09");
                skuList.add("timefish.snapback_10");
                skuList.add("timefish.special_02");
                skuList.add("timefish.remove_ads");
                skuArray = new JSONArray(skuList);

                mHelper.queryInventoryAsync(true, skuList, mGotInventoryListener);
            }
        });

        //
        // Import this code, to check Android Device ID
        //
//        String android_id = Secure.getString(this.getContext().getContentResolver(), Secure.ANDROID_ID);
//        //Log.e("C2DXChartBoost", versionName);
    }
    
    
    // Listener that's called when we finish querying the items and subscriptions we own
    IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
            public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
                //Log.d(TAG, "Query inventory finished.");
                
                // Have we been disposed of in the meantime? If so, quit.
                if (mHelper == null) return;
                
                // Is it a failure?
                if (result.isFailure()) {
//                    complain("Failed to query inventory: " + result);
                    return;
                }
                
                //Log.d(TAG, "Query inventory was successful.");
                
                /*
                 * Check for items we own. Notice that for each purchase, we check
                 * the developer payload to see if it's correct! See
                 * verifyDeveloperPayload().
                 */
    
                //Log.d(TAG, "skuArray:" + Integer.toString(skuArray.length()));
                for(int i = 0; i < skuArray.length(); i++){
                    try {
                        String SKU = skuArray.getString(i);
                        
                        SkuDetails skuDetail = inventory.getSkuDetails(SKU);
                        if(skuDetail != null){
                            
                            //
                            // Check Alread purchased
                            //
                            boolean already = false;
                            if (inventory.hasPurchase(SKU)) {
                                Purchase purchase = inventory.getPurchase(SKU);
                                if (purchase != null) {
                                    //
                                    // getPurchaseState()
                                    // 0 is for purchased history
                                    // 1 is for canceled history
                                    // 2 is for refunded history
                                    //
                                    if (purchase.getPurchaseState() == 0) {
                                        already = true;
                                    }
                                    
                                }
                            }

                            //Log.v(TAG, "id: " + skuDetail.getSku());
                            //Log.v(TAG, "price: " + skuDetail.getPrice());
                            //Log.v(TAG, "purchased: " + Boolean.toString(already));

                            onRequestProduct(skuDetail.getSku(), skuDetail.getPrice(), skuDetail.getCurrency(), already);
                        }else{
                            //Log.v(TAG, "SKU RETURNED NULL: " + SKU);
                        }
                    } catch (JSONException e) {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                    }
                }

                // Do we have the premium upgrade?
//                Purchase premiumPurchase = inventory.getPurchase(SKU_PREMIUM);
//                mIsPremium = (premiumPurchase != null && verifyDeveloperPayload(premiumPurchase));
//                //Log.d(TAG, "User is " + (mIsPremium ? "PREMIUM" : "NOT PREMIUM"));
//                
//                // Do we have the infinite gas plan?
//                Purchase infiniteGasPurchase = inventory.getPurchase(SKU_INFINITE_GAS);
//                mSubscribedToInfiniteGas = (infiniteGasPurchase != null &&
//                                            verifyDeveloperPayload(infiniteGasPurchase));
//                //Log.d(TAG, "User " + (mSubscribedToInfiniteGas ? "HAS" : "DOES NOT HAVE")
//                      + " infinite gas subscription.");
//                if (mSubscribedToInfiniteGas) mTank = TANK_MAX;
//                
//                // Check for gas delivery -- if we own gas, we should fill up the tank immediately
//                Purchase gasPurchase = inventory.getPurchase(SKU_GAS);
//                if (gasPurchase != null && verifyDeveloperPayload(gasPurchase)) {
//                    //Log.d(TAG, "We have gas. Consuming it.");
//                    mHelper.consumeAsync(inventory.getPurchase(SKU_GAS), mConsumeFinishedListener);
//                    return;
//                }
    
//                updateUi();
//                setWaitScreen(false);
                //Log.d(TAG, "Initial inventory query finished; enabling main UI.");
        }
    };

    // Listener that's called when we finish querying the items and subscriptions we own
    IabHelper.QueryInventoryFinishedListener mGotPurchaseListener = new IabHelper.QueryInventoryFinishedListener() {
        public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
            //Log.d(TAG, "Query inventory finished.");

            // Have we been disposed of in the meantime? If so, quit.
            if (mHelper == null) return;

            // Is it a failure?
            if (result.isFailure()) {
                return;
            }

            //Log.d(TAG, "Query inventory was successful.");

            /*
             * Check for items we own. Notice that for each purchase, we check
             * the developer payload to see if it's correct! See
             * verifyDeveloperPayload().
             */

//            try {
                SkuDetails skuDetail = inventory.getSkuDetails(currSKU);
                if(skuDetail != null){
                    //
                    // Check Alread purchased
                    //
                    boolean already = false;
                    if (inventory.hasPurchase(currSKU)) {
                        Purchase purchase = inventory.getPurchase(currSKU);
                        if (purchase != null) {
                            //
                            // getPurchaseState()
                            // 0 is for purchased history
                            // 1 is for canceled history
                            // 2 is for refunded history
                            //
                            if (purchase.getPurchaseState() == 0) {
                                already = true;

                                //
                                // Tracks a purchase with JSON data from the Google Play store
                                //
                                /*
                                 skuDetails – a String in JSON Object format that contains product item details (according to the Specification on Google Play)
                                 purchaseData – a String in JSON format that contains details about the purchase order. It is returned by the operating system after you make the purchase call. Use null to not use validation.
                                 dataSignature – String containing the signature of the purchase data that the developer signed with their private key. It is returned by the operating system after you make the purchase call. Use null to not use validation.
                                 campaignId – the campaign id of the PurchaseRequest if it initiated this purchase. This can be null if you don’t wish to track which campaign initiated this purchase.
                                 */
                                //Log.v(TAG, "id: " + skuDetail.getSku());
                                //Log.v(TAG, "price: " + skuDetail.getPrice());
                                //Log.v(TAG, "purchased: " + Boolean.toString(already));
                                //Log.v(TAG, "json: " + skuDetail.getOriginalJSON());

                                Tapjoy.trackPurchase(skuDetail.getOriginalJSON(), purchase.getOriginalJson(), purchase.getSignature(), "0");

                                //
                                onFinishPurchase(purchase.getOrderId(), purchase.getSku(), false, true);
                            }
                            else {
                                onFinishPurchase(purchase.getOrderId(), purchase.getSku(), false, false);
                            }
                        }
                        else {
                            onFinishPurchase(null, null, true, true);
                        }
                    }
                    else {
                        onFinishPurchase(null, null, true, true);
                    }
                } else {
                    //Log.v(TAG, "SKU RETURNED NULL: " + currSKU);
                    onFinishPurchase(null, null, true, true);
                }
//            } catch (JSONException e) {
//                // TODO Auto-generated catch block
//                e.printStackTrace();
//            }

            //Log.d(TAG, "Initial inventory query finished; enabling main UI.");
        }
    };


//    // Called when consumption is complete
//    IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
//        public void onConsumeFinished(Purchase purchase, IabResult result) {
//            //Log.d(TAG, "Consumption finished. Purchase: " + purchase + ", result: " + result);
//
//            // if we were disposed of in the meantime, quit.
//            if (mHelper == null) return;
//
//            // We know this is the "gas" sku because it's the only one we consume,
//            // so we don't check which sku was consumed. If you have more than one
//            // sku, you probably should check...
//            if (result.isSuccess()) {
//                // successfully consumed, so we apply the effects of the item in our
//                // game world's logic, which in our case means filling the gas tank a bit
//                //Log.d(TAG, "Consumption successful. Provisioning.");
//    //            mTank = mTank == TANK_MAX ? TANK_MAX : mTank + 1;
//    //            saveData();
////                alert("You filled 1/4 tank. Your tank is now " + String.valueOf(mTank) + "/4 full!");
//            }
//            else {
//                complain("Error while consuming: " + result);
//            }
////            updateUi();
////            setWaitScreen(false);
//            //Log.d(TAG, "End consumption flow.");
//        }
//    };

    /** Verifies the developer payload of a purchase. */
    boolean verifyDeveloperPayload(Purchase p) {
        String payload = p.getDeveloperPayload();

        /*
         * TODO: verify that the developer payload of the purchase is correct. It will be
         * the same one that you sent when initiating the purchase.
         *
         * WARNING: Locally generating a random string when starting a purchase and
         * verifying it here might seem like a good approach, but this will fail in the
         * case where the user purchases an item on one device and then uses your app on
         * a different device, because on the other device you will not have access to the
         * random string you originally generated.
         *
         * So a good developer payload has these characteristics:
         *
         * 1. If two different users purchase an item, the payload is different between them,
         *    so that one user's purchase can't be replayed to another user.
         *
         * 2. The payload must be such that you can verify it even when the app wasn't the
         *    one who initiated the purchase flow (so that items purchased by the user on
         *    one device work on other devices owned by the user).
         *
         * Using your own server to store and verify developer payloads across app
         * installations is recommended.
         */

        return true;
    }

    void complain(String message) {
        //Log.e(TAG, "**** TrivialDrive Error: " + message);
    }

    public static void showRestoreAllPopup(String title, String msg, String btn) {
            //
            C2DXSocialBridge.showAlertDialog(title, msg, btn);
    }

    public static void beginPurchaseFreeshSkin(String skinName) {
//        //Log.d(TAG, "beginPurchaseFreeshSkin.");
        sActivity.beginPurchaseWithProductName(skinName.toLowerCase());
    }
    public static void beginPurchasePackage(String packageName) {
//        //Log.d(TAG, "beginPurchaseFreeshSkin.");
        sActivity.beginPurchaseWithProductName(packageName.toLowerCase());
    }
    public static void beginPurchaseRemoveAds(String productName) {
//        //Log.d(TAG, "beginPurchaseRemoveAds.");
        sActivity.beginPurchaseWithProductName(productName.toLowerCase());
    }
//    public static void restorePurchasesItems() {
//        //Log.d(TAG, "restorePurchasesItems.");
//    }

    public void beginPurchaseWithProductName(String skinName) {
        /* TODO: for security, generate your payload here for verification. See the comments on
         *        verifyDeveloperPayload() for more info. Since this is a SAMPLE, we just use
         *        an empty string, but on a production app you should carefully generate this. */
        String payload = "";

        String SKU = "timefish." + skinName;

//        Log.d(TAG, "Launching purchase flow." + SKU);

        mHelper.launchPurchaseFlow(this,
            SKU, IabHelper.ITEM_TYPE_INAPP,
            RC_REQUEST, mPurchaseFinishedListener, payload);
    }

    // Callback for when a purchase is finished
    IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
//            Log.d(TAG, "Purchase finished: " + result + ", purchase: " + purchase);

            // if we were disposed of in the meantime, quit.
            if (mHelper == null) {
                onFinishPurchase(null, null, false, false);
                return;
            }

            if (result.isFailure()) {
                //
                // Already Purchased...set restored
                //
                if (result.getResponse() == IabHelper.BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED) {
                    onFinishPurchase(null, null, true, true);
                }
                else {
                    onFinishPurchase(null, null, false, false);
                }
//                complain("Error purchasing: " + result);
                return;
            }

//            VerifiedPurchase verifiedPurchase = new VerifiedPurchase(Integer.toString(purchase.getPurchaseState()),
//                purchase.getToken(), purchase.getSku(), purchase.getOrderId(), purchase.getPurchaseTime(), purchase.getDeveloperPayload());
//            FuseSDK.registerInAppPurchase(verifiedPurchase);

            //
            if (!verifyDeveloperPayload(purchase)) {
                onFinishPurchase(null, null, false, false);
//                complain("Error purchasing. Authenticity verification failed.");
                return;
            }

//            Log.d(TAG, "Purchase successful: " + purchase.getSku());

            currSKU = purchase.getSku();
            ArrayList<String> skuList = new ArrayList<String> ();
            skuList.add(purchase.getSku());
            mHelper.queryInventoryAsync(true, skuList, mGotPurchaseListener);

        }
    };

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        //Log.d(TAG, "onActivityResult(" + requestCode + "," + resultCode + "," + data);
        if (mHelper == null) return;

        // Pass on the activity result to the helper for handling
        if (!mHelper.handleActivityResult(requestCode, resultCode, data)) {
            // not handled, so handle it ourselves (here's where you'd
            // perform any handling of activity results not related to in-app
            // billing...
            super.onActivityResult(requestCode, resultCode, data);
        }
        else {
            //Log.d(TAG, "onActivityResult handled by IABUtil.");
        }
    }


    //
    public static void registerLocalNotification(String message, int seconds, int tag) {
        //Log.v(TAG, "registerLocalNotification:" + Integer.toString(tag));
        PendingIntent sender = getPendingIntent(message, tag);

        Calendar calendar = Calendar.getInstance();
        calendar.setTimeInMillis(System.currentTimeMillis());
        calendar.add(Calendar.SECOND, seconds);

        AlarmManager am = (AlarmManager)sActivity.getSystemService(ALARM_SERVICE);
        am.set(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), sender);
    }
    
    public static void cancelAllNotifications() {
        //Log.v(TAG, "cancelAllNotifications");
        //
        // 8 notifications
        //
        for (int i=0; i<8; i++) {
            int tag = i + 1;
            
            Intent it = new Intent(sActivity.getApplicationContext(), LocalNotificationReceiver.class);
            
            PendingIntent sender = PendingIntent.getBroadcast(sActivity, tag, it, PendingIntent.FLAG_NO_CREATE);
            
            if (sender != null) {
                AlarmManager am = (AlarmManager)sActivity.getSystemService(ALARM_SERVICE);
                am.cancel(sender);
                sender.cancel();
            }

        }
    }
    
    private static PendingIntent getPendingIntent(String message, int tag) {
        Intent i = new Intent(sActivity.getApplicationContext(), LocalNotificationReceiver.class);
        i.putExtra("notification_id", tag);
        i.putExtra("message", message);

        PendingIntent sender = PendingIntent.getBroadcast(sActivity, tag, i, PendingIntent.FLAG_UPDATE_CURRENT);
        return sender;
    }
    
    //

    @Override
    public void onStart() {
        super.onStart();
        Chartboost.onStart(this);
        Tapjoy.onActivityStart(this);

        //
        FlurryAgent.onStartSession(this.getApplicationContext(), "Z6YWGZRRZ54RJSR4DXJT");
    }
    
    @Override
    protected void onPause() {
        super.onPause();
        Chartboost.onPause(this);
        AdColony.pause();
//        FuseSDK.pauseSession();
    }
    
    @Override
    protected void onResume() {
        Chartboost.onResume(this);
        AdColony.resume( this );
//        FuseSDK.resumeSession(this);
        super.onResume();
    }
    
    @Override
    public void onStop() {
        Chartboost.onStop(this);
        Tapjoy.onActivityStop(this);

        super.onStop();

        //
        FlurryAgent.onEndSession(this.getApplicationContext());
    }
    
    @Override
    public void onDestroy() {
        C2DXSocialBridge.releaseC2DXSocialBridge();

        super.onDestroy();
        Chartboost.onDestroy(this);
//        FuseSDK.endSession();
    }

    @Override
    public void onBackPressed() {
        // If an interstitial is on screen, close it.
        if (Chartboost.onBackPressed())
            return;
        else
            super.onBackPressed();
    }
}
