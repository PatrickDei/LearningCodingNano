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
#include "PoolObject.hpp"
#include "MoveableObject.hpp"
#include "Constants.hpp"
#include "BallCollider.hpp"
USING_NS_CC;

class Ball : public PoolObject{
public:
    bool scored;
    
    Ball(float x, float y, float radius)
        : friction(0.9)
        , PoolObject(x, y, radius, radius, new MoveableObject(), new BallCollider())
        , previousPosition(x, y)
        , scored(false) {}
                
    void addBallToScoreboard(int numOfScoredBalls);
    
    void resetWhiteBall(CCSize tableSize, float imageScale);
    
    virtual void updatePosition(float dt) override;
    
private:
    float friction;
    
    float _velocityX;
    float _velocityY;
    
    Position previousPosition;
    
    float getVelocityX();
    
    void setVelocityX(float velocity);
    
    float getVelocityY();
    
    void setVelocityY(float velocity);
    };

#endif /* Ball_hpp */
