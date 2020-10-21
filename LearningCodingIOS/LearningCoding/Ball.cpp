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

void Ball::updatePosition(float dt){
    positionX = positionX + velocityX * friction * dt * 30;
    positionY = positionY + velocityY * friction * dt * 30;
    
    //slow the ball down
    velocityX *= friction;
    velocityY *= friction;
}
