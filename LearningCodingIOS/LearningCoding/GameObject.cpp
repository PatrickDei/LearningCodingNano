//
//  GameObject.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 24/10/2020.
//

#include "GameObject.hpp"
#include "Constants.hpp"

bool GameObject::isInCollision(GameObject* obj1, GameObject* obj2, float scale, CCSize size){
    return collideable->isInCollision(obj1, obj2, scale, size);
}

void GameObject::calculateVelocities(GameObject* obj1, GameObject* obj2){
    moveable->calculateVelocities(obj1, obj2);
}

void GameObject::bounce(GameObject* wall, GameObject* ball){
    moveable->bounce(wall, ball);
}

float GameObject::getPositionX() const{
    return positionX;
}

void GameObject::setPositionX(float pos){
    positionX = pos;
}

float GameObject::getPositionY() const{
    return positionY;
}

void GameObject::setPositionY(float pos){
    positionY = pos;
}

float GameObject::getVelocityX() const{
    return velocityX;
}

void GameObject::setVelocityX(float _velocityX){
    velocityX = (_velocityX < MAX_SPEED) ? _velocityX : MAX_SPEED;
}

float GameObject::getVelocityY() const{
    return velocityY;
}

void GameObject::setVelocityY(float _velocityY){
    velocityY = (_velocityY < MAX_SPEED) ? _velocityY : MAX_SPEED;
}

CCPoint GameObject::getPos(){
    CCPoint point = CCPoint(positionX, positionY);
    return point;
}

float GameObject::getSize() const{
    return sizeOfObject;
}

bool GameObject::isScored() const{
    return scored;
}

std::string GameObject::getType() const{
    return type;
}

CCPoint GameObject::getPoint1() const{
    return point1;
}

CCPoint GameObject::getPoint2() const{
    return point2;
}
