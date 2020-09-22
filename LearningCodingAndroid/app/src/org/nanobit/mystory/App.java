package org.nanobit.mystory;

import android.support.multidex.MultiDexApplication;

import com.bugfender.sdk.Bugfender;

/**
 * Created by fkirsek on 14/02/17.
 */

public class App extends MultiDexApplication {
    @Override
    public void onCreate() {
        super.onCreate();
        Bugfender.init(this, "nQPijJmGfJgZlH9eY9gJuvCsimQomBoP", BuildConfig.DEBUG);
    }
}
