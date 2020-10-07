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
    
    CCPoint point1;
    CCPoint point2;
    
    CollisionDelegate* collideable;
    MovingDelegate* moveable;
    
    MyObject(CollisionDelegate* c, MovingDelegate* m, float x, float y, float size) : collideable(c), moveable(m), positionX(x), positionY(y), sizeOfObject(size){}
    
    MyObject(CollisionDelegate* c, MovingDelegate* m, CCPoint one, CCPoint two) : collideable(c), moveable(m), point1(one), point2(two){}
    
    bool isInCollision(MyObject* obj1, MyObject* obj2){
        return collideable->isInCollision(obj1, obj2);
    }
    
    void calculateVelocities(MyObject* obj1, MyObject* obj2){
        moveable->calculateVelocities(obj1, obj2);
    }
    
    bool wallCollision(MyObject* wall, MyObject* ball){
        return collideable->wallCollision(wall, ball);
    }
    
    void bounce(MyObject* wall, MyObject* ball){
        moveable->bounce(wall, ball);
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
