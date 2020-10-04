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
        float distance = sqrt(pow(positionInPath.x - obj2->positionX, 2) + pow(positionInPath.y - obj2->positionY, 2));
            if(distance <= obj1->sizeOfObject - 1)
                return true;
    }
    return false;
}

bool Collideable::wallCollision(MyObject* wall, MyObject* ball){
    //to-do
    return true;
}
