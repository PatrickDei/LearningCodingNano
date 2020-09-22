/*
//
//  MailController.cpp
//  NanoGirl
//
//  Created by Matko Juribasic on 18/03/14.
//
//

#include "MailController.h"
#include "CCNotificationCenter.h"
#include "CCBool.h"
#include "AppDelegate.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

//#import "MailController_objc.h"

MailController *MailController::get()
{
    return AppDelegate::getInstance()->getMailController();
}

void MailController::sendMail(const char *subject, const char *body)
{
//    [[MailController_objc sharedMailController_objc] sendMail:[NSString stringWithUTF8String:subject] body:[NSString stringWithUTF8String:body] handler:this];
	JNIsendMail(subject,body);
}

bool MailController::canSendMail()
{
//    return [MailController_objc canSendMail];
    return true;
}

void MailController::mailSent(bool success)
{
	cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationMailControllerMailSent, cocos2d::CCBool::create(success));
//    cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationMailControllerMailSent, cocos2d::CCBool::create(success));
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

void MailController::JNIsendMail(std::string subject, std::string body)
{
  cocos2d::JniMethodInfo methodInfo;

  if (! getJNIStaticMethodInfo(methodInfo, "sendEmail", "(Ljava/lang/String;Ljava/lang/String;)V")) {
	return;
  }

  jstring subjectArg = methodInfo.env->NewStringUTF(subject.c_str());
  jstring bodyArg = methodInfo.env->NewStringUTF(body.c_str());
  methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, subjectArg, bodyArg);
  methodInfo.env->DeleteLocalRef(subjectArg);
  methodInfo.env->DeleteLocalRef(bodyArg);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
}
*/
