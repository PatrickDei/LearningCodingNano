//
//  BilliardsMenu.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 28/09/2020.
//

#include "BilliardsMenu.hpp"
#include "Pool.hpp"

int BilliardsMenu::numOfBalls = 1;

ccColor3B BilliardsMenu::colorOfWhiteBall = ccc3(255, 255, 255);
bool BilliardsMenu::init(){
    
    setMenuItemsInMap();

    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);

    NCLManagerSettings settings;
    settings.deviceScale = NCLDeviceInfo::getDeviceScale();
    settings.imageScale = settings.deviceScale;
    settings.fontScale = settings.deviceScale;
    settings.responder = this;
    manager = new NCLDeviceNodeManager("BilliardSetup.plist", settings);

    CCNode *rootNode = manager->getCCNode("rootNode");
    
    box = dynamic_cast<CCSprite*>(manager->getCCNode("box"));
    ballNumber = dynamic_cast<CCLabelTTF*>(manager->getCCNode("ballNumber"));
    ballNumber->setString(std::to_string(numOfBalls).c_str());

    rootNode->setPosition(ccp(CCDirector::sharedDirector()->getVisibleSize().width / 2 - rootNode->getContentSize().width / 2, CCDirector::sharedDirector()->getVisibleSize().height / 2 - rootNode->getContentSize().height / 2));

    selectedWhite();
    
    this->addChild(rootNode, 1, 2);
    
    onEnterAnim();
        
    return true;
}

void BilliardsMenu::onEnterAnim(){
    CCObject *node = NULL;
    CCARRAY_FOREACH(manager->getCCNode("rootNode")->getChildren(), node)
    {
        static_cast<CCNode*>(node)->setScale(0.1 * static_cast<CCNode*>(node)->getScaleX());
        CCScaleBy* scaleItem = CCScaleBy::create(0.1, 10);
        static_cast<CCNode*>(node)->runAction(scaleItem);
    }
}

void BilliardsMenu::buttonAnim(CCNode* button, bool moveTheBox){
    CCScaleBy* scaleDown = CCScaleBy::create(0.03, 0.75);
    CCScaleBy* scaleUp = CCScaleBy::create(0.03, 1.333);
    
    if(moveTheBox){
        CCMoveBy* moveBox = CCMoveBy::create(0.06, ccp(button->getPosition().x - box->getPosition().x - 13, button->getPosition().y - box->getPosition().y - 10));
        box->runAction(moveBox);
    }

    CCCallFunc* startOfAnimation = CCCallFunc::create(this, callfunc_selector(BilliardsMenu::animationStart));
    CCCallFunc* endOfAnimation = CCCallFunc::create(this, callfunc_selector(BilliardsMenu::animationEnd));
    
    CCSequence* animationSequence = CCSequence::create(startOfAnimation, scaleDown, scaleUp, endOfAnimation, NULL);
    
    button->runAction(animationSequence);
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
    menuItemsMap["selectedRed"] = menu_selector(BilliardsMenu::selectedRed);
    menuItemsMap["selectedCyan"] = menu_selector(BilliardsMenu::selectedCyan);
    menuItemsMap["selectedBlue"] = menu_selector(BilliardsMenu::selectedBlue);
    menuItemsMap["selectedMagenta"] = menu_selector(BilliardsMenu::selectedMagenta);
}

void BilliardsMenu::closeMenu(){
    buttonAnim(manager->getCCNode("quitButton"), false);
    this->removeFromParentAndCleanup(true);
}

void BilliardsMenu::playGame(){
    buttonAnim(manager->getCCNode("playButton"), false);
    Pool::gameRestart = true;
    this->removeFromParentAndCleanup(true);
}

void BilliardsMenu::increaseNumOfBalls(){
    if(numOfBalls > 1){
        numOfBalls--;
        ballNumber->setString(std::to_string(numOfBalls).c_str());
    }
    buttonAnim(manager->getCCNode("leftArrow"), false);
}

void BilliardsMenu::decreaseNumOfBalls(){
    if(numOfBalls < 15){
        numOfBalls++;
        ballNumber->setString(std::to_string(numOfBalls).c_str());
    }
    buttonAnim(manager->getCCNode("rightArrow"), false);
}

void BilliardsMenu::selectedWhite(){
    colorOfWhiteBall = ccc3(255, 255, 255);
    CCNode* node = manager->getCCNode("whiteColor");
    //printf("%f", node->getColorAtPoint(ccp()).r);
    buttonAnim(node, true);
}

void BilliardsMenu::selectedYellow(){
    colorOfWhiteBall = ccc3(255, 255, 0);
    CCNode* node = manager->getCCNode("yellowColor");
    buttonAnim(node, true);
}

void BilliardsMenu::selectedRed(){
    colorOfWhiteBall = ccc3(255, 0, 0);
    CCNode* node = manager->getCCNode("redColor");
    buttonAnim(node, true);
}

void BilliardsMenu::selectedCyan(){
    colorOfWhiteBall = ccc3(0, 255, 255);
    CCNode* node = manager->getCCNode("cyanColor");
    buttonAnim(node, true);
}

void BilliardsMenu::selectedBlue(){
    colorOfWhiteBall = ccc3(0, 0, 255);
    CCNode* node = manager->getCCNode("blueColor");
    buttonAnim(node, true);
}

void BilliardsMenu::selectedMagenta(){
    colorOfWhiteBall = ccc3(255, 0, 255);
    CCNode* node = manager->getCCNode("magentaColor");
    buttonAnim(node, true);
}

void BilliardsMenu::animationStart(){
    this->setTouchEnabled(false);
}

void BilliardsMenu::animationEnd(){
    setTouchEnabled(true);
}
