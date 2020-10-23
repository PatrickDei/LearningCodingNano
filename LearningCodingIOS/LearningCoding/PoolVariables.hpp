//
//  PoolVariables.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 23/10/2020.
//

#ifndef PoolVariables_hpp
#define PoolVariables_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class PoolVariables{
public:
    static CCSize tableSize;
    static float tableScale;
        
private:
    float positionX;
    float positionY;
    
    float velocityY;
    float velocityX;
    
    float sizeOfObject;
    
    CCPoint point1;
    CCPoint point2;
    
    bool scored;
    
    std::string type;
    
public:
    
    float getPositionX() const{
        return positionX;
    }
    
    void setPositionX(float pos){
        positionX = pos;
    }
    
    float getPositionY() const{
        return positionY;
    }
    
    void setPositionY(float pos){
        positionY = pos;
    }
    
    float getVelocityX() const{
        return velocityX;
    }

    void setVelocityX(float _velocityX){
        velocityX = (_velocityX < 300) ? _velocityX : 300;
    }

    float getVelocityY() const{
        return velocityY;
    }

    void setVelocityY(float _velocityY){
        velocityY = (_velocityY < 300) ? _velocityY : 300;
    }
    
    CCPoint getPos(){
        CCPoint point = CCPoint(positionX, positionY);
        return point;
    }
    
    void setSize(float size){
        sizeOfObject = size;
    }
    
    float getSize() const{
        return sizeOfObject;
    }
    
    bool isScored() const{
        return scored;
    }
    
    void setScored(bool b){
        scored = b;
    }
    
    void setType(std::string t){
        type = t;
    }
    
    std::string getType() const{
        return type;
    }
    
    void setPoint1(CCPoint p){
        point1 = p;
    }
    
    void setPoint2(CCPoint p){
        point2 = p;
    }
    
    CCPoint getPoint1() const{
        return point1;
    }
    
    CCPoint getPoint2() const{
        return point2;
    }
};

#endif /* PoolVariables_hpp */
