//
//  Pool.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 07/09/2020.
//

#include "Pool.hpp"

#define pi 3.141592654

CCScene* Pool::scene(){
    CCScene *scene = CCScene::create();
    
    Pool* layer = Pool::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool Pool::init(){
    if (!CCLayer::init()){
        return false;
    }
    
    windowSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* table = CCSprite::create("pool_table.png");
    
    imageScale = windowSize.width / table->getContentSize().width;
    
    table->setScale(imageScale);
    table->setAnchorPoint(CCPointZero);
    table->setPosition(CCPointZero);
    this->addChild(table);
    
    tableSize = table->getContentSize();
    
    setTheBalls();
    
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);
    
    this->scheduleUpdate();
    //printf("\n%f\n%f\n%f\n%f\n%f", tan(pi), atan(4 * pi / 3), atan(3.14159) * 180 / pi, atan(2 * pi / 3), atan(2) * 180 / pi);
    
    return true;
}

bool Pool::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(whiteBallIsTapped(pTouch->getLocation()))
        return true;
    return false;
}



//this is the stuff
void Pool::update(float dt){
    for(int i = 0; i < balls.size(); i++){
        if(balls[i]->getVelocityX() > 0.0001 || balls[i]->getVelocityY() > 0.0001)
            calculatePosition(i);
        balls[i]->updatePosition();
        this->getChildByTag(i)->setPosition(balls[i]->getPositionOfBall());
        checkForEdgeCollision(balls[i]);
        //clearOutBalls(balls);
    }
}

void Pool::clearOutBalls(std::vector<Ball> balls){
    bool canInitiate = true;
    for(auto b : balls)
        if(b.getVelocityY() > 0.0001 || b.getVelocityX() > 0.0001)
            canInitiate = false;
    //if(can)
}

void Pool::checkForEdgeCollision(Ball* b){
    if(b->getPositionOfBall().x <= tableSize.width / 11 * imageScale || b->getPositionOfBall().x >= (tableSize.width - tableSize.width / 11) * imageScale){
        b->setAppropriatePosition(1, tableSize.width * imageScale);
        b->setVelocityX(-b->getVelocityX());
    }
        
    if(b->getPositionOfBall().y <= tableSize.height / 6 * imageScale || b->getPositionOfBall().y >= (tableSize.height - tableSize.height / 6) * imageScale){
        b->setAppropriatePosition(2, tableSize.height * imageScale);
        b->setVelocityY(-b->getVelocityY());
    }
}

void Pool::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint touch = pTouch->getLocation();
    balls.front()->setVelocityX(balls.front()->positionX - touch.x);
    balls.front()->setVelocityY(balls.front()->positionY - touch.y);
    
}

bool Pool::whiteBallIsTapped(CCPoint touch){
    float distance;
    distance = sqrt(pow(balls.front()->getPositionOfBall().x - touch.x, 2) + pow(balls.front()->getPositionOfBall().y - touch.y, 2));
    return (distance <= ballSize * ballScale) ? true : false;
}

void Pool::setTheBalls(){
    int tagIndex = 0;
    CCSprite* whiteBall = CCSprite::create("white_ball.png");
    CCSprite* blackBall = CCSprite::create("black_ball.png");
    
    ballScale = (windowSize.width / 20) / whiteBall->getContentSize().width;
    
    Ball* b = new Ball(tableSize.width * imageScale * 3 / 4, tableSize.height * imageScale / 2);
    balls.push_back(b);

    whiteBall->setPosition(balls.front()->getPositionOfBall());
    whiteBall->setScale(ballScale);
    this->addChild(whiteBall, 1, tagIndex++);
    
    ballSize = whiteBall->getContentSize().height;
    float rowOffset = sqrt(pow(ballSize, 2) - pow(ballSize / 2, 2));
    float startPosition = tableSize.height * imageScale / 2 - 2 * ballSize * ballScale;
    
    
    
    for(int i = 5; i >= 1 ; i--){
        float initialOffset = (5 * ballSize / 2 - (i * ballSize / 2)) * ballScale;
        
        for(int j = 0; j < i ; j++){
            
            
            if(i == 3 && j == 1){
                Ball* b = new Ball(200 + (6 - i) * rowOffset * ballScale, initialOffset + startPosition + j * ballSize * ballScale);
                balls.push_back(b);

                blackBall->setPosition(balls.back()->getPositionOfBall());
                blackBall->setScale(ballScale);
                this->addChild(blackBall, 1, tagIndex++);
            }
            else{
                CCSprite* redBall = CCSprite::create("red_ball.png");
                Ball* b = new Ball(200 + (6 - i) * rowOffset * ballScale, initialOffset + startPosition + j * ballSize * ballScale);
                balls.push_back(b);
                redBall->setPosition(balls.back()->getPositionOfBall());
                redBall->setScale(ballScale);
                this->addChild(redBall, 1, tagIndex++);
            }
        }
    }
}

void Pool::calculatePosition(int indexOfBall){
    CCPoint savePosition = balls[indexOfBall]->getPositionOfBall();
    for(int i = 20; i >= 1; i--){
        CCPoint positionInPath = balls[indexOfBall]->getPositionOfBall();
        positionInPath.x += balls[indexOfBall]->getVelocityX() / i;
        positionInPath.y += balls[indexOfBall]->getVelocityY() / i;
        //check if any ball is in that position
        for(int j = 0; j < balls.size(); j++){
            if(distance(positionInPath, balls[j]->getPositionOfBall()) <= ballSize * ballScale - 1 && j != indexOfBall){
                //set ball position to the one before this
                balls[indexOfBall]->positionX = savePosition.x;
                balls[indexOfBall]->positionY = savePosition.y;
                //exchange velocities
                exchangeVelocities(indexOfBall, j);
            }
        }
        savePosition = positionInPath;
    }
}

float Pool::distance(CCPoint posA, CCPoint posB){
    return sqrt(pow(posA.x - posB.x, 2) + pow(posA.y - posB.y, 2));
}

void Pool::exchangeVelocities(int indexA, int indexB){
    //first i need the angle between the two balls (their centers)
    CCPoint whiteBallCenter = balls[indexA]->getPositionOfBall();
    CCPoint redBallCenter = balls[indexB]->getPositionOfBall();
    
    float xDistance = redBallCenter.x - whiteBallCenter.x;
    float yDistance = redBallCenter.y - whiteBallCenter.y;
    //float xDistance = balls[indexA]->getVelocityX() - balls[indexB]->getVelocityX();
    //float yDistance = balls[indexA]->getVelocityY() - balls[indexB]->getVelocityY();


    /*if(xDistance < 1)
        xDistance = 1;
    if(yDistance < 1)
        yDistance = 1;*/
    //get the angle of the white ball (before impact @alpha) and the red ball (after impact @beta)
    float alpha = atan(balls[indexA]->getVelocityY() / balls[indexA]->getVelocityX());
    float beta = atan(yDistance / xDistance);
    //prvi kvadrant
    if(xDistance < 0 && yDistance > 0)
        beta += pi;
    //treci kvadrant
    if(xDistance < 0 && yDistance < 0)
        beta += pi;
    //cetvrti kvadrant
    if(xDistance > 0 && yDistance < 0)
        beta += 2 * pi;
    
    float gamma;
    if(alpha < beta)
        gamma = beta + pi / 2;
    else
        gamma = beta - pi / 2;
    //printf("\nindexs 1: %d 2: %d\nx: %f y: %f", indexA, indexB, xDistance, yDistance);
    printf("\n%f\n", beta * 180 / pi);
    //now for the velocities
    
    
    
    
    
    float x3 = /*balls[indexA]->getVelocityX() - x2*/ (balls[indexA]->getVelocityY() - balls[indexA]->getVelocityX() * tan(beta)) / (tan(gamma) - tan(beta));
    float x2 = balls[indexA]->getVelocityX() - x3;
    float y2 = x2 * tan(beta);
    float y3 = balls[indexA]->getVelocityY() - y2;
    
    //printf("\nwhite (before): x: %f y: %f\nwhite (after): x: %f y: %f\nred (after): x: %f y: %f\n", balls[indexA]->getVelocityX(), balls[indexA]->getVelocityY(), x3, y3, x2, y2);
    
    //give them proper velocities
    balls[indexA]->setVelocityX(x3);
    balls[indexA]->setVelocityY(y3);
    
    balls[indexB]->setVelocityX(x2);
    balls[indexB]->setVelocityY(y2);
}
