//
//  Games.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 06/09/2020.
//

#ifndef Games_hpp
#define Games_hpp

#include <stdio.h>
#include <string>
#include "cocos2d.h"
#include "AppMacros.h"
#include "HelloWorldScene.h"

USING_NS_CC;

/*template <class T>
static cocos2d::CCMenuItemToggle* createButton(const char* text, cocos2d::CCObject* parent, cocos2d::CCMenuItemImage* button){
        
    CCMenuItemToggle* toggleButton = CCMenuItemToggle::createWithTarget(parent, menu_selector(T::callScene), button, NULL);
        
    cocos2d::CCLabelTTF* menuLabel = cocos2d::CCLabelTTF::create(text, "Arial", MENU_FONT_SIZE);
        
    toggleButton->addChild(menuLabel);
    menuLabel->setPosition(ccp(menuLabel->getParent()->getContentSize().width/2, menuLabel->getParent()->getContentSize().height/2));
        
    return toggleButton;
}

static CCMenu* createBackButton(CCObject* parent, CCSize windowSize){
    CCMenuItemImage* backButtonImage = CCMenuItemImage::create("button_close.png", "button_close_selected.png");
    CCMenuItemToggle* backButton = CCMenuItemToggle::createWithTarget(parent, menu_selector(HelloWorld::returnToMainMenu), backButtonImage, NULL);
    
    backButton->setPosition(ccp(backButton->getContentSize().width / 2, windowSize.height - backButton->getContentSize().height / 2));
    CCMenu* menu = CCMenu::create(backButton, NULL);
    menu->setPosition(CCPointZero);
    return menu;
}

static CCMenuItemImage* getButtonImage(){
    CCMenuItemImage* button = CCMenuItemImage::create("BtnBg.png", "BtnBgSelected.png");
    return button;
}
*/

#endif /* Games_hpp */
