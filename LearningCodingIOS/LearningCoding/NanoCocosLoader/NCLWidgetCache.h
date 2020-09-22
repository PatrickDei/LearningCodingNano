//
//  NCLWidgetCache.h
//  KnightKingdom
//
//  Created by Bruno Mikus on 29/09/14.
//
//

#ifndef __KnightKingdom__NCLWidgetCache__
#define __KnightKingdom__NCLWidgetCache__

#include <map>
#include "NCLLoadingSettings.h"

class NCLWidget;
class NCLWidgetNode;
namespace cocos2d {
	class CCDictionary;
}

class NCLWidgetCache
{
private:
	static NCLWidgetCache* singleton;
	NCLWidgetCache();
	~NCLWidgetCache();
	
	NCLLoadingSettings settings;
	std::map<std::string, cocos2d::CCDictionary*> widgetDictCache;
	std::map<std::string, NCLLoadingSettings> widgetSettingsCache;
	
	void addWidgetDictionaryAndSettings( cocos2d::CCDictionary* widgetDict, const NCLLoadingSettings& widgetSettings, const std::string& widgetName );
	void removeWidgetDictionaryAndSettings( const std::string& name );
	bool widgetExist( const std::string& name );
	
public:
	static NCLWidgetCache* get();
	static void loadWidgetCache( const NCLLoadingSettings& _settings, const char* nextPlist, ... ) CC_REQUIRES_NULL_TERMINATION;
	void clearWidgetCache();
	
	bool addWidget( cocos2d::CCDictionary* widgetDict, NCLLoadingSettings* settings = nullptr, const char* widgetName = nullptr );
	bool addFileAsAWidget( const std::string& fileName, NCLLoadingSettings* settings = nullptr, const char* widgetName = nullptr );
	void addWidgetMenu( const std::string& fileName, NCLLoadingSettings* settings = nullptr );

	void setDefaultLoadingSettins( const NCLLoadingSettings& settings );
	
	void removeWidget( const std::string& name );

	/**
	 * @brief Constructs a new wigdet. When calling this method you assume the ownership of the widget. Delete it when not needed anymore.
	 */
	NCLWidget* constructAWidget( const std::string& name, NCLLoadingSettings* overrideSettings = nullptr );
	static NCLWidget* sConstructAWidget( const std::string& name, NCLLoadingSettings* overrideSettings );
	
	NCLWidgetNode* createAWidgetNode( const std::string& name, const NCLLoadingSettings& wSettings );
	static NCLWidgetNode* sCreateAWidgetNode( const std::string& name, const NCLLoadingSettings& wSettings );
	
	cocos2d::CCDictionary* getWidgetDictionary( const std::string& wName );

};

#endif /* defined(__KnightKingdom__NCLWidgetCache__) */
