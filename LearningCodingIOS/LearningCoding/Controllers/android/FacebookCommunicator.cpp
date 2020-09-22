//
//  FacebookCommunicator.cpp
//
//
//

#include "FacebookCommunicator.h"
#include "AppDelegate.h"
#include "AnalyticsController.h"
#include "IosFunctions.h"
#include "SaveGameController.h"
#include "NGSTime.h"
#include "NewsFeed.h"
#include "NewsContent.h"
#include "GameState.h"
#include "NanoExtensions.h"
#include "Parameters.h"
#include "RewardPopup.h"
#include "CCUserDefault.h"
#include "CCNotificationCenter.h"
#include "DownloaderController.h"
#include <fstream>
#include "platform/android/jni/JniHelper.h"
#include "CCJSONConverter.h"
#include "PlayerSocial.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

CODING_DEFINE (FacebookCommunicator);

#define CLASS_FACEBOOK_COMMUNICATOR "org/nanobit/mystory/NAFacebookCommunicator"

#pragma mark - Init

extern "C" {
JNIEXPORT void JNICALL Java_org_nanobit_mystory_NAFacebookCommunicator_fcSessionCallBack (JNIEnv *env, jobject thiz,
                                                                                          jboolean sessionOpened,
                                                                                          jboolean isRead)
{
    if (isRead && FacebookCommunicator::get ()) {
        FacebookCommunicator::get ()->sessionReadOpened (sessionOpened);
    }
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_NAFacebookCommunicator_fbLogedOut (JNIEnv *env, jobject thiz)
{
    if (FacebookCommunicator::get ()) {
        FacebookCommunicator::get ()->logedOut (true);
    }
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_NAFacebookCommunicator_fbDialogDisplayed (JNIEnv *env, jobject thiz,
                                                                                          jboolean isDisplayed)
{
    if (FacebookCommunicator::get ()) {
        FacebookCommunicator::get ()->gameRequestDialogPresented (isDisplayed);
    }
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_NAFacebookCommunicator_userFriendsFetched (JNIEnv *env, jobject thiz,
                                                                                           jboolean num,
                                                                                           jstring jsfriends)
{
    if (FacebookCommunicator::get ()) {
        std::string sFriends = JniHelper::jstring2string (jsfriends);
        CCDictionary *friends = CCJSONConverter::sharedConverter ()->dictionaryFrom (sFriends.c_str ());
        FacebookCommunicator::get ()->addFetchedFriends (friends);
        if (num) {
            FacebookCommunicator::get ()->friendsFetched ({});
        } else {
            FacebookCommunicator::get ()->friendsFetchFailed ();
        }
    }

    env->DeleteLocalRef (jsfriends);
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_NAFacebookCommunicator_userDataFetched (
    JNIEnv *env, jobject thiz, jboolean num, jstring userName, jstring usrID, jstring profilepicUrl )
{
    if (FacebookCommunicator::get ()) {
        if (num) {
            FacebookCommunicator::get ()->userDataFetched (JniHelper::jstring2string (userName),
                                                           JniHelper::jstring2string (usrID),
                                                           JniHelper::jstring2string (profilepicUrl));
            env->DeleteLocalRef (userName);
            env->DeleteLocalRef (usrID);
            env->DeleteLocalRef (profilepicUrl);
        } else {
            FacebookCommunicator::get ()->userDataFetchFailed ();
        }
    }
}
};

#pragma mark - FacebookCommunicator

FacebookCommunicator *FacebookCommunicator::get ()
{
    if (GameState::get ())
        return GameState::get ()->getFacebookCommunicator ();
    else
        return nullptr;
}

FacebookCommunicator::FacebookCommunicator ()
{
    CCNotificationCenter::sharedNotificationCenter ()->addObserver (
            this, callfuncO_selector (FacebookCommunicator::fbDoneWaitForGL),
            kNotificationFacebookCommunicatorRefreshCompleted, nullptr);
    CCNotificationCenter::sharedNotificationCenter ()->addObserver (
            this, callfuncO_selector (FacebookCommunicator::fbDoneWaitForGL),
            kNotificationFacebookCommunicatorSessionOpened, nullptr);
    CCNotificationCenter::sharedNotificationCenter ()->addObserver (
            this, callfuncO_selector (FacebookCommunicator::glDoneWaitForFB),
            GL_CONTEXT_READY, nullptr);
    CCNotificationCenter::sharedNotificationCenter ()->addObserver (
            this, callfuncO_selector (FacebookCommunicator::fbAndGlReady),
            kNotificationFacebookCommunicatorSessionClosed, nullptr);
}

FacebookCommunicator::~FacebookCommunicator ()
{
    CCNotificationCenter::sharedNotificationCenter ()->removeAllObservers (this);
}

std::string FacebookCommunicator::getFacebookId ()
{
    std::string fcId;
    if (GameState::get ()) {
        fcId = FacebookCommunicator::get ()->facebookId;
    } else {
        fcId = CCUserDefault::sharedUserDefault ()->getStringForKey (FACEBOOK_ID);
    }

    return fcId;
}
void FacebookCommunicator::updateData ()
{
    // open new facebook read session to initialize sdk
    if (isOpen () || requestedFacebookAccess) {
        openReadSession ();
    }
}

#pragma mark - Facebook handler

void FacebookCommunicator::sessionReadOpened (bool success)
{
    if (actionInProgress == kActionRefresh) {
        refreshSessionOpened (success);
    }
    CCNotificationCenter::sharedNotificationCenter ()->postNotification (kNotificationFacebookCommunicatorSessionOpened,
                                                                         CCBool::create (success));
}

void FacebookCommunicator::sessionPublishOpened (bool success)
{
    if (actionInProgress == kActionPost) {
        postSessionOpened (success);
    }

    CCNotificationCenter::sharedNotificationCenter ()->postNotification (kNotificationFacebookCommunicatorSessionOpened,
                                                                         CCBool::create (success));
}

void FacebookCommunicator::logedOut (bool success)
{
    if (success) {
        setRequestFacebookAccess (false);
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationFacebookCommunicatorSessionClosed, CCBool::create (true));
        AnalyticsController::facebookLogOut (facebookId);
    }
}

void FacebookCommunicator::userDataFetchFailed ()
{
    if (actionInProgress == kActionRefresh) {
        refreshUserDataFetched (CCBool::create (false));
    } else {
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationFacebookCommunicatorUserDataFetched, CCBool::create (false));
    }
}

void FacebookCommunicator::userDataFetched (const std::string &fName, const std::string &fId,
                                            const std::string &fprofilePictureURL)
{
    if ((name != fName && fName.length () > 0) || (facebookId != fId && fId.length () > 0)) {
        dirty = true;
    }

    // update fields
    if (fName.length () > 0) {
        name = fName;
    }

    if (fId.length () > 0) {
        facebookId = fId;
        CCUserDefault::sharedUserDefault ()->setStringForKey (FACEBOOK_ID, facebookId);
    }

    if (fprofilePictureURL.length () > 0) {
        profilePictureUrl = fprofilePictureURL;
    }

    // download profile image
    CCNotificationCenter::sharedNotificationCenter ()->addObserver (
        this, callfuncO_selector (FacebookCommunicator::refreshUserProfileImageFetched),
        kNotificationFileDownloaderDownloadComplete, nullptr);

    std::string profilePictureUrl =
        "https://graph.facebook.com/v2.8/" + facebookId + "/picture?type=normal&width=254&height=254";
    AppDelegate::getInstance ()->getDownloaderController ()->addFileToNanoDownload (profilePictureUrl, "facebook",
                                                                                        facebookId + ".jpg");

    lastRefreshTime = NGSTime::currentTimeServer ();

    if (actionInProgress == kActionRefresh) {
        refreshUserDataFetched (CCBool::create (true));
    } else {
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationFacebookCommunicatorUserDataFetched, CCBool::create (true));
    }
}

void FacebookCommunicator::friendsFetchFailed ()
{
    if (actionInProgress == kActionRefresh) {
        refreshFriendsFetched (CCBool::create (false));
    } else {
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationFacebookCommunicatorFriendsFetched, CCBool::create (false));
    }
}

void FacebookCommunicator::friendsFetched (const std::map<std::string, std::string> &fFriends)
{
    for (auto fFriend : fetchedFriends) {
        if (friends.find (fFriend.first) == friends.end ())
            newFriendsIds.push_back (fFriend.first);
    }

    if (newFriendsIds.size () > 0) {
        dirty = true;
    }

    friends = fetchedFriends;
    fetchedFriends.clear ();

    // request friend profile image
    for (auto f : friends) {
        std::string fbId = f.first;
        PlayerSocial::downloadProfileImage(fbId, shouldRefresh());
        CCLOG ("FBComm-> for fFriend={%s : %s}", f.first.c_str (), f.second.c_str ());
    }

    if (actionInProgress == kActionRefresh) {
        refreshFriendsFetched (CCBool::create (true));
    } else {
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationFacebookCommunicatorFriendsFetched, CCBool::create (true));
    }
}

void FacebookCommunicator::addFetchedFriends (CCDictionary *friends)
{
    CCDictElement *element = nullptr;
    CCDICT_FOREACH (friends, element)
    {
        std::string fbId = element->getStrKey ();
        std::string name = static_cast<CCString *> (element->getObject ())->getCString ();
        fetchedFriends[fbId] = name;
    }
}

void FacebookCommunicator::imagePosted (bool success)
{
    if (actionInProgress == kActionPost) {
        postImagePosted (success);
    } else {
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationFacebookCommunicatorImagePosted, CCBool::create (success));
    }
}

void FacebookCommunicator::gameRequestDialogPresented (bool success)
{
    if (actionInProgress == kActionDialog) {
        dialogCompleted (success);
    } else {
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationFacebookCommunicatorRequestDialogPresented, CCBool::create (success));
    }
}

#pragma mark - Refresh method

bool FacebookCommunicator::shouldRefresh ()
{
    return NGSTime::currentTimeServer () > lastRefreshTime + facebookImagesRefreshWaitTime;
}

void FacebookCommunicator::refresh (bool allowLoginUI)
{
    if (NGSTime::currentTimeServer () > lastRefreshTime + facebookRefreshWaitTime) {
        actionInProgress = kActionRefresh;
        openReadSession ();
    } else {
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationFacebookCommunicatorRefreshCompleted, CCBool::create (true));
    }
}

void FacebookCommunicator::refreshSessionOpened (bool success)
{
    if (success) {
        setRequestFacebookAccess (true);
        fetchUserData ();
    } else {
        actionInProgress = kActionNoAction;
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationFacebookCommunicatorRefreshCompleted, CCBool::create (false));
    }
}

void FacebookCommunicator::refreshUserDataFetched (bool success)
{
    if (success) {
        fetchFriends ();
        AnalyticsController::facebookConnect (facebookId, " ");
    } else {
        actionInProgress = kActionNoAction;
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationFacebookCommunicatorUserDataFetched, CCBool::create (success));
    }
}

void FacebookCommunicator::refreshUserProfileImageFetched (cocos2d::CCObject *str)
{
    CCString *imageName = (CCString *)str;
    std::string fileName = facebookId + ".jpg";
    if (imageName->compare (fileName.c_str ()) == 0) {
        CCNotificationCenter::sharedNotificationCenter ()->removeObserver (this,
                                                                           kNotificationFileDownloaderDownloadComplete);
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationFacebookCommunicatorUserDataFetched, CCBool::create (true));
    }
}

void FacebookCommunicator::refreshFriendsFetched (bool success)
{
    actionInProgress = kActionNoAction;

    if (dirty) {
        SaveGameController::get ()->save ();
    }

    if (success) {
        lastRefreshTime = NGSTime::currentTimeServer ();

        if (isRedrawReady()) {
            rewardConnect();
        } else {
            CCNotificationCenter::sharedNotificationCenter ()->addObserver (
                    this, callfuncO_selector (FacebookCommunicator::rewardConnect), kNotificationFacebookCommunicatorCanRedraw,
                    nullptr);
        }
    }

    CCNotificationCenter::sharedNotificationCenter ()->postNotification (
        kNotificationFacebookCommunicatorRefreshCompleted, CCBool::create (success));
}

void FacebookCommunicator::rewardConnect ()
{
    if (!GameState::get ()->isFacebookConnectRewardCollected ()) {
        int reward = Parameters::getParameters ()->intForKey ("facebookConnectReward");
        if (reward > 0) {
            GameState::get ()->addPlayerGems (reward);
            GameState::get ()->setFacebookConnectRewardCollected (true);

            RewardPopUp *rewardPopup = RewardPopUp::create("Congratulations", "", reward, RewardPopupType::Gems, "Collect");
            HudLayer::get()->showPopup(rewardPopup, false);
            NewsFeed::get()->sendNewsFeedEvent(NewsContentType::FriendStartedPlaying);
        }
    }
}

void FacebookCommunicator::clearDirty ()
{
    dirty = false;
    newFriendsIds.clear ();
}

#pragma mark - Post method

void FacebookCommunicator::post (const char *filename, const char *message)
{
    imageFilename = filename;
    postMessage = message;

    actionInProgress = kActionPost;

    openPublishSession ();
}

void FacebookCommunicator::postSessionOpened (bool success)
{
    if (success)
        postImageWithMessage (imageFilename.c_str (), postMessage.c_str ());
    else {
        actionInProgress = kActionNoAction;
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationFacebookCommunicatorPostCompleted, CCBool::create (false));
    }
}

void FacebookCommunicator::postImagePosted (bool success)
{
    actionInProgress = kActionNoAction;

    if (success) {
        //        AnalyticsController::facebookShare ();

        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationFacebookCommunicatorPostCompleted, CCBool::create (true));
    } else
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationFacebookCommunicatorPostCompleted, CCBool::create (false));
}

#pragma mark - Request dialog method

void FacebookCommunicator::dialog (const char *title, const char *message)
{
    actionInProgress = kActionDialog;

    postTitle = title;
    postMessage = message;

    openReadSession ();
}

void FacebookCommunicator::dialogSessionOpened (bool success)
{
    if (success)
        presentGameRequestDialog (postTitle.c_str (), postMessage.c_str ());
    else {
        actionInProgress = kActionNoAction;
        CCNotificationCenter::sharedNotificationCenter ()->postNotification (
            kNotificationFacebookCommunicatorDialogCompleted, CCBool::create (false));
    }
}

void FacebookCommunicator::dialogCompleted (bool success)
{
    actionInProgress = kActionNoAction;

    CCNotificationCenter::sharedNotificationCenter ()->postNotification (
        kNotificationFacebookCommunicatorDialogCompleted, CCBool::create (success));
}

bool FacebookCommunicator::connectEnabled ()
{
    return Parameters::getParameters ()->boolForKey ("facebookConnectEnabled");
}

#pragma mark - Facebook SDK functions

void FacebookCommunicator::openReadSession ()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/NAFacebookCommunicator", "openReadSession", "()V")) {
        t.env->CallStaticVoidMethod (t.classID, t.methodID);
        t.env->DeleteLocalRef (t.classID);
    }
}

void FacebookCommunicator::openPublishSession ()
{
    // TODO:
}

void FacebookCommunicator::logOut ()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/NAFacebookCommunicator", "logOut", "()V")) {
        t.env->CallStaticVoidMethod (t.classID, t.methodID);
        t.env->DeleteLocalRef (t.classID);
    }
}

void FacebookCommunicator::fetchUserData ()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/NAFacebookCommunicator", "fetchUserData", "()V")) {
        t.env->CallStaticVoidMethod (t.classID, t.methodID);
        t.env->DeleteLocalRef (t.classID);
    }
}

void FacebookCommunicator::fetchFriends ()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/NAFacebookCommunicator", "fetchFriends", "()V")) {
        newFriendsIds.clear ();
        t.env->CallStaticVoidMethod (t.classID, t.methodID);
        t.env->DeleteLocalRef (t.classID);
    }
}

void FacebookCommunicator::presentGameRequestDialog (const char *title, const char *message)
{
    JniMethodInfo t;

    std::string titleStr (title);
    std::string messageStr (message);

    bool retValue = false;
    if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/NAFacebookCommunicator", "showWebDialog",
                                        "(Ljava/lang/String;Ljava/lang/String;)V")) {
        jstring fileNameArg = t.env->NewStringUTF (titleStr.c_str ());
        jstring messageArg = t.env->NewStringUTF (messageStr.c_str ());
        t.env->CallStaticVoidMethod (t.classID, t.methodID, fileNameArg, messageArg);
        t.env->DeleteLocalRef (messageArg);
        t.env->DeleteLocalRef (fileNameArg);
        t.env->DeleteLocalRef (t.classID);
    }
}

void FacebookCommunicator::postImageWithMessage (const char *filename, const char *message) {}

bool FacebookCommunicator::isOpen ()
{
    JniMethodInfo t;
    bool retValue = false;
    if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/NAFacebookCommunicator", "isSessionOpened", "()Z")) {
        retValue = t.env->CallStaticBooleanMethod (t.classID, t.methodID);
        t.env->DeleteLocalRef (t.classID);
    }
    return retValue;
}

std::pair<std::string, std::string> FacebookCommunicator::separateFirstAndLastName (std::string name)
{
    auto indexOfDel = name.find (' ');
    std::string firstName = name;
    std::string surname = "";
    if (indexOfDel != std::string::npos) {
        firstName = name.substr (0, indexOfDel);
        surname = name.substr (indexOfDel + 1);
    }
    return pair<std::string, std::string> (firstName, surname);
}

#pragma mark - Save/Load

void FacebookCommunicator::encode (Encoder &encoder)
{
    encoder.add (name, "name");
    encoder.add (facebookId, "id");
    encoder.add (profilePictureUrl, "profilePictureUrl");
    encoder.add (friends, "friends");
    encoder.add (newFriendsIds, "newFriendsIds");
    encoder.add (lastRefreshTime, "lastRefreshTime");
    encoder.add (dirty, "dirty");
    encoder.add (requestedFacebookAccess, "requestedFacebookAccess");
    encoder.add ((int)actionInProgress, "actionInProgress");
}

void FacebookCommunicator::initWithDecoder (Decoder &decoder)
{
    decoder.get (name, "name");
    decoder.get (facebookId, "id");
    decoder.get (profilePictureUrl, "profilePictureUrl");
    decoder.get (friends, "friends");
    decoder.get (newFriendsIds, "newFriendsIds");
    decoder.get (lastRefreshTime, "lastRefreshTime");
    decoder.get (dirty, "dirty");
    decoder.get (requestedFacebookAccess, "requestedFacebookAccess");
    int state;
    decoder.get (state, "actionInProgress");
    actionInProgress = (ActionInProgress)state;
}

void FacebookCommunicator::fbDoneWaitForGL ()
{
    fbNotificationReceived = true;
    checkIfRedrawReady();
}

void FacebookCommunicator::glDoneWaitForFB ()
{
    glNotificationReceived = true;
    checkIfRedrawReady();
}

void FacebookCommunicator::fbAndGlReady ()
{
    fbNotificationReceived = true;
    glNotificationReceived = true;
    checkIfRedrawReady();
}

void FacebookCommunicator::checkIfRedrawReady ()
{
    if (fbNotificationReceived && glNotificationReceived) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(
                kNotificationFacebookCommunicatorCanRedraw, CCBool::create(true));
        fbNotificationReceived = false;
        glNotificationReceived = false;
    }
}

bool FacebookCommunicator::isRedrawReady ()
{
    return fbNotificationReceived && glNotificationReceived;
}