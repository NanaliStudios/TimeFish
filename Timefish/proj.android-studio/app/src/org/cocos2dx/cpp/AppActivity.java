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

import org.cocos2dx.lib.Cocos2dxActivity;

public class AppActivity extends Cocos2dxActivity {
    private static final String TAG = "C2DX";

    private static AppActivity sActivity;

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

        //
        C2DXSocialBridge.initC2DXSocialBridge(this);

        //
        sActivity = this;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        //Log.d(TAG, "onActivityResult(" + requestCode + "," + resultCode + "," + data);

        super.onActivityResult(requestCode, resultCode, data);
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
    }
    
    @Override
    protected void onPause() {
        super.onPause();
    }
    
    @Override
    protected void onResume() {
        super.onResume();
    }
    
    @Override
    public void onStop() {
        super.onStop();
    }

    @Override
    public void onDestroy() {
        C2DXSocialBridge.releaseC2DXSocialBridge();

        super.onDestroy();
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
    }
}
