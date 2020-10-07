//
//  Ball.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 19/09/2020.
//

#include "Ball.hpp"
#include "Pool.hpp"

void Ball::addBallToScoreboard(int numOfScoredBalls){
    this->setVelocityX(0);
    this->setVelocityY(0);
    this->positionX = this->sizeOfObject + this->sizeOfObject * numOfScoredBalls;
    this->positionY = CCDirector::sharedDirector()->getVisibleSize().height / 2;
    this->scored = true;
}

void Ball::resetWhiteBall(CCSize tableSize, float imageScale){
    this->positionX = tableSize.width * imageScale * 3 / 4;
    this->positionY = tableSize.height * imageScale / 2;
    this->setVelocityX(0);
    this->setVelocityY(0);
}

void Ball::updatePosition(){
    positionX = positionX + velocityX * friction;
    positionY = positionY + velocityY * friction;
    
    //slow the ball down
    velocityX *= friction;
    velocityY *= friction;
}


void Ball::setAppropriatePosition(float scale, CCSize size){
    size.width *= scale;
    size.height *= scale;
    if(!scored){
        //if(direction == 1){
            /*float howManyPixelsBehind = positionX - size / 11;
            //move it upwards for an appropriate amount
            positionY = (velocityY * positionX)
            //set it back*/
            if(positionX > size.width - size.width / 11)
                positionX = size.width - size.width / 11 - 1;
            if(positionX < size.width / 11)
                positionX = size.width / 11 + 1;
        //}
        //else{
            if(positionY > size.height - size.height / 6)
                positionY = size.height - size.height / 6 - 1;
            if(positionY < size.height / 6)
                positionY = size.height / 6 + 1;
        //}
    }
}
