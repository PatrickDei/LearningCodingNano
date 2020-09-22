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
#include "Puzzle.hpp"
#include "AppMacros.h"

USING_NS_CC;


    template <class T>
    static cocos2d::CCMenuItemToggle* createButton(const char* text, cocos2d::CCObject* parent, cocos2d::CCMenuItemImage* button){
        
        CCMenuItemToggle* toggleButton = CCMenuItemToggle::createWithTarget(parent, menu_selector(T::callScene), button, NULL);
        
        
        cocos2d::CCLabelTTF* menuLabel = cocos2d::CCLabelTTF::create(text, "Arial", MENU_FONT_SIZE);
        
        toggleButton->addChild(menuLabel);
        menuLabel->setPosition(ccp(menuLabel->getParent()->getContentSize().width/2, menuLabel->getParent()->getContentSize().height/2));
        
        return toggleButton;
    }


#endif /* Games_hpp */
