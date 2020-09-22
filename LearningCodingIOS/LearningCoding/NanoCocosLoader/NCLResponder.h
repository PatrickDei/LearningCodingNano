//
//  NCLResponder.h
//  NCLCpp
//
//  Created by Bruno on 6/28/13.
//
//

#ifndef __NCLCpp__NCLResponder__
#define __NCLCpp__NCLResponder__

#include "cocos2d.h"

class NCLResponder {
	
public:
	virtual cocos2d::SEL_MenuHandler getHandleForSelector( std::string handleName ) = 0;
	virtual cocos2d::CCObject* getTarget( ) = 0;
	virtual ~NCLResponder( ){};
};

#endif /* defined(__NCLCpp__NCLResponder__) */
