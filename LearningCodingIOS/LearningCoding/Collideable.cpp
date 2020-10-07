//
//  Collideable.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 02/10/2020.
//

#include "Collideable.hpp"
#include "MyObject.hpp"

bool Collideable::isInCollision(MyObject* obj1, MyObject* obj2){
    int precision = 20;
    for(int i = 1; i < precision; i++){
        CCPoint positionInPath = obj1->getPos();
        positionInPath.x += (obj1->getVelocityX() / precision) * i;
        positionInPath.y += (obj1->getVelocityY() / precision) * i;
        float distance = distanceBetweenPoints(positionInPath, obj2->getPos());
        if(distance <= obj1->sizeOfObject - 1)
            return true;
    }
    return false;
}


//these two with TYPE* dynamic_cast<TYPE*> (object)
bool Collideable::wallCollision(MyObject* wall, MyObject* ball){
    int precision = 50;
    for(int i = 1; i < precision; i++){
        CCPoint positionInPath = wall->point1;
        positionInPath.x += ((wall->point2.x - wall->point1.x) / precision) * i;
        positionInPath.y += ((wall->point2.y - wall->point1.y) / precision) * i;
        float distance = distanceBetweenPoints(positionInPath, ball->getPos());
        if(distance <= ball->sizeOfObject / 2 - 1)
            return true;
    }
    return false;
}

float Collideable::distanceBetweenPoints(CCPoint one, CCPoint two){
    float distance;
    distance = sqrt(pow(one.x - two.x, 2) + pow(one.y - two.y, 2));
    return distance;
}
