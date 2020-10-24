//
//  CollisionDelegate.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 02/10/2020.
//

#ifndef CollisionDelegate_hpp
#define CollisionDelegate_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class GameObject;

class CollisionDelegate : public CCLayer{
public:
    virtual bool isInCollision(GameObject* obj1, GameObject* obj2, float scale, CCSize size) = 0;
};

#endif /* CollisionDelegate_hpp */
