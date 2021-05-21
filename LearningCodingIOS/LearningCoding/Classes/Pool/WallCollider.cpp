//
//  WallCollider.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 21/05/2021.
//

#include "WallCollider.hpp"
#include "PoolObject.hpp"

std::set<Position> WallCollider::getEdges(PoolObject object){
    std::set<Position> edgePoints;
    
    float objectsPositionX = object.getPostition().x;
    float objectsPositionY = object.getPostition().y;
    
    //horizontal edges
    for(int i = objectsPositionX; i <= objectsPositionX + object.getSize().width; i++){
        edgePoints.insert(
            Position(i, objectsPositionY)
        );
        edgePoints.insert(
            Position(i, objectsPositionY + object.getSize().height)
        );
    }
    
    // vertical edges
    for(int i = objectsPositionY; i <= objectsPositionY + object.getSize().height; i++){
        edgePoints.insert(
            Position(objectsPositionX, i)
        );
        edgePoints.insert(
            Position(objectsPositionX + object.getSize().width, i)
        );
    }
    
    return edgePoints;
}
