//
//  Pool.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 07/09/2020.
//

#include "Pool.hpp"
#include "Games.hpp"

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
    
    this->addChild(createBackButton(this, windowSize));
    
    CCSprite* table = CCSprite::create("pool_table.png");
    
    imageScale = windowSize.width / table->getContentSize().width;
    
    table->setScale(imageScale);
    table->setAnchorPoint(CCPointZero);
    table->setPosition(CCPointZero);
    this->addChild(table);
    
    tableSize = table->getContentSize();
    
    numOfScoredballs = 0;
    consecutive = 1;
    
    setTheBalls();
    
    score = CCLabelTTF::create(std::to_string(numOfScoredballs).c_str(), "Arial", 50);
    score->setPosition(ccp(windowSize.width / 2, windowSize.height - score->getContentSize().height));
    this->addChild(score);
    
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);
    
    this->scheduleUpdate();
    
    return true;
}

bool Pool::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(whiteBallIsTapped(pTouch->getLocation()))
        return true;
    return false;
}

bool Pool::whiteBallIsTapped(CCPoint touch){
    float distance;
    distance = sqrt(pow(balls.front()->getPositionOfBall().x - touch.x, 2) + pow(balls.front()->getPositionOfBall().y - touch.y, 2));
    return (distance <= ballSize * ballScale) ? true : false;
}




void Pool::update(float dt){
    for(int i = 0; i < balls.size(); i++){
        checkForEdgeCollision(i);
        if(balls[i]->getVelocityX() != 0 || balls[i]->getVelocityY() != 0)
            calculatePosition(i);
        balls[i]->updatePosition();
        this->getChildByTag(i)->setPosition(balls[i]->getPositionOfBall());
    }
}

void Pool::checkForEdgeCollision(int index){
    
    if(balls[index]->getPositionOfBall().x <= tableSize.width / 11 * imageScale || balls[index]->getPositionOfBall().x >= (tableSize.width - tableSize.width / 11) * imageScale){
        if(!inTableHole(index, 0)){
            balls[index]->setAppropriatePosition(1, tableSize.width * imageScale);
            balls[index]->setVelocityX(-balls[index]->getVelocityX());
        }
    }
        
    if(balls[index]->getPositionOfBall().y <= tableSize.height / 6 * imageScale || balls[index]->getPositionOfBall().y >= (tableSize.height - tableSize.height / 6) * imageScale){
        if(!inTableHole(index, 1)){
            balls[index]->setAppropriatePosition(2, tableSize.height * imageScale);
            balls[index]->setVelocityY(-balls[index]->getVelocityY());
        }
    }
}

void Pool::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint touch = pTouch->getLocation();
    balls.front()->setVelocityX(balls.front()->positionX - touch.x);
    balls.front()->setVelocityY(balls.front()->positionY - touch.y);
}



//tableSize * image scale fix



bool Pool::inTableHole(int index, int direction){
    if(abs(balls[index]->positionX - tableSize.width * imageScale / 2) <= 20){
        if(balls[index]->positionY <= tableSize.height * imageScale / 11 || balls[index]->positionY <= tableSize.height - tableSize.height * imageScale / 11)
            addToScoreboard(balls[index]);
        return true;
    }
    if(abs(balls[index]->positionX - tableSize.width * imageScale / 11) <= 20 && (abs(balls[index]->positionY - tableSize.height * imageScale / 6) <= 20 || abs(-balls[index]->positionY + tableSize.height * imageScale - tableSize.height * imageScale / 6) <= 20)){
        if(balls[index]->positionX < tableSize.width * imageScale / 11 || balls[index]->positionY < tableSize.height * imageScale / 11 || balls[index]->positionY >= tableSize.height - tableSize.height * imageScale / 11)
            addToScoreboard(balls[index]);
        return true;
    }
    if(abs(balls[index]->positionX - (tableSize.width * imageScale - tableSize.width * imageScale / 11)) <= 20 && (abs(balls[index]->positionY - tableSize.height * imageScale / 6) <= 20 || abs(-balls[index]->positionY + tableSize.height * imageScale - tableSize.height * imageScale / 6) <= 20)){
        if(balls[index]->positionX > (tableSize.width * imageScale - tableSize.width * imageScale / 11) || balls[index]->positionY < tableSize.height * imageScale / 11 || balls[index]->positionY >= tableSize.height - tableSize.height * imageScale / 11)
            addToScoreboard(balls[index]);
        return true;
    }
    return false;
}

void Pool::addToScoreboard(Ball* b){
    if(b->getPositionOfBall().x == balls[11]->getPositionOfBall().x && b->getPositionOfBall().y == balls[11]->getPositionOfBall().y && numOfScoredballs != 14){
        score->setString("game over");
        b->setVelocityX(0);
        b->setVelocityY(0);
        b->positionX = ballSize + ballScale * ballSize * numOfScoredballs++;
        b->positionY = windowSize.height / 2;
        b->scored = true;
    }
    else if(b->getPositionOfBall().x == balls.front()->getPositionOfBall().x && b->getPositionOfBall().y == balls.front()->getPositionOfBall().y){
        consecutive = 1;
        b->positionX = tableSize.width * imageScale * 3 / 4;
        b->positionY = tableSize.height * imageScale / 2;
        b->setVelocityX(0);
        b->setVelocityY(0);
    }
    else{
        b->setVelocityX(0);
        b->setVelocityY(0);
        b->positionX = ballSize + ballScale * ballSize * numOfScoredballs++;
        b->positionY = windowSize.height / 2;
        b->scored = true;
        int scoreSoFar = atoi(score->getString());
        score->setString(std::to_string(scoreSoFar + consecutive).c_str());
        consecutive++;
    }
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
                //balls[indexOfBall]->positionX = savePosition.x;
                //balls[indexOfBall]->positionY = savePosition.y;
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
    if(balls[indexA]->getVelocityX() < 0 && balls[indexA]->getVelocityY() < 0)
        alpha += pi;
        
    float gamma;
    if(alpha < beta)
        gamma = beta + pi / 2;
    else
        gamma = beta - pi / 2;
    
    //now for the velocities
    float x3 = (balls[indexA]->getVelocityY() - balls[indexA]->getVelocityX() * tan(beta)) / (tan(gamma) - tan(beta));
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
