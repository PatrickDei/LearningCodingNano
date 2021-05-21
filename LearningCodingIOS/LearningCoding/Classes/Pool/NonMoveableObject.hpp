//
//  NonMoveableObject.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 21/05/2021.
//

#ifndef NonMoveableObject_hpp
#define NonMoveableObject_hpp

#include "UpdateDelegate.hpp"

class NonMoveableObject : public UpdateDelegate{
    virtual void updatePosition(float x, float y, PoolObject object) override;
};

#endif /* NonMoveableObject_hpp */
