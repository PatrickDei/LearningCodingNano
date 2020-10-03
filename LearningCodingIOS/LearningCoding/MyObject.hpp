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
    
    float velocityY;
    float velocityX;
    
    float sizeOfObject;
    
    CollisionDelegate* collideable;
    MovingDelegate* moveable;
    
    MyObject(CollisionDelegate* c, MovingDelegate* m, float x, float y, float size) : collideable(c), moveable(m), positionX(x), positionY(y), sizeOfObject(size){}
    
    bool isInCollision(MyObject* obj1, MyObject* obj2){
        return (collideable->isInCollision(obj1, obj2)) ? true : false;
    }
    
    void calculateVelocities(MyObject* obj1, MyObject* obj2){
        moveable->calculateVelocities(obj1, obj2);
    }
    
    CCPoint getPos(){
        CCPoint point = CCPoint(positionX, positionY);
        return point;
    }
    
    float getVelocityX() const{
        return velocityX;
    }

    void setVelocityX(float _velocityX){
        velocityX = (_velocityX < 300) ? _velocityX : 300;
    }


    float getVelocityY() const{
        return velocityY;
    }

    void setVelocityY(float _velocityY){
        velocityY = (_velocityY < 300) ? _velocityY : 300;
    }
};

#endif /* MyObject_hpp */
