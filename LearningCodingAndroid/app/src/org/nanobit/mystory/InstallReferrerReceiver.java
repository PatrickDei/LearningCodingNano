package org.nanobit.mystory;


import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import org.cocos2dx.lib.Cocos2dxHelper;

/**
 * Created by Neven Grubelic on 18/04/18.
 */

public class InstallReferrerReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent)
    {
//        Log.d("SHARE:", "SHARE: install referrer receiver invoke");
//        String rawReferrerString = intent.getStringExtra("referrer");
//        if(rawReferrerString != null && !rawReferrerString.contains("&") && !rawReferrerString.contains("=")) {
//            Log.d("SHARE:", "referrer string = " + rawReferrerString);
//            Cocos2dxHelper.setStringForKey("referralUsername", rawReferrerString);
//        } else {
//            Cocos2dxHelper.setStringForKey("referralUsername", "");
//        }
    }
}
