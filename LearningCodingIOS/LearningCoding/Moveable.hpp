//
//  Moveable.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 02/10/2020.
//

#ifndef Moveable_hpp
#define Moveable_hpp

#include "MovingDelegate.hpp"

class MovingDelegate;

class Moveable : public MovingDelegate{
protected:
    
    virtual void calcPosition(){}
};

#endif /* Moveable_hpp */
