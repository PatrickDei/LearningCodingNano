package org.nanobit.mystory;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONArray;
import org.json.JSONException;

import android.content.Context;
import android.content.Intent;
import android.content.IntentSender.SendIntentException;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.GamesActivityResultCodes;
import com.google.android.gms.games.Player;
import com.google.android.gms.games.PlayerBuffer;
import com.google.android.gms.games.Players;
import com.google.android.gms.games.Players.LoadPlayersResult;

public class NAGoogleGamesCommunicator implements GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener{
	
	final private GoogleApiClient gApiClient;
	private boolean isSignedIn = false;
	private boolean mResolvingError;
	private boolean showAchievementsAttempt = false;
	private static final int REQUEST_RESOLVE_ERROR = 1001;
	private static final int ACHIEVMENTS_INTENT = 1002;
	static final String DID_CANCEL_LOGIN = "google_play_login_canceled";
	
	public NAGoogleGamesCommunicator() {
		
		gApiClient = new GoogleApiClient.Builder(Cocos2dxActivity.getContext())
        .addConnectionCallbacks(this)
        .addOnConnectionFailedListener(this)
        .addApi(Games.API).addScope(Games.SCOPE_GAMES)
        .build();
	}

	public void loginPlayer(){
		final SharedPreferences sharedPreferences =  StoryGame.getContext().getSharedPreferences(StoryGame.class.getSimpleName(), Context.MODE_PRIVATE);
//		Log.d("Lana ", "DID_CANCEL_LOGIN " + sharedPreferences.getBoolean(DID_CANCEL_LOGIN, false));
        if (!gApiClient.isConnecting() && !gApiClient.isConnected() && !sharedPreferences.getBoolean(DID_CANCEL_LOGIN, false)) {
        	// Log.v("IAP", "Trying to connect with gApiClient");
            gApiClient.connect();
        }
	}

	public void fetchFriends() {
		
		Games.Players.loadInvitablePlayers(gApiClient,20, true).setResultCallback(new ResultCallback<Players.LoadPlayersResult>() {
			
			@Override
			public void onResult(final LoadPlayersResult loadedPlayers) {

				StoryGame myStardroid = (StoryGame) StoryGame.getContext();
				myStardroid.runOnGLThread(new Runnable() {
					@Override
					public void run() {
						if (!loadedPlayers.getStatus().isSuccess()) {
							friendsFetched(false);
							return;
						} 
						PlayerBuffer playerBuffer = loadedPlayers.getPlayers();
						int playerCount = playerBuffer.getCount();
						for (int i = 0; i < playerCount; i++) {
							Player friendPlayer = playerBuffer.get(i);
							String friendID = friendPlayer.getPlayerId();
							String friendName = friendPlayer.getDisplayName(); 
							addFriendToMap(friendID, friendName);
						}
						friendsFetched(true);
					}
				});
			}
		});
	}

	@Override
	public void onConnectionFailed(ConnectionResult result) {
	
        if (mResolvingError) {
            // Already attempting to resolve an error.
            return;
        } else if (result.hasResolution()) {
            try {
                mResolvingError = true;
                result.startResolutionForResult((StoryGame)StoryGame.getContext(), REQUEST_RESOLVE_ERROR);
                ((StoryGame)StoryGame.getContext()).blockGLResume(true);
                NanoAndroidFunctions.hideActivityDialog();
            } catch (SendIntentException e) {
                // There was an error with the resolution intent. Try again.
                gApiClient.connect();
            }
        } else if (!showAchievementsAttempt){
            //Show dialog using GooglePlayServicesUtil.getErrorDialog()
            showErrorDialog(result.getErrorCode());
			StoryGame myStardroid = (StoryGame) StoryGame.getContext();
    		myStardroid.runOnGLThread(new Runnable() {
    			@Override
    			public void run() {
    				googleApiConnectFailed();
    			}
    		});
            mResolvingError = true;
        }
	}

	private void showErrorDialog(int errorCode) {
		
	}

	@Override
	public void onConnected(Bundle arg0) {
		if( gApiClient == null ){
			return;
		}
		
		isSignedIn = true;
		if (showAchievementsAttempt) {
			showAchievmentsUI();
			showAchievementsAttempt = false;
			return;
		}
		
		try{
			final String playerIDString = Games.Players.getCurrentPlayer(gApiClient).getPlayerId();
			final String playerNameString = Games.Players.getCurrentPlayer(gApiClient).getDisplayName();
			StoryGame myStardroid = (StoryGame) StoryGame.getContext();
			myStardroid.runOnGLThread(new Runnable() {
				@Override
				public void run() {
					googleApiConnected(playerIDString, playerNameString);	
				}
			});
		}
		catch(NullPointerException e)
		{		

			isSignedIn = false;
		}
		catch( RuntimeException e )	// Package manager has died? Hockeyapp error report
		{
			isSignedIn = false;	
		}
//		Log.d("Lana", "DID_CANCEL_LOGIN onConected" + isSignedIn);
		final SharedPreferences sharedPreferences =  StoryGame.getContext().getSharedPreferences(StoryGame.class.getSimpleName(), Context.MODE_PRIVATE);
		SharedPreferences.Editor editor = sharedPreferences.edit();
		editor.putBoolean(DID_CANCEL_LOGIN, !isSignedIn);
		editor.commit();
	}

	@Override
	public void onConnectionSuspended(int arg0) {
		//attempt to reconnect
		try {
			gApiClient.connect();
		} catch (Exception e) {
			// TODO: handle exception
		}
	}
	
	public boolean getIsConnected(){

		Log.d("tag","getIsConnected");
		if (gApiClient != null) 
		{
			return gApiClient.isConnected();
		}
		return false;
	}
	
	public void resolutionResult(int requestCode, int resultCode, Intent data){		
        mResolvingError = false;
        if (resultCode == StoryGame.RESULT_OK) {
        	// Log.v("IAP", "Result OK");
            if (!gApiClient.isConnecting() && !gApiClient.isConnected()) {
            	// Log.v("IAP", "Calling connect");
                gApiClient.connect();
            }
        }
        else{
        	// Log.v("IAP", "Result NOT OK");
        	// Log.v("IAP", "Request Code: " + requestCode);
        	// Log.v("IAP", "Result code: " + resultCode);
			StoryGame myStardroid = (StoryGame) StoryGame.getContext();
    		myStardroid.runOnGLThread(new Runnable() {
    			@Override
    			public void run() {
    				// Log.v("IAP", "calling googleApiConnectFailed");
    				googleApiConnectFailed();
    			}
    		});
        }
	}
	
	public boolean getResolvingError(){
		return mResolvingError;
	}
	
	public void setResolvingError(boolean bValue){
		mResolvingError = bValue;
	}

	public boolean reportAchievment(String achID) {
		boolean isReported = false;

		if (getIsConnected())
		{
			Games.Achievements.unlock(gApiClient, achID);
			isReported = true;
		}

		return isReported;
	}

	public boolean reportSavedAchievements(String jsonString) {
		boolean isReported = false;

		if (getIsConnected())
		{
			try {
				JSONArray json = new JSONArray(jsonString);
				for(int i = 0; i < json.length(); i++)
				{
					String achId = json.getString(i);
					Games.Achievements.unlock(gApiClient, achId);
				}

				isReported = true;
			} catch (JSONException e) {
				isReported = false;
			}
		}

		return isReported;
	}

	public void signOut(){

		Log.d("bla", "signeout");
		Games.signOut(gApiClient);
		gApiClient.disconnect();
	}
	
	public void showAchievmentsUI(){
		try {
			// Log.v("IAP", "Starting achievementsIntent");
			StoryGame myActivity = (StoryGame) StoryGame.getContext();
			myActivity.blockGLResume(true);
			myActivity.startActivityForResult(Games.Achievements.getAchievementsIntent(gApiClient), ACHIEVMENTS_INTENT);
		} catch (IllegalStateException e) {
			// Log.v("IAP", "First you have to login");
			showAchievementsAttempt = true;
			final SharedPreferences sharedPreferences =  StoryGame.getContext().getSharedPreferences(StoryGame.class.getSimpleName(), Context.MODE_PRIVATE);
			SharedPreferences.Editor editor = sharedPreferences.edit();
			editor.putBoolean(DID_CANCEL_LOGIN, false);
			editor.commit();
			loginPlayer();
		}
	}
	
	public void onActivityResult(int result, Intent data){
		// Log.v("IAP", "onActivityResult");
		if (  GamesActivityResultCodes.RESULT_RECONNECT_REQUIRED == result ) {
			 gApiClient.disconnect();
		}
	}
	
	public static native void signedOut(boolean status);
	public static native void googleApiConnectFailed();
	public static native void friendsFetched(boolean isSuccess);
	public static native void addFriendToMap(String friendID, String friendName);
	public static native void googleApiConnected(String playerID, String playerName);
}
