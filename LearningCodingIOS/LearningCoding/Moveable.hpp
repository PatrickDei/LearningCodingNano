//
//  Moveable.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 02/10/2020.
//

#ifndef Moveable_hpp
#define Moveable_hpp

#include "MovingDelegate.hpp"
#include "MyObject.hpp"

class Moveable : public MovingDelegate{
protected:
    virtual void calculateVelocities(MyObject* obj1, MyObject* obj2) override;
};

#endif /* Moveable_hpp */
