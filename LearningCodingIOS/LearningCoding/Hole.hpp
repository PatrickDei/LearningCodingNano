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
#include "GameObject.hpp"
#include "Collideable.hpp"
#include "Immoveable.hpp"

USING_NS_CC;

class Hole : public GameObject{
public:
    Hole(float x, float y, float size) : GameObject(new Collideable(), new Immoveable(), x, y, size, "hole"){}
};

#endif /* Hole_hpp */
