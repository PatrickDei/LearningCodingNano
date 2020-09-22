
//
//  AppDelegate.h
//  LearningCoding
//
//  Created by Lana Lisjak on 18/05/16.
//  Copyright __MyCompanyName__ 2016. All rights reserved.
//

#ifndef _APP_DELEGATE_H_
#define _APP_DELEGATE_H_

#include "cocos2d.h"
#include "CCApplication.h"
#include "CCNative.h"

//#include "NotificationHandler.h"

#define kNotificationAppDelegateApplicationDidBecomeActive "a9d96c4c-5281-48ef-ae14-a99439ead088"
#define kNotificationAppDelegateApplicationWillResignActive "f1c9ba01-6f71-4a9c-8377-231967cdf8e9"
#define kNotificationAppDelegateApplicationDidFinishLaunching "885ccdae-498b-48c7-86eb-816a72b98908"
#define kNotificationAppDelegateApplicationWillTerminate "a42b78df-7e09-463c-8737-e9b0a01d6833"
#define kNotificationAppDelegateApplicationWillEnterForeground "3759d0d5-e6e6-4023-afcd-30e4eaf55f9e"
#define kNotificationAppDelegateApplicationDidEnterBackground "c6f2ce7a-d1c4-4b42-88a6-44154ed2b7b7"

class AppDelegate : private cocos2d::CCApplication
{
  public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
    virtual void applicationDidBecomeActive();
    virtual void applicationWillTerminate();
    virtual void applicationWillResignActive();
    
    void setSearchPaths();
};

#endif // _APP_DELEGATE_H_
