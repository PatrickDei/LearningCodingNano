package org.nanobit.mystory;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

import org.cocos2dx.lib.Cocos2dxActivity;

public class NetworkChangeReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(final Context context, final Intent intent) {
        ConnectivityManager cm = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        if(cm != null) {
            NetworkInfo networkInfo = cm.getActiveNetworkInfo();
            if (networkInfo != null && networkInfo.isConnected()) {
                setGameOffline(0);
            } else {
                setGameOffline(1);
            }
        }
    }

    public void setGameOffline(final int value) {
        final StoryGame myActivity = (StoryGame) Cocos2dxActivity.getContext();
        if(myActivity != null) {
            myActivity.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    StoryGame.setApplicatonOffline(value);
                }
            });
        }
    }
}
