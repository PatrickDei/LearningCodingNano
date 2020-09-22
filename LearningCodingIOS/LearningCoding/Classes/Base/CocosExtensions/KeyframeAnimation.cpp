//
//  KeyframeAnimation.cpp
//  Hollywood
//
//  Created by Luka Piljek on 5/15/14.
//
//

#include "KeyframeAnimation.h"

using namespace cocos2d;

KeyframeAnimation *KeyframeAnimation::create(float duration)
{
    KeyframeAnimation *pRet = new KeyframeAnimation();
    pRet->initWithDuration(duration);
    pRet->autorelease();

    return pRet;
}

bool KeyframeAnimation::initWithDuration(float duration)
{
    if(!CCActionInterval::initWithDuration(duration))
        return false;

    return true;
}

CCObject *KeyframeAnimation::copyWithZone(CCZone *pZone)
{
    CCZone *pNewZone = NULL;
    KeyframeAnimation *pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) {
        // in case of being called at sub class
        pCopy = (KeyframeAnimation *)(pZone->m_pCopyObject);
    } else {
        pCopy = new KeyframeAnimation();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void KeyframeAnimation::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    //	_startSize = pTarget->getContentSize();
}

void KeyframeAnimation::update(float time)
{
    if(m_pTarget) {
        m_pTarget->setPosition(CCPointZero);
        m_pTarget->setScale(1.0f);
        m_pTarget->setRotation(0.0f);
        m_pTarget->setVisible(true);
        // color & opacity
        // flip
        // image?
    }
}
