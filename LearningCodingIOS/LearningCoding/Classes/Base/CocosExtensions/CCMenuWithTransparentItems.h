//
//  CCMenuWithTransparentItems.h
//  Hollywood
//
//  Created by Saša Stanko on 13/08/2014.
//
//

#ifndef __Hollywood__CCMenuWithTransparentItems__
#define __Hollywood__CCMenuWithTransparentItems__

#include "cocos2d.h"

class CCMenuWithTransparentItems : public cocos2d::CCMenu
{
  public:
    static CCMenuWithTransparentItems *create();

    virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);

  protected:
    cocos2d::CCMenuItem *itemForTouch(cocos2d::CCTouch *touch);
};

#endif /* defined(__Hollywood__CCMenuWithTransparentItems__) */
