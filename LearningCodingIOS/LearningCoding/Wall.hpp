//
//  Wall.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 04/10/2020.
//

#ifndef Wall_hpp
#define Wall_hpp

#include <stdio.h>
#include "PoolObject.hpp"
#include "NonMoveableObject.hpp"
#include "WallCollider.hpp"

class Wall : public PoolObject{
public:
    Wall(float x, float y, float width, float height) : PoolObject(x, y, width, height, new NonMoveableObject(), new WallCollider()){}
};

#endif /* Wall_hpp */
