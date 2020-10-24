//
//  ButtonCreator.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 25/10/2020.
//

#include "ButtonCreator.hpp"

CCMenuItemImage* getNewButton(std::vector<CCMenuItemImage*> buttons){
    buttons.push_back(CCMenuItemImage::create("BtnBg.png", "BtnBgSelected.png"));
    return buttons.back();
}
