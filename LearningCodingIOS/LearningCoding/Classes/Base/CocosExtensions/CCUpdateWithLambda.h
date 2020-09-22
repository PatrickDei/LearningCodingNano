//
//  CCUpdateWithLambda.h
//  Hollywood
//
//  Created by Saša Stanko on 08/04/2015.
//
//

#ifndef __Hollywood__CCUpdateWithLambda__
#define __Hollywood__CCUpdateWithLambda__

#include "cocos2d.h"

NS_CC_BEGIN

class CCUpdateWithLambda : public CCActionInterval
{
  public:
    static CCUpdateWithLambda *create(std::function<void()> func);
    virtual bool initWithFunc(std::function<void()> func);

    virtual CCObject *copyWithZone(CCZone *pZone);
    //	virtual void startWithTarget(CCNode* pTarget);
    //	virtual void step(float dt);
    virtual void update(float time);
    virtual bool isDone(void);
    //	virtual CCActionInterval* reverse(void);

  protected:
    std::function<void()> m_func;
};

NS_CC_END

#endif /* defined(__Hollywood__CCUpdateWithLambda__) */
