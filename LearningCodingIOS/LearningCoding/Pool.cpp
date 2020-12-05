//
//  Pool.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 07/09/2020.
//

#include "Pool.hpp"
#include "BilliardsMenu.hpp"

bool Pool::gameRestart = false;

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
    
    startTheMenu();
        
    CCSize windowSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCMenuItemImage* image = CCMenuItemImage::create("button_close.png", "button_close_selected.png");
    CCMenuItemToggle* menuButton = CCMenuItemToggle::createWithTarget(this, menu_selector(Pool::startTheMenu), image, NULL);
    menuButton->setPosition(ccp(menuButton->getContentSize().width / 2, windowSize.height - menuButton->getContentSize().height / 2));
    CCMenu* menu = CCMenu::create(menuButton, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 2);
    
    CCSprite* table = CCSprite::create("pool_table.png");
    
    imageScale = windowSize.width / table->getContentSize().width;
    
    table->setScale(imageScale);
    table->setAnchorPoint(CCPointZero);
    table->setPosition(CCPointZero);
    this->addChild(table);
    
    tableSize = table->getContentSize();
    
    numOfScoredballs = 0;
    consecutive = 1;

    score = CCLabelTTF::create(std::to_string(numOfScoredballs).c_str(), "Arial", 50);
    score->setPosition(ccp(windowSize.width / 2, windowSize.height - score->getContentSize().height));
    this->addChild(score);
    
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);
    
    this->scheduleUpdate();

    return true;
}

void Pool::startTheMenu(){
    BilliardsMenu* billiardsMenu = BilliardsMenu::create();
    billiardsMenu->setZOrder(2);
    this->addChild(billiardsMenu);
}

bool Pool::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(whiteBallIsTapped(pTouch->getLocation()))
        return true;
    return false;
}

void Pool::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint touch = pTouch->getLocation();
    objectList.front()->setVelocityX(objectList.front()->getPositionX() - touch.x);
    objectList.front()->setVelocityY(objectList.front()->getPositionY() - touch.y);
}

bool Pool::whiteBallIsTapped(CCPoint touch){
    float distance;
    distance = sqrt(pow(objectList.front()->getPos().x - touch.x, 2) + pow(objectList.front()->getPos().y - touch.y, 2));
    return (distance <= ballSize * ballScale) ? true : false;
}



void Pool::update(float dt){
    if(!gameRestart)
        for(int i = 0; i < objectList.size(); i++){
            if(Ball* b = dynamic_cast<Ball*>(objectList[i])){
                checkForEdgeCollision(i);
                checkHoles(i);
                //"expensive" operation -> jsut checking for the ball in ball bug :D
                if(objectList[i]->getVelocityX() != 0 || objectList[i]->getVelocityY() != 0)
                    handleCollisions(i);
                dynamic_cast<Ball*>(objectList[i])->updatePosition(dt);
                this->getChildByTag(i)->setPosition(objectList[i]->getPos());
            }
        }
    else
        restartGame();
}



void Pool::checkHoles(int index){
    for(auto o : objectList)
        //check only the holes
        if(Hole* h = dynamic_cast<Hole*>(o))
            if(abs(objectList[index]->getPositionX() - o->getPositionX()) <= o->getSize() && abs(objectList[index]->getPositionY() - o->getPositionY()) <= o->getSize()){
                //black ball
                if(objectList[index]->getPos().x == objectList[blackBallIndex]->getPos().x
                    && objectList[index]->getPos().y == objectList[blackBallIndex]->getPos().y
                    && numOfScoredballs != 14){
                        score->setString("game over");
                        dynamic_cast<Ball*>(objectList[index])->addBallToScoreboard(numOfScoredballs++);
                }
                //white ball
                else if(objectList[index]->getPos().x == objectList.front()->getPos().x
                    && objectList[index]->getPos().y == objectList.front()->getPos().y){
                        consecutive = 1;
                        dynamic_cast<Ball*>(objectList[index])->resetWhiteBall(tableSize, imageScale);
                }
                //red ball
                else{
                    dynamic_cast<Ball*>(objectList[index])->addBallToScoreboard(numOfScoredballs++);
                    score->setString(std::to_string(atoi(score->getString()) + consecutive).c_str());
                    consecutive++;
                }
            }
}

void Pool::checkForEdgeCollision(int index){
    for(auto o : objectList)
        if(Wall* h = dynamic_cast<Wall*>(o))
            if(objectList[index]->isInCollision(objectList[index], o, imageScale, tableSize))
                o->bounce(o, objectList[index]);
}

void Pool::handleCollisions(int indexOfBall){
    for(int j = 0; j < objectList.size(); j++)
        if(j != indexOfBall)
            if(objectList[indexOfBall]->isInCollision(objectList[indexOfBall], objectList[j], imageScale, tableSize))
                objectList[indexOfBall]->calculateVelocities(objectList[indexOfBall], objectList[j]);
}

void Pool::restartGame(){
    for(int i = 0; i < objectList.size(); i++)
        this->removeChildByTag(i, true);
    
    objectList.clear();
    
    numOfScoredballs = 0;
    consecutive = 1;
    
    setTheBalls();
    setTheWalls();
    setTheHoles();
    
    score->setValue(numOfScoredballs);
    gameRestart = false;
}

void Pool::setTheWalls(){
    tableSize.width *= imageScale;
    tableSize.height *= imageScale;
    std::vector<CCPoint> points;
    points.push_back(CCPoint(tableSize.width / 12 + 20, tableSize.height / 6 - ballSize / 2));
    points.push_back(CCPoint(tableSize.width / 2 - 30, tableSize.height / 6 - ballSize / 2));

    points.push_back(CCPoint(tableSize.width / 2 + 30, tableSize.height / 6 - ballSize / 2));
    points.push_back(CCPoint(tableSize.width - (tableSize.width / 12 + 20), tableSize.height / 6 - ballSize / 2));

    points.push_back(CCPoint(tableSize.width - tableSize.width / 12 + ballSize / 2, tableSize.height / 6 + 20));
    points.push_back(CCPoint(tableSize.width - tableSize.width / 12 + ballSize / 2, tableSize.height - (tableSize.height / 6 + 20)));

    points.push_back(CCPoint(tableSize.width - (tableSize.width / 12 + 20), tableSize.height - tableSize.height / 6 + ballSize / 2));
    points.push_back(CCPoint(tableSize.width / 2 + 30, tableSize.height - tableSize.height / 6 + ballSize / 2));

    points.push_back(CCPoint(tableSize.width / 2 - 30, tableSize.height - tableSize.height / 6 + ballSize / 2));
    points.push_back(CCPoint(tableSize.width / 12 + 20, tableSize.height - tableSize.height / 6 + ballSize / 2));
    
    points.push_back(CCPoint(tableSize.width / 12 - ballSize / 2, tableSize.height / 6 + 20));
    points.push_back(CCPoint(tableSize.width / 12 - ballSize / 2, tableSize.height - (tableSize.height / 6 + 20)));
    
    /*points.push_back(CCPoint(190, 150));
    points.push_back(CCPoint(290, 250));
    
    
    points.push_back(CCPoint(490, 100));
    points.push_back(CCPoint(540, 250));*/
    
    tableSize.width /= imageScale;
    tableSize.height /= imageScale;
    
    for(int i = 0; i < points.size(); i+=2){
        Wall* w = new Wall(points[i], points[i + 1]);
        CCDrawNode* line = CCDrawNode::create();
        line->drawSegment(points[i], points[i + 1], 5, ccc4f(1, 1, 1, 1));
        this->addChild(line);
        objectList.push_back(w);
    }
}

void Pool::setTheHoles(){
    tableSize.width *= imageScale;
    tableSize.height *= imageScale;
    
    std::vector<CCPoint> holes;
    holes.push_back(CCPoint(tableSize.width / 11 - ballSize / 2, tableSize.height / 6 - ballSize / 2));
    holes.push_back(CCPoint(tableSize.width / 2, tableSize.height / 6 - ballSize * 3 / 4));
    holes.push_back(CCPoint(tableSize.width - tableSize.width / 11 + ballSize / 2, tableSize.height / 6 - ballSize / 2));
    holes.push_back(CCPoint(tableSize.width - tableSize.width / 11 + ballSize / 2, tableSize.height - tableSize.height / 6 + ballSize / 2));
    holes.push_back(CCPoint(tableSize.width / 2, tableSize.height - tableSize.height / 6 + ballSize * 3 / 4));
    holes.push_back(CCPoint(tableSize.width / 11 - ballSize / 2, tableSize.height - tableSize.height / 6 + ballSize / 2));
    
    tableSize.width /= imageScale;
    tableSize.height /= imageScale;
    
    for(auto h : holes){
        CCDrawNode* dot = CCDrawNode::create();
        dot->drawDot(h, ballSize * ballScale, ccc4f(0, 1, 0, 0.3));
        this->addChild(dot);
        Hole* hole = new Hole(h.x, h.y, ballSize * ballScale);
        objectList.push_back(hole);
    }
}

void Pool::setTheBalls(){
    //int tagIndex = 0;
    CCSprite* whiteBall = CCSprite::create("white_ball.png");
    CCSprite* blackBall = CCSprite::create("black_ball.png");
    
    CCSize windowSize = CCDirector::sharedDirector()->getVisibleSize();
    
    ballSize = whiteBall->getContentSize().height;
    ballScale = (windowSize.width / 20) / whiteBall->getContentSize().width;
    
    Ball* b = new Ball(tableSize.width * imageScale * 3 / 4, tableSize.height * imageScale / 2, whiteBall->getContentSize().width * ballScale);
    whiteBall->setColor(BilliardsMenu::colorOfWhiteBall);
    objectList.push_back(b);

    drawBall(whiteBall);
    
    float rowOffset = sqrt(pow(ballSize, 2) - pow(ballSize / 2, 2));
    float startPosition = tableSize.height * imageScale / 2 - 2 * ballSize * ballScale;
    
    int totalBalls = 0;
    
    for(int i = 5; i >= 1; i--){
        float initialOffset = (5 * ballSize / 2 - (i * ballSize / 2)) * ballScale;
        
        for(int j = 0; j < i ; j++){
            if(totalBalls == BilliardsMenu::numOfBalls)
                return;
            
            if((i == 3 && j == 1) || (BilliardsMenu::numOfBalls < 11 && totalBalls == BilliardsMenu::numOfBalls - 1)){
                Ball* b = new Ball(200 + (6 - i) * rowOffset * ballScale, initialOffset + startPosition + j * ballSize * ballScale, whiteBall->getContentSize().width * ballScale);
                objectList.push_back(b);

                drawBall(blackBall);
                blackBallIndex = objectList.size() - 1;
            }
            else{
                CCSprite* redBall = CCSprite::create("red_ball.png");
                Ball* b = new Ball(200 + (6 - i) * rowOffset * ballScale, initialOffset + startPosition + j * ballSize * ballScale, whiteBall->getContentSize().width * ballScale);
                objectList.push_back(b);
                
                drawBall(redBall);
            }
            totalBalls++;
        }
    }
}

void Pool::drawBall(CCSprite* image){
    image->setPosition(objectList.back()->getPos());
    image->setScale(ballScale);
    this->addChild(image, 1, objectList.size() - 1);
}
