//
//  Moveable.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 02/10/2020.
//

#ifndef Moveable_hpp
#define Moveable_hpp

#include "MovingDelegate.hpp"
#include "GameObject.hpp"

class Moveable : public MovingDelegate{
protected:
    virtual void calculateVelocities(GameObject* obj1, GameObject* obj2) override;
};

#endif /* Moveable_hpp */
