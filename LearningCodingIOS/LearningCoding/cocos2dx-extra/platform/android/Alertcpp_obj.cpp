/*
 * Alertcpp_obj.cpp

 *
 *  Created on: Feb 4, 2015
 *      Author: tomislav
 */

#include "Alertcpp_obj.h"
#include "jni.h"
#include "JniHelper.h"

int Alertcpp_obj::objectCounter = 0;
bool Alertcpp_obj::alertVisible = false;
std::map<int,cocos2d::extension::CCAlertViewDelegate*> Alertcpp_obj::delegates;

using namespace cocos2d;
using namespace cocos2d::extension;

Alertcpp_obj::Alertcpp_obj()
{
}

Alertcpp_obj::~Alertcpp_obj()
{
}

Alertcpp_obj* Alertcpp_obj::createWithParameters(const char* title, const char* message, const char* cancelButtonText, const char* okButtonText, const char* neutralButtonText)
{
	Alertcpp_obj* alertcpp = new Alertcpp_obj();
	alertcpp->initWithParameters(title, message, cancelButtonText, okButtonText, neutralButtonText);
	alertcpp->autorelease();
	return alertcpp;
}

void Alertcpp_obj::initWithParameters(const char* title, const char* message, const char* cancelButtonText, const char* okButtonText, const char* neutralButtonText)
{
	CCLOG("nanoGirl initWithParameters");
	if(alertVisible) return;

	CCLOG("nanoGirl initWithParameters2");
	m_objectID = ++objectCounter;
	m_title = title;
	m_message = message;
	m_cancelButtonText = cancelButtonText;
	m_okButtonText = okButtonText;
    m_neutralButtonText = neutralButtonText;

	createAndroidDialog();
}

void Alertcpp_obj::addDelegate(CCAlertViewDelegate* delegate)
{
	if (delegate)
	{
		if(delegates.find(objectCounter) == delegates.end())
			delegates[objectCounter] = delegate;
	}
}

void Alertcpp_obj::processDelegate(int objID, int buttonIndex)
{
	CCLOG("nanoGirl processDelegate");
	if (!objID )
	{
		return;
	}
	if(delegates.find(objID) != delegates.end()) {
		CCAlertViewDelegate* del = delegates[objID];
		del->alertViewClickedButtonAtIndex(buttonIndex);
		delegates.erase(objID);
		alertVisible = false;
		CCLOG("nanoGirl processDelegate alertvisible false");
	}
	alertVisible = false; // fix ??
}

void Alertcpp_obj::createAndroidDialog()
{
    JniMethodInfo t;
    std::string str1(m_title);
    std::string str2(m_message);
    std::string str3(m_cancelButtonText);
    std::string str4(m_okButtonText);
    std::string str5(m_neutralButtonText);

    CCLOG("nanoGirl createAndroidDialog");
    if (JniHelper::getStaticMethodInfo(t, "org/nanobit/mystory/NanoAndroidFunctions", "nanoDialogSpawner","(Ljava/lang/String;I)V")) {
        jstring stringArg1 = t.env->NewStringUTF((str1 + ";" + str2 + ";" + str3 + ";" + str4 + ";" + str5).c_str());
//        jstring stringArg2 = t.env->NewStringUTF(str2.c_str());
//        jstring stringArg3 = t.env->NewStringUTF(str3.c_str());
//        jstring stringArg4 = t.env->NewStringUTF(str4.c_str());

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1,m_objectID);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(stringArg1);
//        t.env->DeleteLocalRef(stringArg2);
//        t.env->DeleteLocalRef(stringArg3);
//        t.env->DeleteLocalRef(stringArg4);

        alertVisible = true;
    }
}

void Alertcpp_obj::cancelAlert()
{
	JniMethodInfo t;

	CCLOG("nanoGirl cancelAlert");
	if (JniHelper::getStaticMethodInfo(t, "org/nanobit/mystory/NanoAlertDialog", "cancelAlert","()V")) {

		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		alertVisible = false;
	}
}
