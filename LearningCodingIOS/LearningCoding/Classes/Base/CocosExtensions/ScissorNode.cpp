//
//  ScissorNode.cpp
//  NanoGirl
//
//  Created by Luka Piljek on 2/11/14.
//
//

#include "ScissorNode.h"

using namespace cocos2d;

CCRect ScissorNode::getViewRect()
{
    CCPoint screenPos = this->convertToWorldSpace(CCPointZero);

    float scaleX = this->getScaleX();
    float scaleY = this->getScaleY();

    for(CCNode *p = m_pParent; p != NULL; p = p->getParent()) {
        scaleX *= p->getScaleX();
        scaleY *= p->getScaleY();
    }

    // Support negative scaling. Not doing so causes intersectsRect calls
    // (eg: to check if the touch was within the bounds) to return false.
    // Note, CCNode::getScale will assert if X and Y scales are different.
    if(scaleX < 0.f) {
        screenPos.x += m_obContentSize.width * scaleX;
        scaleX = -scaleX;
    }
    if(scaleY < 0.f) {
        screenPos.y += m_obContentSize.height * scaleY;
        scaleY = -scaleY;
    }

    return CCRectMake(screenPos.x, screenPos.y, m_obContentSize.width * scaleX, m_obContentSize.height * scaleY);
}

void ScissorNode::beforeDraw()
{
    if(m_bClippingToBounds) {
        m_bScissorRestored = false;
        CCRect frame = getViewRect();
        if(CCEGLView::sharedOpenGLView()->isScissorEnabled()) {
            m_bScissorRestored = true;
            m_tParentScissorRect = CCEGLView::sharedOpenGLView()->getScissorRect();
            // set the intersection of m_tParentScissorRect and frame as the new
            // scissor rect
            if(frame.intersectsRect(m_tParentScissorRect)) {
                float x = MAX(frame.origin.x, m_tParentScissorRect.origin.x);
                float y = MAX(frame.origin.y, m_tParentScissorRect.origin.y);
                float xx = MIN(frame.origin.x + frame.size.width,
                               m_tParentScissorRect.origin.x + m_tParentScissorRect.size.width);
                float yy = MIN(frame.origin.y + frame.size.height,
                               m_tParentScissorRect.origin.y + m_tParentScissorRect.size.height);
                CCEGLView::sharedOpenGLView()->setScissorInPoints(x, y, xx - x, yy - y);
            } else {
                CCEGLView::sharedOpenGLView()->setScissorInPoints(0, 0, 0.0f, 0.0f);
            }
        } else {
            glEnable(GL_SCISSOR_TEST);
            CCEGLView::sharedOpenGLView()->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width,
                                                              frame.size.height);
        }
    }
}

/**
 * retract what's done in beforeDraw so that there's no side effect to
 * other nodes.
 */
void ScissorNode::afterDraw()
{
    if(m_bClippingToBounds) {
        if(m_bScissorRestored) { // restore the parent's scissor rect
            CCEGLView::sharedOpenGLView()->setScissorInPoints(
                m_tParentScissorRect.origin.x, m_tParentScissorRect.origin.y, m_tParentScissorRect.size.width,
                m_tParentScissorRect.size.height);
        } else {
            glDisable(GL_SCISSOR_TEST);
        }
    }
}