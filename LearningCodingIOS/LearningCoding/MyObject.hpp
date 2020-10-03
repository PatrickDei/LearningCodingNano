//
//  MyObject.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 02/10/2020.
//

#ifndef MyObject_hpp
#define MyObject_hpp

#include <stdio.h>
#include "CollisionDelegate.hpp"
#include "MovingDelegate.hpp"

class MyObject{
public:
    float positionX;
    float positionY;
    
    float sizeOfObject;
    
    CollisionDelegate* collideable;
    MovingDelegate* moveable;
    
    MyObject(CollisionDelegate* c, MovingDelegate* m, float x, float y, float size) : collideable(c), moveable(m), positionX(x), positionY(y), sizeOfObject(size){}
    
    bool isInCollision(MyObject* obj1, MyObject* obj2){
        return (collideable->isInCollision(obj1, obj2)) ? true : false;
    }
    
    void calcPosition(){
        moveable->calcPosition();
    }
};

#endif /* MyObject_hpp */
