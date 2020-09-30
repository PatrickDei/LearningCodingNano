//
//  PolygonDraw.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 07/09/2020.
//

#include "PolygonDraw.hpp"
#include <math.h>
#include <string>

CCScene* PolygonDraw::scene(){
    CCScene *scene = CCScene::create();
    
    PolygonDraw* layer = PolygonDraw::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool PolygonDraw::init(){
    if (!CCLayer::init()){
        return false;
    }
    
    CCSize screenSize = CCDirector::sharedDirector()->getVisibleSize();
    
    this->addChild(createBackButton(this, screenSize));
    
    
    CCMenuItemToggle* saveButton = CCMenuItemToggle::createWithTarget(this, menu_selector(PolygonDraw::saveButtonFunc), getButtonImage(), NULL);
    CCMenuItemToggle* loadButton = CCMenuItemToggle::createWithTarget(this, menu_selector(PolygonDraw::loadButtonFunc), getButtonImage(), NULL);
    CCMenuItemToggle* delButton = CCMenuItemToggle::createWithTarget(this, menu_selector(PolygonDraw::delButtonFunc), getButtonImage(), NULL);
    
    saveButton->setPosition(ccp(screenSize.width / 4, saveButton->getContentSize().height));
    loadButton->setPosition(ccp(screenSize.width / 2, saveButton->getContentSize().height));
    delButton->setPosition(ccp(screenSize.width / 4 * 3, saveButton->getContentSize().height));

    CCMenu* menu = CCMenu::create(saveButton, loadButton, delButton, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);

    
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);
    
    isLatestShapeFinished = false;
    dotsInShape.push_back(0);
    sumOfPoints = 0;
    
    return true;
}


bool PolygonDraw::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    //limited the screen above the buttons
    if(pTouch->getLocation().y > 100){
        points.push_back(pTouch->getLocation());
        
        shapeIsCompleted = isCompleted();
        if(isNewShape()){
            drawDot();
            isLatestShapeFinished = false;
        }
        else{
            drawVertex();
            if(!shapeIsCompleted)
                drawDot();
            if(shapeIsCompleted)
                isLatestShapeFinished = true;
        }
    }
    return true;
}


bool PolygonDraw::isNewShape(){
    return (points.size() - sumOfPoints == 1) ? true : false;
}

void PolygonDraw::drawDot(){
    CCDrawNode* dot = CCDrawNode::create();
    dot->drawDot(points.back(), 15, ccc4f(0, 0, 1, 1));
    this->addChild(dot, 2, points.size() + 1000);
    dotsInShape[dotsInShape.size() - 1]++;
}

bool PolygonDraw::isCompleted(){
    //shape needs at least 3 points
    if(points.size() - sumOfPoints > 2)
        return (distanceFromStart() <= 20) ? true : false;
    return false;
}

float PolygonDraw::distanceFromStart(){
    float distance = 0;
    distance = sqrt(pow(points[0 + sumOfPoints].x - points[points.size()-1].x, 2) + pow(points[0 + sumOfPoints].y - points[points.size()-1].y, 2));
    
    return distance;
}


void PolygonDraw::drawVertex(){
    CCDrawNode* line = CCDrawNode::create();
    
    //draw a regular line if you didnt complete yet
    if(!shapeIsCompleted)
        line->drawSegment(points[points.size() - 1], points[points.size()-2], 15, ccc4f(1, 0, 0, 1));
    
    else{
        //dont need 2 members of same value (same value as start)
        points.erase(points.begin() + points.size() - 1);
        line->drawSegment(points[points.size() - 1], points[0 + sumOfPoints], 15, ccc4f(0, 1, 0, 1));
        setAllToGreen();
        dotsInShape.push_back(0);
        sumOfPoints = points.size();
    }
    this->addChild(line, 1, points.size());
}

void PolygonDraw::setAllToGreen(){
    for(int i = sumOfPoints; i < points.size() - 1; i++){
        //remove the red lines
        this->removeChild(this->getChildByTag(i));
        //draw green lines
        redrawLines(i);
    }
}

void PolygonDraw::redrawLines(int index){
    CCDrawNode* line = CCDrawNode::create();
    line->setZOrder(1);
    line->drawSegment(points[index], points[index + 1], 15, ccc4f(0, 1, 0, 1));
    this->addChild(line, 1, index);
}

void PolygonDraw::saveButtonFunc(){
    dotsInShape.push_back(0);
    int helpSum = 0;
    //num of shapes
    CCUserDefault::sharedUserDefault()->setIntegerForKey("numOfShapes", (dotsInShape.back() == 0) ? dotsInShape.size() - 1 : dotsInShape.size());
    for(int i = 0; i < dotsInShape.size(); i++){
        //num of dots in shape
        std::string shape = "numOfDots";
        shape.append(std::to_string(i));
        CCUserDefault::sharedUserDefault()->setIntegerForKey(shape.c_str(), dotsInShape[i]);

        //is the last shape finished
        if(i == dotsInShape.size() - 1)
            CCUserDefault::sharedUserDefault()->setBoolForKey("isShapeFinished", isLatestShapeFinished);
        for(int j = 0; j < dotsInShape[i]; j++){
            float xCoordinate = points[j + helpSum].x;
            float yCoordinate = points[j + helpSum].y;
            std::string keyForCoordinate = "x";
            keyForCoordinate.append(std::to_string(j + helpSum));
            CCUserDefault::sharedUserDefault()->setIntegerForKey(keyForCoordinate.c_str(), xCoordinate);
            
            keyForCoordinate = "y";
            keyForCoordinate.append(std::to_string(j + helpSum));
            CCUserDefault::sharedUserDefault()->setIntegerForKey(keyForCoordinate.c_str(), yCoordinate);
        }
        helpSum += dotsInShape[i];
    }
    
    CCUserDefault::sharedUserDefault()->flush();
}

void PolygonDraw::loadButtonFunc(){
    delButtonFunc();
    dotsInShape.clear();
    int helpSum = 0;
    int numOfshapes = CCUserDefault::sharedUserDefault()->getIntegerForKey("numOfShapes");
    for(int i = 0; i < numOfshapes; i++){
        std::string shape = "numOfDots";
        shape.append(std::to_string(i));
        dotsInShape.push_back(CCUserDefault::sharedUserDefault()->getIntegerForKey(shape.c_str()));

        
        if(i == numOfshapes - 1)
            isLatestShapeFinished = CCUserDefault::sharedUserDefault()->getBoolForKey("isShapeFinished");
        
        for(int j = 0; j < dotsInShape[i]; j++){
            std::string search = "x";
            search.append(std::to_string(j+helpSum));
            float x = CCUserDefault::sharedUserDefault()->getFloatForKey(search.c_str());
            
            search = "y";
            search.append(std::to_string(j+helpSum));
            float y = CCUserDefault::sharedUserDefault()->getFloatForKey(search.c_str());

            CCPoint point = CCPoint(x, y);
            points.push_back(point);
        }
        if(i != numOfshapes - 1 && !isLatestShapeFinished)
            sumOfPoints += dotsInShape[i];
        if(isLatestShapeFinished)
            sumOfPoints += dotsInShape[i];
        helpSum += dotsInShape[i];
    }
    drawFromData();
}


void PolygonDraw::delButtonFunc(){
    for(int i = 0; i < points.size() * 2; i++){
        while(this->getChildByTag(i) != NULL)
            this->removeChildByTag(i);
        this->removeChildByTag(i + 1000);
    }
    
    points.clear();
    dotsInShape.clear();
    
    isLatestShapeFinished = false;
    dotsInShape.push_back(0);
    sumOfPoints = 0;
        
    //CCUserDefault::sharedUserDefault()->purgeSharedUserDefault();
}

void PolygonDraw::drawFromData(){
    int helpSum = 0;
    for(int i = 0; i < dotsInShape.size(); i++){
        for(int j = 0; j < dotsInShape[i]; j++){
            CCDrawNode* dot = CCDrawNode::create();
            CCDrawNode* line = CCDrawNode::create();
            CCPoint point = CCPoint(points[j + helpSum].x, points[j + helpSum].y);
            dot->drawDot(point, 15, ccc4f(0, 0, 1, 1));
            this->addChild(dot, 2, j + helpSum + 1000);
            if(j != dotsInShape[i] - 1 && i != dotsInShape.size() - 1){
                line->drawSegment(points[j + helpSum], points[j + helpSum + 1], 15, ccc4f(0, 1, 0, 1));
                this->addChild(line, 1, j + helpSum);

            }
            else if(j == dotsInShape[i] - 1 && i != dotsInShape.size() - 1){
                line->drawSegment(points[j + helpSum], points[helpSum], 15, ccc4f(0, 1, 0, 1));
                this->addChild(line, 1, j + helpSum);

            }
            else if(dotsInShape.size() - 1 == i && !isLatestShapeFinished){
                if(j != dotsInShape[i] - 1){
                    line->drawSegment(points[j + helpSum], points[j + helpSum + 1], 15, ccc4f(1, 0, 0, 1));
                    this->addChild(line, 1, j + helpSum);

                }
            }
            else if(isLatestShapeFinished && j == dotsInShape[i] - 1){
                line->drawSegment(points[j + helpSum], points[helpSum], 15, ccc4f(0, 1, 0, 1));
                this->addChild(line, 1, j + helpSum);

            }
            else{
                line->drawSegment(points[j + helpSum], points[j + helpSum + 1], 15, ccc4f(0, 1, 0, 1));
                this->addChild(line, 1, j + helpSum);
            }
        }
        helpSum += dotsInShape[i];
    }
}
