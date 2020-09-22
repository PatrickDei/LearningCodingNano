//
//  NCLWidgetNode.h
//  KnightKingdom
//
//  Created by Bruno Mikus on 19/11/14.
//
//

#ifndef __KnightKingdom__NCLWidgetNode__
#define __KnightKingdom__NCLWidgetNode__

#include "NCLWidget.h"
#include "CCNode.h"

class NCLWidgetNode : public NCLWidget, public cocos2d::CCNodeRGBA {
	
public:
	NCLWidgetNode();
	~NCLWidgetNode();
	
	static NCLWidgetNode* create( const char* plistFile, const NCLLoadingSettings& settings = NCLLoadingSettings() );
	static NCLWidgetNode* create( cocos2d::CCDictionary* dictionary, const NCLLoadingSettings& settings = NCLLoadingSettings() );
	static NCLWidgetNode* create();
	
protected:
	virtual cocos2d::CCNode* createNode( cocos2d::CCDictionary* cDict, cocos2d::CCNode* parent, bool putIntoAddressBook );
	
};


#endif /* defined(__KnightKingdom__NCLWidgetNode__) */
