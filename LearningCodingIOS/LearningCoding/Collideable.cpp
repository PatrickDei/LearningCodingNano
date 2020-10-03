//
//  Collideable.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 02/10/2020.
//

#include "Collideable.hpp"
#include "MyObject.hpp"
#include <math.h>
#include "Pool.hpp"

bool Collideable::isInCollision(MyObject* obj1, MyObject* obj2){
    float distance = sqrt(pow(obj1->positionX - obj2->positionX, 2) + pow(obj1->positionY - obj2->positionY, 2));
    return (distance <= obj1->sizeOfObject - 1) ? true : false;
}
