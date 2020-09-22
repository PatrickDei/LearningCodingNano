//
//  IosFunctions.h
//  NanoGirl
//
//  Created by Matko Juribasic on 09/01/14.
//
//

#ifndef __NanoGirl__IosFunctions__
#define __NanoGirl__IosFunctions__

#include <string>
#include "cocos2d.h"

class IosFunctions
{
  public:
    static bool createDirectory(const char *path);
    static bool removeFileOrDirectory(const char *path);
    static bool removeAllFilesInDirectory(const char *path);
    static bool copyItem(const char *fromPath, const char *toPath);

    static std::string getBundleIdentifier();
    static std::string getBundleVersion();
    static std::string getStoreVersion();
    static std::string getDeviceName();
    static std::string getDeviceModel();
    static std::string getSystemVersion();

    static bool canOpenURL(const char *url);
    static void openURL(const char *url);
    static void openAdjustURL(const char *url);

    static void addToPastebard(const char *str);

    static bool addSkipBackupAttributeToItem(const char *path);

    static void causeCrash(int crashId = -1);

    static bool isPushNotificationEnabled();
    static void setCostumFabricLog(const char *value);
    static std::string versionAndroid;

    static void requestReview();

    static void shareUserInvite(const char *link, const char *title, cocos2d::CCPoint worldPos);

    static bool isIOSAtLeastVersion(const char *requiredVersion);

    static std::string getLocaleCountryCode();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    static void showQuitGameDialog();
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    static void registerForNotifications();
#endif
    
    static bool hasTopNotch ();
    static int getTopNotch ();
    static int getBottomNotch ();
    
    //    static void readUserDefaults(const char *plistPath);
    //    static bool copyAllItems(const char *fromPath, const char *toPath, const
    //    char *extension = nullptr);

    static cocos2d::CCDictionary *adjustParameters();
};

#endif /* defined(__NanoGirl__IosFunctions__) */
