//
//  Immoveable.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 04/10/2020.
//

#ifndef Immoveable_hpp
#define Immoveable_hpp

#include "MovingDelegate.hpp"
#include "MyObject.hpp"

class Immoveable : public MovingDelegate{
protected:
    
    virtual void bounce(MyObject* wall, MyObject* ball) override;
};

#endif /* Immoveable_hpp */
