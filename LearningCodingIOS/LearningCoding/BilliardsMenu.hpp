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
#include "NanoCocosLoader/NCLDeviceNodeManager.h"
#include "NanoCocosLoader/NCLDeviceInfo.h"
#include "NanoCocosLoader/NCLResponder.h"

USING_NS_CC;

class BilliardsMenu : public CCLayer, public NCLResponder{
public:
    virtual bool init() override;
    
    virtual cocos2d::SEL_MenuHandler getHandleForSelector( std::string handleName) override;
    virtual cocos2d::CCObject* getTarget() override;
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override{
        return true;
    }

    void increaseNumOfBalls();
    
    void decreaseNumOfBalls();
    
    void updateLabel(std::string labelString);
    
    CREATE_FUNC(BilliardsMenu);
    
    
private:
    int numOfBalls;
};

#endif /* BilliardsMenu_hpp */
