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
    touchLocation = pTouch->getLocation();
    
    vertexIndex = getVertexIndex();
    
    getShapeVertecies(vertexIndex);
    
    moveTheVertex = (vertexIndex != -1) ? true : false;

    return true;
}


void PolygonManipulation::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint position = pTouch->getLocation();
    
    CCDrawNode* dot = CCDrawNode::create();
    CCDrawNode* line = CCDrawNode::create();
    CCDrawNode* line2 = CCDrawNode::create();

    //if its in the middle of the shape
    if(moveTheVertex){
        ccColor4F colorForLine = (!isLatestShapeFinished && shapeIndex == dotsInShape.size() - 1) ? ccc4f(1, 0, 0, 1) : ccc4f(0, 1, 0, 1);
        
        if(vertexIndex - numOfDotsBeforeShape != 0 && vertexIndex - numOfDotsBeforeShape != dotsInShape[shapeIndex] - 1){
            this->removeChild(getChildByTag(1000 + vertexIndex));
            dot->drawDot(position, 15, ccc4f(0, 0, 1, 1));
            this->addChild(dot, 2, 1000+vertexIndex);
            points[vertexIndex] = position;
            
            //now for the lines
            this->removeChild(getChildByTag(vertexIndex));
            this->removeChild(getChildByTag(vertexIndex - 1));
            line->drawSegment(points[vertexIndex], points[vertexIndex - 1], 15, colorForLine);
            this->addChild(line, 1, vertexIndex - 1);
            line2->drawSegment(points[vertexIndex], points[vertexIndex + 1], 15, colorForLine);
            this->addChild(line2, 1, vertexIndex);
        }
        if(vertexIndex - numOfDotsBeforeShape == dotsInShape[shapeIndex] - 1 && (shapeIndex < dotsInShape.size() - 1 || isLatestShapeFinished)){
            this->removeChild(getChildByTag(1000 + vertexIndex));
            dot->drawDot(position, 15, ccc4f(0, 0, 1, 1));
            this->addChild(dot, 2, 1000+vertexIndex);
            points[vertexIndex] = position;
            
            //now for the lines
            this->removeChild(getChildByTag(vertexIndex));
            this->removeChild(getChildByTag(vertexIndex - 1));
            line->drawSegment(points[vertexIndex], points[vertexIndex - 1], 15, colorForLine);
            this->addChild(line, 1, vertexIndex - 1);
            line2->drawSegment(points[vertexIndex], points[numOfDotsBeforeShape], 15, colorForLine);
            this->addChild(line2, 1, vertexIndex);
        }
        if(vertexIndex - numOfDotsBeforeShape == 0 && (shapeIndex < dotsInShape.size() - 1 || isLatestShapeFinished)){
            this->removeChild(getChildByTag(1000 + vertexIndex));
            dot->drawDot(position, 15, ccc4f(0, 0, 1, 1));
            this->addChild(dot, 2, 1000+vertexIndex);
            points[vertexIndex] = position;
            
            //now for the lines
            this->removeChild(getChildByTag(vertexIndex));
            this->removeChild(getChildByTag(dotsInShape[shapeIndex] + numOfDotsBeforeShape - 1));
            line->drawSegment(points[vertexIndex], points[dotsInShape[shapeIndex] + numOfDotsBeforeShape - 1], 15, colorForLine);
            this->addChild(line, 1, dotsInShape[shapeIndex] + numOfDotsBeforeShape - 1);
            line2->drawSegment(points[vertexIndex], points[vertexIndex + 1], 15, colorForLine);
            this->addChild(line2, 1, vertexIndex);
        }
        if(vertexIndex - numOfDotsBeforeShape == dotsInShape[shapeIndex] - 1 && shapeIndex == dotsInShape.size() - 1 && !isLatestShapeFinished){
            this->removeChild(getChildByTag(1000 + vertexIndex));
            dot->drawDot(position, 15, ccc4f(0, 0, 1, 1));
            this->addChild(dot, 2, 1000+vertexIndex);
            points[vertexIndex] = position;
            
            this->removeChild(getChildByTag(vertexIndex - 1));
            line->drawSegment(points[vertexIndex], points[vertexIndex - 1], 15, colorForLine);
            this->addChild(line, 1, vertexIndex - 1);
        }
        if(vertexIndex - numOfDotsBeforeShape == 0 && shapeIndex == dotsInShape.size() - 1 && !isLatestShapeFinished){
            this->removeChild(getChildByTag(1000 + vertexIndex));
            dot->drawDot(position, 15, ccc4f(0, 0, 1, 1));
            this->addChild(dot, 2, 1000+vertexIndex);
            points[vertexIndex] = position;
            
            this->removeChild(getChildByTag(vertexIndex));
            line->drawSegment(points[vertexIndex], points[vertexIndex + 1], 15, colorForLine);  
            this->addChild(line, 1, vertexIndex);
        }
    }
}


void PolygonManipulation::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    moveTheVertex = false;
    numOfDotsBeforeShape = 0;
    shapeIndex = 0;
}




int PolygonManipulation::getVertexIndex(){
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
