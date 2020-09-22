//
//  CCSetStringWithFunc.h
//  Hollywood
//
//  Created by Marko Vrljičak on 19/02/15.
//
//

#ifndef __Hollywood__CCSetStringWithFunc__
#define __Hollywood__CCSetStringWithFunc__

#include "cocos2d.h"
#include <stdio.h>

NS_CC_BEGIN

class CCSetStringWithFunc : public CCActionInterval
{
  public:
    static CCSetStringWithFunc *create(float duration, std::function<std::string(float t)> func);
    bool initWithDuration(float duration, std::function<std::string(float t)> func);

    virtual CCObject *copyWithZone(CCZone *pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);

  protected:
    std::function<std::string(float t)> m_func;
    CCLabelTTF *m_label;
};

NS_CC_END

#endif /* defined(__Hollywood__CCSetStringWithFunc__) */
