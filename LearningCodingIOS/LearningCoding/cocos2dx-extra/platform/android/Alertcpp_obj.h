/*
 * Alertcpp_obj.h
 *
 *  Created on: Feb 4, 2015
 *      Author: tomislav
 */

#ifndef ALERTCPP_OBJ_H_
#define ALERTCPP_OBJ_H_

#include "cocos2d.h"
#include "cocos2dx_extensions.h"
#include "CCAlertViewDelegate.h"

class Alertcpp_obj: public cocos2d::CCObject
{

public:
	 Alertcpp_obj();
	~Alertcpp_obj();

	static Alertcpp_obj* createWithParameters(const char* title, const char* message, const char* cancelButtonText, const char* okButtonText, const char* neutralButtonText = "");
	void initWithParameters(const char* title, const char* message, const char* cancelButtonText, const char* okButtonText, const char* neutralButtonText = "");

	static void addDelegate(cocos2d::extension::CCAlertViewDelegate* delegate);
	static void processDelegate(int objID, int buttonIndex);
	static void cancelAlert();
	static bool alertVisible;

	int getObjectID(){ return m_objectID; }


private:
	static int objectCounter;
	int m_objectID;
	std::string m_title;
	std::string m_message;
	std::string m_cancelButtonText;
	std::string m_okButtonText;
	std::string m_neutralButtonText;

	void createAndroidDialog();
	static std::map<int,cocos2d::extension::CCAlertViewDelegate*> delegates;
};


#endif /* ALERTCPP_OBJ_H_ */
