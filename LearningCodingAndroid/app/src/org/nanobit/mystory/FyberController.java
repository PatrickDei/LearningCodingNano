package org.nanobit.mystory;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.util.Log;

import com.fyber.Fyber;
import com.fyber.ads.AdFormat;
import com.fyber.currency.VirtualCurrencyErrorResponse;
import com.fyber.currency.VirtualCurrencyResponse;
import com.fyber.requesters.InterstitialRequester;
import com.fyber.requesters.OfferWallRequester;
import com.fyber.requesters.RequestCallback;
import com.fyber.requesters.RequestError;
import com.fyber.requesters.RewardedVideoRequester;
import com.fyber.requesters.VirtualCurrencyCallback;
import com.fyber.requesters.VirtualCurrencyRequester;
import com.fyber.user.User;
import com.fyber.utils.FyberLogger;
import com.fyber.utils.testsuite.IntegrationAnalyzer;

public class FyberController {

		private final static String TAG 									= FyberController.class.getSimpleName();
		
		private final static String FYBER_APP_ID 							= "91571";
		private final static String FYBER_SECURITY_TOKEN					= "2d033f248db837b2fa5d26ed1ae999de";
		private final static long CHECK_FOR_VIDEO_DELAY						= 30;
		private final static long CHECK_FOR_VIDEO_DELAY_LONG				= 100;
		
		public final static int FYBER_INTERSTITIAL_REQUEST_CODE				= 1234;
		public final static int FYBER_VIDEO_REQUEST_CODE					= 1233;
		public final static int FYBER_OFFER_WALL_REQUEST_CODE				= 123456789;

		private final static String FYBER_APP_ID_DEBUG 						= "114529";
		private final static String FYBER_SECURITY_TOKEN_DEBUG  			= "12c328c27a02b764369e8dc3253ef5fb";
		
		private boolean isVideoAvailable = false;
		private String userId = "6b626b18d04c9f725ab629586a42272900a2679c"; // test user Id, debugging purposes
		
		private Context mContext = null;
		private static Cocos2dxActivity mActivity = null;
		
		private static Intent videoIntent = null;
		private static Intent offerWallIntent = null;
		private static Intent interstitialIntent = null;
		private static FyberController controller = null;
		
		public FyberController(Cocos2dxActivity activity) 
		{
			this.mContext = activity.getApplicationContext();
			mActivity = activity;
			
			controller = this;
			
			this.isVideoAvailable = false;
			
		}
		
		public void Initialize(String userId)
		{
			this.userId = userId;

			try {
				Log.d(TAG, "FyberController: Fyber starting");
				Fyber.Settings settings;
				User.clearGdprConsentData(StoryGame.getContext());
                if(BuildConfig.DEBUG){
					settings = Fyber.with(FYBER_APP_ID_DEBUG, mActivity).withUserId(this.userId).withSecurityToken(FYBER_SECURITY_TOKEN_DEBUG).start();
				}
				else{
					settings = Fyber.with(FYBER_APP_ID, mActivity).withUserId(this.userId).withSecurityToken(FYBER_SECURITY_TOKEN).start();
				}
				settings.notifyUserOnCompletion(false);
				settings.notifyUserOnReward(false);
			} catch (RuntimeException e){
				e.printStackTrace();
		    }
            FyberLogger.enableLogging(BuildConfig.DEBUG);
			//IntegrationAnalyzer.showTestSuite(mActivity);
		}
		
		
		public static void setFyberParameters(String param) {
			String[] parts = param.split(";");
			final String sMoneySpent = parts[0];
			final String daysPlayed = parts[1];
			final String playerLevel = parts[2];
			int numberOfSessions = Integer.parseInt(parts[3]);
			Log.d(TAG, "fyber setFyberParameters sMoneySpent: " + sMoneySpent + " sDaysPlayed " + daysPlayed + " playerLevel " + playerLevel  + " NumberOfSessions " + numberOfSessions);
			Float moneySpent = Float.parseFloat(sMoneySpent);
			if(moneySpent == Float.POSITIVE_INFINITY || moneySpent == Float.NEGATIVE_INFINITY || moneySpent == Float.NaN)
			{
				moneySpent = 0.f;
			}
			User.setIap(moneySpent > 0);
			User.setIapAmount(moneySpent);
            User.setNumberOfSessions(numberOfSessions);
			User.addCustomValue("daysPlayed", daysPlayed);
			User.addCustomValue("playerLevel", playerLevel);
		}

		// JNI calls
		public static void initFyber(final String userId)
	    {
	    	mActivity.runOnUiThread(new Runnable() {	// TODO: error message was Can't create handler inside thread that has not called Looper.prepare()
	            public void run() {		// TODO: mixed crash?  java.lang.NoClassDefFoundError: org/nanobit/hollywood/g AND java.lang.RuntimeException: Can't create handler inside thread that has not called Looper.prepare()
	            	if (controller != null)
	            	{
	            		controller.Initialize(userId);
	            	}
	            }
	        });
	    }
		
		
		/* -------------------------- VIDEO -------------------------------------------- */
		
		public void PlayVideo() 
		{
			if( mActivity == null )
			return;

			((StoryGame)mActivity).blockGLResume(true);
			try {
				mActivity.startActivityForResult(videoIntent, FYBER_VIDEO_REQUEST_CODE);
				videoIntent = null;
				isVideoAvailable();
			} catch (Exception e) {
				e.printStackTrace();
			}

		}

		public void RequestVideo(boolean serverSideReward){
			try{
				Log.d(TAG, "FyberController: RequestVideo");
				if(serverSideReward){
					RewardedVideoRequester.create(videoRequestCallback).request(mActivity);
				}
				else{
					RewardedVideoRequester.create(videoRequestCallback).withVirtualCurrencyRequester(mVCSVideoRequester).request(mActivity);
				}
				 // TODO: crash with message java.lang.RuntimeException: Package manager has died
			}
			catch (Exception e){
				Log.d(TAG, "FyberController: RequestVideo Exception");
				e.printStackTrace();
			}
		}
		
		public static void requestFyberVideo(boolean serverSideReward)
		{
			final boolean serverSideRewardFinal = serverSideReward;
			mActivity.runOnUiThread(new Runnable() {	
	            public void run() {
	            	if (controller != null)
	            	{	
	            		controller.RequestVideo(serverSideRewardFinal);
	            	}
	            	else{
						Log.d(TAG, "FyberController: controller == null");
					}
	            }
	        });
		}

	private RequestCallback videoRequestCallback = new RequestCallback() {
		void videoAvailable(final boolean value) {
			mActivity.runOnGLThread(new Runnable() {
				@Override
				public void run() {
					setVideoAvailable(value);
				}
			});
		}
		@Override
		public void onAdAvailable(Intent intent) {
			Log.d(TAG, "FyberController: Video is available");

			videoIntent = intent;
			videoAvailable(true);
		}

		@Override
		public void onAdNotAvailable(AdFormat adFormat) {
			Log.d(TAG, "FyberController: No video available");

			videoIntent = null;
			videoAvailable(false);
		}

		@Override
		public void onRequestError(RequestError requestError) {
			Log.d(TAG, "FyberController: Something went wrong with the request: " + requestError.getDescription());

			videoIntent = null;
			videoAvailable(false);
		}
	};

	// response listener
	private VirtualCurrencyCallback mVCSVideoListener = new VirtualCurrencyCallback() {

		@Override
		public void onRequestError(RequestError requestError) {
			Log.d("VCSVideoListener", "VCS error recieved - " + requestError.getDescription());
		}

		@Override
		public void onError(VirtualCurrencyErrorResponse virtualCurrencyErrorResponse) {
			Log.d("VCSVideoListener", "VCS error recieved - " + virtualCurrencyErrorResponse.getErrorMessage());
		}

		@Override
		public void onSuccess(VirtualCurrencyResponse virtualCurrencyResponse) {
			Log.d("VCSVideoListener", "VCS coins received - " + virtualCurrencyResponse.getDeltaOfCoins());
			if(virtualCurrencyResponse.getDeltaOfCoins() > 0) {
				mActivity.runOnGLThread(new Runnable() {
					@Override
					public void run() {
						rewardVideoWatched();
					}
				});
			}
		}
	};

  	private VirtualCurrencyRequester mVCSVideoRequester = VirtualCurrencyRequester.create(mVCSVideoListener);


	public static void playFyberVideo()
	{
		mActivity.runOnUiThread(new Runnable() {
			public void run() {
				//Log//Log.v(TAG, "running on UI thread");
				if (controller != null)
				{
					controller.PlayVideo();
				}
			}
		});
	}


	public static boolean isVideoAvailable()
	{
		boolean videoAvailable = videoIntent!=null;
		return videoAvailable;
	}

	public native static void setVideoAvailable(boolean available);
	public native static void rewardVideoWatched();

	/* ----------------------------------- OFFER WALL ------------------------------------------ */

	public void RunOfferWall()
	{
		// Log.d(TAG, "Fyber - run offer wall");
		if( mActivity == null )
			return;

		OfferWallRequester.create(offerWallRequestCallback).addParameter("pub0", "OFFERWALL").closeOnRedirect(false).request(mActivity);
	}

	public static void runFyberOfferWall()
	{
		mActivity.runOnUiThread(new Runnable() {
			public void run() {
				//Log//Log.v(TAG, "running on UI thread");
				if (controller != null)
				{
					controller.RunOfferWall();
				}
			}
		});
	}

	private RequestCallback offerWallRequestCallback = new RequestCallback() {

		@Override
		public void onRequestError(RequestError requestError) {
			Log.d(TAG, "FyberController: Something went wrong with the request: " + requestError.getDescription());
		}

		@Override
		public void onAdAvailable(Intent intent) {
			Log.d(TAG, "FyberController: Offers are available");

			offerWallIntent = intent;

			((StoryGame)mActivity).blockGLResume(true);
			try {
				mActivity.startActivityForResult(offerWallIntent, FYBER_OFFER_WALL_REQUEST_CODE);
				offerWallIntent = null;
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		@Override
		public void onAdNotAvailable(AdFormat adFormat) {
			Log.d(TAG, "FyberController: No offerWall available");

			offerWallIntent = null;
		}
	};

	public native static void offerWallClosed();



	/* ------------------------- INTERSTITIALS ------------------------------ */

	private RequestCallback interstitialRequestCallback = new RequestCallback() {
		void interstitialAvailable(final boolean available) {
			mActivity.runOnGLThread(new Runnable() {
				@Override
				public void run() {
					setInterstitialAvailable(available);
				}
			});
		}

		@Override
		public void onAdAvailable(Intent intent) {
			interstitialIntent = intent;
			interstitialAvailable(true);

		}

		@Override
		public void onAdNotAvailable(AdFormat adFormat) {
			interstitialIntent = null;
			interstitialAvailable(false);
		}

		@Override
		public void onRequestError(RequestError requestError) {
			interstitialIntent = null;
			interstitialAvailable(false);
		}
	};

	public static void requestFyberInterstitial( )
		{
			Log.v(TAG, "FyberController: Interstitial: before running on UI thread");
			mActivity.runOnUiThread(new Runnable() {	
	            public void run() {
					Log.v(TAG, "FyberController: Interstitial: running on UI thread");
	            	if (controller != null)
	            	{
	            		
	            		controller.RequestInterstitial();
	            	}
	            }
	        });
		}
		
	public void RequestInterstitial()
	{
		 Log.v(TAG, "FyberController:  -> request interstitial");
		// todo lana fyber
		try {
			InterstitialRequester.create(interstitialRequestCallback).request(mActivity);
		} catch( RuntimeException e ){
			 // TODO: crash with message java.lang.RuntimeException: Package manager has died
		}
	}

	public static void showFyberInterstitial()
	{
		mActivity.runOnUiThread(new Runnable() {
			public void run() {
				if (controller != null)
				{
					controller.ShowInterstitial();
				}
			}
		});
	}

	public void ShowInterstitial()
	{
		if( mActivity == null )
			return;
		((StoryGame)mActivity).blockGLResume(true);
		try {
			 mActivity.startActivityForResult(interstitialIntent, FYBER_INTERSTITIAL_REQUEST_CODE);
			 interstitialIntent = null;

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static boolean isInterstitialAvailable()
	{
		boolean interstitialAvailable = interstitialIntent != null;
		return interstitialAvailable;
	}

	public native static void setInterstitialAvailable(boolean available);
	public native static void interstitialWatched();

	public static void setFyberGdprConsentGiven()
	{
		User.setGdprConsent(true, StoryGame.getContext());
	}
}
