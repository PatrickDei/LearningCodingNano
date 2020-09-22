package org.nanobit.mystory;

//import com.MyCompany.AwesomeGame.R;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationCompat.BigTextStyle;
import android.support.v4.app.NotificationCompat.Builder;
//import android.util.Log;

public class LocalNotificationReceiver extends BroadcastReceiver {
	
	public boolean isLollipop() 
	{
		//Log//Log.d("androidVersion ", String.valueOf(android.os.Build.VERSION.SDK_INT));
	
		if(android.os.Build.VERSION.SDK_INT>=21) {
			return true;
		}
		return false;
	} 

    @Override
    public void onReceive(Context context, Intent intent) {
        int notificationId = intent.getIntExtra("notification_id", 0);
        
        // fix requested by google for featuring on google play store
        //just one notification is shown
        //notificationId = 0;
        String message = intent.getStringExtra("message");
        String title = intent.getStringExtra("title");
        CharSequence action = intent.getStringExtra("action");

        Intent intent2 = new Intent(context, StoryGame.class);
        intent2.putExtra("pushMsg", message != null ? message : "ERROR");
        intent2.setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
        PendingIntent pendingIntent = PendingIntent.getActivity(context, notificationId , intent2,
                PendingIntent.FLAG_UPDATE_CURRENT);

        Bitmap largeIcon = BitmapFactory.decodeResource(context.getResources(),
                R.mipmap.ic_launcher);

        NotificationManager manager = (NotificationManager) context
                .getSystemService(Context.NOTIFICATION_SERVICE);

        String id = "ms_channel";
        if(android.os.Build.VERSION.SDK_INT>=26) {
            NotificationChannel mChannel = manager.getNotificationChannel(id);
            if (mChannel == null) {
                mChannel = new NotificationChannel(id, "msChannel", NotificationManager.IMPORTANCE_DEFAULT);
                manager.createNotificationChannel(mChannel);
            }
        }


        Builder builder = new NotificationCompat.Builder(context, id);
        if(!title.isEmpty()) {
            builder.setContentTitle(title);
        }
        builder.setContentText(message);
        if (isLollipop())
        {
            builder.setSmallIcon(R.drawable.icon_statusbar_ms);
        }
        else
        {
        	builder.setSmallIcon(R.mipmap.ic_launcher);
        }
        builder.setLargeIcon(largeIcon);
        builder.setTicker(message);
        builder.setAutoCancel(true);
        builder.setDefaults(Notification.DEFAULT_ALL);
        builder.setContentIntent(pendingIntent);
        builder.addAction(0, action, pendingIntent);
        
        BigTextStyle bigStyle = new BigTextStyle();
        bigStyle.bigText(message);
        builder.setStyle(bigStyle);
        builder.setGroup("grup_id");
        
        //Log//Log.v( Cocos2dxActivity.class.getSimpleName(),"Entered Receiver -> about to post notification :" + Integer.toString(notificationId));

        manager.notify(notificationId, builder.build());
    }

}
