//
//  Ball.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 19/09/2020.
//

#include "Ball.hpp"
#include "Pool.hpp"

void Ball::addBallToScoreboard(int numOfScoredBalls){
    variables.setVelocityX(0);
    variables.setVelocityY(0);
    variables.setPositionX(variables.getSize() + variables.getSize() * numOfScoredBalls);
    variables.setPositionY(CCDirector::sharedDirector()->getVisibleSize().height / 2);
    variables.setScored(true);
}

void Ball::resetWhiteBall(CCSize tableSize, float imageScale){
    variables.setPositionX(tableSize.width * imageScale * 3 / 4);
    variables.setPositionY(tableSize.height * imageScale / 2);
    variables.setVelocityX(0);
    variables.setVelocityY(0);
}

void Ball::updatePosition(float dt){
    variables.setPositionX(variables.getPositionX() + variables.getVelocityX() * friction * dt * 30);
    positionY = positionY + velocityY * friction * dt * 30;
    
    //slow the ball down
    velocityX *= friction;
    velocityY *= friction;
}
