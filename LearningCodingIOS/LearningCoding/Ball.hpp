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
    
    Ball(float x, float y, float size) : friction(0.9), MyObject(new Collideable(), new Moveable(), x, y, size, false) {}
    
    void updatePosition(float dt);
            
    void addBallToScoreboard(int numOfScoredBalls);
    
    void resetWhiteBall(CCSize tableSize, float imageScale);
    
private:
    float friction;
};

#endif /* Ball_hpp */
