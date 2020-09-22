//
//  NCLWidgetCache.cpp
//  KnightKingdom
//
//  Created by Bruno Mikus on 29/09/14.
//
//

#include "NCLWidgetCache.h"
#include "NCLWidget.h"
#include "NCLWidgetNode.h"
#include "NCLUtils.h"
#include "CCPlatformMacros.h"
#include "CCDictionary.h"
#include "CCArray.h"
#include "CCString.h"

#define MAX_NAME_LEN 128

using namespace NCLUtils;

NCLWidgetCache* NCLWidgetCache::singleton = nullptr;

NCLWidgetCache::NCLWidgetCache()
{

}

NCLWidgetCache::~NCLWidgetCache()
{
	clearWidgetCache();
	singleton = nullptr;
}

NCLWidgetCache* NCLWidgetCache::get()
{
	if( !singleton )
	{
		singleton = new NCLWidgetCache();
	}
	return singleton;
}

void NCLWidgetCache::loadWidgetCache(const NCLLoadingSettings& _settings, const char* nextPlist, ...)
{
	NCLWidgetCache* cache = get();
	cache->setDefaultLoadingSettins(_settings);
	
	va_list ap;
	va_start(ap, nextPlist);
	while( nextPlist )
	{
		std::string widgetPlist = std::string( nextPlist );
		std::string widgetName = NCLUtils::stripExtensionAndPath( widgetPlist );
		if( !cache->widgetExist(widgetName) )
		{
			cache->addFileAsAWidget( widgetPlist, &cache->settings, widgetName.c_str() );
		}
		nextPlist = va_arg( ap, const char* );
	}
	va_end(ap);
}

void NCLWidgetCache::clearWidgetCache()
{
	for (auto& widgetDictIt : widgetDictCache)
	{
		widgetDictIt.second->release();
	}
	widgetDictCache.clear();
		
	widgetSettingsCache.clear();
}

void NCLWidgetCache::addWidgetDictionaryAndSettings( cocos2d::CCDictionary* widgetDict, const NCLLoadingSettings& widgetSettings, const std::string& widgetName )
{
	removeWidgetDictionaryAndSettings(widgetName);
	
	widgetDictCache[widgetName] = widgetDict;
	widgetDict->retain();
	
	widgetSettingsCache[widgetName] = widgetSettings;
}

void NCLWidgetCache::removeWidgetDictionaryAndSettings(const std::string &name)
{
	auto foundIt = widgetDictCache.find(name);
	if( foundIt != widgetDictCache.end() )
	{
		foundIt->second->release();
		widgetDictCache.erase(foundIt);
	}
	
	auto foundSettingsIt = widgetSettingsCache.find(name);
	if( foundSettingsIt != widgetSettingsCache.end() )
	{
		widgetSettingsCache.erase(foundSettingsIt);
	}
}

bool NCLWidgetCache::widgetExist( const std::string& name )
{
	auto foundIt = widgetDictCache.find(name);
	return foundIt != widgetDictCache.end();
}

bool NCLWidgetCache::addWidget( cocos2d::CCDictionary* widgetDict, NCLLoadingSettings* _settings, const char* widgetName )
{
	std::string name;
	if( widgetName )
	{
		name = std::string(widgetName);
	}
	else
	{
		cocos2d::CCString* dictName = static_cast<cocos2d::CCString*>(widgetDict->objectForKey("name"));
		if( dictName )
		{
			name = std::string(dictName->getCString());
		}
	}
	if( name.length() == 0 )
	{
		cocos2d::CCLog("Missing widget name, not provided as argument and not found in the widget dictionary.");
		return false;
	}
	
	// If settings were provides use them, otherwise use the default settings.
	// Reference to avoid copies
	NCLLoadingSettings& loadingSettings = _settings != nullptr ? *_settings : settings;
	addWidgetDictionaryAndSettings(widgetDict, loadingSettings, name);
	
	return true;
}

bool NCLWidgetCache::addFileAsAWidget( const std::string& fileName, NCLLoadingSettings* _settings, const char* widgetName )
{
	std::string name = widgetName != nullptr ? std::string(widgetName) : stripExtensionAndPath(fileName);
	if( name.length() == 0 )
	{
		return false;
	}
	
	// If settings were provides use them, otherwise use the default settings.
	// Reference to avoid copies
	NCLLoadingSettings& loadingSettings = _settings != nullptr ? *_settings : settings;
	
	// No need to load this menu, we just need the dictionary to load
	// Remember doLoad because it might stem from the default settings or the given settings
	bool doLoad = loadingSettings.doLoad;
	loadingSettings.doLoad = false;
	NCLWidget* widget = new NCLWidget(fileName.c_str(), loadingSettings);
	loadingSettings.doLoad = doLoad;
	
	bool didAdd = addWidget(widget->originalPlist, &loadingSettings, widgetName);
	delete widget;
	
	return didAdd;
}

void NCLWidgetCache::addWidgetMenu( const std::string& fileName, NCLLoadingSettings* _settings )
{
	// If settings were provides use them, otherwise use the default settings.
	// Reference to avoid copies
	NCLLoadingSettings& loadingSettings = _settings != nullptr ? *_settings : settings;
	
	// No need to load this menu, we just need the dictionary to load
	// Remember doLoad because it might stem from the default settings or the given settings
	bool doLoad = loadingSettings.doLoad;
	loadingSettings.doLoad = false;
	NCLWidget* widget = new NCLWidget( fileName.c_str(), loadingSettings );
	loadingSettings.doLoad = doLoad;
	
	cocos2d::CCArray *children = static_cast<cocos2d::CCArray*>( widget->originalPlist->objectForKey("children") );
	if( NULL != children )
	{
		for( int i=0; i<children->data->num; i++ )
		{
			cocos2d::CCDictionary *childDictionary = static_cast<cocos2d::CCDictionary*>(children->objectAtIndex(i));
			if( !addWidget( childDictionary, _settings ) )
			{
				CCLOG("Failed to add a widget child (%s).", fileName.c_str());
			}
		}
	}
	delete widget;
}

void NCLWidgetCache::setDefaultLoadingSettins( const NCLLoadingSettings& _settings )
{
	settings = _settings;
}

void NCLWidgetCache::removeWidget( const std::string &name )
{
	removeWidgetDictionaryAndSettings(name);
}

NCLWidget* NCLWidgetCache::constructAWidget( const std::string& name, NCLLoadingSettings* overrideSettings )
{
	auto foundDictIt = widgetDictCache.find(name);
	if( foundDictIt != widgetDictCache.end() )
	{
		auto foundSettingsIt = widgetSettingsCache.find(name);
		NCLLoadingSettings& widgetSettings = overrideSettings != nullptr ? *overrideSettings :
													(foundSettingsIt != widgetSettingsCache.end() ? foundSettingsIt->second : settings);
		
		NCLWidget* widget = new NCLWidget( foundDictIt->second, widgetSettings );
		return widget;
	}
	
	cocos2d::CCLog("No widget %s in cache.", name.c_str());
	return nullptr;
}

NCLWidget* NCLWidgetCache::sConstructAWidget( const std::string& name, NCLLoadingSettings* overrideSettings )
{
	return get()->constructAWidget(name, overrideSettings);
}

NCLWidgetNode* NCLWidgetCache::createAWidgetNode( const std::string& name, const NCLLoadingSettings& wSettings )
{
	auto foundDictIt = widgetDictCache.find(name);
	if( foundDictIt != widgetDictCache.end() )
	{
		NCLWidgetNode* widget = NCLWidgetNode::create( foundDictIt->second, wSettings );
		return widget;
	}
	
	cocos2d::CCLog("No widget %s in cache.", name.c_str());
	return nullptr;
}

NCLWidgetNode* NCLWidgetCache::sCreateAWidgetNode( const std::string& name, const NCLLoadingSettings& wSettings )
{
	return get()->createAWidgetNode(name, wSettings);
}

cocos2d::CCDictionary* NCLWidgetCache::getWidgetDictionary(const std::string &wName)
{
	auto foundDictIt = widgetDictCache.find(wName);
	if( foundDictIt != widgetDictCache.end() )
	{
		return foundDictIt->second;
	}
	return nullptr;
}

