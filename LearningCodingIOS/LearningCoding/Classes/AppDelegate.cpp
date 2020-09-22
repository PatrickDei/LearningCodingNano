//
//  AppDelegate.cpp
//  LearningCoding
//
//  Created by Lana Lisjak on 18/05/16.
//  Copyright Nanobit 2016. All rights reserved.
//

#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "NCLDeviceInfo.h"
#include "HelloWorldScene.h"
#include "AppMacros.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "Jni.h"
#include "JniHelper.h"
#endif

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocos2d;
using namespace std;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C" {

JNIEXPORT void JNICALL Java_org_nanobit_mystory_StoryGame_blockGLResume(JNIEnv *env, jobject thiz, jboolean bValue)
{
    AppDelegate::blockGLResume = bValue;
}
JNIEXPORT void JNICALL Java_org_nanobit_mystory_StoryGame_onCreateCalled(JNIEnv *env, jobject thiz)
{
    AppDelegate::onCreateCalled = true;
}

JNIEXPORT jint JNICALL Java_org_nanobit_mystory_StoryGame_backgroundUpdateFunctions(JNIEnv *env, jobject thiz)
{
    int taskNum = 0;
    if (cocos2d::CCApplication::sharedApplication())
        taskNum = cocos2d::CCApplication::sharedApplication()->backgroundUpdate(0.01f);
    return taskNum;
}

JNIEXPORT void JNICALL Java_org_nanobit_mystory_StoryGame_setPushRegistrationId(JNIEnv *env, jobject thiz,
                                                                                jstring regId)
{
    deviceToken = JniHelper::jstring2string(regId);
    CCLOG("deviceToken %s", deviceToken.c_str());
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationAppDelegateDeviceTokenReceived,
                                                                       CCString::create(deviceToken));
    env->DeleteLocalRef(regId);
}
JNIEXPORT void JNICALL Java_org_nanobit_mystory_StoryGame_setApplicatonOffline(JNIEnv *env, jobject thiz, jint iValue)
{
    bool statusChanged = AppDelegate::offline != iValue;
    AppDelegate::offline = iValue;

    if (statusChanged && AppDelegate::getInstance() && GameState::get()) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(kAppOfflineStatusChanged, nullptr);
    }
}
}

#endif

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate()
{
}

#pragma mark - Application Status -

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCEGLView *view = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(view);

    srand(time(nullptr));

    pDirector->setContentScaleFactor(1.0);
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    setSearchPaths();
    CCScene *pScene = HelloWorld::scene();
    CCDirector::sharedDirector()->runWithScene(pScene);
    CCDirector::sharedDirector()->mainLoop();

    return true;
}

// This function will be called when the app is inactive. When comes a phone
// call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
    CCDirector::sharedDirector()->stopAnimation();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::getInstance()->resumeAllEffects();
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

    CCNotificationCenter::sharedNotificationCenter()->postNotification(
        kNotificationAppDelegateApplicationWillEnterForeground);
}

// this is never called on android
void AppDelegate::applicationDidBecomeActive()
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(
        kNotificationAppDelegateApplicationDidBecomeActive);
}

void AppDelegate::applicationWillTerminate()
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(
        kNotificationAppDelegateApplicationWillTerminate);
}

void AppDelegate::applicationWillResignActive()
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(
        kNotificationAppDelegateApplicationWillResignActive);
}

#pragma mark - Other -

void AppDelegate::setSearchPaths()
{
    NCLDeviceInfo::setupExtensions("", "", "-iphone5", "-ipad", "-ipadhd");

    // search paths list
    vector<string> searchPath;
    CCSize frameSize = CCDirector::sharedDirector()->getOpenGLView()->getFrameSize();
    if (frameSize.height > mediumResource.size.height)
    {
        searchPath.push_back(largeResource.directory);
    }
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    {
        searchPath.push_back(mediumResource.directory);
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
    else
    {
        searchPath.push_back(smallResource.directory);
    }
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
}
