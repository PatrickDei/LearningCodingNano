//
//  Collideable.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 02/10/2020.
//

#ifndef Collideable_hpp
#define Collideable_hpp

#include "CollisionDelegate.hpp"

class MyObject;

class Collideable : public CollisionDelegate{
protected:
    virtual bool isInCollision(MyObject* obj1, MyObject* obj2, float scale, CCSize size) override;
    
    float distanceBetweenPoints(CCPoint one, CCPoint two);
};

#endif /* Collideable_hpp */
