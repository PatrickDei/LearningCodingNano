//
//  Collideable.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 02/10/2020.
//

#ifndef Collideable_hpp
#define Collideable_hpp

#include "CollisionDelegate.hpp"

class GameObject;

class Collideable : public CollisionDelegate{
protected:
    virtual bool isInCollision(GameObject* obj1, GameObject* obj2, float scale, CCSize size) override;
    
    float distanceBetweenPoints(CCPoint one, CCPoint two);
};

#endif /* Collideable_hpp */
