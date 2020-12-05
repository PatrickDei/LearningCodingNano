//
//  BilliardsMenu.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 28/09/2020.
//

#ifndef BilliardsMenu_hpp
#define BilliardsMenu_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "NanoCocosLoader/NCLDeviceNodeManager.h"
#include "NanoCocosLoader/NCLDeviceInfo.h"
#include "NanoCocosLoader/NCLResponder.h"

USING_NS_CC;

class BilliardsMenu : public CCLayer, public NCLResponder{
public:
    static _ccColor3B colorOfWhiteBall;
    
    static int numOfBalls;
    
    
    virtual bool init() override;
    
    virtual cocos2d::SEL_MenuHandler getHandleForSelector( std::string handleName) override;
    virtual cocos2d::CCObject* getTarget() override;
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override{
        return true;
    }
    
    CREATE_FUNC(BilliardsMenu);

private:
    CCLabelTTF* ballNumber;
    CCSprite* box;
    
    NCLDeviceNodeManager* manager;
    
    std::map<std::string, SEL_MenuHandler> menuItemsMap;
        
    void setMenuItemsInMap();
    
    void increaseNumOfBalls();
    void decreaseNumOfBalls();
    
    void selectedWhite();
    void selectedYellow();
    void selectedRed();
    void selectedCyan();
    void selectedBlue();
    void selectedMagenta();

    void closeMenu();
    void playGame();
    
    void onEnterAnim();
    void buttonAnim(CCNode* button, bool moveTheBox);
    
    void animationStart();
    void animationEnd();
};

#endif /* BilliardsMenu_hpp */
