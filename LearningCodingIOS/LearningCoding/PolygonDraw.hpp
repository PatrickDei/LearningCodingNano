//
//  PolygonDraw.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 07/09/2020.
//

#ifndef PolygonDraw_hpp
#define PolygonDraw_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "HelloWorldScene.h"
#include <vector>
#include "Games.cpp"


USING_NS_CC;

class PolygonDraw : public CCLayer{
public:
    
    virtual bool init() override;

    void callScene(){
        CCDirector::sharedDirector()->replaceScene(scene());
    }
    
    static CCScene* scene();

    CREATE_FUNC(PolygonDraw);

    
private:
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override;
    

protected:
    std::vector<CCPoint> points;
    
    std::vector<int> dotsInShape;
    
    int sumOfPoints;
    
    bool isNewShape();

    bool shapeIsCompleted;
    
    bool isLatestShapeFinished;
    
    
    
    void drawDot();
    
    void drawVertex();
    
    float distanceFromStart();
    
    bool isCompleted();
    
    void setAllToGreen();

    void redrawLines(int index);
    
    void saveButtonFunc();
    
    void loadButtonFunc();

    void delButtonFunc();
    
    void drawFromData();
};

#endif /* PolygonDraw_hpp */
