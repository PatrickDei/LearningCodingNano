/**
 * Copyright (c) 2015-2017 by Helpshift, Inc. All Rights Reserved.
 *
 */

//
//  HelpshiftUser.h
//  Helpshift Cocos2dx SDK
//

#ifndef HelpshiftCocos2dxUser
#define HelpshiftCocos2dxUser
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

enum HSAuthenticationFailureReason{
    HSAuthTokenNotProvided,
    HSInvalidAuthToken
};

class HelpshiftCocosUser
{
private:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jclass helpshiftUserClass;
    JNIEnv *env;
#endif
public:
    void *helpshiftUser;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jobject userObject;
#endif
    HelpshiftCocosUser(void* hsUser);
    const char* getIdentifier();
    const char* getEmail();
    const char* getName();
    const char* getAuthToken();
};

class HelpshiftCocosUserBuilder
{
private:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jclass builderClass;
    jobject builderObject;
    JNIEnv *env;
#endif
    void *hsBuilder;
public:
    HelpshiftCocosUserBuilder(char* identifier, char* email);
    HelpshiftCocosUserBuilder& setName(char* name);
    HelpshiftCocosUserBuilder& setAuthToken(char* authToken);
    HelpshiftCocosUser build();
};

#endif
