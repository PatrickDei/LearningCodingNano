package org.nanobit.mystory;

import android.os.Bundle;

import com.adjust.sdk.Adjust;
import com.facebook.AccessToken;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.facebook.share.model.GameRequestContent;
import com.facebook.share.widget.GameRequestDialog;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONArray;
import org.json.JSONObject;

import java.util.Arrays;
import java.util.List;

public class NAFacebookCommunicator {
    private static final StoryGame myActivity = (StoryGame) Cocos2dxActivity.getContext();
    public static void openReadSession() {

        AccessToken accessToken = AccessToken.getCurrentAccessToken();

        final StoryGame myActivity = (StoryGame) Cocos2dxActivity.getContext();
        List<String> permissionsList = Arrays.asList("public_profile", "user_friends");

        if (accessToken == null) {
            LoginManager loginManager = LoginManager.getInstance();
            loginManager.registerCallback(StoryGame.callbackManager, new FacebookCallback<LoginResult>() {
                @Override
                public void onSuccess(LoginResult loginResult) {
                    android.util.Log.d("FacebookCommunicator", "onSuccess");
                    fcSessionCallBackOnGLThread(true, true);
                }

                @Override
                public void onCancel() {
                    android.util.Log.d("FacebookCommunicator", "onCancel");
                    fcSessionCallBackOnGLThread(false, true);
                }

                @Override
                public void onError(FacebookException error) {
                    android.util.Log.d("FacebookCommunicator", "onError");
                    fcSessionCallBackOnGLThread(false, true);
                }
            });

            android.util.Log.d("FacebookCommunicator", "blockGLResume");
            myActivity.blockGLResume(true);

            loginManager.logInWithReadPermissions(myActivity, permissionsList);

        } else {
            AccessToken.refreshCurrentAccessTokenAsync(new AccessToken.AccessTokenRefreshCallback() {
                @Override
                public void OnTokenRefreshed(AccessToken accessToken) {
                    fcSessionCallBackOnGLThread(true, true);
                }

                @Override
                public void OnTokenRefreshFailed(FacebookException exception) {
                    fcSessionCallBackOnGLThread(false, true);
                }
            });
        }
    }

    public static void logOut() {
        LoginManager loginManager = LoginManager.getInstance();
        loginManager.logOut();
        fbLogedOutOnGLThread();
    }

    public static void fetchUserData() {
        final GraphRequest graphRequest = GraphRequest.newMeRequest(AccessToken.getCurrentAccessToken(), new GraphRequest.GraphJSONObjectCallback() {
            @Override
            public void onCompleted(JSONObject object, GraphResponse response) {

                if (response.getError() != null) {
                    userDataFetchedOnGLThread(false, "", "", "");
                } else {
                    String username = object.optString("name");
                    String profilePic = object.optString("picture");
                    String userId = object.optString("id");

                    userDataFetchedOnGLThread(true, username, userId, profilePic);
                }

            }
        });

        Bundle parameters = new Bundle();
        parameters.putString("fields", "name,id,picture,gender,age_range");

        graphRequest.setParameters(parameters);

        myActivity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                graphRequest.executeAndWait();
            }
        });
    }

    public static void fetchFriends() {

        final GraphRequest friendsRequest = GraphRequest.newMyFriendsRequest(AccessToken.getCurrentAccessToken(), new GraphRequest.GraphJSONArrayCallback() {
            @Override
            public void onCompleted(JSONArray objects, GraphResponse response) {
                if (response.getError() != null) {
                    userFriendsFetchedOnGLThread(false, "");
                } else {
                    JSONObject friends = new JSONObject();

                    try {
                        int count = objects.length();
                        for (int i = 0; i < count; i++) {
                            JSONObject object = objects.optJSONObject(i);

                            if (object == null)
                                continue;

                            String id = object.optString("id");
                            String name = object.optString("name");

                            friends.put(id,name);
                        }

                    } catch (Exception e) {
                        android.util.Log.d("NAFacebook", "fetch friends crashed");
                    }
                    if(friends.toString().equals("")) {
                        userFriendsFetchedOnGLThread(false, "");
                    } else {
                        userFriendsFetchedOnGLThread(true, friends.toString());
                    }
                }
            }
        });

        Bundle parameters = new Bundle();
        parameters.putString("fields", "name,id");

        friendsRequest.setParameters(parameters);

        friendsRequest.executeAndWait();
    }

    public static boolean isSessionOpened() {
        return AccessToken.getCurrentAccessToken() != null;
    }

    public static void showWebDialog(String title, String message) {

        final Cocos2dxActivity myActivity = (Cocos2dxActivity) Cocos2dxActivity
                .getContext();

        final GameRequestContent gameRequestContent = new GameRequestContent.Builder().setMessage(message).setTitle(title).build();

        final GameRequestDialog gameRequestDialog = new GameRequestDialog(myActivity);
        gameRequestDialog.registerCallback(StoryGame.callbackManager, new FacebookCallback<GameRequestDialog.Result>() {
            @Override
            public void onSuccess(GameRequestDialog.Result result) {
                fbDialogDisplayedOnGLThread(true);
            }

            @Override
            public void onCancel() {
                fbDialogDisplayedOnGLThread(false);
            }

            @Override
            public void onError(FacebookException error) {
                fbDialogDisplayedOnGLThread(false);
            }
        });

        ((StoryGame) myActivity).blockGLResume(true);
        myActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                gameRequestDialog.show(gameRequestContent);
            }
        });

        fbDialogDisplayedOnGLThread(true);
    }

    public static void fbLogedOutOnGLThread(){
        myActivity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                fbLogedOut();

            }
        });
    }

    public static void fbDialogDisplayedOnGLThread(final boolean bValue){
        myActivity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                fbDialogDisplayed(bValue);

            }
        });
    }

    public static void userFriendsFetchedOnGLThread(final boolean success, final String friends){
        myActivity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                userFriendsFetched(success, friends);

            }
        });
    }

    public static void userDataFetchedOnGLThread(final boolean success, final String usrName,
                                              final String usrID, final String fprofilePictureURLe){
        myActivity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                userDataFetched(success, usrName, usrID, fprofilePictureURLe);

            }
        });
    }

    public static void fcSessionCallBackOnGLThread(final boolean sessionOpened, final boolean isRead){
        myActivity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                fcSessionCallBack(sessionOpened, isRead);

            }
        });
    }

    public native static void fbLogedOut();

    public static boolean isConnected() {
        return isSessionOpened();
    }

    public static native void fbDialogDisplayed(boolean bValue);

    public static native void userFriendsFetched(boolean success, String friends);

    public native static void userDataFetched(boolean success, String usrName,
                                              String usrID, String fprofilePictureURLe);

    public native static void fcSessionCallBack(boolean sessionOpened, boolean isRead);
}
