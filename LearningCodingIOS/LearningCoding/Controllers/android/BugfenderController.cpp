//
//  BugfenderController.cpp
//  StoryGame
//
//  Created by Filip on 13/02/17.
//
//

#include "BugfenderController.h"
#include "JniHelper.h"
#include "NGSDataManager.h"
#include <jni.h>

using namespace cocos2d;
void BugfenderController::init ()
{
    JniMethodInfo methodInfo;
    if (!JniHelper::getStaticMethodInfo (methodInfo, "org/nanobit/mystory/NanoAndroidFunctions", "bugfenderInit",
                                         "(Ljava/lang/String;)V")) {
        return;
    }
    std::string udid = NGSDataManager::getUdid ();
    jstring jparam = methodInfo.env->NewStringUTF (udid.c_str ());

    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID, jparam);
    methodInfo.env->DeleteLocalRef (jparam);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}

void BugfenderController::setUsername (std::string username)
{
    JniMethodInfo methodInfo;
    if (!JniHelper::getStaticMethodInfo (methodInfo, "org/nanobit/mystory/NanoAndroidFunctions", "bugfenderSetUsername",
                                         "(Ljava/lang/String;)V")) {
        return;
    }

    jstring jparam = methodInfo.env->NewStringUTF (username.c_str ());

    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID, jparam);
    methodInfo.env->DeleteLocalRef (jparam);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}

void BugfenderController::bflog (std::string log)
{
    JniMethodInfo methodInfo;
    if (!JniHelper::getStaticMethodInfo (methodInfo, "org/nanobit/mystory/NanoAndroidFunctions", "bugfenderLog",
                                         "(Ljava/lang/String;)V")) {
        return;
    }

    jstring jparam = methodInfo.env->NewStringUTF (log.c_str ());

    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID, jparam);
    methodInfo.env->DeleteLocalRef (jparam);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}

void BugfenderController::bflogwarning (std::string log)
{
    JniMethodInfo methodInfo;
    if (!JniHelper::getStaticMethodInfo (methodInfo, "org/nanobit/mystory/NanoAndroidFunctions", "bugfenderWarn",
                                         "(Ljava/lang/String;)V")) {
        return;
    }

    jstring jparam = methodInfo.env->NewStringUTF (log.c_str ());

    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID, jparam);
    methodInfo.env->DeleteLocalRef (jparam);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}

void BugfenderController::bflogerror (std::string log)
{
    JniMethodInfo methodInfo;
    if (!JniHelper::getStaticMethodInfo (methodInfo, "org/nanobit/mystory/NanoAndroidFunctions", "bugfenderError",
                                         "(Ljava/lang/String;)V")) {
        return;
    }

    jstring jparam = methodInfo.env->NewStringUTF (log.c_str ());

    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID, jparam);
    methodInfo.env->DeleteLocalRef (jparam);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}
