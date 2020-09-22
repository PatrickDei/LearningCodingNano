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
    
    //CCSprite* ballImage;
    
    Ball(float x, float y) : friction(0.85), positionX(x), positionY(y) {
        printf("\nadded ball with x: %f y: %f", x, y);
    }
    
    ~Ball(){
        printf("destructor called");
    }
        
    void updatePosition();
    
    float getVelocityX() const;
    void setVelocityX(float _velocityX);
    
    float getVelocityY() const;
    void setVelocityY(float _velocityY);
    
    CCPoint getPositionOfBall();
    
    void setAppropriatePosition(int direction, float size);
    
private:
    float velocityX;
    float velocityY;
    
    float friction;
    
};

#endif /* Ball_hpp */
