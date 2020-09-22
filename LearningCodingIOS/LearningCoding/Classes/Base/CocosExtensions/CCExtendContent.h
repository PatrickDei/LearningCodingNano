//
//  CCExtendContent.h
//  Hollywood
//
//  Created by Luka Piljek on 5/15/14.
//
//

#ifndef __Hollywood__CCExtendContent__
#define __Hollywood__CCExtendContent__

#include "cocos2d.h"

NS_CC_BEGIN

class CCExtendContentTo : public CCActionInterval
{
  public:
    static CCExtendContentTo *create(float duration, const CCSize &toSize);
    bool initWithDuration(float duration, const CCSize &toSize);

    virtual CCObject *copyWithZone(CCZone *pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);

  protected:
    cocos2d::CCSize _startSize;
    cocos2d::CCSize _endSize;
};

NS_CC_END

#endif /* defined(__Hollywood__CCExtendContent__) */
