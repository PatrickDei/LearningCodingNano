//
//  Collideable.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 02/10/2020.
//

#include "Collideable.hpp"
#include "GameObject.hpp"

bool Collideable::isInCollision(GameObject* obj1, GameObject* obj2, float scale, CCSize size){
    int precision = 50;
    bool collidedWithABall = obj2->type == "ball";
    float checkDistanceX = (collidedWithABall) ? obj1->getVelocityX() / precision : (obj2->getPoint2().x - obj2->getPoint1().x) / precision;
    float checkDistanceY = (collidedWithABall) ? obj1->getVelocityY() / precision : (obj2->getPoint2().y - obj2->getPoint1().y) / precision;
    
    for(int i = 1; i < precision; i++){
        CCPoint positionInPath = (collidedWithABall) ? obj1->getPos() : obj2->getPoint1();
        positionInPath.x += (checkDistanceX) * i;
        positionInPath.y += (checkDistanceY) * i;
        float distance = distanceBetweenPoints(positionInPath, (collidedWithABall) ? obj2->getPos() : obj1->getPos());
        float sizeToCheck = (collidedWithABall) ? obj1->getSize() - 1 : obj1->getSize() / 2 - 1;
        
        if(distance <= sizeToCheck){
            if(!collidedWithABall){
                size.width *= scale;
                size.height *= scale;
                if(!obj1->isScored()){
                        if(obj1->getPositionX() > size.width - size.width / 11)
                            obj1->setPositionX(size.width - size.width / 11 - 1 + obj1->getSize() / 4);
                        if(obj1->getPositionX() < size.width / 11)
                            obj1->setPositionX(size.width / 11 + 1 - obj1->getSize() / 4);
                        if(obj1->getPositionY() > size.height - size.height / 6)
                            obj1->setPositionY(size.height - size.height / 6 - 1 + obj1->getSize() / 4);
                        if(obj1->getPositionY() < size.height / 6)
                            obj1->setPositionY(size.height / 6 + 1 - obj1->getSize() / 4);
                }
            }
            return true;
        }
    }
    return false;
}

float Collideable::distanceBetweenPoints(CCPoint one, CCPoint two){
    float distance;
    distance = sqrt(pow(one.x - two.x, 2) + pow(one.y - two.y, 2));
    return distance;
}
