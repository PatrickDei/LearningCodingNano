//
//  CCDelayedRepeatForever.cpp
//  Hollywood
//
//  Created by Saša Stanko on 27/11/2014.
//
//

#include "CCDelayedRepeatForever.h"
#include "cocos2d.h"

NS_CC_BEGIN

CCDelayedRepeatForever *CCDelayedRepeatForever::create(float delay, CCRepeatForever *action)
{
    return create(CCDelayTime::create(delay), action);
}

CCDelayedRepeatForever *CCDelayedRepeatForever::create(CCActionInterval *delayAction, CCRepeatForever *action)
{
    CCDelayedRepeatForever *pRet = new CCDelayedRepeatForever();
    if(pRet && pRet->init(delayAction, action))
        pRet->autorelease();
    else
        CC_SAFE_RELEASE_NULL(pRet);
    return pRet;
}

bool CCDelayedRepeatForever::init(float delay, CCRepeatForever *action) { return init(delayAction, action); }

bool CCDelayedRepeatForever::init(CCActionInterval *delayAction, CCRepeatForever *action)
{
    if(!delayAction || !action)
        return false;

    this->delayAction = delayAction;
    this->action = action;
    delayAction->retain();
    action->retain();
    return true;
}

CCDelayedRepeatForever::~CCDelayedRepeatForever()
{
    delayAction->release();
    action->release();
}

CCObject *CCDelayedRepeatForever::copyWithZone(CCZone *pZone)
{
    CCZone *pNewZone = NULL;
    CCDelayedRepeatForever *pRet = NULL;
    if(pZone && pZone->m_pCopyObject) // in case of being called at sub class
    {
        pRet = (CCDelayedRepeatForever *)(pZone->m_pCopyObject);
    } else {
        pRet = new CCDelayedRepeatForever();
        pZone = pNewZone = new CCZone(pRet);
    }
    CCActionInterval::copyWithZone(pZone);
    pRet->init(static_cast<CCActionInterval *>(delayAction->copy()->autorelease()),
               static_cast<CCRepeatForever *>(action->copy()->autorelease()));
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

void CCDelayedRepeatForever::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    delayAction->startWithTarget(pTarget);
    action->startWithTarget(pTarget);
}

void CCDelayedRepeatForever::step(float dt)
{
    CCActionInterval *currentAction = (delayAction->isDone() ? action : delayAction);

    currentAction->step(dt);
    if(currentAction->isDone()) {
        float diff = action->getElapsed() - action->getDuration();
        action->startWithTarget(m_pTarget);
        action->step(0.0f);
        action->step(diff);
    }
}

bool CCDelayedRepeatForever::isDone() { return false; }

CCDelayedRepeatForever *CCDelayedRepeatForever::reverse()
{
    CCAssert(false, "CCDelayedRepeatForever: reverse makes no sense.");
    return NULL;
}

NS_CC_END
