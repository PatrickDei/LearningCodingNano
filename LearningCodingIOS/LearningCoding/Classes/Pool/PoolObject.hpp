//
//  PoolObject.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 21/05/2021.
//

#ifndef PoolObject_hpp
#define PoolObject_hpp

#include "UpdateDelegate.hpp"
#include "ColliderDelegate.hpp"

/**
 Structure for position of object (used instead of std::pair)
 Overloaded == operator
 */
struct Position{
public:
    float x;
    float y;
    
    Position(float _x, float _y) : x(_x), y(_y) {}
    
    bool operator== (const Position& point) const{
        return (x == point.x && y == point.y);
    }
};


/**
 Structure for size of object (used instead of std::pair)
 Overloaded == operator
 */
struct Size{
public:
    float width;
    float height;
    
    Size(float _width, float _height) : width(_width), height(_height) {}
};


/**
 Basic object present inside the pool game
 Determines position and size of object
 */
class PoolObject{
private:
    Position _position;
    Size _size;
    UpdateDelegate* _u;
    ColliderDelegate* _c;
    
protected:
    
    PoolObject(float x, float y, float width, float height, UpdateDelegate* u, ColliderDelegate* c)
        : _position(x, y)
        , _size(width, height)
        , _u(u)
        , _c(c){}
    
    void performUpdate(Position newPosition){
        _u->updatePosition(newPosition.x, newPosition.y, *this);
    }
    
public:
    Position getPostition();
    
    void setPosition(float x, float y);
    
    Size getSize();
    
    void setSize(float width, float height);
    
    virtual void updatePosition(float dt){
        performUpdate(_position);
    }
    
    bool isInCollision(PoolObject object);
};


#endif /* PoolObject_hpp */
