//
//  BilliardsMenu.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 28/09/2020.
//

#include "BilliardsMenu.hpp"

bool BilliardsMenu::init(){
    
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);
    

    NCLManagerSettings settings;
    settings.deviceScale = NCLDeviceInfo::getDeviceScale();
    settings.imageScale = settings.deviceScale;
    settings.fontScale = settings.deviceScale;
    settings.responder = this;
    NCLDeviceNodeManager* manager = new NCLDeviceNodeManager("PoolMenu.plist", settings);

    CCNode *rootNode = manager->getCCNode("background");

    this->addChild(rootNode);
    
    return true;
}


cocos2d::SEL_MenuHandler BilliardsMenu::getHandleForSelector( std::string handleName){
    return nullptr;
}
cocos2d::CCObject* BilliardsMenu::getTarget(){
    return this;
}
