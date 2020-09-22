# Add project specific ProGuard rules here.
# By default, the flags in this file are appended to flags specified
# in /Users/lanalisjak/AndoridSDK/adt-bundle-mac-x86_64-20140702/sdk/tools/proguard/proguard-android.txt
# You can edit the include path and order by changing the proguardFiles
# directive in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# Add any project specific keep options here:

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}

-dontshrink
-dontoptimize
-dontpreverify

-dontwarn javax.management.**
-dontwarn java.lang.management.**
-dontwarn org.apache.log4j.**
-dontwarn org.apache.commons.logging.**
-dontwarn org.slf4j.**
-dontwarn org.json.**
-dontwarn android.**
-dontwarn com.unity3d.**
-dontwarn com.facebook.**
-dontwarn com.google.**
-dontwarn com.vungle.**
-dontwarn com.chartboost.**
-dontwarn com.tapjoy.**
-dontwarn net.hockeyapp.**
-dontwarn com.amazonaws.util.**
-dontwarn com.fyber.**
-dontwarn com.squareup.**
-dontwarn com.moat.**
-keep class com.facebook.** { *;}
-keep class com.fyber.** { *;}
-keep class com.tapjoy.** { *; }
-dontwarn com.hyprmx.android.sdk.api.data.Ad
-dontwarn com.hyprmx.android.sdk.core.HyprMX
-dontwarn com.hyprmx.android.sdk.core.HyprMXController

#Google Play Services

-keep class * extends java.util.ListResourceBundle {
    protected Object[][] getContents();
}

-keep public class com.google.android.gms.common.internal.safeparcel.SafeParcelable {
    public static final *** NULL;
}

-keepnames @com.google.android.gms.common.annotation.KeepName class *
-keepclassmembernames class * {
    @com.google.android.gms.common.annotation.KeepName *;
}

-keepnames class * implements android.os.Parcelable {
    public static final ** CREATOR;
}


#NanoAndroidFunc
-keep public class org.nanobit.mystory.*
{
	*;
}

-keep class org.cocos2dx.lib.**
{
	*;
}

#InAppPurchase
-keep class org.nanobit.mystory.purchase.InappPurchaseManager
{
	*;
}

#Helpshift
-keep class android.support.** { *; }
-keep class com.helpshift.** { *; }

# Fyber Mediation

-keep class com.fyber.mediation.MediationConfigProvider {
    public static *;
}
-keep class com.fyber.mediation.MediationAdapterStarter {
    public static *;
}

-keepclassmembers class com.fyber.ads.videos.mediation.** {
    void setValue(java.lang.String);
}

-keep class org.jetbrains.annotations.*{ *; }
-dontwarn org.jetbrains.annotations.**

-keepattributes SourceFile,LineNumberTable
-keepattributes JavascriptInterface
-keep class com.unity3d.ads.android.** { *; }

-keep class com.flurry.** { *; }
-dontwarn com.flurry.**
-keepattributes *Annotation*,EnclosingMethod
-keepclasseswithmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet, int);
}

-keep class com.vungle.**
-keep class javax.inject.*
-keepattributes *Annotation*
-keepattributes Signature
-keep class dagger.**

-keep class com.inmobi.** { *; }
-dontwarn com.inmobi.**

#Hockeyapp
-keep class java.lang.management.** { *; }
-keep class org.apache.log4j.** { *;}
-keep class org.apache.commons.logging.** { *;}
-keep class org.apache.http.**
-keep interface org.apache.http.**
-keep class org.slf4j.** { * ;}
-keep class org.json.** { *;}

-keep public class javax.net.ssl.**
-keepclassmembers public class javax.net.ssl.** {
  *;
}

#Android webkit
-keep class android.webkit.JavascriptInterface

# Required for attribution
-keep class com.google.android.gms.ads.** { *; }
-keep class com.google.ads.** { *; }


# Adjust SDK
-keep public class com.adjust.sdk.** { *; }
-keep class com.google.android.gms.common.ConnectionResult {
    int SUCCESS;
}
-keep class com.google.android.gms.ads.identifier.AdvertisingIdClient {
    com.google.android.gms.ads.identifier.AdvertisingIdClient$Info getAdvertisingIdInfo(android.content.Context);
}
-keep class com.google.android.gms.ads.identifier.AdvertisingIdClient$Info {
    java.lang.String getId();
    boolean isLimitAdTrackingEnabled();
}
-keep public class com.android.installreferrer.** { *; }

# soomla
-keep public class com.soomla.traceback.* {
    public *;
}
-keep public class com.soomla.traceback.integrations.** {
    public *;
}
-keep public class com.soomla.traceback.unity.* {
    public *;
}
-dontwarn com.soomla.traceback.**

#For AppLovin integration
-keepclassmembers class com.applovin.sdk.AppLovinSdk {
    static *;
}
#For Facebook integration
-keepclassmembers class com.facebook.ads.internal.AdSdkVersion {
    static *;
}
# In case you use Kochava for Attribution
-keep class com.kochava.android.tracker.Tracker
-keep class com.kochava.android.tracker.InputParams

# Google Advertising Id

-keep class com.google.android.gms.ads.identifier.AdvertisingIdClient { *; }
-keep class com.google.android.gms.ads.identifier.** { *; }

-keepclassmembers class com.ironsource.sdk.controller.IronSourceWebView$JSInterface {
    public *;
}
-keepclassmembers class * implements android.os.Parcelable {
    public static final android.os.Parcelable$Creator *;
}
-keep public class com.google.android.gms.ads.** {
   public *;
}
-keep class com.ironsource.adapters.** { *;
}
-dontwarn com.moat.**
-keep class com.moat.** { public protected private *; }
