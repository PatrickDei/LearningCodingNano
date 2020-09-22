//
//  FyberWrapper.cpp
//  NanoGirl
//
//  Created by Dino Milačić on 01/12/14.
//
//

#include "FyberWrapper.h"
#include "NGSDataManager.h"
#include "GameScene.h"
#include "NanoIdentifierWrapper.h"
#include "SoundManager.h"
#include "NanoExtensions.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "CCNative.h"
#include <math.h>
#include "AdController.h"
#include "InAppPurchaseManager.h"
#include "NGSTime.h"
#include "InAppMenu.h"

using namespace cocos2d;

extern "C" {
JNIEXPORT void JNICALL Java_org_nanobit_mystory_FyberController_setVideoAvailable (JNIEnv *env, jobject thiz,
                                                                                   jboolean available)
{
    FyberWrapper::setRewardedVideoAvailable (available);
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_FyberController_rewardVideoWatched (JNIEnv *env, jobject thiz)
{
    FyberWrapper::rewardedVideoWatched ();
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_FyberController_setInterstitialAvailable (JNIEnv *env, jobject thiz,
                                                                                          jboolean available)
{
    FyberWrapper::setInterstitialAvailable (available);
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_FyberController_interstitialWatched (JNIEnv *env, jobject thiz)
{
    FyberWrapper::interstitialWatched ();
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_FyberController_offerWallClosed (JNIEnv *env, jobject thiz)
{
    // FyberWrapper::offerWallClosed();
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_StoryGame_rewardVideoEnded (JNIEnv *env, jobject thiz)
{
    if (AppDelegate::getInstance()) {
        int hasUncollectedRewrds =
                cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(
                        kUserDefaultUncollectedFyberRewards);
        // collect
        if (hasUncollectedRewrds > AdController::RewardType::NONE) {
            AdController::rewardedVideoWatched("Fyber");
            cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(
                    kUserDefaultUncollectedFyberRewards,
                    AdController::RewardType::NONE);
        }
        FyberWrapper::isRewardVideoShowing = false;
        FyberWrapper::get()->scheduleVideoRequest();
    }
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_StoryGame_rewardVideoDismissed (JNIEnv *env, jobject thiz)
{
    if (AppDelegate::getInstance()) {
        FyberWrapper::get()->scheduleVideoRequest();
        FyberWrapper::isRewardVideoShowing = false;

        cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification(
                kNotificationRefreshInAppMenuFreeGems);
    }
}
}

#define CLASS_NAME "org/nanobit/mystory/FyberController"
static bool getJNIStaticMethodInfo (cocos2d::JniMethodInfo &methodinfo, const char *methodName, const char *paramCode)
{
    return cocos2d::JniHelper::getStaticMethodInfo (methodinfo, CLASS_NAME, methodName, paramCode);
}

FyberWrapper *FyberWrapper::instance = nullptr;
bool FyberWrapper::sVideoAvailable = false;
bool FyberWrapper::sInterstitialAvailable = false;
bool FyberWrapper::isRewardVideoShowing = false;
bool FyberWrapper::isInterstitialShowing = false;
double FyberWrapper::videoRequestEndTime = 0;
double FyberWrapper::interstitialRequestEndTime = 0;

FyberWrapper *FyberWrapper::get ()
{
    if (instance == nullptr) {
        instance = new FyberWrapper ();
    }

    return instance;
}

FyberWrapper::FyberWrapper () {
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FyberWrapper::appDidEnterBackground), kNotificationAppDelegateApplicationDidEnterBackground,
                                                                  nullptr);
#if DISABLE_ADS == 0
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FyberWrapper::onlineStatusChanged), kAppOfflineStatusChanged,
                                                                  nullptr);
#endif
};

FyberWrapper::~FyberWrapper () { CCDirector::sharedDirector ()->getScheduler ()->unscheduleAllForTarget (this); };

void FyberWrapper::init ()
{

    std::string userId = NanoIdentifierWrapper::uuidForDevice ();

    cocos2d::JniMethodInfo methodInfo;
    if (!getJNIStaticMethodInfo (methodInfo, "initFyber", "(Ljava/lang/String;)V")) {
        return;
    }

    jstring userIdArg = methodInfo.env->NewStringUTF (userId.c_str ());
    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID, userIdArg);
    methodInfo.env->DeleteLocalRef (userIdArg);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}

void FyberWrapper::start ()
{
#if DISABLE_ADS == 0
    CCDirector::sharedDirector ()->getScheduler ()->unscheduleSelector (
        schedule_selector (FyberWrapper::scheduleVideoRequest), this);
    CCDirector::sharedDirector ()->getScheduler ()->scheduleSelector (
        schedule_selector (FyberWrapper::scheduleVideoRequest), this, 0.0f, 0, SECONDS_MIN_DELAY, false);
#endif
}

void FyberWrapper::appDidEnterBackground ()
{
    CCDirector::sharedDirector ()->getScheduler ()->unscheduleSelector (
        schedule_selector (FyberWrapper::scheduleVideoRequest), this);
}

void FyberWrapper::onlineStatusChanged() {
    if (!isVideoAvailable() && !AppDelegate::getInstance ()->isGameOffline () && videoRequestEndTime > 0) {
       scheduleVideoRequest();
    } else {
        CCDirector::sharedDirector ()->getScheduler ()->unscheduleSelector(schedule_selector(FyberWrapper::scheduleVideoRequest), this);
    }

    cocos2d::CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationRefreshInAppMenuFreeGems);
}


void FyberWrapper::scheduleVideoRequest () {
    CCDirector::sharedDirector ()->getScheduler ()->unscheduleSelector(schedule_selector(FyberWrapper::scheduleVideoRequest), this);

    double timePassedSinceLastVideoRequest = NGSTime::currentTimeServer () - videoRequestEndTime;
    
    int threshold = requestDelay ();
    
    if (timePassedSinceLastVideoRequest >= threshold) {
        requestVideo ();
    } else {
        double dt = threshold - timePassedSinceLastVideoRequest;
        CCDirector::sharedDirector ()->getScheduler ()->scheduleSelector (
            schedule_selector (FyberWrapper::scheduleVideoRequest), this, 0.0f, 0, dt, false);
    }
}

bool FyberWrapper::shouldRequestVideo ()
{
    bool retVal = false;

    double timePassedSinceLastVideoRequest = NGSTime::currentTimeServer () - videoRequestEndTime;
    int threshold = requestDelay ();

    if (timePassedSinceLastVideoRequest >= threshold) {
        retVal = true;
    }

    return retVal;
}

void FyberWrapper::playVideo ()
{
    cocos2d::JniMethodInfo methodInfo;

    if (!getJNIStaticMethodInfo (methodInfo, "playFyberVideo", "()V")) {
        return;
    }

    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);
    FyberWrapper::isRewardVideoShowing = true;
}

void FyberWrapper::setRewardedVideoAvailable (bool value)
{
    FyberWrapper::sVideoAvailable = value;

    cocos2d::CCNotificationCenter::sharedNotificationCenter ()->postNotification (
        kNotificationRefreshInAppMenuFreeGems);
}

void FyberWrapper::setInterstitialAvailable (bool value)
{
    FyberWrapper::sInterstitialAvailable = value;
}

void FyberWrapper::rewardedVideoWatched ()
{
    AdController::rewardedVideoWatched ("Fyber");
    FyberWrapper::get ()->scheduleVideoRequest ();
}

bool FyberWrapper::isVideoAvailable ()
{
    GameState *gameState = GameState::get ();
    if (gameState == nullptr || AppDelegate::getInstance()->isGameOffline()) {
        return false;
    }

    // JNI part
    cocos2d::JniMethodInfo methodInfo;
    if (!getJNIStaticMethodInfo (methodInfo, "isVideoAvailable", "()Z")) {
        return false;
    }
    jboolean jniIsAvailable = methodInfo.env->CallStaticBooleanMethod (methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);

    FyberWrapper::sVideoAvailable = jniIsAvailable;

    return sVideoAvailable;
}

void FyberWrapper::requestVideo ()
{
    if (shouldRequestVideo ()) {
        CCLOG ("FyberWrapper::requestVideo");

        bool fyberServerReward = Parameters::getParameters ()->boolForKey ("fyberServerReward");
        cocos2d::JniMethodInfo methodInfo;
        GameState *gameState = GameState::get ();
        if (gameState == nullptr)
            return;
        if (!getJNIStaticMethodInfo (methodInfo, "requestFyberVideo", "(Z)V")) {
            return;
        }
        methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID, fyberServerReward);
        methodInfo.env->DeleteLocalRef (methodInfo.classID);
        double currentTime = NGSTime::currentTimeServer ();
        videoRequestEndTime = currentTime;
        get ()->scheduleVideoRequest ();
    }
}

#pragma mark - Interstitials

bool FyberWrapper::shouldRequestInterstitial ()
{
    bool retVal = false;

    double timePassedSinceLastVideoRequest = NGSTime::currentTimeServer () - interstitialRequestEndTime;
    double threshold = requestDelay ();

    if (timePassedSinceLastVideoRequest >= threshold) {
        retVal = true;
    }

    return retVal;
}

void FyberWrapper::showInterstitial ()
{
    FyberWrapper::isInterstitialShowing = true;

    cocos2d::JniMethodInfo methodInfo;

    if (!getJNIStaticMethodInfo (methodInfo, "showFyberInterstitial", "()V")) {
        return;
    }

    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}

void FyberWrapper::requestInterstitial ()
{
    if (shouldRequestInterstitial ()) {
        CCLOG ("FyberWrapper::requestInterstitial");

        cocos2d::JniMethodInfo methodInfo;
        GameState *gameState = GameState::get ();
        if (gameState == nullptr)
            return;

        if (!getJNIStaticMethodInfo (methodInfo, "requestFyberInterstitial", "()V")) {
            return;
        }

        methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef (methodInfo.classID);

        double currentTime = NGSTime::currentTimeServer ();
        interstitialRequestEndTime = currentTime;
        GameState::get ()->setInterstitialRequestEndTime (interstitialRequestEndTime);
    }
}

bool FyberWrapper::isInterstitialAvailable ()
{
    GameState *gameState = GameState::get ();
    if (gameState == nullptr || AppDelegate::getInstance()->isGameOffline()) {
        return false;
    }

    // JNI part
    cocos2d::JniMethodInfo methodInfo;
    if (!getJNIStaticMethodInfo (methodInfo, "isInterstitialAvailable", "()Z")) {
        return false;
    }
    jboolean jniIsAvailable = methodInfo.env->CallStaticBooleanMethod (methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);

    FyberWrapper::sInterstitialAvailable = jniIsAvailable;

    return sInterstitialAvailable;
}

void FyberWrapper::interstitialWatched ()
{
    FyberWrapper::isInterstitialShowing = false;
    AdController::interstitialWatched ();
}

#pragma mark - Offerwall

void FyberWrapper::showOfferWall ()
{
    cocos2d::JniMethodInfo methodInfo;

    if (!getJNIStaticMethodInfo (methodInfo, "runFyberOfferWall", "()V")) {
        return;
    }

    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}

#pragma mark - Parameters

int FyberWrapper::requestDelay ()
{
    int delay = (AppDelegate::getInstance() && Parameters::get()) ? Parameters::getParameters ()
            ->intForKey ("fyberRequestDelay")
                                                 : FYBER_REQUEST_DELAY;
        return MAX(FYBER_REQUEST_DELAY_MIN, delay);
}