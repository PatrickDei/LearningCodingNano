/*
//
//  SMSController.cpp
//  NanoGirl
//
//  Created by Matko Juribasic on 19/03/14.
//
//

#include "SMSController.h"
#include "CCNotificationCenter.h"
#include "CCBool.h"
#include "AppDelegate.h"


SMSController *SMSController::get()
{
    return AppDelegate::getInstance()->getSMSController();
}

void SMSController::sendSMS(const char *body)
{
//    [[SMSController_objc sharedSMSController_objc] sendSMS:[NSString stringWithUTF8String:body] handler:this];
	JNIsendSMS(body);
}

bool SMSController::canSendSMS()
{
//    return [SMSController_objc canSendSMS];
	return JNIcanSendSMS();
}

void SMSController::smsSent(bool success)
{
//    cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationSMSControllerSMSSent, cocos2d::CCBool::create(success));
}

// JNI methods
#define  CLASS_NAME "org/nanobit/hollywood/Hollywood"

static bool getJNIStaticMethodInfo(cocos2d::JniMethodInfo &methodinfo,
										const char *methodName,
										const char *paramCode) {
  return cocos2d::JniHelper::getStaticMethodInfo(methodinfo,
												 CLASS_NAME,
												 methodName,
												 paramCode);
}

void SMSController::JNIsendSMS(std::string body)
{
  cocos2d::JniMethodInfo methodInfo;

  if (! getJNIStaticMethodInfo(methodInfo, "sendSMS", "(Ljava/lang/String;)V")) {
	return;
  }

  jstring bodyArg = methodInfo.env->NewStringUTF(body.c_str());
  methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, bodyArg);
  methodInfo.env->DeleteLocalRef(bodyArg);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

bool SMSController::JNIcanSendSMS()
{
  cocos2d::JniMethodInfo methodInfo;

  if (! getJNIStaticMethodInfo(methodInfo, "canSendSMS", "()Z")) {
	return false;
  }

  bool canSendSMS = (bool) methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);

  CCLOG("Can send SMS => %i",canSendSMS);
  return canSendSMS;
}
*/
