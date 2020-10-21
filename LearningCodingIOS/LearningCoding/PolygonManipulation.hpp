//
//  PolygonManipulation.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 07/09/2020.
//

#ifndef PolygonManipulation_hpp
#define PolygonManipulation_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "PolygonDraw.hpp"

USING_NS_CC;

class PolygonManipulation : public PolygonDraw{
public:
    virtual bool init() override;

    void callScene(){
        CCDirector::sharedDirector()->replaceScene(scene());
    }
    
    static CCScene* scene();
    
    CREATE_FUNC(PolygonManipulation);
    

private:
    
    int vertexIndex;
    
    bool moveTheVertex;
    
    int numOfDotsBeforeShape;
    
    int shapeIndex;

    
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override;

    void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) override;
    
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) override;
    
    int getVertexIndex(CCPoint touchLocation);
    
    void getShapeVertecies(int indexOfVertex);
    
    void redrawElements(CCDrawNode* extraLine, CCPoint position, ccColor4F colorForLine, int tagForFirstPoint, int tagForSecondPoint);
    
    void redrawDot(CCPoint position);
};

#endif /* PolygonManipulation_hpp */
