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
#include "GameObject.hpp"
#include "Moveable.hpp"
#include "Collideable.hpp"
USING_NS_CC;

class Ball : public GameObject{
public:
    
    Ball(float x, float y, float size) : friction(0.9), GameObject(new Collideable(), new Moveable(), x, y, size, "ball") {}
    
    void updatePosition(float dt);
            
    void addBallToScoreboard(int numOfScoredBalls);
    
    void resetWhiteBall(CCSize tableSize, float imageScale);
    
private:
    float friction;
};

#endif /* Ball_hpp */
