//
//  FyberWrapper.h
//  NanoGirl
//
//  Created by Dino Milačić on 01/12/14.
//
//

#ifndef __NanoGirl__FyberWrapper__
#define __NanoGirl__FyberWrapper__

#include <stdio.h>
#include "CCObject.h"

#define kNotificationFyberCommunicatorIsVideoAvailable "notificationFyberCommunicatorIsVideoAvailable"

class FyberWrapper : public  cocos2d::CCObject
{
  public:

    static FyberWrapper * get ();
    static void init ();

    void  start ();
    void scheduleVideoRequest ();

    void appDidEnterBackground ();
    void onlineStatusChanged ();

    static bool sVideoAvailable;
    static bool sInterstitialAvailable;
    static double videoRequestEndTime;
    static double interstitialRequestEndTime;

    static bool isRewardVideoShowing;
    static bool isInterstitialShowing;

    static void setRewardedVideoAvailable (bool value);
    static void setInterstitialAvailable (bool value);
    static bool isVideoAvailable ();
    static void requestVideo ();
    static void playVideo ();
    static void rewardedVideoWatched ();

    static bool isInterstitialAvailable ();
    static void showInterstitial ();
    static void requestInterstitial ();
    static void interstitialWatched ();

    static void showOfferWall ();

private:
    FyberWrapper ();
    ~FyberWrapper ();

    static bool shouldRequestVideo ();

    static FyberWrapper *instance;

    static bool shouldRequestInterstitial ();

    static int requestDelay ();
};

#endif /* defined(__NanoGirl__FyberWrapper__) */
