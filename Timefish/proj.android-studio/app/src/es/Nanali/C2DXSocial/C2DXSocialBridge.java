package es.Nanali.C2DXSocial;

import java.io.UnsupportedEncodingException;
import java.util.HashMap;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.IOException;
import java.util.Calendar;
import java.util.UUID;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.preference.PreferenceManager;
import android.provider.Settings;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.net.Uri;
import android.content.*;

import com.zplay.timefishit.R;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.SystemClock;
import android.os.Environment;

public class C2DXSocialBridge {
    private static final String TAG = "C2DXSocial";
    private static Cocos2dxActivity s_activity;

    public static native void onCancelCallback();
    
    // the method must be called in main thread, before any other method
    public static void initC2DXSocialBridge(Cocos2dxActivity activity){
        C2DXSocialBridge.s_activity = activity;
    }

    public static void releaseC2DXSocialBridge() {
    }

    //
    // NOTE: 아래 링크 참고함
    // http://lhh3520.tistory.com/321
    //
    private final static String CACHE_DEVICE_ID = "CacheDeviceID";
    public static String getDeviceUUID() {
        UUID deviceUUID = null;
        SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(s_activity);
        String cachedDeviceID = sharedPreferences.getString(CACHE_DEVICE_ID, "");
        if (cachedDeviceID != "") {
            deviceUUID = UUID.fromString(cachedDeviceID);
        } else {
            final String androidUniqueID = Settings.Secure.getString(s_activity.getContentResolver(),
                    Settings.Secure.ANDROID_ID);
            try {
                if (androidUniqueID != "") {
                    deviceUUID = UUID.nameUUIDFromBytes(androidUniqueID.getBytes("utf8"));
                } else {
                    final String anotherUniqueID = ((TelephonyManager) s_activity.getSystemService(Context.TELEPHONY_SERVICE)).getDeviceId();
                    if (anotherUniqueID != null) {
                        deviceUUID = UUID.nameUUIDFromBytes(anotherUniqueID.getBytes("utf8"));
                    } else {
                        deviceUUID = UUID.randomUUID();
                    }
                }
            } catch (UnsupportedEncodingException e) {
                throw new RuntimeException(e);
            }
        }
        // save cur UUID.
        sharedPreferences.edit().putString(CACHE_DEVICE_ID, deviceUUID.toString()).apply();
        return deviceUUID.toString();
    }

    public static byte[] readFileToByte(File file) throws IOException {
        InputStream is = new FileInputStream(file);
        
        // Get the size of the file
        long length = file.length();
        
        // You cannot create an array using a long type.
        // It needs to be an int type.
        // Before converting to an int type, check
        // to ensure that file is not larger than Integer.MAX_VALUE.
        if (length > Integer.MAX_VALUE) {
            throw new IOException("Could not completely read file " + file.getName() + " as it is too long (" + length + " bytes, max supported " + Integer.MAX_VALUE + ")");
        }
        
        // Create the byte array to hold the data
        byte[] bytes = new byte[(int)length];
        
        // Read in the bytes
        int offset = 0;
        int numRead = 0;
        while (offset < bytes.length && (numRead=is.read(bytes, offset, bytes.length-offset)) >= 0) {
            offset += numRead;
        }
        
        // Ensure all the bytes have been read in
        if (offset < bytes.length) {
            throw new IOException("Could not completely read file " + file.getName());
        }
        
        // Close the input stream and return bytes
        is.close();
        return bytes;
    }

    //
    // http://hyoromo.hatenablog.com/entry/2014/09/05/214739
    //
    public static void openActionSheetWithMessage(String message, String path) {
        String _message = message + "-> goo.gl/3Dvmjs";
        Intent sharingIntent = new Intent(Intent.ACTION_SEND);
        
//        Log.i(TAG, "openActionSheetWithMessage: " + path);

        if (path != null) {
            byte[] data;
            try {
                data = readFileToByte(new File(path));
            } catch (Exception e) {
                return;
            }

//            Calendar c = Calendar.getInstance();
//            int seconds = c.get(Calendar.SECOND);
//            String fileName = "screenshot." + Integer.toString(seconds) + ".jpeg";
//            
//            Log.i(TAG, "fileName: " + fileName);

            
//            File savePath = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES);
            File savePath = C2DXSocialBridge.s_activity.getApplicationContext().getExternalFilesDir(Environment.DIRECTORY_PICTURES);
            File saveFile = new File(savePath, "screenshot.jpeg");

            if (!savePath.exists()) {
                savePath.mkdir();
            }
            
            if (saveFile.exists()) {
                saveFile.delete();
            }

            FileOutputStream fos = null;
            
            try {
                fos = new FileOutputStream(saveFile);
                fos.write(data);
                fos.close();
            } catch (Exception e) {
                return;
            }
            
            Uri uri = Uri.fromFile(saveFile);
            sharingIntent.setType("image/*");
            sharingIntent.putExtra(Intent.EXTRA_STREAM, uri);
            sharingIntent.putExtra(Intent.EXTRA_TEXT, _message);
        }
        else {
            sharingIntent.setType("text/plain");
            sharingIntent.putExtra(android.content.Intent.EXTRA_TEXT, _message);
        }
        C2DXSocialBridge.s_activity.startActivity(Intent.createChooser(sharingIntent, _message));
    }

    //
    //
    //
    public static float getCurrentTimeSinceLastBootInSec() {
        long milliSec = SystemClock.elapsedRealtime();
        return milliSec / 1000;
    }

    //
    //
    //
    public static void rateApp() {
        Context _context = C2DXSocialBridge.s_activity.getContext();
        Uri uri = Uri.parse("market://details?id=" + _context.getPackageName());
        Intent goToMarket = new Intent(Intent.ACTION_VIEW, uri);
        goToMarket.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY | Intent.FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET | Intent.FLAG_ACTIVITY_MULTIPLE_TASK);
        try {
            _context.startActivity(goToMarket);
        } catch (ActivityNotFoundException e) {
            _context.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("http://play.google.com/store/apps/details?id=" + _context.getPackageName())));
        }
    }
    
    public static void openFacebookPage() {
        Context _context = C2DXSocialBridge.s_activity.getContext();
        Uri uri = Uri.parse("fb://profile/628031447322532");
        Intent goToFacebook = new Intent(Intent.ACTION_VIEW, uri);
        goToFacebook.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY | Intent.FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET | Intent.FLAG_ACTIVITY_MULTIPLE_TASK);
        try {
            _context.startActivity(goToFacebook);
        } catch (ActivityNotFoundException e) {
            _context.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("https://www.facebook.com/freeshtimon")));
        }
    }
    
    public static void openHomepage() {
        Context _context = C2DXSocialBridge.s_activity.getContext();
        Uri uri = Uri.parse("fb://profile/271071806318884");
        Intent goToFacebook = new Intent(Intent.ACTION_VIEW, uri);
        goToFacebook.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY | Intent.FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET | Intent.FLAG_ACTIVITY_MULTIPLE_TASK);
        try {
            _context.startActivity(goToFacebook);
        } catch (ActivityNotFoundException e) {
            _context.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("https://www.facebook.com/NanaliStudios")));
        }
    }
    
    public static void openYoutubePage() {
        Context _context = C2DXSocialBridge.s_activity.getContext();
        Uri uri = Uri.parse("vnd.youtube:8z2Y0z0zXRU");
        Intent goToYoutube = new Intent(Intent.ACTION_VIEW, uri);
        goToYoutube.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY | Intent.FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET | Intent.FLAG_ACTIVITY_MULTIPLE_TASK);
        try {
            _context.startActivity(goToYoutube);
        } catch (ActivityNotFoundException e) {
            _context.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("https://youtu.be/8z2Y0z0zXRU")));
        }
    }
    
    public static void openArtworkPage() {
        Context _context = C2DXSocialBridge.s_activity.getContext();
        _context.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("https://www.behance.net/gallery/28576369/Timefish")));
    }
    
    public static void showExitPopup(){
        C2DXSocialBridge.s_activity.runOnUiThread(new Runnable() {
            public void run() {
                AlertDialog.Builder dialog = new AlertDialog.Builder(C2DXSocialBridge.s_activity);
                dialog.setTitle(R.string.exit_title); // "Exit Confirm"
                
                dialog.setMessage(R.string.exit_message); // "Are you sure you want to leave me behind?"
                dialog.setPositiveButton(R.string.exit_confirm, // "Come Back Later"
                                         new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface arg0, int arg1) {
                        //
                        // BYE,BYE!
                        //
                        C2DXSocialBridge.s_activity.finish();
                        System.exit(0);
                    }
                    
                });
                
                dialog.setNegativeButton(R.string.exit_cancel,//"Cancel",
                                         new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface arg0, int arg1) {
                        //
                        // Canceled? Thank you!!!
                        //
                        onCancelCallback();
                    }
                });
                
                dialog.setOnCancelListener(new DialogInterface.OnCancelListener() {
                    @Override
                    public void onCancel(DialogInterface dialog) {
                        //do whatever you want the back key to do
                        onCancelCallback();
                    }
                });
                
                dialog.show();
            }
        });
    }
    
    public static void showAlertDialog(final String title, final String msg, final String btn){
        C2DXSocialBridge.s_activity.runOnUiThread(new Runnable() {
            public void run() {
                AlertDialog.Builder dialog = new AlertDialog.Builder(C2DXSocialBridge.s_activity);
                dialog.setTitle(title);
                dialog.setMessage(msg);
                dialog.setNeutralButton(btn, null);
                
                dialog.show();
            }
        });
    }
}

