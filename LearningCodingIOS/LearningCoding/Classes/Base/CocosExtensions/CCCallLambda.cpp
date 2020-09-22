//
//  CCCallLambda.cpp
//  Hollywood
//
//  Created by Luka Piljek on 5/7/14.
//
//

#include "CCCallLambda.h"

using namespace cocos2d;

CCCallLambda *CCCallLambda::create(std::function<void()> func)
{
    CCCallLambda *pRet = new CCCallLambda();
    if(pRet->initWithFunc(func)) {
        pRet->autorelease();
    } else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool CCCallLambda::initWithFunc(std::function<void()> func)
{
    m_func = func;
    return true;
}

CCObject *CCCallLambda::copyWithZone(CCZone *pZone)
{
    CCZone *pNewZone = NULL;
    CCCallLambda *pRet = NULL;

    if(pZone && pZone->m_pCopyObject) {
        // in case of being called at sub class
        pRet = (CCCallLambda *)(pZone->m_pCopyObject);
    } else {
        pRet = new CCCallLambda();
        pZone = pNewZone = new CCZone(pRet);
    }

    CCActionInstant::copyWithZone(pZone);
    pRet->initWithFunc(m_func);

    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

void CCCallLambda::update(float time)
{
    CC_UNUSED_PARAM(time);
    this->execute();
}

void CCCallLambda::execute() { m_func(); }
