//
//  BilliardsMenu.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 28/09/2020.
//

#include "BilliardsMenu.hpp"

bool BilliardsMenu::init(){
    numOfBalls = 15;
    
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);

    NCLManagerSettings settings;
    settings.deviceScale = NCLDeviceInfo::getDeviceScale();
    settings.imageScale = settings.deviceScale;
    settings.fontScale = settings.deviceScale;
    settings.responder = this;
    NCLDeviceNodeManager* manager = new NCLDeviceNodeManager("BilliardSetup.plist", settings);

    CCNode *rootNode = manager->getCCNode("rootNode");

    this->addChild(rootNode, 1, 2);
        
    return true;
}


cocos2d::SEL_MenuHandler BilliardsMenu::getHandleForSelector(std::string handleName){
    if(handleName == "increaseNumOfBalls")
        return menu_selector(BilliardsMenu::increaseNumOfBalls);
    else
        return menu_selector(BilliardsMenu::decreaseNumOfBalls);
}

cocos2d::CCObject* BilliardsMenu::getTarget(){
    return this;
}

void BilliardsMenu::increaseNumOfBalls(){
    if(numOfBalls > 1){
        numOfBalls--;
        updateLabel(std::to_string(numOfBalls));
    }
}

void BilliardsMenu::decreaseNumOfBalls(){
    if(numOfBalls < 15){
        numOfBalls++;
        updateLabel(std::to_string(numOfBalls));
    }
}

void BilliardsMenu::updateLabel(std::string labelString){
    CCLabelTTF *label = CCLabelTTF::create(labelString.c_str(), "Arial", 30);
    label->setPosition(this->getChildByTag(2)->getChildByTag(50)->getPosition());
    label->setAnchorPoint(CCPointZero);
    this->getChildByTag(2)->removeChildByTag(50);
    this->getChildByTag(2)->addChild(label, 1, 50);
}
