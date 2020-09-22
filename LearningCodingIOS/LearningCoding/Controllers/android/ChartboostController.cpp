//
// Created by Dominik Baričević on 18/10/2018.
//

#include "ChartboostController.h"
#include "AdController.h"
#include "Constants.h"
#include "SoundManager.h"
#include "GameState.h"

#include "platform/android/jni/JniHelper.h"
#include <jni.h>

using namespace cocos2d;
using namespace std;

#define  CLASS_NAME "org/nanobit/mystory/ChartboostController"

extern "C"
{
JNIEXPORT void JNICALL Java_org_nanobit_mystory_ChartboostController_SetVideoAvailable(JNIEnv*  env, jobject thiz, jstring location, jboolean available)
{
    std::string type = JniHelper::jstring2string(location);
    if (type == CROSS_PROMO_VIDEO)
    {
        ChartboostController::isVideoAvailable = available;
    }
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_ChartboostController_WillDisplayVideo(JNIEnv*  env, jobject thiz, jstring location)
{
    std::string type = JniHelper::jstring2string(location);
    if (type == CROSS_PROMO_VIDEO)
    {
        SoundManager::get()->pauseBackgroundMusic();
    }
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_ChartboostController_DidCompleteRewardedVideo(JNIEnv*  env, jobject thiz, jstring location)
{
    std::string type = JniHelper::jstring2string(location);
    if (type == CROSS_PROMO_VIDEO)
    {
        ChartboostController::isVideoAvailable = false;

        //cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationRefreshInAppMenu);
        cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(kUserDefaultUncollectedAdRewards, 1);
        AdController::rewardedVideoWatched("Chartboost");
        ChartboostController::resumeMusic();
        ChartboostController::checkHasVideo();
    }
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_ChartboostController_DidDismissRewardedVideo(JNIEnv*  env, jobject thiz, jstring location)
{
    std::string type = JniHelper::jstring2string(location);
    if (type == CROSS_PROMO_VIDEO)
    {
        ChartboostController::resumeMusic();
        ChartboostController::checkHasVideo();
    }
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_ChartboostController_DidCloseRewardedVideo(JNIEnv*  env, jobject thiz, jstring location)
{
    std::string type = JniHelper::jstring2string(location);
    if (type == CROSS_PROMO_VIDEO)
    {
        ChartboostController::resumeMusic();
        ChartboostController::checkHasVideo();
    }
}
}

bool ChartboostController::isVideoAvailable = false;

void ChartboostController::cacheVideo()
{
    JniMethodInfo methodInfo;

    if (!JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "CacheRewardedVideo", "(Ljava/lang/String;)V")) {
        return;
    }

    jstring stringArg = methodInfo.env->NewStringUTF(CROSS_PROMO_VIDEO);
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    methodInfo.env->DeleteLocalRef(stringArg);
}

bool ChartboostController::hasVideo()
{
    return ChartboostController::isVideoAvailable;
}

void ChartboostController::showCrossPromo()
{
    JniMethodInfo methodInfo;

    if (!JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "ShowRewardedVideo", "(Ljava/lang/String;)V")) {
        return;
    }

    jstring stringArg = methodInfo.env->NewStringUTF(CROSS_PROMO_VIDEO);
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    methodInfo.env->DeleteLocalRef(stringArg);
}

void ChartboostController::showInterstitial()
{
    JniMethodInfo methodInfo;

    if (!JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "ShowInterstitial", "(Ljava/lang/String;)V")) {
        return;
    }

    jstring stringArg = methodInfo.env->NewStringUTF(CROSS_PROMO_INTERSTITIAL);
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    methodInfo.env->DeleteLocalRef(stringArg);
}

void ChartboostController::resumeMusic()
{
    GameState* gs = GameState::get();
    if (gs != nullptr && gs->getProperties()->intForKey(kPropertySettingsMusicOn)) {
        SoundManager::get()->resumeBackgroundMusic();
    }
}

void ChartboostController::checkHasVideo()
{
    JniMethodInfo methodInfo;

    if (!JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "CheckHasVideo", "(Ljava/lang/String;)V")) {
        return;
    }

    jstring stringArg = methodInfo.env->NewStringUTF(CROSS_PROMO_VIDEO);
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    methodInfo.env->DeleteLocalRef(stringArg);
}

void ChartboostController::hasInterstitial()
{

}

