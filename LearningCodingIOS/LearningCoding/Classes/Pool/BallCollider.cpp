//
//  BallCollider.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 21/05/2021.
//

#include "BallCollider.hpp"
#include "PoolObject.hpp"
#include <math.h>

std::set<Position> BallCollider::getEdges(PoolObject object){
    std::set<Position> edgePoints;
    
    for(int i = 0; i < 360; i++)
        edgePoints.insert(
            Position(object.getPostition().x + cos(i) * object.getSize().width
                     , object.getPostition().y + sin(i) * object.getSize().height)
        );
    
    return edgePoints;
}
