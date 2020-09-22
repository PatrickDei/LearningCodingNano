//
//  NCLLoadingSettings.cpp
//  KnightKingdom
//
//  Created by Bruno Mikus on 26/09/14.
//
//

#include "NCLLoadingSettings.h"
#include "NCLResponder.h"

#pragma mark - NCLManagerSettings

void NCLLoadingSettings::addSelector( std::string handleName, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler handle )
{
	if( handleName.length() == 0 || target == nullptr || handle == nullptr )
		return;
	selectorMap[handleName] = NCLSelector( target, handle );
}

void NCLLoadingSettings::getSelector( std::string handleName, NCLSelector& selector ) const
{
	// Default search for responder which is deprecated but primary
	if( responder )
	{
		selector.handle = responder->getHandleForSelector(handleName);
		selector.target = responder->getTarget();
	}
	// When responder fails, look for selector in the selectorMap
	if( !selector.handle || !selector.target )
	{
		auto foundIt = selectorMap.find(handleName);
		if( foundIt != selectorMap.end() )
		{
			selector.target = foundIt->second.target;
			selector.handle = foundIt->second.handle;
		}
	}
}