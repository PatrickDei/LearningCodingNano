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
public:
    virtual bool isInCollision(MyObject* obj1, MyObject* obj2) override;
    
    virtual bool wallCollision(MyObject* wall, MyObject* ball) override;
};

#endif /* Collideable_hpp */
