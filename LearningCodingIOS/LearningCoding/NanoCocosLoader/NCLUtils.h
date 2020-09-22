//
//  NCLUtils.h
//  KnightKingdom
//
//  Created by Bruno on 2/27/14.
//
//

#ifndef __KnightKingdom__NCLUtils__
#define __KnightKingdom__NCLUtils__

#include "cocos2d.h"

namespace NCLUtils {
	
	cocos2d::CCPoint pointFromString( cocos2d::CCString *pointString, cocos2d::CCPoint defaultPoint = cocos2d::CCPoint(0, 0) );
	cocos2d::CCSize sizeFromString( cocos2d::CCString *sizeString, cocos2d::CCSize defaultSize = cocos2d::CCSize(0, 0) );
	bool boolFromString( cocos2d::CCString *boolString );
	float floatFromString( cocos2d::CCString *floatString );
	int intFromString( cocos2d::CCString *intString );
	cocos2d::CCTextAlignment horizontalAlignFromString( cocos2d::CCString* alignString );
	cocos2d::CCVerticalTextAlignment verticalAlignFromString( cocos2d::CCString* alignString );
	cocos2d::ccColor3B color3FromString( cocos2d::CCString *colorString );

	void normalizeNewline( std::string& content );

	std::string stripExtensionAndPath( const std::string& fileName );

	void sortNodesByZ( cocos2d::CCArray* array );
	cocos2d::CCMenu* createMenuByStrippingMenuItems( cocos2d::CCNode* firstItem, ... ) CC_REQUIRES_NULL_TERMINATION;

}

#endif /* defined(__KnightKingdom__NCLUtils__) */
