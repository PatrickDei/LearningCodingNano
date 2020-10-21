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
        if(distance <= obj1->getSize() - 1)
            return true;
    }
    return false;
}


//these two with TYPE* dynamic_cast<TYPE*> (object)
bool Collideable::wallCollision(MyObject* wall, MyObject* ball, float scale, CCSize size){
    int precision = 50;
    for(int i = 1; i < precision; i++){
        CCPoint positionInPath = wall->getPoint1();
        positionInPath.x += ((wall->getPoint2().x - wall->getPoint1().x) / precision) * i;
        positionInPath.y += ((wall->getPoint2().y - wall->getPoint1().y) / precision) * i;
        float distance = distanceBetweenPoints(positionInPath, ball->getPos());
        if(distance <= ball->getSize() / 2 - 1){
            //place the ball back in the playing area (don't get stuck in a wall) and then switch those velocities
            size.width *= scale;
            size.height *= scale;
            if(!ball->scored){
                    if(ball->getPositionX() > size.width - size.width / 11)
                        ball->setPositionX(size.width - size.width / 11 - 1 + ball->getSize() / 4);
                    if(ball->getPositionX() < size.width / 11)
                        ball->setPositionX(size.width / 11 + 1 - ball->getSize() / 4);
                    if(ball->getPositionY() > size.height - size.height / 6)
                        ball->setPositionY(size.height - size.height / 6 - 1 + ball->getSize() / 4);
                    if(ball->getPositionY() < size.height / 6)
                        ball->setPositionY(size.height / 6 + 1 - ball->getSize() / 4);
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
