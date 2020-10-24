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
#include "Wall.hpp"
#include "Hole.hpp"
#include <math.h>
#include "GameObject.hpp"

using namespace cocos2d;

class Pool : public cocos2d::CCLayer{
public:
    virtual bool init() override;

    void callScene(){
        CCDirector::sharedDirector()->replaceScene(scene());
    }
    
    static CCScene* scene();
    
    virtual void update(float dt) override;
    
    CREATE_FUNC(Pool);
    
    static bool gameRestart;
    
private:
    
    float imageScale;
    
    float ballScale;
    
    CCSize windowSize;
    
    CCSize tableSize;
    
    float ballSize;
    
    std::vector<GameObject*> objectList;
    
    int numOfScoredballs;
    
    CCLabelTTF* score;
    
    int consecutive;


    void setTheBalls();
    void setTheHoles();
    void setTheWalls();
    void createWalls(std::vector<CCPoint> points);
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override;
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) override;
    bool whiteBallIsTapped(CCPoint touch);
    
    void checkForEdgeCollision(int index);
    
    void handleCollisions(int indexOfBall);
    
    void checkHoles(int index);
    
    void restartGame();
};

#endif /* Pool_hpp */
