package org.nanobit.mystory;

import android.app.Activity;
import android.util.Log;

import com.chartboost.sdk.Chartboost;
import com.chartboost.sdk.ChartboostDelegate;
import com.chartboost.sdk.Model.CBError;

public class ChartboostController {

    private final static String TAG = ChartboostController.class.getSimpleName();

    private static final String CHARTBOOST_APP_ID = "5b0c70a634cd5d0b5dfa87ba";
    private static final String CHARTBOOST_APP_SIGNATURE = "38715bbf99806db2295a2672d87dfddb201f4194";

    private static ChartboostController mSingleton = null;
    private static Activity mActivity = null;
    private static boolean sIsVideoAvailable = false;
    private static boolean sPendingShowVideo = false;

    public static ChartboostController get()
    {
        if (mSingleton == null)
        {
            mSingleton = new ChartboostController();
        }

        return mSingleton;
    }

    public void initialize(Activity activity)
    {
        mActivity = activity;
        sPendingShowVideo = false;
        sIsVideoAvailable = false;

        Chartboost.startWithAppId(activity, CHARTBOOST_APP_ID, CHARTBOOST_APP_SIGNATURE);
        Chartboost.onCreate(activity);

        Chartboost.setDelegate(chartboostDelegate);
    }

    public void cacheRewardedVideo(String location)
    {
        if (!Chartboost.hasRewardedVideo(location))
        {
            Chartboost.cacheRewardedVideo(location);
        }
    }

    public void showVideo(String location)
    {
        if (Chartboost.hasRewardedVideo(location))
        {
            Chartboost.showRewardedVideo(location);
        }
        else if (Chartboost.hasInterstitial(location))
        {
            Chartboost.showInterstitial(location);
        }
        else
        {
            sPendingShowVideo = true;
            cacheRewardedVideo(location);
        }
    }

    public void showInterstitial(String location)
    {
        Chartboost.showInterstitial(location);
    }

    public void checkHasVideo(String location)
    {
        SetVideoAvailable(location, Chartboost.hasRewardedVideo(location));
    }

    public static void CacheRewardedVideo(final String location)
    {
        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (mSingleton != null)
                {
                    mSingleton.cacheRewardedVideo(location);
                }
            }
        });
    }

    public static void ShowRewardedVideo(final String location)
    {
        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (mSingleton != null)
                {
                    mSingleton.showVideo(location);
                }
            }
        });
    }

    public static void ShowInterstitial(final String location)
    {
//        if( mActivity == null )
//            return;
//
//        ((StoryGame)mActivity).blockGLResume(true);
//        try {
//            if (mSingleton != null)
//            {
//                mSingleton.showInterstitial(location);
//            }
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (mSingleton != null)
                {
                    mSingleton.showInterstitial(location);
                }
            }
        });
    }

    public static void CheckHasVideo(final String location)
    {
        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (mSingleton != null)
                {
                    mSingleton.checkHasVideo(location);
                }
            }
        });
    }

    private static ChartboostDelegate chartboostDelegate = new ChartboostDelegate() {
        @Override
        public void didCacheRewardedVideo(String location) {
            super.didCacheRewardedVideo(location);
            Log.d(TAG, "Rewarded video cached");

            if (sPendingShowVideo)
            {
                sPendingShowVideo = false;
                mSingleton.showVideo(location);
            }

            SetVideoAvailable(location, true);
        }

        @Override
        public void willDisplayVideo(String location) {
            super.willDisplayVideo(location);
            WillDisplayVideo(location);
        }

        @Override
        public void didCompleteRewardedVideo(String location, int reward) {
            super.didCompleteRewardedVideo(location, reward);
            Log.d(TAG, "Rewarded video completed");
            DidCompleteRewardedVideo(location);
        }

        @Override
        public void didDismissRewardedVideo(String location) {
            super.didDismissRewardedVideo(location);
            Log.d(TAG, "Rewarded video dismissed");
            DidDismissRewardedVideo(location);
        }

        @Override
        public void didCloseRewardedVideo(String location) {
            super.didCloseRewardedVideo(location);
            Log.d(TAG, "Rewarded video closed");
            DidCloseRewardedVideo(location);
        }

        @Override
        public void didFailToLoadRewardedVideo(String location, CBError.CBImpressionError error) {
            super.didFailToLoadRewardedVideo(location, error);
            Log.d(TAG, "Failed to load rewarded video - " + error.toString());
        }
    };

    public native static void SetVideoAvailable(String location, boolean available);
    public native static void WillDisplayVideo(String location);
    public native static void DidCompleteRewardedVideo(String location);
    public native static void DidDismissRewardedVideo(String location);
    public native static void DidCloseRewardedVideo(String location);
}

