/*
#include "NanigansController.h"

#include "JniHelper.h"
#include <jni.h>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

#define CLASS_NAME "org/nanobit/hollywood/NanigansManager"


void NanigansController::trackTutorial() {
	JniMethodInfo methodInfo;
	if (!JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "trackTutorial", "()V")) {
		return;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
}


void NanigansController::trackPurchase(const char* sku, float priceInDollars) {
	JniMethodInfo methodInfo;
	if (!JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "trackPurchase", "(Ljava/lang/String;Ljava/lang/String;)V")) {
		return;
	}

	jstring jPriceInDollars = methodInfo.env->NewStringUTF(std::to_string(priceInDollars).c_str());
	jstring jSku = methodInfo.env->NewStringUTF(sku);

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jSku, jPriceInDollars);

	methodInfo.env->DeleteLocalRef(jPriceInDollars);
	methodInfo.env->DeleteLocalRef(jSku);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
}
*/
