//
//  Immoveable.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 04/10/2020.
//

#ifndef Immoveable_hpp
#define Immoveable_hpp

#include "MovingDelegate.hpp"
#include "GameObject.hpp"

class Immoveable : public MovingDelegate{
protected:
    virtual void bounce(GameObject* wall, GameObject* ball) override;
    
private:
    float angleBetweenLineAndDot(GameObject* wall, GameObject* ball, float fi);
};

#endif /* Immoveable_hpp */
