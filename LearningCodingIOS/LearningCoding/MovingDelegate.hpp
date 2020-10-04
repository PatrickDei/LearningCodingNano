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

class MyObject;

class MovingDelegate : public CCLayer{
public:
    virtual void calculateVelocities(MyObject* obj1, MyObject* obj2){}
    
    virtual void bounce(MyObject* wall, MyObject* ball){}
};

#endif /* MovingDelegate_hpp */
