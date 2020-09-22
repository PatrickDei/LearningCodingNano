//
//  CCMoveWithFunc.h
//  NanoGirl
//
//  Created by Saša Stanko on 10/03/2014.
//
//

#ifndef __NanoGirl__CCMoveWithFunc__
#define __NanoGirl__CCMoveWithFunc__

#include "cocos2d.h"

NS_CC_BEGIN

class CCMoveWithFunc : public CCActionInterval
{
  public:
    static CCMoveWithFunc *create(float duration, std::function<CCPoint(float t)> func);
    bool initWithDuration(float duration, std::function<CCPoint(float t)> func);

    virtual CCObject *copyWithZone(CCZone *pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);

  protected:
    std::function<CCPoint(float t)> m_func;
    CCPoint m_lastPoint;
};

NS_CC_END

#endif /* defined(__NanoGirl__CCMoveWithFunc__) */
