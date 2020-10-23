//
//  Collideable.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 02/10/2020.
//

#include "Collideable.hpp"
#include "MyObject.hpp"

bool Collideable::isInCollision(MyObject* obj1, MyObject* obj2, float scale, CCSize size){
    int precision = 50;
    bool collidedWithABall = obj2->variables.getType() == "ball";
    float checkDistanceX = (collidedWithABall) ? obj1->variables.getVelocityX() / precision : (obj2->variables.getPoint2().x - obj2->variables.getPoint1().x) / precision;
    float checkDistanceY = (collidedWithABall) ? obj1->variables.getVelocityY() / precision : (obj2->variables.getPoint2().y - obj2->variables.getPoint1().y) / precision;
    
    for(int i = 1; i < precision; i++){
        CCPoint positionInPath = (collidedWithABall) ? obj1->variables.getPos() : obj2->variables.getPoint1();
        positionInPath.x += (checkDistanceX) * i;
        positionInPath.y += (checkDistanceY) * i;
        float distance = distanceBetweenPoints(positionInPath, (collidedWithABall) ? obj2->variables.getPos() : obj1->variables.getPos());
        float sizeToCheck = (collidedWithABall) ? obj1->variables.getSize() - 1 : obj1->variables.getSize() / 2 - 1;
        
        if(distance <= sizeToCheck){
            if(!collidedWithABall){
                size.width *= scale;
                size.height *= scale;
                if(!obj1->variables.isScored()){
                        if(obj1->variables.getPositionX() > size.width - size.width / 11)
                            obj1->variables.setPositionX(size.width - size.width / 11 - 1 + obj1->variables.getSize() / 4);
                        if(obj1->variables.getPositionX() < size.width / 11)
                            obj1->variables.setPositionX(size.width / 11 + 1 - obj1->variables.getSize() / 4);
                        if(obj1->variables.getPositionY() > size.height - size.height / 6)
                            obj1->variables.setPositionY(size.height - size.height / 6 - 1 + obj1->variables.getSize() / 4);
                        if(obj1->variables.getPositionY() < size.height / 6)
                            obj1->variables.setPositionY(size.height / 6 + 1 - obj1->variables.getSize() / 4);
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
