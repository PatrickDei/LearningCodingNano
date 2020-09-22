//
//  AppController.mm
//  LearningCoding
//
//  Created by Lana Lisjak on 18/05/16.
//  Copyright __MyCompanyName__ 2016. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "AppController.h"
#import "AppDelegate.h"
#import "EAGLView.h"
#import "cocos2d.h"

#import "RootViewController.h"

@implementation AppController

@synthesize window;
@synthesize viewController;

static AppDelegate s_sharedApplication;

#pragma mark -
#pragma mark Application lifecycle
// cocos2d application instance

+ (AppController *)sharedAppController { return (AppController *)[[UIApplication sharedApplication] delegate]; }

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [self initTestingFlags];

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    EAGLView *__glView = [EAGLView viewWithFrame:[window bounds]
                                     pixelFormat:kEAGLColorFormatRGBA8
                                     depthFormat:GL_DEPTH24_STENCIL8_OES // GL_DEPTH_COMPONENT16
                              preserveBackbuffer:NO
                                      sharegroup:nil
                                   multiSampling:NO
                                 numberOfSamples:0];

    // Use RootViewController manage EAGLView
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.extendedLayoutIncludesOpaqueBars = YES;
    viewController.view = __glView;

    // Set RootViewController to window
    if ([[UIDevice currentDevice].systemVersion floatValue] < 6.0) {
        // warning: addSubView doesn't work on iOS6
        [window addSubview:viewController.view];
    } else {
        // use this method on ios6
        [window setRootViewController:viewController];
    }

    [window makeKeyAndVisible];

    [UIApplication sharedApplication].statusBarHidden = YES;

    cocos2d::CCApplication::sharedApplication()->run();
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state.
     This can occur for certain types of temporary interruptions (such as an
     incoming phone call or SMS message) or when the user quits the application
     and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down
     OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();
    cocos2d::CCApplication::sharedApplication()->applicationWillResignActive();
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    /*
     Restart any tasks that were paused (or not yet started) while the application
     was inactive. If the application was previously in the background, optionally
     refresh the user interface.
     */
    cocos2d::CCDirector::sharedDirector()->resume();
    cocos2d::CCApplication::sharedApplication()->applicationDidBecomeActive();
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    /*
     Use this method to release shared resources, save user data, invalidate
     timers, and store enough application state information to restore your
     application to its current state in case it is terminated later.
     If your application supports background execution, called instead of
     applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
    [[NSNotificationCenter defaultCenter] postNotificationName:@kNotificationAppDelegateApplicationDidEnterBackground
                                                        object:nil];
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    /*
     Called as part of  transition from the background to the inactive state: here
     you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    cocos2d::CCApplication::sharedApplication()->applicationWillTerminate();
}


#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{

    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk)
     later.
     */
    cocos2d::CCDirector::sharedDirector()->purgeCachedData();
}


#pragma mark - Detecting Test Sessions

- (void)initTestingFlags
{
    _isSandboxed = [self isApplicationSandboxed];
#ifdef DISTRIBUTION
    _isNotDistribution = NO;
#else
    _isNotDistribution = YES;
#endif

#if TARGET_IPHONE_SIMULATOR
    _isSimulator = YES;
#else
    _isSimulator = NO;
#endif

    _isTestingVersion = _isSandboxed || _isSimulator || _isNotDistribution;

    if (_isTestingVersion) {
        NSLog(@"-- Application is in TESTING mode --");
    } else {
        NSLog(@"-- Application is in STORE mode --");
    }
}

- (BOOL)isApplicationSandboxed
{
    // [NSBundle appStoreReceiptURL] is only available in iOS 7 and up
    @try {
        if (floor(NSFoundationVersionNumber) >= NSFoundationVersionNumber_iOS_7_0) {
            NSURL *receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
            NSString *receiptURLString = [receiptURL path];
            if ([receiptURLString rangeOfString:@"sandboxReceipt"].location != NSNotFound) {
                NSLog(@"-- Application is SANDBOXED --");
                return YES;
            }
            return NO;
        }
    } @catch (NSException *exception) {
        NSLog(@"Warning: appStoreReceiptURL not supported");
    }

    return [self isApplicationEmbeded]; // if it failed, try this
}

- (BOOL)isApplicationEmbeded
{
    if ([[NSBundle mainBundle] pathForResource:@"embedded" ofType:@"mobileprovision"]) {
        NSLog(@"-- Application is EMBEDDED --");
        return YES;
    }
    return NO;
}

- (BOOL)isStoreVersion { return !_isTestingVersion; }

@end
