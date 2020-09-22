//
//  CCExtendContent.cpp
//  Hollywood
//
//  Created by Luka Piljek on 5/15/14.
//
//

#include "CCExtendContent.h"

using namespace cocos2d;

CCExtendContentTo *CCExtendContentTo::create(float duration, const CCSize &toSize)
{
    CCExtendContentTo *pRet = new CCExtendContentTo();
    pRet->initWithDuration(duration, toSize);
    pRet->autorelease();

    return pRet;
}

bool CCExtendContentTo::initWithDuration(float duration, const CCSize &toSize)
{
    if(!CCActionInterval::initWithDuration(duration))
        return false;

    _endSize = toSize;
    return true;
}

CCObject *CCExtendContentTo::copyWithZone(CCZone *pZone)
{
    CCZone *pNewZone = NULL;
    CCExtendContentTo *pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) {
        // in case of being called at sub class
        pCopy = (CCExtendContentTo *)(pZone->m_pCopyObject);
    } else {
        pCopy = new CCExtendContentTo();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, _endSize);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCExtendContentTo::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    _startSize = pTarget->getContentSize();
}

void CCExtendContentTo::update(float time)
{
    if(m_pTarget) {
        m_pTarget->setContentSize(_startSize * (1.0f - time) + _endSize * time);
    }
}
