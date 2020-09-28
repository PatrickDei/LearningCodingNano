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
USING_NS_CC;

class Ball : public CCLayer{
public:
    float positionX;
    float positionY;
    
    bool scored;
    
    Ball(float x, float y) : friction(0.9), positionX(x), positionY(y), scored(false) {}
    
    ~Ball(){
        printf("destructor called");
    }
    
    
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
