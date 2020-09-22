
#include "CCNative.h"
#include "cocos2d.h"
#include "OpenUDID.h"
#include "IosFunctions.h"
#include "Jni.h"
#include "JniHelper.h"
#include "Alertcpp_obj.h"
//#include "ServerCommunicator.h"
//#import <UIKit/UIKit.h>
//#import "CCNative_objc.h"
//#import "OpenUDID_objc.h"
//#import "AudioToolbox/AudioServices.h"

NS_CC_EXT_BEGIN

#pragma mark -
#pragma mark activity indicator


std::string CCNative::bundleVer = "";
cocos2d::extension::CCAlertViewDelegate* CCNative::delegate_m = nullptr;
std::string CCNative::deviceName = "";


extern "C"
{
		JNIEXPORT void JNICALL Java_org_nanobit_mystory_NanoAlertDialog_buttonClicked(JNIEnv*  env, jobject thiz, jint num, jint objectID)
		{

			Alertcpp_obj::processDelegate(objectID, num);
		}

}

void CCNative::showActivityIndicator(CCActivityIndicatorViewStyle style)
{
    JniMethodInfo t;

//    if (JniHelper::getStaticMethodInfo(t, "org/nanobit/stardroid/NanoAndroidFunctions", "showActivityDialog","()V")) {
//        t.env->CallStaticVoidMethod(t.classID, t.methodID);
//        t.env->DeleteLocalRef(t.classID);
//    }
}

void CCNative::hideActivityIndicator(void)
{
    JniMethodInfo t;
//
//    if (JniHelper::getStaticMethodInfo(t, "org/nanobit/stardroid/NanoAndroidFunctions", "hideActivityDialog","()V")) {
//        t.env->CallStaticVoidMethod(t.classID, t.methodID);
//        t.env->DeleteLocalRef(t.classID);
//    }
}

#pragma mark -
#pragma mark alert view

void CCNative::createAlert(const char* title,
                           const char* message,
                           const char* cancelButtonTitle,
                           const char* okButtonTitle,
                           const char* neutralButtonTitle)
{
	CCLOG("nanoGirl createAlert");
	Alertcpp_obj* alertObj = Alertcpp_obj::createWithParameters(title,message,cancelButtonTitle,okButtonTitle,neutralButtonTitle);
}

int CCNative::addAlertButton(const char* buttonTitle)
{
   //depricated for android by Tomislav
	return 0;
}

void CCNative::showAlert(CCAlertViewDelegate* delegate)
{
	CCLOG("nanoGirl show alert");
	Alertcpp_obj::addDelegate(delegate);
}

void CCNative::cancelAlert(void)
{
	CCLOG("nanoGirl cancelAlert");
    Alertcpp_obj::cancelAlert();
}

void CCNative::openURL(const char* url)
{
	IosFunctions::openURL(url);
}

const std::string CCNative::generateUUID(void)
{
	std::string udid;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/nanobit/mystory/NanoAndroidFunctions", "generateUUID","()Ljava/lang/String;")) {
        jstring udidJavaStr = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        udid = JniHelper::jstring2string(udidJavaStr);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(udidJavaStr);
    }
    return udid;
}

/** @brief Get the NSBundle version */
const std::string CCNative::bundleVersion(void)
{
	return IosFunctions::getBundleIdentifier();
}

#pragma mark -
#pragma mark OpenUDID

const std::string CCNative::getOpenUDID(void)
{
	std::string str = "";
    if (isUDIDinitializedJni())
    	str = getUDIDJni();
    else{
    	initUDIDJni();
    	str = getUDIDJni();
    }
	return str;
}

const char* CCNative::getDeviceName(void)
{
	return "";
}

void CCNative::vibrate()
{
    //AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
void CCNative::showAlertObjc(void *delegate)
{
   // [[CCNative_objc sharedInstance] showAlertViewWithObjcDelegate:(id<UIAlertViewDelegate>)delegate];
}
#endif

#if CC_LUA_ENGINE_ENABLED > 0
int CCNative::addAlertButtonLua(const char* buttonTitle)
{
    //return addAlertButton(buttonTitle) + 1;
}

void CCNative::showAlertLua(cocos2d::LUA_FUNCTION listener)
{
   // [[CCNative_objc sharedInstance] showAlertViewWithLuaListener:listener];
}
#endif

NS_CC_EXT_END
