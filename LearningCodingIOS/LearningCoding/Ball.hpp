//
//  Ball.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 19/09/2020.
//

#ifndef Ball_hpp
#define Ball_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "MyObject.hpp"
#include "Moveable.hpp"
#include "Collideable.hpp"
USING_NS_CC;

class Ball : public MyObject{
public:
    /*float positionX;
    float positionY;*/
    
    bool scored;
    
    Ball(float x, float y, float size) : friction(0.9), MyObject(new Collideable(), new Moveable(), x, y, size), scored(false) {}
    
    
    float getVelocityX() const;
    void setVelocityX(float _velocityX);
    
    float getVelocityY() const;
    void setVelocityY(float _velocityY);
    
    void updatePosition();
    
    CCPoint getPositionOfBall();
    
    void setAppropriatePosition(int direction, float size);
    
    
    
private:
    float velocityX;
    float velocityY;
    
    float friction;
};

#endif /* Ball_hpp */
