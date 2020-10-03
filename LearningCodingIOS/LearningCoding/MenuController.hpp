//
//  MenuController.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 02/10/2020.
//

#ifndef MenuController_hpp
#define MenuController_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class MenuController : public CCLayer{
    virtual void increaseNumOfBalls() = 0;
    
    virtual void decreaseNumOfBalls() = 0;
};

#endif /* MenuController_hpp */
