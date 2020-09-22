//
//  CCUpdateWithLambda.cpp
//  Hollywood
//
//  Created by Saša Stanko on 08/04/2015.
//
//

#include "CCUpdateWithLambda.h"
using namespace cocos2d;
using namespace std;

CCUpdateWithLambda *CCUpdateWithLambda::create(std::function<void()> func)
{
    CCUpdateWithLambda *pRet = new CCUpdateWithLambda();
    if(pRet->initWithFunc(func))
        pRet->autorelease();
    else
        CC_SAFE_DELETE(pRet);
    return pRet;
}

bool CCUpdateWithLambda::initWithFunc(std::function<void()> func)
{
    if(func == nullptr)
        return false;
    m_func = func;
    return true;
}

CCObject *CCUpdateWithLambda::copyWithZone(CCZone *pZone)
{
    CCZone *pNewZone = NULL;
    CCUpdateWithLambda *pRet = NULL;

    if(pZone && pZone->m_pCopyObject) {
        // in case of being called at sub class
        pRet = (CCUpdateWithLambda *)(pZone->m_pCopyObject);
    } else {
        pRet = new CCUpdateWithLambda();
        pZone = pNewZone = new CCZone(pRet);
    }

    CCActionInterval::copyWithZone(pZone);
    pRet->initWithFunc(m_func);

    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

void CCUpdateWithLambda::update(float time)
{
    CC_UNUSED_PARAM(time);
    m_func();
}

bool CCUpdateWithLambda::isDone() { return false; }
