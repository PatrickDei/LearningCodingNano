//
//  AppController.h
//  LearningCoding
//
//  Created by Lana Lisjak on 18/05/16.
//  Copyright __MyCompanyName__ 2016. All rights reserved.
//

@class RootViewController;
#import <UserNotifications/UserNotifications.h>

@interface AppController : NSObject <UIAccelerometerDelegate, UIApplicationDelegate, UNUserNotificationCenterDelegate> {
    UIWindow *window;
    RootViewController *viewController;
}

+ (AppController *)sharedAppController;

@property (nonatomic, readonly)
    BOOL isSandboxed; // apps that are not downloaded from the store are sandboxed (TestFlight)
@property (nonatomic, readonly) BOOL isSimulator; // builds that are run in simulator, not sandboxed for some reason
@property (nonatomic, readonly) BOOL isNotDistribution; // not built with DISTRIBUTION defined

@property (nonatomic, readonly) BOOL isTestingVersion; // if any of the above is true, app is in testing mode
- (BOOL)isStoreVersion;                                // else app is in store version
- (void)initTestingFlags;

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *viewController;
@end
