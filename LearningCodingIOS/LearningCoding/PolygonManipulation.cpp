//
//  PolygonManipulation.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 07/09/2020.
//

#include "PolygonManipulation.hpp"

CCScene* PolygonManipulation::scene(){
    CCScene *scene = CCScene::create();
    
    PolygonManipulation* layer = PolygonManipulation::create();
    
    scene->addChild(layer);
    
    return scene;
}


bool PolygonManipulation::init(){
    if (!CCLayer::init()){
        return false;
    }
    
    CCSize screenSize = CCDirector::sharedDirector()->getVisibleSize();
    
    this->addChild(createBackButton(this, screenSize));
    
    PolygonDraw::loadButtonFunc();
    
    numOfDotsBeforeShape = 0;
    shapeIndex = 0;
    
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);
    
    return true;
}




bool PolygonManipulation::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint touchLocation = pTouch->getLocation();
    
    vertexIndex = getVertexIndex(touchLocation);
    
    getShapeVertecies(vertexIndex);
    
    moveTheVertex = (vertexIndex != -1) ? true : false;

    return true;
}


void PolygonManipulation::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint position = pTouch->getLocation();
    
    CCDrawNode* extraLine = CCDrawNode::create();

    if(moveTheVertex){
        ccColor4F colorForLine = (!isLatestShapeFinished && shapeIndex == dotsInShape.size() - 1) ? ccc4f(1, 0, 0, 1) : ccc4f(0, 1, 0, 1);
        
        if(vertexIndex - numOfDotsBeforeShape == dotsInShape[shapeIndex] - 1){
            
            if(shapeIndex < dotsInShape.size() - 1 || isLatestShapeFinished)
                redrawElements(extraLine, position, colorForLine, vertexIndex - 1, numOfDotsBeforeShape);
            else
                redrawElements(NULL, position, colorForLine, vertexIndex - 1, vertexIndex + 1);
        }
        
        else if(vertexIndex - numOfDotsBeforeShape == 0){
            
            if(shapeIndex < dotsInShape.size() - 1 || isLatestShapeFinished)
                redrawElements(extraLine, position, colorForLine, dotsInShape[shapeIndex] + numOfDotsBeforeShape - 1, vertexIndex + 1);
            else
                redrawElements(NULL, position, colorForLine, vertexIndex, vertexIndex + 1);
        }
        
        else
            redrawElements(extraLine, position, colorForLine, vertexIndex - 1, vertexIndex + 1);
    }
}

void PolygonManipulation::redrawElements(CCDrawNode* extraLine, CCPoint position, ccColor4F colorForLine, int tagForFirstPoint, int tagForSecondPoint){
    redrawDot(position);
    
    //now for the lines
    CCDrawNode* line = CCDrawNode::create();
    //remove the lines
    this->removeChild(getChildByTag(vertexIndex));
    this->removeChild(getChildByTag(tagForFirstPoint));
    
    //special case for when the red line has 3 or more dots
    line->drawSegment(points[vertexIndex], points[(tagForFirstPoint == vertexIndex) ? tagForFirstPoint + 1 : tagForFirstPoint], 15, colorForLine);
    this->addChild(line, 1, tagForFirstPoint);
    if(extraLine != NULL){
        extraLine->drawSegment(points[vertexIndex], points[tagForSecondPoint], 15, colorForLine);
        this->addChild(extraLine, 1, vertexIndex);
    }
}

void PolygonManipulation::redrawDot(CCPoint position){
    CCDrawNode* dot = CCDrawNode::create();
    this->removeChild(getChildByTag(1000 + vertexIndex));
    dot->drawDot(position, 15, ccc4f(0, 0, 1, 1));
    this->addChild(dot, 2, 1000 + vertexIndex);
    points[vertexIndex] = position;
}

void PolygonManipulation::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    moveTheVertex = false;
    numOfDotsBeforeShape = 0;
    shapeIndex = 0;
}

int PolygonManipulation::getVertexIndex(CCPoint touchLocation){
    for(int i = 0; i < points.size(); i++)
        if(20 >= sqrt(pow(points[i].x - touchLocation.x, 2) + pow(points[i].y - touchLocation.y, 2)))
            return i;
    return -1;
}

void PolygonManipulation::getShapeVertecies(int indexOfVertex){
    while(indexOfVertex > 0){
        indexOfVertex -= dotsInShape[shapeIndex];
        if(indexOfVertex >= 0){
            numOfDotsBeforeShape += dotsInShape[shapeIndex];
            shapeIndex++;
        }
    }
}
