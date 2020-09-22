package org.nanobit.mystory;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;

import android.support.v4.app.NotificationManagerCompat;
import org.cocos2dx.lib.Cocos2dxActivity;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GoogleApiAvailability;

public class FCMHelper {

    private static final String PROPERTY_REG_ID = "registration_id";

    // ----------------------------Google Play Services & FCM----------------------------------

    public static void checkIfGooglePlayServicesAvailable(Activity activity) {
        int resultCode = GoogleApiAvailability.getInstance().isGooglePlayServicesAvailable(activity);
        if (resultCode != ConnectionResult.SUCCESS) {
            GoogleApiAvailability.getInstance().makeGooglePlayServicesAvailable(activity);
        }
    }

    /**
     * Stores the registration ID and app versionCode in the application's
     * {@code SharedPreferences}.
     *
     * @param regId
     *            registration ID
     */
    public static void storeRegistrationId(String regId) {
        final SharedPreferences prefs = getFCMPreferences();

        SharedPreferences.Editor editor = prefs.edit();
        editor.putString(PROPERTY_REG_ID, regId);
        editor.apply();
    }

    public static boolean areNotificationsEnabled ()
    {
        final StoryGame myActivity = (StoryGame) Cocos2dxActivity.getContext();
        boolean notificationsEnabled = false;

        try {
            NotificationManagerCompat notificationManagerCompat = NotificationManagerCompat.from(myActivity);
            notificationsEnabled = notificationManagerCompat.areNotificationsEnabled();
        } catch (Exception e) {
            e.printStackTrace();
        }

        return notificationsEnabled;
    }

    /**
     * @return Application's {@code SharedPreferences}.
     */
    private static SharedPreferences getFCMPreferences() {
        // This sample app persists the registration ID in shared preferences,
        // but
        // how you store the registration ID in your app is up to you.
        return StoryGame.getContext().getSharedPreferences(StoryGame.class.getSimpleName(),
                Context.MODE_PRIVATE);
    }

    public static String getRegistrationId() {
        final SharedPreferences prefs = getFCMPreferences();
        String registrationId = prefs.getString(PROPERTY_REG_ID, "");
        if (registrationId.isEmpty()) {
            return "";
        }
        return registrationId;
    }
}
