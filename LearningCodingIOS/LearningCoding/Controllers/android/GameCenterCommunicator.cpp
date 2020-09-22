//
//  GameCenterCommunicator.mm
//  NanoGirl
//
//  Created by Matko Juribasic on 11/14/13.
//
//

#include "GameCenterCommunicator.h"

#include "AppDelegate.h"
#include "CCNotificationCenter.h"
#include "GameState.h"
#include "NGSTime.h"
#include "SaveGameController.h"
#include "value.h"

#include <jni.h>
#include "JniHelper.h"

CODING_DEFINE (GameCenterCommunicator);

extern "C" {

JNIEXPORT void JNICALL Java_org_nanobit_mystory_NAGoogleGamesCommunicator_googleApiConnected (JNIEnv *env, jobject thiz,
                                                                                              jstring playerID,
                                                                                              jstring playerName)
{
    GameCenterCommunicator *gc = GameCenterCommunicator::get ();
    if (gc)
        GameCenterCommunicator::get ()->googleApiConnectCallback (true, JniHelper::jstring2string (playerID),
                                                                  JniHelper::jstring2string (playerName));
    env->DeleteLocalRef (playerID);
    env->DeleteLocalRef (playerName);
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_NAGoogleGamesCommunicator_googleApiConnectFailed (JNIEnv *env,
                                                                                                  jobject thiz)
{
    GameCenterCommunicator *gc = GameCenterCommunicator::get ();
    if (gc)
        gc->googleApiConnectCallback (false, "", "");
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_NAGoogleGamesCommunicator_friendsFetched (JNIEnv *env, jobject thiz,
                                                                                          jboolean isSuccess)
{
    GameCenterCommunicator::get ()->friendsFetched (isSuccess);
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_NAGoogleGamesCommunicator_addFriendToMap (JNIEnv *env, jobject thiz,
                                                                                          jstring friendId,
                                                                                          jstring friendName)
{
    std::string str1 = JniHelper::jstring2string (friendId);
    std::string str2 = JniHelper::jstring2string (friendName);
    GameCenterCommunicator::get ()->addFriendToMap (str1, str2);
    env->DeleteLocalRef (friendId);
    env->DeleteLocalRef (friendName);
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_NAGoogleGamesCommunicator_signedOut (JNIEnv *env, jobject thiz,
                                                                                     jboolean isSuccess)
{
    CCNotificationCenter::sharedNotificationCenter ()->postNotification (
        kNotificationGameCenterCommunicatorRefreshCompleted, CCBool::create (false));
}
}

bool GameCenterCommunicator::authanticatingGPG = false;

#pragma mark - Init

GameCenterCommunicator *GameCenterCommunicator::get ()
{
    if (AppDelegate::getInstance () == nullptr || GameState::get () == nullptr) {
        return nullptr;
    }
    return GameState::get ()->getGameCenterCommunicator ();
}

GameCenterCommunicator::~GameCenterCommunicator ()
{
    CCNotificationCenter::sharedNotificationCenter ()->removeAllObservers (this);
}

const std::string GameCenterCommunicator::getGameCenterId ()
{
    std::string gameId;
    if (GameState::get ()) {
        gameId = GameCenterCommunicator::get ()->gamecenterId;
    } else {
        gameId = CCUserDefault::sharedUserDefault ()->getStringForKey (GAME_CENTER_ID);
    }

    return gameId;
}

bool GameCenterCommunicator::isAuthenticated ()
{
    JniMethodInfo t;
    bool isConnected = false;
    jobject myNAGGC = nullptr;
    if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/StoryGame", "getNaGoogleGamesCommunicator",
                                        "()Lorg/nanobit/mystory/NAGoogleGamesCommunicator;")) {
        myNAGGC = t.env->CallStaticObjectMethod (t.classID, t.methodID);
        t.env->DeleteLocalRef (t.classID);
    }
    if (JniHelper::getMethodInfo (t, "org/nanobit/mystory/NAGoogleGamesCommunicator", "getIsConnected", "()Z")) {
        isConnected = t.env->CallBooleanMethod (myNAGGC, t.methodID);
        t.env->DeleteLocalRef (myNAGGC);
        t.env->DeleteLocalRef (t.classID);
    }

    return isConnected;
}

#pragma mark - Game center functions

void GameCenterCommunicator::authenticatePlayer (bool allowNewLogin)
{
    if (authanticatingGPG)
        return;

    if (isAuthenticated ()) {
        // already authenticated
        if (refreshInProgress)
            fetchFriends ();
        else
            CCNotificationCenter::sharedNotificationCenter ()->postNotification (
                kNotificationGameCenterCommunicatorAuthenticated, CCBool::create (true));

        reportSavedAchievements ();

        return;
    } else if (allowNewLogin == false && !isSignedIn) {
        // don't authenticate
        if (refreshInProgress)
            CCNotificationCenter::sharedNotificationCenter ()->postNotification (
                kNotificationGameCenterCommunicatorRefreshCompleted, CCBool::create (false));
        else
            CCNotificationCenter::sharedNotificationCenter ()->postNotification (
                kNotificationGameCenterCommunicatorAuthenticated, CCBool::create (false));
    } else {
        authanticatingGPG = true;

        JniMethodInfo t;
        jobject myNAGGC;
        if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/StoryGame", "getNaGoogleGamesCommunicator",
                                            "()Lorg/nanobit/mystory/NAGoogleGamesCommunicator;")) {
            myNAGGC = t.env->CallStaticObjectMethod (t.classID, t.methodID);
            t.env->DeleteLocalRef (t.classID);
        }
        if (JniHelper::getMethodInfo (t, "org/nanobit/mystory/NAGoogleGamesCommunicator", "loginPlayer", "()V")) {
            t.env->CallVoidMethod (myNAGGC, t.methodID);
            t.env->DeleteLocalRef (myNAGGC);
            t.env->DeleteLocalRef (t.classID);
        }
    }
}

bool GameCenterCommunicator::getAuthInProgress () { return GameCenterCommunicator::authanticatingGPG; }

void GameCenterCommunicator::googleApiConnectCallback (bool isSuccess, std::string playerID, std::string playerName)
{
    CCLOG ("GCC: googleApiConnectCallback");
    authanticatingGPG = false;
    if (isSuccess) {
        CCLOG ("GCC: Success!");
        isSignedIn = true;

        std::string newName = playerName;
        std::string newGooglePlayId = playerID;

        CCLOG ("name %s =", newName.c_str ());
        CCLOG ("id %s =", newGooglePlayId.c_str ());

        if (newName != name || newGooglePlayId != googlePlayGamesId)
            shouldUpdate = true;

        googlePlayGamesId = newGooglePlayId;
        name = newName;
        SaveGameController::get ()->save ();

        if (refreshInProgress)
            fetchFriends ();
        else
            CCNotificationCenter::sharedNotificationCenter ()->postNotification (
                kNotificationGameCenterCommunicatorAuthenticated, CCBool::create (true));

        reportSavedAchievements ();
    } else {
        CCLOG ("GCC: NOT Success!");
        isSignedIn = false;
        if (refreshInProgress)
            CCNotificationCenter::sharedNotificationCenter ()->postNotification (
                kNotificationGameCenterCommunicatorRefreshCompleted, CCBool::create (false));
        else
            CCNotificationCenter::sharedNotificationCenter ()->postNotification (
                kNotificationGameCenterCommunicatorAuthenticated, CCBool::create (false));
    }
}

void GameCenterCommunicator::signOut ()
{
    JniMethodInfo t;
    jobject myNAGGC;

    if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/StoryGame", "getNaGoogleGamesCommunicator",
                                        "()Lorg/nanobit/mystory/NAGoogleGamesCommunicator;")) {
        myNAGGC = t.env->CallStaticObjectMethod (t.classID, t.methodID);
        t.env->DeleteLocalRef (t.classID);
    }
    if (JniHelper::getMethodInfo (t, "org/nanobit/mystory/NAGoogleGamesCommunicator", "signOut", "()V")) {
        tempFriendsBuffer.clear ();
        t.env->CallVoidMethod (myNAGGC, t.methodID);
        t.env->DeleteLocalRef (myNAGGC);
        t.env->DeleteLocalRef (t.classID);
    }
    isSignedIn = false;
}

void GameCenterCommunicator::fetchFriends ()
{
    JniMethodInfo t;
    jobject myNAGGC;

    if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/StoryGame", "getNaGoogleGamesCommunicator",
                                        "()Lorg/nanobit/mystory/NAGoogleGamesCommunicator;")) {
        myNAGGC = t.env->CallStaticObjectMethod (t.classID, t.methodID);
        t.env->DeleteLocalRef (t.classID);
    }
    if (JniHelper::getMethodInfo (t, "org/nanobit/mystory/NAGoogleGamesCommunicator", "fetchFriends", "()V")) {
        tempFriendsBuffer.clear ();
        t.env->CallVoidMethod (myNAGGC, t.methodID);
        t.env->DeleteLocalRef (myNAGGC);
        t.env->DeleteLocalRef (t.classID);
    }
}

void GameCenterCommunicator::friendsFetched (bool isSuccess)
{
    if (!isSuccess) {
        if (refreshInProgress)
            CCNotificationCenter::sharedNotificationCenter ()->postNotification (
                kNotificationGameCenterCommunicatorRefreshCompleted, CCBool::create (false));
        else
            CCNotificationCenter::sharedNotificationCenter ()->postNotification (
                kNotificationGameCenterCommunicatorFriendsFetched, CCBool::create (false));
        return;
    } else if (tempFriendsBuffer.size () > 0) {
        std::map<std::string, std::string>::iterator it;

        for (it = tempFriendsBuffer.begin (); it != tempFriendsBuffer.end (); it++) {
            if (friends.find (it->first) == friends.end ()) {
                newGooglePlayFriendsIds.push_back (it->first);
            }
        }

        if (newGooglePlayFriendsIds.size () > 0)
            shouldUpdate = true;

        friends = tempFriendsBuffer;
        SaveGameController::get ()->save ();
    }

    if (refreshInProgress)
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationGameCenterCommunicatorRefreshCompleted, CCBool::create (true));
    else
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationGameCenterCommunicatorFriendsFetched, CCBool::create (true));
}

void GameCenterCommunicator::addFriendToMap (std::string friendID, std::string friendName)
{
    tempFriendsBuffer[friendID] = friendName;
}

void GameCenterCommunicator::fetchFriendsData () {}

void GameCenterCommunicator::reportAchievementProgress (const char *achId, double perc)
{
    // This method is used only on iOS. On android, achievement is only reported on google play when it's completed.
}

void GameCenterCommunicator::reportAchivementFinished (const char *achId)
{
    achToReportFinished.insert (achId);

    SaveGameController::get ()->save ();

    JniMethodInfo t;
    jobject myNAGGC;
    if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/StoryGame", "getNaGoogleGamesCommunicator",
                                        "()Lorg/nanobit/mystory/NAGoogleGamesCommunicator;")) {
        myNAGGC = t.env->CallStaticObjectMethod (t.classID, t.methodID);
        t.env->DeleteLocalRef (t.classID);
    }

    bool retValue = false;

    if (JniHelper::getMethodInfo (t, "org/nanobit/mystory/NAGoogleGamesCommunicator", "reportAchievment",
                                  "(Ljava/lang/String;)Z")) {
        jstring argStr = t.env->NewStringUTF (achId);
        retValue = t.env->CallBooleanMethod (myNAGGC, t.methodID, argStr);
        t.env->DeleteLocalRef (myNAGGC);
        t.env->DeleteLocalRef (argStr);
        t.env->DeleteLocalRef (t.classID);
    }

    if (retValue) {
        achToReportFinished.erase (achId);
    }
}

void GameCenterCommunicator::showAchievementsUI ()
{
    JniMethodInfo t;
    jobject myNAGGC;

    if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/StoryGame", "getNaGoogleGamesCommunicator",
                                        "()Lorg/nanobit/mystory/NAGoogleGamesCommunicator;")) {
        myNAGGC = t.env->CallStaticObjectMethod (t.classID, t.methodID);
        t.env->DeleteLocalRef (t.classID);
    }

    if (JniHelper::getMethodInfo (t, "org/nanobit/mystory/NAGoogleGamesCommunicator", "showAchievmentsUI", "()V")) {
        t.env->CallVoidMethod (myNAGGC, t.methodID);
        t.env->DeleteLocalRef (t.classID);
        t.env->DeleteLocalRef (myNAGGC);
    }
}

#pragma mark - Refresh

void GameCenterCommunicator::refresh (bool allowNewLogin)
{
    if (NGSTime::currentTimeServer () > lastRefreshTime + gameCenterRefreshWaitTime) {
        refreshInProgress = true;

        authenticatePlayer (allowNewLogin);
    } else {
        refreshComplete (true);
    }
}

void GameCenterCommunicator::refreshComplete (bool success)
{
    refreshInProgress = false;
    CCNotificationCenter::sharedNotificationCenter ()->postNotification (
        kNotificationGameCenterCommunicatorRefreshCompleted, CCBool::create (success));

    if (dirty)
        SaveGameController::get ()->save ();
}

#pragma mark - Achievements

void GameCenterCommunicator::reportSavedAchievements ()
{
    if (achToReportFinished.size () > 0) {
        Json::Value json (Json::arrayValue);

        for (auto ach : achToReportFinished) {
            json.append (ach);
        }

        JniMethodInfo t;
        jobject myNAGGC;
        if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/StoryGame", "getNaGoogleGamesCommunicator",
                                            "()Lorg/nanobit/mystory/NAGoogleGamesCommunicator;")) {
            myNAGGC = t.env->CallStaticObjectMethod (t.classID, t.methodID);
            t.env->DeleteLocalRef (t.classID);
        }

        bool retValue = false;

        if (JniHelper::getMethodInfo (t, "org/nanobit/mystory/NAGoogleGamesCommunicator", "reportSavedAchievements",
                                      "(Ljava/lang/String;)Z")) {
            jstring argStr = t.env->NewStringUTF (json.toStyledString ().c_str ());
            retValue = t.env->CallBooleanMethod (myNAGGC, t.methodID, argStr);
            t.env->DeleteLocalRef (myNAGGC);
            t.env->DeleteLocalRef (argStr);
            t.env->DeleteLocalRef (t.classID);
        }

        if (retValue) {
            achToReportFinished.clear ();
        }
    }
}

void GameCenterCommunicator::encode (Encoder &encoder)
{
    encoder.add (name, "name");
    encoder.add (googlePlayGamesId, "googleId");
    encoder.add (friends, "friends");
    encoder.add (newGooglePlayFriendsIds, "newGooglePlayFriendsIds");
    encoder.add (newFriendsIds, "newFriendsIds");
    encoder.add (achToReportFinished, "achToReportFinished");
    encoder.add (shouldUpdate, "shouldUpdate");
    encoder.add (isSignedIn, "isSignedIn");
}

void GameCenterCommunicator::initWithDecoder (Decoder &decoder)
{
    decoder.get (name, "name");
    decoder.get (googlePlayGamesId, "googleId");
    decoder.get (friends, "friends");
    decoder.get (newGooglePlayFriendsIds, "newGooglePlayFriendsIds");
    decoder.get (newFriendsIds, "newFriendsIds");
    decoder.get (achToReportFinished, "achToReportFinished");
    decoder.get (shouldUpdate, "shouldUpdate");
    decoder.get (isSignedIn, "isSignedIn");
}

