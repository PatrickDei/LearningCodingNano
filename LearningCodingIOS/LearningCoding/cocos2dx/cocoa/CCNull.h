//
//  CCNull.h
//  Hollywood
//
//  Created by Matko Juribasic on 12/11/14.
//
//

#ifndef Hollywood_CCNull_h
#define Hollywood_CCNull_h

#include "CCObject.h"

namespace cocos2d
{
    class CCNull : public CCObject {
    public:
        static CCNull * create()
        {
            CCNull * pRet = new CCNull();
            pRet->autorelease();
            return pRet;
        }
    };
}

#endif
