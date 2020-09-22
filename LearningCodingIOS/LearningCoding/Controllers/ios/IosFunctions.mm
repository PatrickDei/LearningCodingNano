//
//  IosFunctions.cpp
//  NanoGirl
//
//  Created by Matko Juribasic on 09/01/14.
//
//

#include "IosFunctions.h"

//#import <StoreKit/StoreKit.h>
#import "AppDelegate.h"
#import "AppController.h"
#import "FileHash.h"

bool IosFunctions::createDirectory(const char *path)
{
    NSString *dataPath = [NSString stringWithUTF8String:path];

    if (![[NSFileManager defaultManager] fileExistsAtPath:dataPath])
        return [[NSFileManager defaultManager] createDirectoryAtPath:dataPath
                                         withIntermediateDirectories:NO
                                                          attributes:nil
                                                               error:nullptr];
    else
        return true;
}

bool IosFunctions::removeFileOrDirectory(const char *path)
{
    NSString *dataPath = [NSString stringWithUTF8String:path];

    if ([[NSFileManager defaultManager] fileExistsAtPath:dataPath])
        return [[NSFileManager defaultManager] removeItemAtPath:dataPath error:nullptr];
    else
        return true;
}

bool IosFunctions::removeAllFilesInDirectory(const char *path)
{
    NSString *dirPath = [NSString stringWithUTF8String:path];

    NSArray *fileArray = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:dirPath error:nil];
    for (NSString *filename in fileArray)
        [[NSFileManager defaultManager] removeItemAtPath:[dirPath stringByAppendingPathComponent:filename]
                                                   error:nullptr];

    return true;
}

bool IosFunctions::copyItem(const char *fromPath, const char *toPath)
{
    NSString *fromPathStr = [NSString stringWithUTF8String:fromPath];
    NSString *toPathStr = [NSString stringWithUTF8String:toPath];

    // create backup file
    bool backupCreated = false;
    if ([[NSFileManager defaultManager] fileExistsAtPath:toPathStr]) {
        if ([[NSFileManager defaultManager] moveItemAtPath:toPathStr
                                                    toPath:[NSString stringWithFormat:@"%@.bak", toPathStr]
                                                     error:nullptr])
            backupCreated = true;
    }

    if ([[NSFileManager defaultManager] copyItemAtPath:fromPathStr toPath:toPathStr error:nullptr]) {
        if (backupCreated)
            // remove backup file
            [[NSFileManager defaultManager] removeItemAtPath:[NSString stringWithFormat:@"%@.bak", toPathStr]
                                                       error:nullptr];

        return true;
    } else {
        if (backupCreated)
            // revert backup file
            [[NSFileManager defaultManager] moveItemAtPath:[NSString stringWithFormat:@"%@.bak", toPathStr]
                                                    toPath:toPathStr
                                                     error:nullptr];

        return false;
    }
}

std::string IosFunctions::getBundleVersion()
{
    NSString *bundleVersionString = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];

    return bundleVersionString.UTF8String;
}

std::string IosFunctions::getStoreVersion()
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

bool IosFunctions::canOpenURL(const char *url)
{
    return [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];
}

void IosFunctions::openURL(const char *url)
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];
}

void IosFunctions::addToPastebard(const char *str)
{
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    pasteboard.string = [NSString stringWithUTF8String:str];
}

std::string IosFunctions::getDeviceName() { return [[UIDevice currentDevice] name].UTF8String; }

std::string IosFunctions::getDeviceModel() { return [[UIDevice currentDevice] model].UTF8String; }

std::string IosFunctions::getSystemVersion() { return [[UIDevice currentDevice] systemVersion].UTF8String; }

bool IosFunctions::addSkipBackupAttributeToItem(const char *path)
{
    NSURL *url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:path]];

    {
        NSError *error = nil;
        bool success =
            [url setResourceValue:[NSNumber numberWithBool:YES] forKey:NSURLIsExcludedFromBackupKey error:&error];
        if (!success)
            NSLog(@"Error excluding %@ from backup %@", [url lastPathComponent], error);

        return success;
    }

    return true;
}

void IosFunctions::causeCrash(int crashId)
{
    if (crashId < 0) {
        crashId = rand() % 6;
    }
    switch (crashId) {
        case 0: {
            int *x = NULL;
            *x = 42;
        }
        case 1: {
            NSArray *array = @[ @1, @2 ];
            [array objectAtIndex:1];
        }
        case 2: {
            throw "something's wrong";
        }
        case 3: {
            abort();
        }
        case 4: {
            assert(false);
        }
        default: {
            @throw NSInternalInconsistencyException;
        }
    }
}

bool IosFunctions::isPushNotificationEnabled()
{
    return false;
}

void IosFunctions::requestReview() {
    //[SKStoreReviewController requestReview];
}

bool IosFunctions::isIOSAtLeastVersion(const char *requiredVersion)
{
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    return [currSysVer compare:[NSString stringWithUTF8String:requiredVersion] options:NSNumericSearch] !=
           NSOrderedAscending;
}

void IosFunctions::shareUserInvite(const char *link, const char *title, cocos2d::CCPoint worldPos)
{
    NSArray *dataToShare =
        @[ [NSString stringWithUTF8String:link] ]; // ...or whatever pieces of data you want to share.

    UIActivityViewController *activityViewController =
        [[UIActivityViewController alloc] initWithActivityItems:dataToShare applicationActivities:nil];
    activityViewController.modalPresentationStyle = UIModalPresentationPopover;

    UIPopoverPresentationController *popController = activityViewController.popoverPresentationController;
    popController.sourceView = [UIApplication sharedApplication].keyWindow.rootViewController.view;
    popController.permittedArrowDirections = UIPopoverArrowDirectionDown;
    
    cocos2d::CCSize size = cocos2d::CCDirector::sharedDirector()->getWinSize();
    popController.sourceRect =
        CGRectMake(worldPos.x / 2, size.height / 2 - worldPos.y / 2, 0, 0); // width and height is apparently NOT important
    //we divide everything with two because of pixels, 1 pixel is two points

    [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:activityViewController
                                                                                 animated:YES
                                                                               completion:nil];
}

std::string IosFunctions::getLocaleCountryCode()
{
    NSLocale *currentLocale = [NSLocale currentLocale];
    std::string countryCode = [[currentLocale objectForKey:NSLocaleCountryCode] UTF8String];
    return countryCode;
}

void IosFunctions::registerForNotifications()
{
}

bool IosFunctions::hasTopNotch()
{
    // if #available(iOS 11.0, tvOS 11.0, *) {
    // with notch: 44.0 on iPhone X, XS, XS Max, XR.
    // without notch: 24.0 on iPad Pro 12.9" 3rd generation, 20.0 on iPhone 8 on iOS 12+.

    if (@available(iOS 11.0, *)) {
        return getTopNotch() > 24;
    }

    return false;
}

int IosFunctions::getTopNotch()
{
    // if #available(iOS 11.0, tvOS 11.0, *) {
    // with notch: 44.0 on iPhone X, XS, XS Max, XR.
    // without notch: 24.0 on iPad Pro 12.9" 3rd generation, 20.0 on iPhone 8 on iOS 12+.

    if (@available(iOS 11.0, *)) {
        UIWindow *window = UIApplication.sharedApplication.keyWindow;
        CGFloat topPadding = window.safeAreaInsets.top;
        return topPadding;
    }

    return 0;
}

int IosFunctions::getBottomNotch()
{
    // if #available(iOS 11.0, tvOS 11.0, *) {
    // with notch: 44.0 on iPhone X, XS, XS Max, XR.
    // without notch: 24.0 on iPad Pro 12.9" 3rd generation, 20.0 on iPhone 8 on iOS 12+.
    
    if (@available(iOS 11.0, *)) {
        UIWindow *window = UIApplication.sharedApplication.keyWindow;
        CGFloat bottomPadding = window.safeAreaInsets.bottom;
        return bottomPadding;
    }
    
    return 0;
}
