//
//  NCLLoadingSettings.h
//  KnightKingdom
//
//  Created by Bruno Mikus on 26/09/14.
//
//

#ifndef __KnightKingdom__NCLLoadingSettings__
#define __KnightKingdom__NCLLoadingSettings__

#include "cocos2d.h"

class NCLResponder;
class CEFlowLayoutDataSource;
class CEFlowLayoutDelegate;

struct NCLSelector
{
	cocos2d::CCObject* target = nullptr;
	cocos2d::SEL_MenuHandler handle = nullptr;
	
	NCLSelector( cocos2d::CCObject* t, cocos2d::SEL_MenuHandler h ): target(t), handle(h)
	{
	}
	
	NCLSelector()
	{
	}
	
	void setupMenuItem( cocos2d::CCMenuItem* item )
	{
		if( item && target && handle )
		{
			item->setTarget(target, handle);
		}
	}
};

struct NCLLoadingSettings {
	bool doLoad = true;
	
	bool addExtension = false;
	float imageScale = 1.0;
	float fontScale = 1.0;
	float deviceScale = 1.0;
    
    cocos2d::CCSize parentSize;
	
	bool suppressDeviceScale = false;
	bool lookForSpriteFrames = true;
	
	NCLResponder *responder = NULL;
	CEFlowLayoutDelegate *flowDelegate = NULL;
	CEFlowLayoutDataSource *flowDataSource = NULL;
	bool doLocalize = true;
	
	// Replaces the need for a NCLResponder
	std::map<std::string, NCLSelector> selectorMap;
	
	NCLLoadingSettings(){}
	
	NCLLoadingSettings( bool addExt, float imScale, bool suppressScale, bool findSpriteFrames, float devScl, float fontScl) :
	addExtension(addExt), imageScale(imScale),suppressDeviceScale(suppressScale),lookForSpriteFrames(findSpriteFrames), deviceScale(devScl), fontScale(fontScl), responder(NULL),
	flowDelegate(NULL), flowDataSource(NULL)
	{
	}
	
	/**
	 * @brief Add a target/handle combination for a handle name used in menu items. Replaces NCLResponder.
	 */
	void addSelector( std::string handleName, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler handle );
	/**
	 * @brief Used in NCLNodeFactory to get menu item selector information.
	 */
	void getSelector( std::string handleName, NCLSelector& selector ) const;
};

typedef NCLLoadingSettings NCLManagerSettings;

#endif /* defined(__KnightKingdom__NCLLoadingSettings__) */
