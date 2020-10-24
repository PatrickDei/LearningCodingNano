//
//  MainMenuCreator.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 05/09/2020.
//

#include "MainMenuCreator.hpp"
#include "Constants.hpp"

void scaleButtons (CCMenuItemToggle* item1, CCMenuItemToggle* item2, CCMenuItemToggle* item3, CCMenuItemToggle* item4){
    item1->setScale(MENU_SCALE);
    item2->setScale(MENU_SCALE);
    item3->setScale(MENU_SCALE);
    item4->setScale(MENU_SCALE);
}

void positionItems (int numberOfCells, float height, CCMenuItemToggle* item1, CCMenuItemToggle* item2, CCMenuItemToggle* item3, CCMenuItemToggle* item4){
    int offset = calculateOffset(height);
    

    item1->setPosition(ccp(0, numberOfCells * offset * 1.5));
    item2->setPosition(ccp(0, numberOfCells * offset / 2));
    item3->setPosition(ccp(0, -numberOfCells * offset / 2));
    item4->setPosition(ccp(0, -numberOfCells * offset * 1.5));
}

int calculateOffset(float height){
    int offset = height / PORTION_OF_SCREEN;
    return offset;
}
