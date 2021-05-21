//
//  UpdateDelegate.hpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 21/05/2021.
//

#ifndef UpdateDelegate_hpp
#define UpdateDelegate_hpp

class PoolObject;

class UpdateDelegate{
public:
    virtual void updatePosition(float x, float y, PoolObject object) = 0;
};

#endif /* UpdateDelegate_hpp */
