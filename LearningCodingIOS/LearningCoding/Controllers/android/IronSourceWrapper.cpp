//
//  FyberWrapper.cpp
//  NanoGirl
//
//  Created by Dino Milačić on 01/12/14.
//
//

#include "IronSourceWrapper.h"
#include "NGSDataManager.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <NanoIdentifierWrapper.h>
#include "AdController.h"

extern "C" {
JNIEXPORT void JNICALL Java_org_nanobit_mystory_IronSourceController_offerWallRewardRecieved (JNIEnv *env, jobject thiz,
                                                                      int amount)
{
    AdController::offerWallReward(amount);
}
}

#define CLASS_NAME "org/nanobit/mystory/IronSourceController"
static bool getJNIStaticMethodInfo (cocos2d::JniMethodInfo &methodinfo, const char *methodName, const char *paramCode)
{
    return cocos2d::JniHelper::getStaticMethodInfo (methodinfo, CLASS_NAME, methodName, paramCode);
}


void IronSourceWrapper::init ()
{

    std::string userId = NanoIdentifierWrapper::uuidForDevice ();

    cocos2d::JniMethodInfo methodInfo;
    if (!getJNIStaticMethodInfo (methodInfo, "initIronSource", "(Ljava/lang/String;)V")) {
        return;
    }

    jstring userIdArg = methodInfo.env->NewStringUTF (userId.c_str ());
    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID, userIdArg);
    methodInfo.env->DeleteLocalRef (userIdArg);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}

void IronSourceWrapper::showOfferWall ()
{
    cocos2d::JniMethodInfo methodInfo;

    if (!getJNIStaticMethodInfo (methodInfo, "showOfferWall", "()V")) {
        return;
    }

    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}

bool IronSourceWrapper::isOfferWallAvailable()
{
    cocos2d::JniMethodInfo methodInfo;

    if (!getJNIStaticMethodInfo (methodInfo, "isOfferwallAvailable", "()Z")) {
        return false;
    }

    jboolean jniIsAvailable = methodInfo.env->CallStaticBooleanMethod (methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);

    return (bool)jniIsAvailable;
}

