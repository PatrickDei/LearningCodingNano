/*
 * OpenUDID.cpp
 *
 *  Created on: Jan 28, 2015
 *      Author: tomislav
 */

#include "JniHelper.h"
#include <jni.h>
#include "OpenUDID.h"
#include "Java_org_cocos2dx_lib_Cocos2dxHelper.h"

#define  CLASS_NAME_MANAGER "org/nanobit/mystory/OpenUDID_manager"
#define  CLASS_NAME_SERVICE "org/nanobit/mystory/OpenUDID_service"

using namespace cocos2d;
using namespace std;

bool initUDIDJni()
{
	JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME_MANAGER,"sync","()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return true;
    }
    return false;
}

bool isUDIDinitializedJni()
{
	JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME_MANAGER,"isInitialized", "()Z")) {
        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return ret;
    }
    return false;
}

std::string getUDIDJni()
{
    JniMethodInfo t;
    std::string ret("");

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME_MANAGER, "getOpenUDID","()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        ret = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(str);
        return ret;
    }
    return "1";
}
