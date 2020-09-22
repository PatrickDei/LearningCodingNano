//
//  CCSetStringWithFunc.cpp
//  Hollywood
//
//  Created by Marko Vrljičak on 19/02/15.
//
//

#include "CCSetStringWithFunc.h"
using namespace cocos2d;

CCSetStringWithFunc *CCSetStringWithFunc::create(float duration, std::function<std::string(float t)> func)
{
    CCSetStringWithFunc *pRet = new CCSetStringWithFunc();
    pRet->initWithDuration(duration, func);
    pRet->autorelease();

    return pRet;
}

bool CCSetStringWithFunc::initWithDuration(float duration, std::function<std::string(float t)> func)
{
    if(!CCActionInterval::initWithDuration(duration))
        return false;

    m_func = func;

    return true;
}

CCObject *CCSetStringWithFunc::copyWithZone(CCZone *pZone)
{
    CCZone *pNewZone = NULL;
    CCSetStringWithFunc *pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) {
        // in case of being called at sub class
        pCopy = (CCSetStringWithFunc *)(pZone->m_pCopyObject);
    } else {
        pCopy = new CCSetStringWithFunc();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_func);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCSetStringWithFunc::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_label = dynamic_cast<CCLabelTTF *>(m_pTarget);
}

void CCSetStringWithFunc::update(float t)
{
    if(m_label)
        m_label->setString(m_func(t).c_str());
}
