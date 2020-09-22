//
//  ActionLoadingExtensions.h
//  Hollywood
//
//  Created by Luka Piljek on 14/01/15.
//
//

#ifndef __Hollywood__ActionLoadingExtensions__
#define __Hollywood__ActionLoadingExtensions__

#include "cocos2d.h"
#include <stdio.h>

NS_CC_BEGIN

class CC_DLL CCAnimateNormalImage : public CCTargetedAction // create with CCMenuItemImage to animate normal
                                                            // image sprite
{
  public:
    virtual void startWithTarget(CCNode *pTarget);
    CCObject *copyWithZone(CCZone *pZone);

    REGISTER_DEC_TYPE(CCAnimateNormalImage);
    virtual bool initWithDict(CCDictionary *dict);
};

NS_CC_END

#endif /* defined(__Hollywood__ActionLoadingExtensions__) */
