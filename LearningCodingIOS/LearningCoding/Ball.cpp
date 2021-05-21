//
//  Ball.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 19/09/2020.
//

#include "Ball.hpp"
#include "Pool.hpp"

float Ball::getVelocityX(){
    return _velocityX;
}

void Ball::setVelocityX(float velocity){
    if(velocity <= MAX_SPEED && velocity >= -MAX_SPEED)
        _velocityX = velocity;
}

float Ball::getVelocityY(){
    return _velocityY;
}

void Ball::setVelocityY(float velocity){
    if(velocity <= MAX_SPEED && velocity >= -MAX_SPEED)
        _velocityY = velocity;
}

void Ball::addBallToScoreboard(int numOfScoredBalls){
    this->setVelocityX(0);
    this->setVelocityY(0);
    
    float horizontalPosition = this->getSize().width * (numOfScoredBalls + 1);
    float verticalPosition = CCDirector::sharedDirector()->getVisibleSize().height / 2;
    
    this->setPosition(horizontalPosition, verticalPosition);
    
    this->scored = true;
}

void Ball::resetWhiteBall(CCSize tableSize, float imageScale){
    this->setPosition(tableSize.width * imageScale * 3 / 4
                      , tableSize.height * imageScale / 2);
    
    this->setVelocityX(0);
    this->setVelocityY(0);
}

void Ball::updatePosition(float dt){
    float delta = friction * dt * 30;
    
    Position newPosition(this->getPostition().x + _velocityX * delta
                         , this->getPostition().y + _velocityY * delta);
    
    performUpdate(newPosition.x, newPosition.y);
    
    _velocityX *= friction;
    _velocityY *= friction;
}
