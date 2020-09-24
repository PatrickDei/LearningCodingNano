//
//  Pool.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 07/09/2020.
//

#ifndef Pool_hpp
#define Pool_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "Ball.hpp"
#include <math.h>

using namespace cocos2d;

class Pool : public cocos2d::CCLayer{
public:
    virtual bool init() override;

    void callScene(){
        CCDirector::sharedDirector()->replaceScene(scene());
    }
    
    static CCScene* scene();
    
    void returnToMainMenu(){
           CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
    }
    
    virtual void update(float dt) override;
    
    CREATE_FUNC(Pool);
    
    
private:
    
    float imageScale;
    
    float ballScale;
    
    CCSize windowSize;
    
    CCSize tableSize;
    
    float ballSize;
    
    CCPoint prevPosition;
    
    std::vector<Ball*> balls;

    
    
    void setTheBalls();
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override;
    
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) override;

    bool whiteBallIsTapped(CCPoint touch);
    
    void checkForEdgeCollision(Ball* b);
    
    void calculatePosition(int indexOfBall);
    
    float distance(CCPoint posA, CCPoint posB);
    
    void exchangeVelocities(int indexA, int indexB);
    
    void clearOutBalls(std::vector<Ball> balls);
};

#endif /* Pool_hpp */
