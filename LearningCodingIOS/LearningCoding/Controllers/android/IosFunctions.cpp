//
//  IosFunctions.cpp
//  NanoGirl
//
//  Created by Matko Juribasic on 09/01/14.
//
//

#include "GameState.h"
#include "IosFunctions.h"
#include "UserPurchases.h"

#include "JniHelper.h"
#include <jni.h>
#include "cocos2d.h"

#include "CCJSONConverter.h"

using namespace cocos2d;
using namespace std;

#define CLASS_NAME_ACTIVITY "org/nanobit/mystory/StoryGame"
#define CLASS_NAME_ANDROIDFUNC "org/nanobit/mystory/NanoAndroidFunctions"
#define CLASS_NAME_FYBER "org/nanobit/mystory/FyberController"
#define CLASS_NAME_COCOS2DX_HELPER "org/cocos2dx/lib/Cocos2dxHelper"

string IosFunctions::versionAndroid = "";

bool IosFunctions::createDirectory (const char *path)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, CLASS_NAME_ANDROIDFUNC, "createDirectory", "(Ljava/lang/String;)Z")) {
        jstring stringArg = t.env->NewStringUTF (path);
        jboolean ret = t.env->CallStaticBooleanMethod (t.classID, t.methodID, stringArg);
        t.env->DeleteLocalRef (t.classID);
        t.env->DeleteLocalRef (stringArg);
        return ret;
    }
    return false;
}

bool IosFunctions::removeFileOrDirectory (const char *path)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, CLASS_NAME_ANDROIDFUNC, "removeFileOrDirectory", "(Ljava/lang/String;)Z")) {
        jstring stringArg = t.env->NewStringUTF (path);
        jboolean ret = t.env->CallStaticBooleanMethod (t.classID, t.methodID, stringArg);
        t.env->DeleteLocalRef (t.classID);
        t.env->DeleteLocalRef (stringArg);
        return ret;
    }
    return false;
}

bool IosFunctions::removeAllFilesInDirectory (const char *path)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, CLASS_NAME_ANDROIDFUNC, "removeAllFilesInDirectory",
                                        "(Ljava/lang/String;)Z")) {
        jstring stringArg = t.env->NewStringUTF (path);
        jboolean ret = t.env->CallStaticBooleanMethod (t.classID, t.methodID, stringArg);
        t.env->DeleteLocalRef (t.classID);
        t.env->DeleteLocalRef (stringArg);
        return ret;
    }
    return false;
}

bool IosFunctions::copyItem (const char *fromPath, const char *toPath)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, CLASS_NAME_ANDROIDFUNC, "copyItem",
                                        "(Ljava/lang/String;Ljava/lang/String;)Z")) {
        jstring stringArg1 = t.env->NewStringUTF (fromPath);
        jstring stringArg2 = t.env->NewStringUTF (toPath);
        jboolean ret = t.env->CallStaticBooleanMethod (t.classID, t.methodID, stringArg1, stringArg2);
        t.env->DeleteLocalRef (t.classID);
        t.env->DeleteLocalRef (stringArg1);
        t.env->DeleteLocalRef (stringArg2);
        return ret;
    }
    return false;
}


std::string IosFunctions::getBundleIdentifier ()
{

    JniMethodInfo t;

    if (versionAndroid.length () > 0)
        return versionAndroid.c_str ();

    if (JniHelper::getStaticMethodInfo (t, CLASS_NAME_ACTIVITY, "getBundleVersion", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod (t.classID, t.methodID);
        IosFunctions::versionAndroid = JniHelper::jstring2string (str);
        t.env->DeleteLocalRef (t.classID);
        t.env->DeleteLocalRef (str);
        return IosFunctions::versionAndroid;
    }
    return "4.4";
}

std::string IosFunctions::getBundleVersion ()
{
    JniMethodInfo t;
    if (versionAndroid.length () > 0)
        return versionAndroid.c_str ();

    if (JniHelper::getStaticMethodInfo (t, CLASS_NAME_ACTIVITY, "getBundleVersion", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod (t.classID, t.methodID);
        IosFunctions::versionAndroid = JniHelper::jstring2string (str);
        t.env->DeleteLocalRef (t.classID);
        t.env->DeleteLocalRef (str);
        return IosFunctions::versionAndroid;
    }
    CCLog ("version 4.4");
    return "1.0";
}

std::string IosFunctions::getStoreVersion ()
{
    std::string storeVersion = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    storeVersion = "AppleAppStore";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    storeVersion = "GooglePlayStore";
#endif

#if FLEXION_BUILD
    storeVersion = "Flexion";
#endif
    return storeVersion;
}

bool IosFunctions::canOpenURL (const char *url)
{
    JniMethodInfo t;
    //    std::string str(url);
    //
    //    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME_ANDROIDFUNC, "canOpenURL","(Ljava/lang/String;)Z")) {
    //        jstring stringArg = t.env->NewStringUTF(str.c_str());
    //        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg);
    //        t.env->DeleteLocalRef(t.classID);
    //        return ret;
    //    }

    return true;
}

void IosFunctions::openURL (const char *url)
{
    JniMethodInfo t;
    std::string str (url);

    if (JniHelper::getStaticMethodInfo (t, CLASS_NAME_ANDROIDFUNC, "openURL", "(Ljava/lang/String;)V")) {
        jstring stringArg = t.env->NewStringUTF (str.c_str ());
        t.env->CallStaticVoidMethod (t.classID, t.methodID, stringArg);
        t.env->DeleteLocalRef (stringArg);
        t.env->DeleteLocalRef (t.classID);
    }
}

void IosFunctions::openAdjustURL (const char *url)
{
    JniMethodInfo t;
    std::string str (url);

    if (JniHelper::getStaticMethodInfo (t, CLASS_NAME_ANDROIDFUNC, "openAdjustURL", "(Ljava/lang/String;)V")) {
        jstring stringArg = t.env->NewStringUTF (str.c_str ());
        t.env->CallStaticVoidMethod (t.classID, t.methodID, stringArg);
        t.env->DeleteLocalRef (stringArg);
        t.env->DeleteLocalRef (t.classID);
    }
}

void IosFunctions::addToPastebard (const char *str)
{
    JniMethodInfo t;
    std::string text (str);
    if (JniHelper::getStaticMethodInfo (t, CLASS_NAME_ANDROIDFUNC, "copyToClipboard", "(Ljava/lang/String;)V")) {
        jstring stringArg = t.env->NewStringUTF (text.c_str ());
        t.env->CallStaticVoidMethod (t.classID, t.methodID, stringArg);
        t.env->DeleteLocalRef (stringArg);
        t.env->DeleteLocalRef (t.classID);
    }
}

std::string IosFunctions::getDeviceName ()
{
    //    return [[UIDevice currentDevice] name].UTF8String;

    JniMethodInfo methodInfo;
    jstring jstr;
    if (JniHelper::getStaticMethodInfo (methodInfo, CLASS_NAME_COCOS2DX_HELPER, "getDeviceModel",
                                        "()Ljava/lang/String;")) {
        jstr = (jstring)methodInfo.env->CallStaticObjectMethod (methodInfo.classID, methodInfo.methodID);
    }
    methodInfo.env->DeleteLocalRef (methodInfo.classID);

    const char *deviceModel = methodInfo.env->GetStringUTFChars (jstr, NULL);

    CCLOG ("DEVICE MODEL NAME = %s", deviceModel);

    string strDevModel = string (deviceModel);

    methodInfo.env->ReleaseStringUTFChars (jstr, deviceModel);
    methodInfo.env->DeleteLocalRef (jstr);

    CCLOG ("DEVICE MODEL NAME = %s (after release)", strDevModel.c_str ());

    return strDevModel.c_str ();
}

std::string IosFunctions::getDeviceModel ()
{
    //    return [[UIDevice currentDevice] name].UTF8String;

    JniMethodInfo methodInfo;
    jstring jstr;
    if (JniHelper::getStaticMethodInfo (methodInfo, CLASS_NAME_COCOS2DX_HELPER, "getDeviceModel",
                                        "()Ljava/lang/String;")) {
        jstr = (jstring)methodInfo.env->CallStaticObjectMethod (methodInfo.classID, methodInfo.methodID);
    }
    methodInfo.env->DeleteLocalRef (methodInfo.classID);

    const char *deviceModel = methodInfo.env->GetStringUTFChars (jstr, NULL);

    string strDevModel = string (deviceModel);

    methodInfo.env->ReleaseStringUTFChars (jstr, deviceModel);
    methodInfo.env->DeleteLocalRef (jstr);

    return strDevModel;
}

std::string IosFunctions::getSystemVersion ()
{
    //    return [[UIDevice currentDevice] name].UTF8String;

    JniMethodInfo methodInfo;
    jstring jstr;
    if (JniHelper::getStaticMethodInfo (methodInfo, CLASS_NAME_COCOS2DX_HELPER, "getSystemVersion",
                                        "()Ljava/lang/String;")) {
        jstr = (jstring)methodInfo.env->CallStaticObjectMethod (methodInfo.classID, methodInfo.methodID);
    }
    methodInfo.env->DeleteLocalRef (methodInfo.classID);

    const char *systemVersion = methodInfo.env->GetStringUTFChars (jstr, NULL);

    string strSystemVersion = string (systemVersion);

    methodInfo.env->ReleaseStringUTFChars (jstr, systemVersion);
    methodInfo.env->DeleteLocalRef (jstr);

    return strSystemVersion;
}

bool IosFunctions::addSkipBackupAttributeToItem (const char *path)
{
    //    NSURL *url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:path]];

    /*
     if (&NSURLIsExcludedFromBackupKey == nil) // < 5.0.1
     {
     const char* filePath = [[url path] fileSystemRepresentation];

     const char* attrName = "com.apple.MobileBackup";
     u_int8_t attrValue = 1;

     int result = setxattr(filePath, attrName, &attrValue, sizeof(attrValue), 0, 0);

     return result == 0;
     }
     else // > 5.1
     */
    //    {
    //        NSError *error = nil;
    //        bool success = [url setResourceValue: [NSNumber numberWithBool: YES]
    //                                      forKey: NSURLIsExcludedFromBackupKey
    //                                       error: &error];
    //        if (!success)
    //            NSLog(@"Error excluding %@ from backup %@", [url lastPathComponent], error);
    //
    //        return success;
    //    }
    //
    return true;
}

void IosFunctions::adjustInitialSubscriptionPurchase ()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, CLASS_NAME_ANDROIDFUNC, "reportAdjustFirstSubscriptionPurchase", "()V")) {
        t.env->CallStaticVoidMethod (t.classID, t.methodID);
        t.env->DeleteLocalRef (t.classID);
    }
}

void IosFunctions::adjustSubscriptionTrackRevenue (double amount)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, CLASS_NAME_ANDROIDFUNC, "reportAdjustSubscriptionTransaction", "(D)V")) {

        t.env->CallStaticVoidMethod (t.classID, t.methodID, amount);
        t.env->DeleteLocalRef (t.classID);
    }
}

void IosFunctions::adjustFirstPurchase ()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, CLASS_NAME_ANDROIDFUNC, "reportAdjustFirstPurchase", "()V")) {
        t.env->CallStaticVoidMethod (t.classID, t.methodID);
        t.env->DeleteLocalRef (t.classID);
    }
}

void IosFunctions::adjustTrackRevenue (double amount)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, CLASS_NAME_ANDROIDFUNC, "reportAdjustTransaction", "(D)V")) {

        t.env->CallStaticVoidMethod (t.classID, t.methodID, amount);
        t.env->DeleteLocalRef (t.classID);
    }
}

void IosFunctions::causeCrash (int crashId)
{
    //    if (crashId < 0) {
    //        crashId = rand() % 6;
    //    }
    //    switch (crashId) {
    //        case 0:
    //        {
    //            int *x = NULL; *x = 42;
    //        }
    //        case 1:
    //        {
    //            NSArray *array = @[@1, @2];
    //            [array objectAtIndex:1];
    //        }
    //        case 2:
    //        {
    //            throw "something's wrong";
    //        }
    //        case 3:
    //        {
    //            abort();
    //        }
    //        case 4:
    //        {
    //            assert(false);
    //        }
    //        default:
    //        {
    //            @throw NSInternalInconsistencyException;
    //        }
    //    }
}

std::string IosFunctions::getAdvertisingIdentifier ()
{
    //    NSString *adId = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
    //    adId = [[adId stringByReplacingOccurrencesOfString:@"-" withString:@""] lowercaseString];
    //
    //    return adId.UTF8String;
    return "";
}

void IosFunctions::setFabricParameter (const char *key, std::string value)
{
    //    set in StoryGame.java
}

void IosFunctions::setCostumFabricLog (const char *value)
{
    JniMethodInfo methodInfo;
    if (!JniHelper::getStaticMethodInfo (methodInfo, CLASS_NAME_ANDROIDFUNC, "setCostumFabricLog",
                                         "(Ljava/lang/String;)V")) {
        return;
    }

    jstring jmsg = methodInfo.env->NewStringUTF (("Custom log " + std::string (value)).c_str ());
    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID, jmsg);

    methodInfo.env->DeleteLocalRef (jmsg);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}

void IosFunctions::setFyberParameters ()
{

    JniMethodInfo methodInfo;
    if (!JniHelper::getStaticMethodInfo (methodInfo, CLASS_NAME_FYBER, "setFyberParameters", "(Ljava/lang/String;)V")) {
        return;
    }

    string moneySpent = std::to_string (GameState::get()->getUserPurchases()->getTotalMoneySpent());
    string daysPlayed = std::to_string (GameState::get ()->getDaysPlayed ());
    string playerLevel = std::to_string (GameState::get ()->getChaptersRead ());
    string numberOfSessions = std::to_string(GameState::get()->getSessionNumAll());


    jstring jparam = methodInfo.env->NewStringUTF ((moneySpent + ";" + daysPlayed + ";" + playerLevel + ";" + numberOfSessions).c_str ());

    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID, jparam);

    methodInfo.env->DeleteLocalRef (jparam);

    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}

void IosFunctions::setFyberGdprConsentGiven()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, CLASS_NAME_FYBER, "setFyberGdprConsentGiven", "()V")) {
        t.env->CallStaticVoidMethod (t.classID, t.methodID);
        t.env->DeleteLocalRef (t.classID);
    }
}

bool IosFunctions::isPushNotificationEnabled ()
{
    JniMethodInfo methodInfo;
    if (!JniHelper::getStaticMethodInfo (methodInfo, CLASS_NAME_ANDROIDFUNC, "isPushNotificationEnabled", "()Z")) {
        return true;
    }

    jboolean ret = methodInfo.env->CallStaticBooleanMethod (methodInfo.classID, methodInfo.methodID);

    bool pushEnabled = ret;

    methodInfo.env->DeleteLocalRef (methodInfo.classID);

    return pushEnabled;
}

/*void IosFunctions::NANTrackLogin(const char *username) {

}
void IosFunctions::NANTrackPurchase(const char *productId, double amountInUsd) {

}*/

void IosFunctions::showQuitGameDialog ()
{
    JniMethodInfo t;

    if (!JniHelper::getStaticMethodInfo (t, CLASS_NAME_ACTIVITY, "showQuitGameDialog", "()V")) {
        return;
    }

    t.env->CallStaticVoidMethod (t.classID, t.methodID);
    t.env->DeleteLocalRef (t.classID);
}

void IosFunctions::soolmaTagUser ()
{

    JniMethodInfo methodInfo;
    if (!JniHelper::getStaticMethodInfo (methodInfo, CLASS_NAME_ANDROIDFUNC, "soolmaTagUser",
                                         "(Ljava/lang/String;)V")) {
        return;
    }

    std::string moneySpent = std::to_string(GameState::get()->getUserPurchases()->getTotalMoneySpent());
    string chaptersRead = std::to_string (GameState::get ()->getChaptersRead ());

    jstring jparam = methodInfo.env->NewStringUTF ((moneySpent + ";" + chaptersRead).c_str ());


    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID, jparam);

    methodInfo.env->DeleteLocalRef (jparam);

    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}

void IosFunctions::requestReview ()
{
    // does nothing
}

bool IosFunctions::isIOSAtLeastVersion (const char *requiredVersion)
{
    // does nothing
    return false;
}

cocos2d::CCDictionary *IosFunctions::adjustParameters ()
{
    JniMethodInfo t;
    std::string extraParam;
    if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/NanoAndroidFunctions", "getNGSExtraParameters",
                                        "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod (t.classID, t.methodID);

        if (str)
            extraParam = JniHelper::jstring2string (str);
        t.env->DeleteLocalRef (t.classID);
        t.env->DeleteLocalRef (str);
    }

    auto attributes = CCJSONConverter::sharedConverter ()->dictionaryFrom (extraParam.c_str ());

    return attributes;
}

void IosFunctions::shareUserInvite (const char *link, const char *title, cocos2d::CCPoint worldPos)
{
    JniMethodInfo methodInfo;
    if (!JniHelper::getStaticMethodInfo (methodInfo, CLASS_NAME_ANDROIDFUNC, "shareUserInvite",
                                         "(Ljava/lang/String;Ljava/lang/String;)V")) {
        return;
    }

    jstring jparam1 = methodInfo.env->NewStringUTF (link);
    jstring jparam2 = methodInfo.env->NewStringUTF (title);

    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID, jparam1, jparam2);

    methodInfo.env->DeleteLocalRef (jparam1);
    methodInfo.env->DeleteLocalRef (jparam2);

    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}

std::string IosFunctions::getLocaleCountryCode()
{
    std::string ret;
    JniMethodInfo t;

    if (JniHelper::getStaticMethodInfo(t, "org/nanobit/mystory/StoryGame", "getCountryCode",
                                       "()Ljava/lang/String;")) {
        jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID);
        const char *str = t.env->GetStringUTFChars(retFromJava, 0);
        ret = str;

        t.env->ReleaseStringUTFChars(retFromJava, str);
        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}

bool IosFunctions::hasTopNotch()
{
    // TODO: check if device has notch
    return false;
}

int IosFunctions::getTopNotch()
{
    // TODO: get notch save area
    return 0;
}
