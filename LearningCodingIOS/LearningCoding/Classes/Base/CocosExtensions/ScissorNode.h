//
//  ScissorNode.h
//  NanoGirl
//
//  Created by Luka Piljek on 2/11/14.
//
//

#ifndef __NanoGirl__ScissorNode__
#define __NanoGirl__ScissorNode__

#include "cocos2d.h"

class ScissorNode : public cocos2d::CCNodeRGBA
{
  public:
    CREATE_FUNC(ScissorNode);

    bool isClipping() { return m_bClippingToBounds; }
    void setClipping(bool clip) { m_bClippingToBounds = clip; }

  private:
    cocos2d::CCRect getViewRect();
    virtual void afterDraw();
    virtual void beforeDraw();

    bool m_bClippingToBounds = true;
    bool m_bScissorRestored;
    cocos2d::CCRect m_tParentScissorRect;
};

#endif /* defined(__NanoGirl__ScissorNode__) */
