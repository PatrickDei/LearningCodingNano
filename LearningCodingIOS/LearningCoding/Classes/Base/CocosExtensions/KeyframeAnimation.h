//
//  KeyframeAnimation.h
//  Hollywood
//
//  Created by Luka Piljek on 5/15/14.
//
//

#ifndef __Hollywood__CCExtendContent__
#define __Hollywood__CCExtendContent__

#include "cocos2d.h"

NS_CC_BEGIN

//		------ WORK IN PROGRESS, DO NOT USE ------

class KeyframeAnimation : public CCActionInterval
{
  public:
    static KeyframeAnimation *create(float duration);
    bool initWithDuration(float duration);

    virtual CCObject *copyWithZone(CCZone *pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);

  protected:
};

NS_CC_END

#endif /* defined(__Hollywood__CCExtendContent__) */
