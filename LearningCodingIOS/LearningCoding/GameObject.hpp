//
//  GameObject.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 24/10/2020.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <stdio.h>
#include <string>
#include "CollisionDelegate.hpp"
#include "MovingDelegate.hpp"

class GameObject{
protected:
    float positionX;
    float positionY;
    
    float velocityY;
    float velocityX;
    
    float sizeOfObject;
    
    CCPoint point1;
    CCPoint point2;
    
    bool scored;
    
public:
    CollisionDelegate* collideable;
    MovingDelegate* moveable;
    
    std::string type;
    
    //ball & hole constructor
    GameObject(CollisionDelegate* c, MovingDelegate* m, float x, float y, float size, std::string t) : collideable(c), moveable(m), positionX(x), positionY(y), sizeOfObject(size), scored(false), type(t){}
    
    //wall constructor
    GameObject(CollisionDelegate* c, MovingDelegate* m, CCPoint one, CCPoint two) : collideable(c), moveable(m), point1(one), point2(two), type("wall"){}
    
    virtual ~GameObject() {}
    
    bool isInCollision(GameObject* obj1, GameObject* obj2, float scale, CCSize size);
    void calculateVelocities(GameObject* obj1, GameObject* obj2);
    void bounce(GameObject* wall, GameObject* ball);
    
    float getPositionX() const;
    void setPositionX(float pos);
    
    float getPositionY() const;
    void setPositionY(float pos);
    
    float getVelocityX() const;
    void setVelocityX(float _velocityX);

    float getVelocityY() const;
    void setVelocityY(float _velocityY);
    
    CCPoint getPos();
    
    float getSize() const;
    
    bool isScored() const;
    
    std::string getType() const;
    
    CCPoint getPoint1() const;
    
    CCPoint getPoint2() const;

};

#endif /* GameObject_hpp */
