//
//  MoveableObject.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 21/05/2021.
//

#include "MoveableObject.hpp"
#include "PoolObject.hpp"

void MoveableObject::updatePosition(float x, float y, PoolObject object){
    object.setPosition(x, y);
}
