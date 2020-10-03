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
    bool scored;
    
    Ball(float x, float y, float size) : friction(0.9), MyObject(new Collideable(), new Moveable(), x, y, size), scored(false) {}
    
    void updatePosition();
        
    void setAppropriatePosition(int direction, float size);
    
    void addBallToScoreboard(float scale, int numOfScoredBalls);
    
    void resetWhiteBall(CCSize tableSize, float imageScale);
    
private:
    float friction;
};

#endif /* Ball_hpp */
