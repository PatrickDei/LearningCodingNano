package org.nanobit.mystory;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.graphics.BitmapFactory;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

import org.cocos2dx.lib.Cocos2dxActivity;

import java.util.Map;

import static org.nanobit.mystory.StoryGame.TAG;

public class FCMListenerReceiver extends FirebaseMessagingService {

    @Override
    public void onMessageReceived(RemoteMessage msg) {
        String from = msg.getFrom();
        Map data = msg.getData();
        String message = (String)data.get("message");
        String pushId = (String)data.get("pushid");
        Intent intent = new Intent(this, StoryGame.class);
        intent.putExtra("pushId", pushId != null ? pushId : "ERROR");
        intent.putExtra("pushMsg", message != null ? message : "ERROR");
        PendingIntent pendingIntent =
                PendingIntent.getActivity(this, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);

        NotificationManager notificationManager= (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
        NotificationCompat.Builder mBuilder = new NotificationCompat.Builder(this)
                .setSmallIcon(R.drawable.icon_statusbar_ms)
                .setLargeIcon(BitmapFactory.decodeResource(this.getResources(),
                        R.mipmap.ic_launcher))
                .setContentTitle("My Story")
                .setContentText(message)
                .setContentIntent(pendingIntent)
                .addAction(0, "Open", pendingIntent);
        if (notificationManager != null)
            notificationManager.notify(1, mBuilder.build());
    }

    @Override
    public void onNewToken(String newToken) {
        Log.d(TAG, "Refreshed token: " + newToken);
        final String tokenString =  newToken;
        final StoryGame myActivity = (StoryGame) Cocos2dxActivity.getContext();
        FCMHelper.storeRegistrationId(newToken);

        myActivity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                StoryGame.setPushRegistrationId(tokenString);
            }
        });
    }




}
