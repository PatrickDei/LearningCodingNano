//
//  CCDelayedRepeatForever.h
//  Hollywood
//
//  Created by Saša Stanko on 27/11/2014.
//
//

#ifndef __Hollywood__CCDelayedRepeatForever__
#define __Hollywood__CCDelayedRepeatForever__

#include "CCActionInterval.h"

NS_CC_BEGIN

// Let's say you wish to run several actions after which
// CCRepeatForever action should be run. Since CCSequence
// doesn't support such a behvaiour, here is our soultion.

class CCDelayedRepeatForever : public CCActionInterval
{
  public:
    static CCDelayedRepeatForever *create(float delay, CCRepeatForever *action);
    static CCDelayedRepeatForever *create(CCActionInterval *delayAction, CCRepeatForever *action);
    bool init(float delay, CCRepeatForever *action);
    bool init(CCActionInterval *delayAction, CCRepeatForever *action);

    CCDelayedRepeatForever() : delayAction(nullptr), action(nullptr) {}
    virtual ~CCDelayedRepeatForever();

    virtual bool isDone(void);
    virtual CCObject *copyWithZone(CCZone *pZone);
    virtual void step(float dt);
    virtual void startWithTarget(CCNode *pTarget);
    virtual CCDelayedRepeatForever *reverse(void);

  protected:
    CCActionInterval *delayAction;
    CCRepeatForever *action;
};

NS_CC_END

#endif /* defined(__Hollywood__CCDelayedRepeatForever__) */
