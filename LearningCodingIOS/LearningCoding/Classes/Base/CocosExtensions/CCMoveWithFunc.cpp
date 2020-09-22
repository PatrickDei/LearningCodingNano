//
//  CCMoveWithFunc.cpp
//  NanoGirl
//
//  Created by Saša Stanko on 10/03/2014.
//
//

#include "CCMoveWithFunc.h"

using namespace cocos2d;

CCMoveWithFunc *CCMoveWithFunc::create(float duration, std::function<CCPoint(float t)> func)
{
    CCMoveWithFunc *pRet = new CCMoveWithFunc();
    pRet->initWithDuration(duration, func);
    pRet->autorelease();

    return pRet;
}

bool CCMoveWithFunc::initWithDuration(float duration, std::function<CCPoint(float t)> func)
{
    if(!CCActionInterval::initWithDuration(duration))
        return false;

    m_func = func;
    return true;
}

CCObject *CCMoveWithFunc::copyWithZone(CCZone *pZone)
{
    CCZone *pNewZone = NULL;
    CCMoveWithFunc *pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) {
        // in case of being called at sub class
        pCopy = (CCMoveWithFunc *)(pZone->m_pCopyObject);
    } else {
        pCopy = new CCMoveWithFunc();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_func);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCMoveWithFunc::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_lastPoint = m_func(0);
}

void CCMoveWithFunc::update(float t)
{
    if(m_pTarget) {
#if CC_ENABLE_STACKABLE_ACTIONS
        CCPoint newPoint = m_func(t);
        CCPoint delta = ccpSub(newPoint, m_lastPoint);
        m_lastPoint = newPoint;
        m_pTarget->setPosition(ccpAdd(m_pTarget->getPosition(), delta));
#else
        m_pTarget->setPosition(m_func(t));
#endif // CC_ENABLE_STACKABLE_ACTIONS
    }
}
