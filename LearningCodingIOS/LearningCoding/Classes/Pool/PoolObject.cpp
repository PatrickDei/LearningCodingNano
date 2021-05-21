//
//  PoolObject.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 21/05/2021.
//

#include "PoolObject.hpp"

Position PoolObject::getPostition(){
    return _position;
}

void PoolObject::setPosition(float x, float y){
    if(x >= 0 && y >= 0){
        _position.x = x;
        _position.y = y;
    }
}

Size PoolObject::getSize(){
    return _size;
}

void PoolObject::setSize(float width, float height){
    _size.width = width;
    _size.height = height;
}

bool PoolObject::isInCollision(PoolObject object){
    return false; //IMPLEMENT SET OVERKAPS
}
