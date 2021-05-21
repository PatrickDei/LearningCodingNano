//
//  Hole.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 24/10/2020.
//

#ifndef Hole_hpp
#define Hole_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "PoolObject.hpp"
#include "NonMoveableObject.hpp"

USING_NS_CC;

class Hole : public PoolObject{
public:
    Hole(float x, float y, float size) : PoolObject(x, y, size, size, new NonMoveableObject()){}
};

#endif /* Hole_hpp */
