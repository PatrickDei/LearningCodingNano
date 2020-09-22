/**
 * Copyright (c) 2015-2017 by Helpshift, Inc. All Rights Reserved.
 *
 */

#include "HelpshiftCocos2dxUser.h"

using namespace std;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

HelpshiftCocosUser::HelpshiftCocosUser(void* helpshiftUser)
{
    this->userObject = (jobject) helpshiftUser;
    this->helpshiftUser = helpshiftUser;
    cocos2d::JniMethodInfo minfo;
    cocos2d::JniHelper::getMethodInfo(minfo,
                                      "com/helpshift/HelpshiftUser",
                                      "<init>",
                                      "(Lcom/helpshift/HelpshiftUser$Builder;)V");

    this->env = minfo.env;
    this->helpshiftUserClass = cocos2d::JniHelper::getClassID("com/helpshift/HelpshiftUser");
}

const char* HelpshiftCocosUser::getIdentifier()
{
    jmethodID getIdentifier = env->GetMethodID(this->helpshiftUserClass, "getIdentifier", "()Ljava/lang/String;");
    jstring identifier =  (jstring) env->CallObjectMethod(userObject, getIdentifier);

    return env->GetStringUTFChars(identifier, 0);
}

const char* HelpshiftCocosUser::getEmail()
{
    jmethodID getEmail = env->GetMethodID(this->helpshiftUserClass, "getEmail", "()Ljava/lang/String;");
    jstring email =  (jstring) env->CallObjectMethod(userObject, getEmail);

    return env->GetStringUTFChars(email, 0);
}

const char* HelpshiftCocosUser::getName()
{
    jmethodID getName = env->GetMethodID(this->helpshiftUserClass, "getName", "()Ljava/lang/String;");
    jstring name =  (jstring) env->CallObjectMethod(userObject, getName);

    return env->GetStringUTFChars(name, 0);

}

const char* HelpshiftCocosUser::getAuthToken()
{
    jmethodID getAuthToken = env->GetMethodID(this->helpshiftUserClass, "getAuthToken", "()Ljava/lang/String;");
    jstring authToken =  (jstring) env->CallObjectMethod(userObject, getAuthToken);

    return env->GetStringUTFChars(authToken, 0);
}

HelpshiftCocosUserBuilder::HelpshiftCocosUserBuilder(char* identifier, char* email)
{
    cocos2d::JniMethodInfo minfo;
    cocos2d::JniHelper::getMethodInfo(minfo,
                                      "com/helpshift/HelpshiftUser$Builder",
                                      "<init>",
                                      "(Ljava/lang/String;Ljava/lang/String;)V");

    this->env = minfo.env;

    builderClass = cocos2d::JniHelper::getClassID("com/helpshift/HelpshiftUser$Builder");
    jmethodID builderConstructor = env->GetMethodID(builderClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");

    jstring idStr = env->NewStringUTF(identifier);
    jstring emailStr = env->NewStringUTF(email);

    builderObject = env->NewObject(builderClass, builderConstructor,
                                         idStr, emailStr);
}

HelpshiftCocosUserBuilder& HelpshiftCocosUserBuilder::setName(char* name)
{
    jstring nameStr = env->NewStringUTF(name);
    jmethodID setName  = env->GetMethodID(builderClass, "setName", "(Ljava/lang/String;)Lcom/helpshift/HelpshiftUser$Builder;");
    env->CallObjectMethod(builderObject, setName, nameStr);

    return *this;
}

HelpshiftCocosUserBuilder& HelpshiftCocosUserBuilder::setAuthToken(char* authToken)
{
    jstring authTokenStr = env->NewStringUTF(authToken);
    jmethodID setAuthToken = env->GetMethodID(builderClass, "setAuthToken", "(Ljava/lang/String;)Lcom/helpshift/HelpshiftUser$Builder;");
    env->CallObjectMethod(builderObject, setAuthToken, authTokenStr);

    return *this;
}

HelpshiftCocosUser HelpshiftCocosUserBuilder::build()
{
    jmethodID build  = env->GetMethodID(builderClass, "build", "()Lcom/helpshift/HelpshiftUser;");
    jobject userObject = env->CallObjectMethod(builderObject, build);

    return HelpshiftCocosUser(userObject);
}

#endif
