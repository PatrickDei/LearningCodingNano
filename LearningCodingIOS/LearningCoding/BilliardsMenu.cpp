//
//  BilliardsMenu.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 28/09/2020.
//

#include "BilliardsMenu.hpp"
#include "Pool.hpp"

ccColor3B BilliardsMenu::colorOfWhiteBall = ccc3(255, 255, 255);
bool BilliardsMenu::init(){
    numOfBalls = 15;
    setMenuItemsInMap();
    selectedWhite();
    
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);

    NCLManagerSettings settings;
    settings.deviceScale = NCLDeviceInfo::getDeviceScale();
    settings.imageScale = settings.deviceScale;
    settings.fontScale = settings.deviceScale;
    settings.responder = this;
    NCLDeviceNodeManager* manager = new NCLDeviceNodeManager("BilliardSetup.plist", settings);

    CCNode *rootNode = manager->getCCNode("rootNode");
    
    rootNode->setPosition(ccp(CCDirector::sharedDirector()->getVisibleSize().width / 2 - rootNode->getContentSize().width / 2, CCDirector::sharedDirector()->getVisibleSize().height / 2 - rootNode->getContentSize().height / 2));

    this->addChild(rootNode, 1, 2);
        
    return true;
}


cocos2d::SEL_MenuHandler BilliardsMenu::getHandleForSelector(std::string handleName){
    return menuItemsMap.find(handleName)->second;
}

cocos2d::CCObject* BilliardsMenu::getTarget(){
    return this;
}

void BilliardsMenu::setMenuItemsInMap(){
    menuItemsMap["increaseNumOfBalls"] = menu_selector(BilliardsMenu::increaseNumOfBalls);
    menuItemsMap["decreaseNumOfBalls"] = menu_selector(BilliardsMenu::decreaseNumOfBalls);

    menuItemsMap["backGame"] = menu_selector(BilliardsMenu::closeMenu);
    menuItemsMap["playGame"] = menu_selector(BilliardsMenu::playGame);
    menuItemsMap["quitGame"] = menu_selector(HelloWorld::returnToMainMenu);
    
    menuItemsMap["selectedWhite"] = menu_selector(BilliardsMenu::selectedWhite);
    menuItemsMap["selectedYellow"] = menu_selector(BilliardsMenu::selectedYellow);

}

void BilliardsMenu::closeMenu(){
    this->removeFromParentAndCleanup(true);
}

void BilliardsMenu::playGame(){
    this->removeFromParentAndCleanup(true);
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

void BilliardsMenu::selectedWhite(){
    colorOfWhiteBall = ccc3(255, 255, 255);
}

void BilliardsMenu::selectedYellow(){
    colorOfWhiteBall = ccc3(255, 255, 0);
}
