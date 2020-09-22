//
//  ActionLoadingExtensions.cpp
//  Hollywood
//
//  Created by Luka Piljek on 14/01/15.
//
//

#include "ActionLoadingExtensions.h"

using namespace cocos2d;

#pragma mark - Interface

bool CCAction::initWithDict(CCDictionary *dict)
{
    CCAssert(false, "CCAction::initWithDict is not implemented for this action!");
    return false;
}

CCAction *CCAction::createWithDict(CCDictionary *dict)
{
    if(CCString *className = (CCString *)dict->objectForKey("className")) {
        if(Serializable *aObject = ObjectFactory::newInstance(className->getCString())) {
            if(CCAction *action = dynamic_cast<CCAction *>(aObject)) {
                if(action->initWithDict(dict)) {
                    action->autorelease();
                    return action;
                }
            }
            delete aObject;
            CCAssert(true, "CCAction::createWithDict - Class not an Action!");
            return nullptr;
        } else {
            CCLOG("CCAction::createWithDict - Unknown type '%s'!", className->getCString());
            CCAssert(true, "CCAction::createWithDict - Unknown type!");
        }
    }

    return nullptr;
}

#pragma mark - Ordering & Sequencing

REGISTER_DEF_TYPE(CCSequence);
bool CCSequence::initWithDict(cocos2d::CCDictionary *dict)
{
    CCArray *actions = dict->arrayForKey("actions");
    if(actions && actions->count() > 0) {
        CCFiniteTimeAction *last =
            dynamic_cast<CCFiniteTimeAction *>(CCAction::createWithDict((CCDictionary *)actions->lastObject()));
        if(last == nullptr) {
            CCAssert(true, "CCSequence::initWithDict - Object not a CCFiniteTimeAction!");
            return false;
        }

        if(actions->count() == 1) {
            return initWithTwoActions(last, CCDelayTime::create(0.0));
        }

        for(int i = actions->count() - 2; i >= 0; --i) {
            CCFiniteTimeAction *prev =
                dynamic_cast<CCFiniteTimeAction *>(CCAction::createWithDict((CCDictionary *)actions->objectAtIndex(i)));
            if(prev == nullptr) {
                CCAssert(true, "CCSequence::initWithDict - Object not a CCFiniteTimeAction!");
                return false;
            }

            if(i == 0) {
                return initWithTwoActions(prev, last);
            } else {
                last = CCSequence::createWithTwoActions(prev, last);
            }
        }
    }

    return false;
}

REGISTER_DEF_TYPE(CCSpawn);
bool CCSpawn::initWithDict(cocos2d::CCDictionary *dict)
{
    CCArray *actions = dict->arrayForKey("actions");
    if(actions && actions->count() > 0) {
        CCFiniteTimeAction *last =
            dynamic_cast<CCFiniteTimeAction *>(CCAction::createWithDict((CCDictionary *)actions->lastObject()));
        if(last == nullptr) {
            CCAssert(true, "CCSpawn::initWithDict - Object not a CCFiniteTimeAction!");
            return false;
        }

        if(actions->count() == 1) {
            return initWithTwoActions(last, CCDelayTime::create(0.0));
        }

        for(int i = actions->count() - 2; i >= 0; --i) {
            CCFiniteTimeAction *prev =
                dynamic_cast<CCFiniteTimeAction *>(CCAction::createWithDict((CCDictionary *)actions->objectAtIndex(i)));
            if(prev == nullptr) {
                CCAssert(true, "CCSpawn::initWithDict - Object not a CCFiniteTimeAction!");
                return false;
            }

            if(i == 0) {
                return initWithTwoActions(prev, last);
            } else {
                last = CCSpawn::createWithTwoActions(prev, last);
            }
        }
    }

    return false;
}

REGISTER_DEF_TYPE(CCRepeat);
bool CCRepeat::initWithDict(cocos2d::CCDictionary *dict)
{
    CCFiniteTimeAction *action =
        dynamic_cast<CCFiniteTimeAction *>(CCAction::createWithDict(dict->dictForKey("action")));
    if(action == nullptr) {
        CCAssert(true, "CCRepeat::initWithDict - Object not a CCFiniteTimeAction!");
        return false;
    }

    return initWithAction(action, dict->intForKey("times"));
}

REGISTER_DEF_TYPE(CCRepeatForever);
bool CCRepeatForever::initWithDict(cocos2d::CCDictionary *dict)
{
    CCActionInterval *action = dynamic_cast<CCActionInterval *>(CCAction::createWithDict(dict->dictForKey("action")));
    if(action == nullptr) {
        CCAssert(true, "CCRepeat::initWithDict - Object not a CCActionInterval!");
        return false;
    }

    return initWithAction(action);
}

REGISTER_DEF_TYPE(CCDelayTime);
bool CCDelayTime::initWithDict(cocos2d::CCDictionary *dict) { return initWithDuration(dict->floatForKey("duration")); }

#pragma mark - Transforming

REGISTER_DEF_TYPE(CCMoveTo);
bool CCMoveTo::initWithDict(CCDictionary *dict)
{
    CCPoint pos = CCPoint(dict->floatForKey("positionX"), dict->floatForKey("positionY"));
    return initWithDuration(dict->floatForKey("duration"), pos);
}

REGISTER_DEF_TYPE(CCRotateTo);
bool CCRotateTo::initWithDict(cocos2d::CCDictionary *dict)
{
    return initWithDuration(dict->floatForKey("duration"), dict->floatForKey("angle"));
}

REGISTER_DEF_TYPE(CCScaleTo);
bool CCScaleTo::initWithDict(cocos2d::CCDictionary *dict)
{
    return initWithDuration(dict->floatForKey("duration"), dict->floatForKey("scaleX"), dict->floatForKey("scaleY"));
}

REGISTER_DEF_TYPE(CCSkewTo);
bool CCSkewTo::initWithDict(cocos2d::CCDictionary *dict)
{
    return initWithDuration(dict->floatForKey("duration"), dict->floatForKey("skewX"), dict->floatForKey("skewY"));
}

#pragma mark - Image & Coloring

REGISTER_DEF_TYPE(CCAnimate);
bool CCAnimate::initWithDict(cocos2d::CCDictionary *dict)
{
    if(CCDictionary *animDict = dict->dictForKey("animation")) {
        CCAnimation *animation = CCAnimation::create(animDict);
        return initWithAnimation(animation);
    }
    return false;
}

REGISTER_DEF_TYPE(CCFadeTo);
bool CCFadeTo::initWithDict(cocos2d::CCDictionary *dict)
{
    return initWithDuration(dict->floatForKey("duration"), dict->intForKey("opacity"));
}

REGISTER_DEF_TYPE(CCTintTo);
bool CCTintTo::initWithDict(cocos2d::CCDictionary *dict)
{
    return initWithDuration(dict->floatForKey("duration"), dict->floatForKey("red"), dict->floatForKey("green"),
                            dict->floatForKey("blue"));
}

#pragma mark - Instant Actions

REGISTER_DEF_TYPE(CCShow);
bool CCShow::initWithDict(cocos2d::CCDictionary *dict) { return true; }

REGISTER_DEF_TYPE(CCHide);
bool CCHide::initWithDict(cocos2d::CCDictionary *dict) { return true; }

REGISTER_DEF_TYPE(CCToggleVisibility);
bool CCToggleVisibility::initWithDict(cocos2d::CCDictionary *dict) { return true; }

REGISTER_DEF_TYPE(CCRemoveSelf);
bool CCRemoveSelf::initWithDict(cocos2d::CCDictionary *dict) { return init(dict->boolForKey("cleanup")); }

REGISTER_DEF_TYPE(CCFlipX);
bool CCFlipX::initWithDict(cocos2d::CCDictionary *dict) { return initWithFlipX(dict->boolForKey("flip")); }

REGISTER_DEF_TYPE(CCFlipY);
bool CCFlipY::initWithDict(cocos2d::CCDictionary *dict) { return initWithFlipY(dict->boolForKey("flip")); }

#pragma mark - Action Ease

REGISTER_DEF_TYPE(CCEaseIn);
bool CCEaseIn::initWithDict(cocos2d::CCDictionary *dict)
{
    CCActionInterval *action = dynamic_cast<CCActionInterval *>(CCAction::createWithDict(dict->dictForKey("action")));
    if(action == nullptr) {
        CCAssert(true, "CCEaseIn::initWithDict - Object not a CCActionInterval!");
        return false;
    }
    return initWithAction(action, dict->floatForKey("rate", 2.0f));
}

REGISTER_DEF_TYPE(CCEaseOut);
bool CCEaseOut::initWithDict(cocos2d::CCDictionary *dict)
{
    CCActionInterval *action = dynamic_cast<CCActionInterval *>(CCAction::createWithDict(dict->dictForKey("action")));
    if(action == nullptr) {
        CCAssert(true, "CCEaseOut::initWithDict - Object not a CCActionInterval!");
        return false;
    }
    return initWithAction(action, dict->floatForKey("rate", 2.0f));
}

REGISTER_DEF_TYPE(CCEaseInOut);
bool CCEaseInOut::initWithDict(cocos2d::CCDictionary *dict)
{
    CCActionInterval *action = dynamic_cast<CCActionInterval *>(CCAction::createWithDict(dict->dictForKey("action")));
    if(action == nullptr) {
        CCAssert(true, "CCEaseInOut::initWithDict - Object not a CCActionInterval!");
        return false;
    }
    return initWithAction(action, dict->floatForKey("rate", 2.0f));
}

#pragma mark - New Actions

REGISTER_DEF_TYPE(CCAnimateNormalImage);
bool CCAnimateNormalImage::initWithDict(cocos2d::CCDictionary *dict)
{
    CCAnimate *animation = new CCAnimate();
    if(animation->initWithDict(dict) && CCActionInterval::initWithDuration(animation->getDuration())) {
        m_pForcedTarget = nullptr;
        m_pAction = animation;
        return true;
    }
    delete animation;
    return false;
}

void CCAnimateNormalImage::startWithTarget(CCNode *pTarget)
{
    if(CCMenuItemSprite *menuItem = dynamic_cast<CCMenuItemSprite *>(pTarget)) {
        if(CCSprite *nSprite = dynamic_cast<CCSprite *>(menuItem->getNormalImage())) {
            nSprite->retain();
            CC_SAFE_RELEASE(m_pForcedTarget);
            m_pForcedTarget = nSprite;

            CCActionInterval::startWithTarget(pTarget);
            m_pAction->startWithTarget(m_pForcedTarget);
        }
    }
}

CCObject *CCAnimateNormalImage::copyWithZone(CCZone *pZone)
{
    CCZone *pNewZone = NULL;
    CCAnimateNormalImage *pRet = NULL;
    if(pZone && pZone->m_pCopyObject) // in case of being called at sub class
    {
        pRet = (CCAnimateNormalImage *)(pZone->m_pCopyObject);
    } else {
        pRet = new CCAnimateNormalImage();
        pZone = pNewZone = new CCZone(pRet);
    }
    CCActionInterval::copyWithZone(pZone);
    // win32 : use the m_pOther's copy object.
    pRet->initWithTarget(m_pForcedTarget, (CCFiniteTimeAction *)m_pAction->copy()->autorelease());
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}
