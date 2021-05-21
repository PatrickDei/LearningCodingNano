//
//  BallCollider.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 21/05/2021.
//

#ifndef BallCollider_hpp
#define BallCollider_hpp

#include "ColliderDelegate.hpp"

class BallCollider : public ColliderDelegate{
    virtual std::set<Position> getEdges(PoolObject object) override;
};

#endif /* BallCollider_hpp */
