//
//  Collideable.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 02/10/2020.
//

#include "Collideable.hpp"
#include "MyObject.hpp"

bool Collideable::isInCollision(MyObject* obj1, MyObject* obj2){
    for(int i = 20; i >= 1; i--){
        CCPoint positionInPath = obj1->getPos();
        positionInPath.x += obj1->getVelocityX() / i;
        positionInPath.y += obj1->getVelocityY() / i;
        float distance = distanceBetweenPoints(positionInPath, obj2->getPos());
            if(distance <= obj1->sizeOfObject - 1)
                return true;
    }
    return false;
}

bool Collideable::wallCollision(MyObject* wall, MyObject* ball){
    for(int i = 50; i > 0; i--){
        CCPoint positionInPath = wall->point1;
        positionInPath.x += (wall->point2.x - wall->point1.x) / i;
        positionInPath.y += (wall->point2.y - wall->point1.y) / i;
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
