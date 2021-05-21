//
//  ColliderDelegate.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 21/05/2021.
//

#ifndef ColliderDelegate_hpp
#define ColliderDelegate_hpp

#include <set>

class PoolObject;
class Position;
/**
 Returns set of points that make up the objects edges
 */
class ColliderDelegate{
public:
    virtual std::set<Position> getEdges(PoolObject object) = 0;
};

#endif /* ColliderDelegate_hpp */
