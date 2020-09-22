package org.nanobit.mystory;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.Log;

import com.ironsource.adapters.supersonicads.SupersonicConfig;
import com.ironsource.mediationsdk.IronSource;
import com.ironsource.mediationsdk.integration.IntegrationHelper;
import com.ironsource.mediationsdk.logger.IronSourceError;
import com.ironsource.mediationsdk.sdk.OfferwallListener;

public class IronSourceController implements OfferwallListener {

    private final static String TAG = IronSourceController.class.getSimpleName();

    private final static String IRONSOURCE_APP_ID = "7072bff5";

    private String userId = ""; // test user Id, debugging purposes

    private Context mContext = null;
    private static Cocos2dxActivity mActivity = null;

    private static IronSourceController controller = null;

    public IronSourceController(Cocos2dxActivity activity) {
        this.mContext = activity.getApplicationContext();
        mActivity = activity;

        controller = this;
    }

    public void Initialize(String userId) {
        this.userId = userId;

        try {
            Log.d(TAG, "IronSourceController: IronSourceController starting");

            if(BuildConfig.DEBUG) {
                IntegrationHelper.validateIntegration(mActivity);
            }

            // set the IronSource offerwall listener
            IronSource.setOfferwallListener(this);
            // set client side callbacks for the offerwall
            SupersonicConfig.getConfigObj().setClientSideCallbacks(true);

            // set the IronSource user id
            IronSource.setUserId(userId);
            // init the IronSource SDK
            IronSource.init(mActivity, IRONSOURCE_APP_ID);
        } catch (RuntimeException e) {
            e.printStackTrace();
        }
    }

    public void RunOfferWall() {
        Log.d(TAG, "IronSource - run offer wall");
        if (mActivity == null)
            return;

        IronSource.showOfferwall();
    }

    public static void showOfferWall() {
        ((StoryGame)mActivity).blockGLResume(true);
        mActivity.runOnUiThread(new Runnable() {
            public void run() {
                //Log//Log.v(TAG, "running on UI thread");
                if (controller != null) {
                    controller.RunOfferWall();
                }
            }
        });
    }

    // JNI calls
    public static void initIronSource(final String userId) {
        mActivity.runOnUiThread(new Runnable() {    // TODO: error message was Can't create handler inside thread that has not called Looper.prepare()
            public void run() {        // TODO: mixed crash?  java.lang.NoClassDefFoundError: org/nanobit/hollywood/g AND java.lang.RuntimeException: Can't create handler inside thread that has not called Looper.prepare()
                if (controller != null) {
                    controller.Initialize(userId);
                }
            }
        });
    }

    public static boolean isOfferwallAvailable() {
        return IronSource.isOfferwallAvailable();
    }
    // --------- IronSource Offerwall Listener ---------

    @Override
    public void onOfferwallAvailable(boolean available) {

    }

    @Override
    public void onOfferwallOpened() {
        // called when the offerwall has opened
        Log.d(TAG, "onOfferwallOpened");
    }

    @Override
    public void onOfferwallShowFailed(IronSourceError ironSourceError) {
        // called when the offerwall failed to show
        // you can get the error data by accessing the IronSourceError object
        //IronSourceError.getErrorCode();
        //IronSourceError.getErrorMessage();
        Log.d(TAG, "onOfferwallShowFailed" + " " + ironSourceError);
    }

    @Override
    public boolean onOfferwallAdCredited(int credits, int totalCredits, boolean totalCreditsFlag) {
        Log.d(TAG, "onOfferwallAdCredited" + " credits:" + credits + " totalCredits:" + totalCredits + " totalCreditsFlag:" + totalCreditsFlag);

        if(credits < 0){
            return false;
        }

        SharedPreferences sharedPref = mContext.getSharedPreferences(IronSourceController.class.getSimpleName(), Context.MODE_PRIVATE);
        int collectedOfferWallReward = sharedPref.getInt("collectedOfferWallReward" , 0);

       // @param totalCreditsFlag - In some cases, we won’t be able to provide the exact
       //  * amount of credits since the last event (specifically if the user clears
       //  * the app’s data). In this case the ‘credits’ will be equal to the ‘totalCredits’, and this flag will be ‘true’.
        // add uncollected reward
        if(totalCreditsFlag == true){
            // reinstalled the game do not collect reward again
            if(collectedOfferWallReward == 0)
                collectedOfferWallReward = totalCredits;

            credits = totalCredits - collectedOfferWallReward;
        }
        offerWallRewardRecieved(credits);
        collectedOfferWallReward += credits;

        SharedPreferences.Editor editor = sharedPref.edit();
        editor.putInt("collectedOfferWallReward", collectedOfferWallReward);
        editor.commit();

        return true;
    }

    @Override
    public void onGetOfferwallCreditsFailed(IronSourceError ironSourceError) {
        // you can get the error data by accessing the IronSourceError object
        //IronSourceError.getErrorCode();
        //IronSourceError.getErrorMessage();
        Log.d(TAG, "onGetOfferwallCreditsFailed" + " " + ironSourceError);
    }

    @Override
    public void onOfferwallClosed() {
        // called when the offerwall has closed
        Log.d(TAG, "onOfferwallClosed");
    }

    public void checkForUncollectedOfferWallRewards(){
        // if iron source is initialized
        if(!userId.isEmpty())
            IronSource.getOfferwallCredits();
    }

    public native static void offerWallRewardRecieved( int credits );
}