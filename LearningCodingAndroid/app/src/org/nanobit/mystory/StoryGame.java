package org.nanobit.mystory;

import android.app.Activity;
import android.app.AlarmManager;
import android.app.AlertDialog;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.util.Log;
import android.view.ContextThemeWrapper;
import android.view.WindowManager;

import com.adjust.sdk.Adjust;
import com.adjust.sdk.AdjustAttribution;
import com.adjust.sdk.AdjustConfig;
import com.adjust.sdk.LogLevel;
import com.adjust.sdk.OnAttributionChangedListener;
import com.crashlytics.android.Crashlytics;
import io.fabric.sdk.android.Fabric;
import com.crashlytics.android.ndk.CrashlyticsNdk;
import com.facebook.CallbackManager;
import com.fyber.ads.videos.RewardedVideoActivity;
import com.fyber.annotations.FyberSDK;
import com.helpshift.HelpshiftBridge;
import com.ironsource.mediationsdk.IronSource;
import com.soomla.traceback.SoomlaConfig;
import com.soomla.traceback.SoomlaTraceback;
import com.chartboost.sdk.Chartboost;
import com.chartboost.sdk.CBLocation;
import com.chartboost.sdk.ChartboostDelegate;

import org.apache.commons.lang3.StringEscapeUtils;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.lib.Cocos2dxHelper;
import org.nanobit.mystory.cocosextension.PortraitGLSurfaceView;
import org.nanobit.mystory.purchase.InappPurchaseManager;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Formatter;
import java.util.List;
import java.util.Locale;

import static org.nanobit.mystory.FyberController.FYBER_INTERSTITIAL_REQUEST_CODE;
import static org.nanobit.mystory.FyberController.interstitialWatched;


@FyberSDK
public class StoryGame extends Cocos2dxActivity {

    public static boolean sActivityRunning = false;

    private static Handler sHandler;
    private static Activity sActivity;
    private static NAGoogleGamesCommunicator sGamesCommunicator;
    private static FyberController sFyberController;
    private static final ArrayList<NotificationData> sNotifications = new ArrayList<>();

    private static IronSourceController sIronSourceController;
    private static BroadcastReceiver networkChangeReceiver;

    protected static final int REWARDED_VIDEO_REQUEST_CODE = 1233;
    private static final String HELPSHIFT_APPKEY = "40d2405348bddfe0cab40fa5f4bbc730";
    private static final String HELPSHIFT_DOMAIN_NAME_STRING = "nanobit.helpshift.com";
    private static final String HELPSHIFT_APP_ID = "nanobit_platform_20170216093718629-e5a36f3eeb8f023";
    private static final String SHARE_PROTOCOL = "https";
    private static final String SHARE_URL = "ms.nanobitgames.com";

    public static final String TAG = Cocos2dxActivity.class.getSimpleName();

    static AlertDialog.Builder alertbox;
    static AlertDialog alertDialog;

    String soomlaAppKey = "d52f38e2-034c-4293-a36b-63cad9e8ff61";

    public static CallbackManager callbackManager;

    private static String sGCMRegId;
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("cocos2dcpp");
    }

    private void hideVirtualButtons() {
        try {
            int uiOptions = Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LOW_PROFILE;

            if (Build.VERSION.SDK_INT >= 19) {
                uiOptions = Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_FULLSCREEN
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
            }

            mGLSurfaceView.setSystemUiVisibility(uiOptions);
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        PackageInfo pInfo;
        int version = 1;
        try {
            pInfo = getPackageManager().getPackageInfo(getPackageName(), 0);
            version = pInfo.versionCode;
        } catch (Exception e) {
            e.printStackTrace();
        }

        hideVirtualButtons();

        // dont change order:
        Cocos2dxHelper.setCodeVersion(version);
        Cocos2dxHelper.setObbPath(Environment.getExternalStorageDirectory()
                + "/Android/obb/"
                + getPackageName()
                + "/"
                + getExpansionAPKFileName(this, true,
                Integer.toString(version)));
        Cocos2dxHelper.init(this, this);

        sHandler = new Handler();
        sActivity = this;

        OpenUDID_manager.sync();

        sFyberController = new FyberController(this);

        sIronSourceController = new IronSourceController(this);

        //Crashlyitcs
        Fabric.with(this, new Crashlytics(), new CrashlyticsNdk());

        String NanoIdentifier = OpenUDID_manager.getOpenUDID();
        Crashlytics.setUserIdentifier(NanoIdentifier);
        Crashlytics.setString("NanoIdentifier", NanoIdentifier);

        //Adjust
        String appToken = null;
        if (BuildConfig.BUILD_TYPE.equals("flexion")) {
            appToken = "6c7wfgqaxszk";
        }
        else {
            appToken = "4cm35j401o5c";
        }
        String environment = BuildConfig.DEBUG ? AdjustConfig.ENVIRONMENT_SANDBOX : AdjustConfig.ENVIRONMENT_PRODUCTION;
        AdjustConfig config = new AdjustConfig(this, appToken, environment);
        config.setLogLevel(LogLevel.VERBOSE);
        config.setOnAttributionChangedListener(new OnAttributionChangedListener() {
            @Override
            public void onAttributionChanged(AdjustAttribution attribution) {
            }
        });

        Adjust.onCreate(config);

        String NanoUsername = Cocos2dxHelper.getStringForKey("NanobitID", "unknown");
        Crashlytics.setUserName(NanoUsername);
        Crashlytics.setString("NanoUsername", NanoUsername);


        InappPurchaseManager.setupInappPurchaseManager(this, "SG",
                OpenUDID_manager.getOpenUDID(), BuildConfig.DEBUG, TAG);

        onCreateCalled();

        getBundleVersion();
        //Keep screen on while the game is downloading

        sGamesCommunicator = new NAGoogleGamesCommunicator();
        sGamesCommunicator.setResolvingError(savedInstanceState != null && savedInstanceState.getBoolean("resolving_error", false));
        sGamesCommunicator.loginPlayer();

        try {
            // Helpshift
            HelpshiftBridge.install(this,
                    HELPSHIFT_APPKEY,
                    HELPSHIFT_DOMAIN_NAME_STRING,
                    HELPSHIFT_APP_ID);
        }
        catch (Exception e){
            e.printStackTrace();
        }

        //Chartboost
        ChartboostController.get().initialize(this);

        // GooglePlay Services
        FCMHelper.checkIfGooglePlayServicesAvailable(this);

        callbackManager = CallbackManager.Factory.create();

        // Soomla
        SoomlaConfig soomlaConfig = new SoomlaConfig.Builder()
                .setTestMode(BuildConfig.DEBUG)
                .setUserId(OpenUDID_manager.getOpenUDID())
                .build();
        SoomlaTraceback.getInstance().initialize(this, soomlaAppKey, soomlaConfig);

        networkChangeReceiver = new NetworkChangeReceiver();
    }

    public Cocos2dxGLSurfaceView onCreateView() {
        Cocos2dxGLSurfaceView glSurfaceView = new PortraitGLSurfaceView(this);

        glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 8);
        glSurfaceView.setMultipleTouchEnabled(false);

        return glSurfaceView;
    }

    /* OVERRIDES */
    @Override
    protected void onStart() {
        // Log.v("Cocos2dxActivity", "on start");
        super.onStart();
        sActivityRunning = true;

        //Chartboost
        Chartboost.onStart(this);

        String NanoUsername = Cocos2dxHelper.getStringForKey("NanobitID", "unknown");
        Crashlytics.setUserName(NanoUsername);
        Crashlytics.setString("NanoUsername", NanoUsername);
    }

    @Override
    protected void onResume() {

        super.onResume();

        //Adjust
        Adjust.onResume();

        //iron source
        IronSource.onResume(this);

        //Chartboost
        Chartboost.onResume(this);

        checkForReferralInIntent();
        checkForPushIdInIntent();

        boolean isGooglePlayConnected= sGamesCommunicator.getIsConnected();
        FCMHelper.checkIfGooglePlayServicesAvailable(this);

//		Log.v("Cocos2dxActivity", "on resume DID_CANCEL_LOGIN " + !isGooglePlayConnected);
        final SharedPreferences sharedPreferences =  StoryGame.getContext().getSharedPreferences(StoryGame.class.getSimpleName(), Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putBoolean(NAGoogleGamesCommunicator.DID_CANCEL_LOGIN, !isGooglePlayConnected);
        editor.apply();

        sIronSourceController.checkForUncollectedOfferWallRewards();

        runOnGLThread(new Runnable() {
            @Override
            public void run() {
                Log.d(TAG, "blockGLResume in resume");
                blockGLResume(false);
            }
        });

        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
        registerReceiver(networkChangeReceiver, intentFilter);

        hideVirtualButtons();

        Log.d(TAG, "onresume");
    }

    @Override
    protected void onRestart() {
        super.onRestart();
    }

    @Override
    protected void onPause() {
        super.onPause();

        //Adjust
        Adjust.onPause();

        //iron source
        IronSource.onPause(this);

        for (NotificationData data : sNotifications) {
            showLocalNotification(data.message, data.title, data.interval, data.tag,
                    data.action);
        }
        sNotifications.clear();

        // Chartboost
        Chartboost.onPause(this);

        //background tasks
        final long startTime = System.currentTimeMillis();
        runOnGLThread(new Runnable() {
            @Override
            public void run() {
                //if it takes more than 5 sec end it
                long elapsedTime = System.currentTimeMillis() - startTime;
                while(backgroundUpdateFunctions() != 0 && elapsedTime < 4000){
                    elapsedTime = System.currentTimeMillis() - startTime;
                }
            }
        });

        unregisterReceiver(networkChangeReceiver);
    }

    @Override
    protected void onStop() {
        sActivityRunning = false;

        super.onStop();

        // Chartboost
        Chartboost.onStop(this);
    }

    @Override
    protected void onDestroy() {

        super.onDestroy();
        InappPurchaseManager.cleanup();
        Chartboost.onDestroy(this);
    }

    @Override
    public void onBackPressed() {
        if (Chartboost.onBackPressed())
            return;
        else
            super.onBackPressed();
    }


    @Override
    public void onLowMemory() {
        super.onLowMemory();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {

        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            hideVirtualButtons();
        }
    }

    @Override
    public void onActivityResult (int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == 1002) {
            // Log.v("IAP", "Code 1002");
            sGamesCommunicator.onActivityResult(resultCode, data);
            return;
        }


        if( InappPurchaseManager.handleActivityResult(requestCode, resultCode, data) ){
            // Log.v("IAP", "InAppPurchase");
            return;
        } else if (requestCode == 1001) {
            // Log.v("IAP", "Code 1001");
            // Log.v("IAP", "Request Code first: " + requestCode);
            // Log.v("IAP", "Result code first: " + resultCode);
            sGamesCommunicator.resolutionResult(requestCode, resultCode, data);
        } else {
            if(callbackManager != null) {
                callbackManager.onActivityResult(requestCode, resultCode, data);
            }
        }

        if (requestCode == FYBER_INTERSTITIAL_REQUEST_CODE) {
            this.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    interstitialWatched();
                }
            });
        }
        // handle the closing of the video
        if (resultCode == RESULT_OK && requestCode == REWARDED_VIDEO_REQUEST_CODE) {
            // check the engagement status
            String engagementResult = data.getStringExtra(RewardedVideoActivity.ENGAGEMENT_STATUS);
            if (engagementResult.equals(RewardedVideoActivity.REQUEST_STATUS_PARAMETER_FINISHED_VALUE)) {
                // reward the user only if they have completely watched the video

                runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        rewardVideoEnded();
                    }
                });
            }
            else if (engagementResult.equals(RewardedVideoActivity.REQUEST_STATUS_PARAMETER_ABORTED_VALUE) || engagementResult.equals(RewardedVideoActivity.REQUEST_STATUS_PARAMETER_ERROR) ) {
                runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        rewardVideoDismissed();
                    }
                });
            }
        }
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        setIntent(intent);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native int backgroundUpdateFunctions();
    public native void blockGLResume(boolean bValue);
    public native void onCreateCalled();

    public native void rewardVideoEnded();
    public native void rewardVideoDismissed();
    public static native void setApplicatonOffline(int value);

    // --------------------------Google Games--------------------------

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putBoolean("resolving_error", sGamesCommunicator.getResolvingError());
    }

    public static NAGoogleGamesCommunicator getNaGoogleGamesCommunicator(){
        return sGamesCommunicator;
    }

    // ------------------------------------Getters------------------------------------

    public static Handler getHandler() {
        return sHandler;
    }

    public static String getBundleVersion() {
        String versionName = "Version not found";
        Context context = getContext();
        try {
            versionName = context.getPackageManager().getPackageInfo(
                    context.getPackageName(), 0).versionName;
        } catch (PackageManager.NameNotFoundException e) {
        }
        if(versionName.isEmpty())
            versionName = "Version not found";
        return versionName;
    }

    public static String getExpansionAPKFileName(Context c, boolean mainFile,
                                                 String versionCode) {
        return (mainFile ? "main." : "patch.") + versionCode + "."
                + c.getPackageName() + ".obb";
    }

    //----------------------------------Local Notifications------------------------


    protected static class NotificationData {
        public String message;
        public int interval;
        public int tag;
        public String action;
        public String title;

        public NotificationData(String aMessage, String title, int aInterval, int aTag,
                                String aAction) {
            message = aMessage;
            interval = aInterval;
            tag = aTag;
            action = aAction;
            title = title;
        }
    }

    public static void showLocalNotification(String message, String title, int interval,
                                             int tag, String action) {

        StringBuilder b = new StringBuilder(message.length());
        Formatter f = new Formatter(b);
        for (char c : message.toCharArray()) {
            if (c < 128) {
                b.append(c);
            } else {
                f.format("\\u%04x", (int) c);
            }
        }

        String uniStr = b.toString();

        String unicodes = StringEscapeUtils.unescapeJava(uniStr);

        message = message.replace(uniStr, unicodes);

        PendingIntent sender = getPendingIntent(message, title,  tag, action);
        if (sender == null) {
            sNotifications.add(new NotificationData(message, title, interval, tag,
                    action));
            return;
        }

        Calendar calendar = Calendar.getInstance();
        calendar.setTimeInMillis(System.currentTimeMillis());
        calendar.add(Calendar.SECOND, interval);

        AlarmManager am = (AlarmManager) sActivity
                .getSystemService(ALARM_SERVICE);
        am.set(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), sender);
    }

    public static void cancelLocalNotification(int tag) {
        PendingIntent sender = getPendingIntent(null, null, tag, null);
        if (sender == null)
            return;

        AlarmManager am = (AlarmManager) sActivity
                .getSystemService(ALARM_SERVICE);
        am.cancel(sender);
        NotificationManager manager = (NotificationManager) sActivity
                .getApplicationContext().getSystemService(
                        Context.NOTIFICATION_SERVICE);
        manager.cancel(tag);
    }

    private static PendingIntent getPendingIntent(String message, String title, int tag,
                                                  String action) {
        if (sActivity == null)
            return null;

        Intent i = new Intent(sActivity.getApplicationContext(),
                LocalNotificationReceiver.class);
        i.putExtra("notification_id", tag);
        i.putExtra("message", message);
        i.putExtra("title", title);
        i.putExtra("action", action);
        PendingIntent sender = PendingIntent.getBroadcast(sActivity, tag, i,
                PendingIntent.FLAG_UPDATE_CURRENT);
        return sender;
    }

    // ----------------------------Firebase Cloud Messaging----------------------------------

    public static native void setPushRegistrationId(String regId);

    public static String getCountryCode() {
        return Locale.getDefault().getCountry();
    }

    // ------------------------ EXIT GAME DIALOG ----------------------

    public static void showQuitGameDialog() {
        //throw new RuntimeException();


            sActivity.runOnUiThread(new Runnable() {
                public void run() {
                    if (!sActivityRunning)
                        return;
                    if (alertDialog != null && alertDialog.isShowing()) {
                        // A dialog is already open, wait for it to be dismissed, do nothing
                        return;
                    }

                    //Log//Log.v(TAG, "running on UI thread");
                    alertbox = new AlertDialog.Builder(new ContextThemeWrapper(Cocos2dxActivity.getContext(), android.R.style.Theme_DeviceDefault_Dialog));
                    alertbox.setMessage("Do you want to exit My Story?");
                    alertbox.setTitle("");

                    alertbox.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            dialog.cancel();
                        }
                    });

                    alertbox.setPositiveButton("QUIT", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
//        			   Cocos2dxActivity myActivity = (Cocos2dxActivity)Cocos2dxActivity.getContext();
                            ((Cocos2dxActivity) sActivity).runOnGLThread(new Runnable() {

                                @Override
                                public void run() {
//        	      			 	sActivity.moveTaskToBack(true);

                                    sActivity.finish();
//							   android.os.Process.killProcess(android.os.Process.myPid());
//							   System.exit(0);
                                }
                            });
                        }
                    });

                    try {
                        alertDialog = alertbox.create();
                        alertDialog.show();
                    } catch (WindowManager.BadTokenException e) {
                        // Nothing, app not active so nothing happens
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
        }



    private void checkForReferralInIntent() {
        final Intent intent = getIntent();
        final Uri uri = intent.getData();
        String s = "";

        if (uri != null) {
            if (uri.getScheme().equals(SHARE_PROTOCOL) && uri.getHost().equals(SHARE_URL)) {
                // Cool, we have a URI addressed to this activity!
                List<String> pathSegments = uri.getPathSegments();
                if(pathSegments.size() > 0) {
                    Cocos2dxHelper.setStringForKey("referralBook", pathSegments.get(0));
                }
            }
        }
    }

    private void checkForPushIdInIntent() {
        final Intent intent = getIntent();
        final Bundle extras = intent.getExtras();
        if (extras != null) {
            final String pushId = extras.getString("pushId");
            final String pushMsg = extras.getString("pushMsg");
            if (pushId != null) {
                Cocos2dxHelper.setStringForKey("pushId", pushId);
            }
            if (pushMsg != null) {
                Cocos2dxHelper.setStringForKey("pushMsg", pushMsg);
            }
        }
    }
}

