//
//  MovingDelegate.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 02/10/2020.
//

#ifndef MovingDelegate_hpp
#define MovingDelegate_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class MovingDelegate : public CCLayer{
public:
    virtual void calcPosition() = 0;
};

#endif /* MovingDelegate_hpp */
