//
//  Pool.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 07/09/2020.
//

#include "Pool.hpp"
#include "Games.hpp"
#include "BilliardsMenu.hpp"

#define pi 3.141592654

CCScene* Pool::scene(){
    CCScene *scene = CCScene::create();
    
    Pool* layer = Pool::create();
    
    scene->addChild(layer);
    
    return scene;
}


bool Pool::gameRestart = false;
int BilliardsMenu::numOfBalls = 1;

bool Pool::init(){
    if (!CCLayer::init()){
        return false;
    }
    
    BilliardsMenu* billiardsMenu = BilliardsMenu::create();
    billiardsMenu->setZOrder(2);
    this->addChild(billiardsMenu);
        
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
    setTheWalls();
    setTheHoles();

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

void Pool::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint touch = pTouch->getLocation();
    balls.front()->setVelocityX(balls.front()->getPositionX() - touch.x);
    balls.front()->setVelocityY(balls.front()->getPositionY() - touch.y);
}

bool Pool::whiteBallIsTapped(CCPoint touch){
    float distance;
    distance = sqrt(pow(balls.front()->getPos().x - touch.x, 2) + pow(balls.front()->getPos().y - touch.y, 2));
    return (distance <= ballSize * ballScale) ? true : false;
}



void Pool::update(float dt){
    if(!gameRestart)
        for(int i = 0; i < balls.size(); i++){
            checkForEdgeCollision(i);
            if(balls[i]->getVelocityX() != 0 || balls[i]->getVelocityY() != 0){
                handleCollisions(i);
                checkHoles(i);
            }
            balls[i]->updatePosition();
            this->getChildByTag(i)->setPosition(balls[i]->getPos());
        }
    else
        resetGame();
}



void Pool::checkHoles(int index){
    for(auto h : holes)
        if(abs(balls[index]->getPositionX() - h.x) <= 10 || abs(balls[index]->getPositionY() - h.y) <= 10){
            printf("yup");
            addToScoreboard(balls[index]);
        }
}


void Pool::checkForEdgeCollision(int index){
    for(auto w : walls)
        if(balls[index]->wallCollision(w, balls[index])){
            w->bounce(w, balls[index]);
            /*if(!inTableHole(index))
                balls[index]->setAppropriatePosition(imageScale, tableSize);*/
        }
}

//tableSize * image scale fix
bool Pool::inTableHole(int index){
    /*if(abs(balls[index]->getPositionX() - tableSize.width * imageScale / 2) <= 20){
        if(balls[index]->getPositionY() <= tableSize.height * imageScale / 11 || balls[index]->getPositionY() <= tableSize.height - tableSize.height * imageScale / 11)
            addToScoreboard(balls[index]);
        return true;
    }
    if(abs(balls[index]->getPositionX() - tableSize.width * imageScale / 11) <= 20 && (abs(balls[index]->getPositionY() - tableSize.height * imageScale / 6) <= 20 || abs(-balls[index]->getPositionY() + tableSize.height * imageScale - tableSize.height * imageScale / 6) <= 20)){
        if(balls[index]->getPositionX() < tableSize.width * imageScale / 11 || balls[index]->getPositionY() < tableSize.height * imageScale / 11 || balls[index]->getPositionY() >= tableSize.height - tableSize.height * imageScale / 11)
            addToScoreboard(balls[index]);
        return true;
    }
    if(abs(balls[index]->getPositionX() - (tableSize.width * imageScale - tableSize.width * imageScale / 11)) <= 20 && (abs(balls[index]->getPositionY() - tableSize.height * imageScale / 6) <= 20 || abs(-balls[index]->getPositionY() + tableSize.height * imageScale - tableSize.height * imageScale / 6) <= 20)){
        if(balls[index]->getPositionX() > (tableSize.width * imageScale - tableSize.width * imageScale / 11) || balls[index]->getPositionY() < tableSize.height * imageScale / 11 || balls[index]->getPositionY() >= tableSize.height - tableSize.height * imageScale / 11)
            addToScoreboard(balls[index]);
        return true;
    }*/
    bool inHole = false;
    tableSize.width *= imageScale;
    tableSize.height *= imageScale;
    printf("\ncall for this func");
    if((balls[index]->getPositionY() > tableSize.height - tableSize.height / 6
        || balls[index]->getPositionY() < tableSize.height / 6)
        && index != 0){
        if(abs(balls[index]->getPositionX() - tableSize.width * imageScale / 2) <= 20){
            inHole = true;
            printf("yupuajshdkjashdjkas");
            addToScoreboard(balls[index]);
        }
        if(balls[index]->getPositionX() < tableSize.width / 11 || balls[index]->getPositionX() > tableSize.width - tableSize.width / 11){
            inHole = true;
            printf("yupuajshdkjashdjkas");

            addToScoreboard(balls[index]);
        }
        if(abs(balls[index]->getPositionX() - tableSize.width / 2) <= ballSize * 1.5){
            inHole = true;
            printf("yupuajshdkjashdjkas");

            addToScoreboard(balls[index]);
        }
    }
    
    tableSize.width /= imageScale;
    tableSize.height /= imageScale;
    return inHole;
}

void Pool::addToScoreboard(Ball* b){
    if(balls.size() > 11)
    if(b->getPos().x == balls[11]->getPos().x && b->getPos().y == balls[11]->getPos().y && numOfScoredballs != 14){
        score->setString("game over");
        b->addBallToScoreboard(numOfScoredballs++);
    }
    else if(b->getPos().x == balls.front()->getPos().x && b->getPos().y == balls.front()->getPos().y){
        consecutive = 1;
        b->resetWhiteBall(tableSize, imageScale);
    }
    else{
        b->addBallToScoreboard(numOfScoredballs++);
        score->setString(std::to_string(atoi(score->getString()) + consecutive).c_str());
        consecutive++;
    }
}

void Pool::setTheWalls(){
    tableSize.width *= imageScale;
    tableSize.height *= imageScale;
    std::vector<CCPoint> points;
    points.push_back(CCPoint(tableSize.width / 11 + 20, tableSize.height / 6 - ballSize / 2));
    points.push_back(CCPoint(tableSize.width / 2 - 30, tableSize.height / 6 - ballSize / 2));

    points.push_back(CCPoint(tableSize.width / 2 + 30, tableSize.height / 6 - ballSize / 2));
    points.push_back(CCPoint(tableSize.width - (tableSize.width / 11 + 20), tableSize.height / 6 - ballSize / 2));

    points.push_back(CCPoint(tableSize.width - tableSize.width / 11 + ballSize / 2, tableSize.height / 6 + 20));
    points.push_back(CCPoint(tableSize.width - tableSize.width / 11 + ballSize / 2, tableSize.height - (tableSize.height / 6 + 20)));

    points.push_back(CCPoint(tableSize.width - (tableSize.width / 11 + 20), tableSize.height - tableSize.height / 6 + ballSize / 2));
    points.push_back(CCPoint(tableSize.width / 2 + 30, tableSize.height - tableSize.height / 6 + ballSize / 2));

    points.push_back(CCPoint(tableSize.width / 2 - 30, tableSize.height - tableSize.height / 6 + ballSize / 2));
    points.push_back(CCPoint(tableSize.width / 11 + 20, tableSize.height - tableSize.height / 6 + ballSize / 2));
    
    points.push_back(CCPoint(tableSize.width / 11 - ballSize / 2, tableSize.height / 6 + 20));
    points.push_back(CCPoint(tableSize.width / 11 - ballSize / 2, tableSize.height - (tableSize.height / 6 + 20)));
    
    points.push_back(CCPoint(190, 150));
    points.push_back(CCPoint(290, 250));
    
    
    points.push_back(CCPoint(490, 100));
    points.push_back(CCPoint(540, 250));
    
    tableSize.width /= imageScale;
    tableSize.height /= imageScale;
    
    createWalls(points);
}

void Pool::createWalls(std::vector<CCPoint> points){
    for(int i = 0; i < points.size(); i+=2){
        Wall* w = new Wall(points[i], points[i + 1]);
        CCDrawNode* line = CCDrawNode::create();
        line->drawSegment(points[i], points[i + 1], 5, ccc4f(1, 1, 1, 1));
        this->addChild(line);
        walls.push_back(w);
    }
}

void Pool::setTheHoles(){
    tableSize.width *= imageScale;
    tableSize.height *= imageScale;
    
    holes.push_back(CCPoint(tableSize.width / 11 - ballSize, tableSize.height / 6 - ballSize));
    holes.push_back(CCPoint(tableSize.width / 2, tableSize.height / 6 - ballSize));
    holes.push_back(CCPoint(tableSize.width - tableSize.width / 11 + ballSize, tableSize.height / 6 - ballSize));
    holes.push_back(CCPoint(tableSize.width - tableSize.width / 11 + ballSize, tableSize.height - tableSize.height / 6 + ballSize));
    holes.push_back(CCPoint(tableSize.width / 2, tableSize.height - tableSize.height / 6 + ballSize));
    holes.push_back(CCPoint(tableSize.width / 11 - ballSize, tableSize.height - tableSize.height / 6 + ballSize));
    
    tableSize.width /= imageScale;
    tableSize.height /= imageScale;
}

void Pool::setTheBalls(){
    int tagIndex = 0;
    CCSprite* whiteBall = CCSprite::create("white_ball.png");
    CCSprite* blackBall = CCSprite::create("black_ball.png");
    
    ballScale = (windowSize.width / 20) / whiteBall->getContentSize().width;
    
    Ball* b = new Ball(tableSize.width * imageScale * 3 / 4, tableSize.height * imageScale / 2, whiteBall->getContentSize().width * ballScale);
    whiteBall->setColor(BilliardsMenu::colorOfWhiteBall);
    balls.push_back(b);

    whiteBall->setPosition(balls.front()->getPos());
    whiteBall->setScale(ballScale);
    this->addChild(whiteBall, 1, tagIndex++);
    
    ballSize = whiteBall->getContentSize().height;
    float rowOffset = sqrt(pow(ballSize, 2) - pow(ballSize / 2, 2));
    float startPosition = tableSize.height * imageScale / 2 - 2 * ballSize * ballScale;
    
    int totalBalls = 0;
    
    for(int i = 5; i >= 1 ; i--){
        float initialOffset = (5 * ballSize / 2 - (i * ballSize / 2)) * ballScale;
        
        for(int j = 0; j < i ; j++){
            
            if(totalBalls == BilliardsMenu::numOfBalls)
                return;
            
            if(i == 3 && j == 1){
                Ball* b = new Ball(200 + (6 - i) * rowOffset * ballScale, initialOffset + startPosition + j * ballSize * ballScale, whiteBall->getContentSize().width * ballScale);
                balls.push_back(b);

                blackBall->setPosition(balls.back()->getPos());
                blackBall->setScale(ballScale);
                this->addChild(blackBall, 1, tagIndex++);
            }
            else{
                CCSprite* redBall = CCSprite::create("red_ball.png");
                Ball* b = new Ball(200 + (6 - i) * rowOffset * ballScale, initialOffset + startPosition + j * ballSize * ballScale, whiteBall->getContentSize().width * ballScale);
                balls.push_back(b);
                redBall->setPosition(balls.back()->getPos());
                redBall->setScale(ballScale);
                this->addChild(redBall, 1, tagIndex++);
            }
            totalBalls++;
        }
    }
}

void Pool::handleCollisions(int indexOfBall){
    for(int j = 0; j < balls.size(); j++)
        if(j != indexOfBall)
            if(balls[indexOfBall]->isInCollision(balls[indexOfBall], balls[j]))
                balls[indexOfBall]->calculateVelocities(balls[indexOfBall], balls[j]);

}

void Pool::resetGame(){
    for(int i = 0; i < balls.size(); i++)
        this->removeChildByTag(i, true);
    
    balls.clear();
    
    numOfScoredballs = 0;
    consecutive = 1;
    
    setTheBalls();
    
    score->setValue(numOfScoredballs);
    gameRestart = false;
}
