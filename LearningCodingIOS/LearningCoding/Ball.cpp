//
//  Ball.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 19/09/2020.
//

#include "Ball.hpp"

float Ball::getVelocityX() const{
    return velocityX;
}

void Ball::setVelocityX(float _velocityX){
    velocityX = (_velocityX < 300) ? _velocityX : 300;
}


float Ball::getVelocityY() const{
    return velocityY;
}

void Ball::setVelocityY(float _velocityY){
    velocityY = (_velocityY < 300) ? _velocityY : 300;
}

CCPoint Ball::getPositionOfBall(){
    CCPoint point = CCPoint(positionX, positionY);
    return point;
}

void Ball::updatePosition(){
    positionX = positionX + velocityX * friction;
    positionY = positionY + velocityY * friction;
    
    //slow the ball down
    velocityX *= friction;
    velocityY *= friction;
}


void Ball::setAppropriatePosition(int direction, float size){
    if(direction == 1){
        /*float howManyPixelsBehind = positionX - size / 11;
        //move it upwards for an appropriate amount
        positionY = (velocityY * positionX)
        //set it back*/
        if(positionX > size / 11)
            positionX = size - size / 11 - 1;
        else
            positionX = size / 11 + 1;
    }
    else{
        if(positionY > size / 6)
            positionY = size - size / 6 - 1;
        else
            positionY = size / 6 + 1;
    }
}
