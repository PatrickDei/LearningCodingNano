//
//  Wall.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 04/10/2020.
//

#ifndef Wall_hpp
#define Wall_hpp

#include <stdio.h>
#include "GameObject.hpp"
#include "Collideable.hpp"
#include "Immoveable.hpp"

class Wall : public GameObject{
public:
    Wall(CCPoint _point1, CCPoint _point2) : GameObject(new Collideable(), new Immoveable(), _point1, _point2){}
};

#endif /* Wall_hpp */
