//
//  Moveable.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 02/10/2020.
//

#include "Moveable.hpp"

#define pi 3.141592654

void Moveable::calculateVelocities(MyObject* obj1, MyObject* obj2){
    //first i need the angle between the two balls (their centers)
    CCPoint whiteBallCenter = obj1->getPos();
    CCPoint redBallCenter = obj2->getPos();
    
    float xDistance = redBallCenter.x - whiteBallCenter.x;
    float yDistance = redBallCenter.y - whiteBallCenter.y;

    //get the angle of the white ball (before impact @alpha) and the red ball (after impact @beta)
    float alpha = atan(obj1->getVelocityY() / obj1->getVelocityX());
    float beta = atan(yDistance / xDistance);
    //prvi kvadrant
    if(xDistance < 0 && yDistance > 0)
        beta += pi;
    //treci kvadrant
    if(xDistance < 0 && yDistance < 0)
        beta += pi;
    //cetvrti kvadrant
    if(xDistance > 0 && yDistance < 0)
        beta += 2 * pi;
    if(obj1->getVelocityX() < 0 && obj1->getVelocityY() < 0)
        alpha += pi;
        
    float gamma;
    if(alpha < beta)
        gamma = beta + pi / 2;
    else
        gamma = beta - pi / 2;
    
    //now for the velocities
    float x3 = (obj1->getVelocityY() - obj1->getVelocityX() * tan(beta)) / (tan(gamma) - tan(beta));
    float x2 = obj1->getVelocityX() - x3;
    float y2 = x2 * tan(beta);
    float y3 = obj1->getVelocityY() - y2;
    
    //printf("\nwhite (before): x: %f y: %f\nwhite (after): x: %f y: %f\nred (after): x: %f y: %f\n", balls[indexA]->getVelocityX(), balls[indexA]->getVelocityY(), x3, y3, x2, y2);
    
    //give them proper velocities
    obj1->setVelocityX(x3);
    obj1->setVelocityY(y3);
    
    obj2->setVelocityX(x2);
    obj2->setVelocityY(y2);
}
