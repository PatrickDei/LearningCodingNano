//
//  CCMenuWithTransparentItems.cpp
//  Hollywood
//
//  Created by Saša Stanko on 13/08/2014.
//
//

#include "CCMenuWithTransparentItems.h"
#include "NanoExtensions.h"
using namespace cocos2d;

CCMenuWithTransparentItems *CCMenuWithTransparentItems::create()
{
    return createObjectFunction<CCMenuWithTransparentItems>();
}

bool CCMenuWithTransparentItems::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    CC_UNUSED_PARAM(event);
    if(m_eState != kCCMenuStateWaiting || !m_bVisible || !isEnabled()) {
        return false;
    }

    for(CCNode *c = this->m_pParent; c != NULL; c = c->getParent()) {
        if(c->isVisible() == false) {
            return false;
        }
    }

    m_pSelectedItem = this->itemForTouch(touch);
    if(m_pSelectedItem) {
        selectedItemInitialSelected = m_pSelectedItem->isSelected();
        m_eState = kCCMenuStateTrackingTouch;
        m_pSelectedItem->selected();
        return true;
    }
    return false;
}

CCMenuItem *CCMenuWithTransparentItems::itemForTouch(CCTouch *touch)
{
    CCPoint touchLocation = touch->getLocation();

    if(m_pChildren && m_pChildren->count() > 0) {
        CCObject *pObject = NULL;
        CCARRAY_FOREACH(m_pChildren, pObject)
        {
            CCMenuItem *pChild = dynamic_cast<CCMenuItem *>(pObject);
            if(pChild && pChild->isVisible() && pChild->isEnabled()) {
                CCPoint local = pChild->convertToNodeSpace(touchLocation);
                CCRect r = pChild->rect();
                r.origin = CCPointZero;

                if(r.containsPoint(local) && pChild->isPointNonTransparent(touchLocation))
                    return pChild;
            }
        }
    }

    return NULL;
}

void CCMenuWithTransparentItems::ccTouchMoved(CCTouch *touch, CCEvent *event)
{
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchMoved] -- invalid state");
    CCMenuItem *currentItem = this->itemForTouch(touch);
    if(currentItem != m_pSelectedItem) {
        if(m_pSelectedItem) {
            if(!selectedItemInitialSelected)
                m_pSelectedItem->unselected();
        }
        m_pSelectedItem = currentItem;
        if(m_pSelectedItem) {
            selectedItemInitialSelected = m_pSelectedItem->isSelected();
            m_pSelectedItem->selected();
        }
    }
}
