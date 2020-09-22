//
//  CCCallLambda.h
//  Hollywood
//
//  Created by Luka Piljek on 5/7/14.
//
//

#ifndef __Hollywood__CCCallLambda__
#define __Hollywood__CCCallLambda__

#include "cocos2d.h"

NS_CC_BEGIN

class CC_DLL CCCallLambda : public CCActionInstant //<NSCopying>
{
  public:
    static CCCallLambda *create(std::function<void()> func);
    virtual bool initWithFunc(std::function<void()> func);

    virtual void execute();
    virtual void update(float time);
    CCObject *copyWithZone(CCZone *pZone);

  protected:
    /** Lambda that will be called */
    std::function<void()> m_func;
};

NS_CC_END

#endif /* defined(__Hollywood__CCCallLambda__) */
