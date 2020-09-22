/*
//
//  KinesisController.cpp
//  NanoGirl
//
//  Created by Kristijan Biscanic on 18/4/16.
//
//

#include "KinesisController.h"

#include "JniHelper.h"
#include <jni.h>
#include "cocos2d.h"

#include "Constants.h"
#include "Stats.h"
#include "GameState.h"
#include "NGSDataManager.h"
#include "NGSTime.h"
#include "IosFunctions.h"
#include "FacebookCommunicator.h"

using namespace cocos2d;
using namespace std;

#define CLASS_NAME "org/nanobit/hollywood/KinesisManager"

void KinesisController::gameStarted()
{
    JniMethodInfo methodInfo;
    if (!JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "gameStarted", "()V")) {
        return;
    }

    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);

    methodInfo.env->DeleteLocalRef(methodInfo.classID);

    CCLOG("KinesisController: gameStarted");
}

void KinesisController::gameEnded()
{
    JniMethodInfo methodInfo;
    if (!JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "gameEnded", "()V")) {
        return;
    }

    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);

    methodInfo.env->DeleteLocalRef(methodInfo.classID);

    CCLOG("KinesisController: gameEnded");
}

void KinesisController::storyStarted(int storyId)
{
    JniMethodInfo methodInfo;
    if (!JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "storyStarted", "(I)V")) {
        return;
    }

    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, (jint) storyId);

    methodInfo.env->DeleteLocalRef(methodInfo.classID);

    CCLOG("KinesisController: storyStarted");
}

void KinesisController::storyCompleted(int storyId)
{
    JniMethodInfo methodInfo;
    if (!JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "storyCompleted", "(I)V")) {
        return;
    }

    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, (jint) storyId);

    methodInfo.env->DeleteLocalRef(methodInfo.classID);

    CCLOG("KinesisController: storyCompleted");
}

void KinesisController::purchaseCompleted(float priceInDollars, bool firstPurchaseB,
    int gemsPurchased, string purchaseType, string offerId, string offerName, string packageName, string transactionId)
{
    JniMethodInfo methodInfo;
    if (!JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "purchaseCompleted", "(FZI"
      "Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        return;
    }

    jfloat jPrice = (jfloat) priceInDollars;
    jboolean jFirstPurchase = (jboolean) firstPurchaseB;
    jint jGemsBought = (jint) gemsPurchased;
    jstring jPurchaseType = methodInfo.env->NewStringUTF(purchaseType.c_str());
    jstring jOfferId = methodInfo.env->NewStringUTF(offerId.c_str());
    jstring jOfferName = methodInfo.env->NewStringUTF(offerName.c_str());
    jstring jPackageName = methodInfo.env->NewStringUTF(packageName.c_str());
    jstring jTransactionId = methodInfo.env->NewStringUTF(transactionId.c_str());

    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jPrice, jFirstPurchase, jGemsBought, jPurchaseType, jOfferId, jOfferName, jPackageName, jTransactionId);

    methodInfo.env->DeleteLocalRef(jPackageName);
    methodInfo.env->DeleteLocalRef(jOfferName);
    methodInfo.env->DeleteLocalRef(jOfferId);
    methodInfo.env->DeleteLocalRef(jPurchaseType);
    methodInfo.env->DeleteLocalRef(jTransactionId);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

extern "C"{
    JNIEXPORT jstring JNICALL Java_org_nanobit_hollywood_KinesisManager_getAppVer(JNIEnv* env, jclass clazz)
    {
        return env->NewStringUTF(GameState::get()->getLatestInstalledVersion().c_str());
    }
    JNIEXPORT jstring JNICALL Java_org_nanobit_hollywood_KinesisManager_getInstallAppVer(JNIEnv* env, jclass clazz)
    {
        return env->NewStringUTF(GameState::get()->getFirstInstalledVersion().c_str());
    }
    JNIEXPORT jstring JNICALL Java_org_nanobit_hollywood_KinesisManager_getUdid(JNIEnv* env, jclass clazz)
    {
        return env->NewStringUTF(NGSDataManager::getUdid().c_str());
    }
    JNIEXPORT jdouble JNICALL Java_org_nanobit_hollywood_KinesisManager_getLtv(JNIEnv* env, jclass clazz)
    {
        return (jdouble)(GameState::get()->getProperties()->doubleForKey(kPropertyTotalMoneySpent) * 0.7);
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_updateEventNum(JNIEnv* env, jclass clazz)
    {
        auto gs = GameState::get();

        gs->setEventNum(gs->getEventNum() + 1);

        return (jint)(gs->getEventNum());
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getSessionNum(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getSessionNum());
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getSessionNumAll(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getSessionNumAll());
    }
    JNIEXPORT jdouble JNICALL Java_org_nanobit_hollywood_KinesisManager_getCurrentTimeServer(JNIEnv* env, jclass clazz)
    {
        return (jdouble)(NGSTime::currentTimeServer());
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getPlayerLevel(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getPlayerLevel());
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getCoinsAmount(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getPlayerMoney());
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getGemsAmount(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getPlayerGems());
    }
    JNIEXPORT jstring JNICALL Java_org_nanobit_hollywood_KinesisManager_getPlayerName(JNIEnv* env, jclass clazz)
    {
        return env->NewStringUTF(GameState::get()->getPlayerName().c_str());
    }
    JNIEXPORT jstring JNICALL Java_org_nanobit_hollywood_KinesisManager_getUsername(JNIEnv* env, jclass clazz)
    {
        return env->NewStringUTF(GameState::get()->getUsername().c_str());
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getBodyLevel(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getPlayerStats()->getStat(SKILL_1));
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getCharismaLevel(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getPlayerStats()->getStat(SKILL_2));
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getCreativityLevel(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getPlayerStats()->getStat(SKILL_4));
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getDramaLevel(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getPlayerStats()->getStat(SKILL_3));
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getFansNumber(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getPlayerStats()->getStat(FANS));
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getPlayerXp(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getPlayerXp());
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getConsecutiveDaysPlaying(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getProperties()->intForKey(kPropertyLoginCollectedDay));
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getDaysPlaying(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getDaysPlayed());
    }
    JNIEXPORT jboolean JNICALL Java_org_nanobit_hollywood_KinesisManager_getFbConnected(JNIEnv* env, jclass clazz)
    {
        return (jboolean)(FacebookCommunicator::get()->everConnected());
    }
    JNIEXPORT jdouble JNICALL Java_org_nanobit_hollywood_KinesisManager_getInstallTime(JNIEnv* env, jclass clazz)
    {
        return (jdouble)(GameState::get()->getProperties()->doubleForKey(kPropertyFirstGameStartTime));
    }
    JNIEXPORT jboolean JNICALL Java_org_nanobit_hollywood_KinesisManager_getGamePushEnabled(JNIEnv* env, jclass clazz)
    {
        return (jboolean)(GameState::get()->getProperties()->boolForKey(kPropertySettingsNotificationsOn));
    }
    JNIEXPORT jboolean JNICALL Java_org_nanobit_hollywood_KinesisManager_getDevicePushEnabled(JNIEnv* env, jclass clazz)
    {
        return (jboolean)(IosFunctions::isPushNotificationEnabled());
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getMoviesShot(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getPlayerStats()->getStat(COMPLETED_MOVIES));
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getStarsAmount(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getPlayerStarPoints());
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getNetWorth(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getNetWorth());
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getStatusId(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getPlayerStats()->getStat(STAR_STATUS));
    }
    JNIEXPORT jdouble JNICALL Java_org_nanobit_hollywood_KinesisManager_getSecSinceLastSession(JNIEnv* env, jclass clazz)
    {
        auto gs = GameState::get();

        double result = -1;

        if(gs->getLastSessionTime() > 0) //new players, and reinstallation, will set this to -1
            result = NGSTime::currentTimeServer() - gs->getLastSessionTime();

        gs->setLastSessionTime(NGSTime::currentTimeServer());

        return (jdouble) result;
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getTotalVideosWatched(JNIEnv* env, jclass clazz)
    {
        return (jint) CCUserDefault::sharedUserDefault()->getIntegerForKey(kUserDefaultTotalAdsWatched, 0);
    }
    JNIEXPORT jdouble JNICALL Java_org_nanobit_hollywood_KinesisManager_getSessionTime(JNIEnv* env, jclass clazz)
    {
        auto gs = GameState::get();

        double result = NGSTime::currentTimeServer() - gs->getLastSessionTime();

        gs->setLastSessionTime(NGSTime::currentTimeServer());

        return (jdouble) result;
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getCoinsSpentSession(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getMoneySpentInSession());
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getEnergySpentSession(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getEnergySpentInSession());
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getGemsSpentSession(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getGemsSpentInSession());
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getItemsSearchedSession(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getSearchesInSession());
    }
    JNIEXPORT jint JNICALL Java_org_nanobit_hollywood_KinesisManager_getStarsSpentSession(JNIEnv* env, jclass clazz)
    {
        return (jint)(GameState::get()->getStarsSpentInSession());
    }
}
*/
