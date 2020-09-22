#include "NotificationHandler.h"
#include "platform/android/jni/JniHelper.h"
#include "NGSTime.h"
#include <jni.h>
#include <cocos2d.h>

using namespace std;

#define CLASS_NAME "org/nanobit/mystory/StoryGame"

static bool getJNIStaticMethodInfo (cocos2d::JniMethodInfo &methodinfo, const char *methodName, const char *paramCode)
{
    return cocos2d::JniHelper::getStaticMethodInfo (methodinfo, CLASS_NAME, methodName, paramCode);
}

void NotificationHandler::scheduleNotification (const char *alertBody, const char *title, double fireDate, const char *alertAction,
                                                int badgeNumber)
{
    cocos2d::JniMethodInfo methodInfo;

    if (!getJNIStaticMethodInfo (methodInfo, "showLocalNotification", "(Ljava/lang/String;Ljava/lang/String;IILjava/lang/String;)V")) {
        return;
    }

    int intervalInt = fireDate - NGSTime::currentTimeServer();
    jstring actionArg = methodInfo.env->NewStringUTF (alertAction);
    jstring stringArg = methodInfo.env->NewStringUTF (alertBody);
    jstring titleArg = methodInfo.env->NewStringUTF (title);
    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID, stringArg, titleArg, intervalInt, badgeNumber,
                                          actionArg);
    methodInfo.env->DeleteLocalRef (stringArg);
    methodInfo.env->DeleteLocalRef (actionArg);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}

void NotificationHandler::cancelAllNotifications ()
{

    // jni call, do not use loop
    NotificationHandler::cancel (1);
    NotificationHandler::cancel (2);
    NotificationHandler::cancel (3);
    NotificationHandler::cancel (4);
    NotificationHandler::cancel (5);
    NotificationHandler::cancel (6);
    NotificationHandler::cancel (7);
    NotificationHandler::cancel (8);
}

void NotificationHandler::cancel (int tag)
{
    cocos2d::JniMethodInfo methodInfo;

    if (!getJNIStaticMethodInfo (methodInfo, "cancelLocalNotification", "(I)V")) {
        return;
    }

    methodInfo.env->CallStaticVoidMethod (methodInfo.classID, methodInfo.methodID, tag);
    methodInfo.env->DeleteLocalRef (methodInfo.classID);
}
