//
//  WallCollider.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 21/05/2021.
//

#ifndef WallCollider_hpp
#define WallCollider_hpp

#include "ColliderDelegate.hpp"

class WallCollider : public ColliderDelegate{
    virtual std::set<Position> getEdges(PoolObject object) override;
};

#endif /* WallCollider_hpp */
