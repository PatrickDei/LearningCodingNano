//
//  MainMenuCreator.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 05/09/2020.
//

#ifndef MainMenuCreator_hpp
#define MainMenuCreator_hpp

#include <stdio.h>
#include "cocos2d.h"

#define MENU_SCALE 3

USING_NS_CC;

void scaleButtons (CCMenuItemToggle* item1, CCMenuItemToggle* item2, CCMenuItemToggle* item3, CCMenuItemToggle* item4);

void positionItems (int numberOfCells, float height, CCMenuItemToggle* item1, CCMenuItemToggle* item2, CCMenuItemToggle* item3, CCMenuItemToggle* item4);

int calculateOffset(float height);

CCMenuItemImage* getNewButton(std::vector<CCMenuItemImage*> buttons);

#endif /* MainMenuCreator_hpp */
