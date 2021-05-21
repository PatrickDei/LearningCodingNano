//
//  MoveableObject.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 21/05/2021.
//

#ifndef MoveableObject_hpp
#define MoveableObject_hpp

#include "UpdateDelegate.hpp"

class MoveableObject : public UpdateDelegate{
    virtual void updatePosition(float x, float y, PoolObject object) override;
};

#endif /* MoveableObject_hpp */
