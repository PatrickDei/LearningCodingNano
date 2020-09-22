//
//  NCLWidget.h
//  KnightKingdom
//
//  Created by Bruno Mikus on 26/09/14.
//
//

#ifndef __KnightKingdom__NCLWidget__
#define __KnightKingdom__NCLWidget__


#include <map>
#include <vector>
#include <string>

#include "cocos2d.h"
#include "NCLLoadingSettings.h"
#include "NCLDeviceInfo.h"

class CEFlowLayout;
class NCLWidgetNode;

#pragma mark - NCLWidget
/**
 * @brief NCLWidget loads node data, replaces NCLDeviceNodeManager (in a way) and enables the widget idea.
 * Inherits from NCLDeviceInfo to maintain backwards compatibility where device info was a part of the node manager.
 */
class NCLWidget : public NCLDeviceInfo
{
public:
	/*
	 * When extensions are added, uses NCLDeviceInfo to add an extension (depending on the device):
	 *  - iPhone + iPhoneHD have the same extension
	 *  - iPhone5
	 *  - iPad + iPadHD have the same extension
	 * For HD devices (except iPhone5, since it is always HD) positions and sizes will be doubled.
	 * Even though the same menu is used for basic and retina device, retina device will try to get an
	 * image with the -hd suffix to the filename (as expected).
	 */
	NCLWidget( const char* plistFile, const NCLLoadingSettings& settings = NCLLoadingSettings() );
	NCLWidget( const char* plistFile, NCLResponder* responder, CEFlowLayoutDataSource* tableDataSource, CEFlowLayoutDelegate* tableDelegate );
	NCLWidget( cocos2d::CCDictionary* dictionary, const NCLLoadingSettings& settings = NCLLoadingSettings() );
	NCLWidget();
	
	~NCLWidget();
	
protected:
	void initWithFile( const char *plistFile, const NCLLoadingSettings& settings );
	void initWithDictionary( cocos2d::CCDictionary *data, const NCLLoadingSettings& settings );
	
private:
	// Referenced by clone book
	cocos2d::CCDictionary *originalPlist = nullptr;
	// All children are retained
	std::vector<cocos2d::CCNode*> cocosChildren;
	// References to named nodes
	std::map<std::string, cocos2d::CCNode*> addressBook;
	// Specialy named nodes used in menu item image
	std::map<std::string, std::vector<cocos2d::CCNode*> > specials;
	// Original Dictionary references for named nodes
	std::map<std::string, cocos2d::CCDictionary*> cloneBook;
	// List of MenuItems used in locking
	std::vector<cocos2d::CCMenuItem*> menuItems;
	// List of tables used in locking
	std::vector<CEFlowLayout*> flowLayouts;
	// List of widgets created within this widget, used to find nodes
	std::vector<NCLWidgetNode*> internalWidgets;
	
	bool isLocked;
	// Names of nodes not to be locked
	std::vector<std::string> lockIgnoreList;
	
	// Original settings, used in loading and factory
	NCLLoadingSettings settings;
		
#pragma mark Factory support
private:
	// NCLNodeFactory should be able to access the methods to register items, other classes should not
	friend class NCLNodeFactory;
	friend class NCLWidgetCache;
	
	void addToAddressBook( cocos2d::CCNode* node, std::string name );
	void addMenuItem( cocos2d::CCMenuItem* item );
	void addFlowLayout( CEFlowLayout* layout );
	void addWidgetNode( NCLWidgetNode* widget );
	void addToCloneBook( cocos2d::CCDictionary* cloneDict, std::string name );
	bool isSpecialyNormal( cocos2d::CCNode* node );
	bool isSpecialySelected( cocos2d::CCNode* node );
	bool isSpecialyDisabled( cocos2d::CCNode* node );
	
public:
	std::string nameForNode( cocos2d::CCNode* node );

	
#pragma mark Loading and setup
private:
	void removeAllCocosChildren();
	static cocos2d::CCDictionary* findNodeDictionary( cocos2d::CCArray* children, cocos2d::CCString* name );
	
protected:
	virtual cocos2d::CCNode* createNode( cocos2d::CCDictionary* cDict, cocos2d::CCNode* parent, bool putIntoAddressBook );
	
public:
	void changeOriginalDictProperty( cocos2d::CCString* nodeName, cocos2d::CCString* property, cocos2d::CCString* value );
	void changeOriginalDictProperty( const std::string& nodeName, const std::string& property, const std::string& value );
	
	void cocosizeInto( cocos2d::CCNode* node );
	cocos2d::CCNode* clone( std::string name );
	cocos2d::CCNode* detachAndClone( std::string name );
	
	bool isLoaded();
	const NCLLoadingSettings& getSettings();
	void setSettings( const NCLLoadingSettings& newSettings );
	void addSelector( std::string handleName, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler handle );
	void setResponder( NCLResponder* responder );

	void reload();
	void unload();
	
	void sendNodeToWidgetCache( const std::string& nodeName );

	
#pragma mark Node search
// Find your nodes in any way your heart desires
private:
	cocos2d::CCNode* vGetCCNodeWithNameFormat( const char *format, va_list ap );
	
public:
	cocos2d::CCNode* getCCNodeWithNameFormat( const char *format, ... ) CC_FORMAT_PRINTF(2, 3);
	cocos2d::CCNode* getCCNode( std::string name );
	const std::vector<cocos2d::CCNode*>& getAllChildren();
	
	template <class CC>
	void getNodePointer( CC **nodePtr, std::string name )
	{
		cocos2d::CCNode *node = getCCNode(name);
		if (node) {
			(*nodePtr) = dynamic_cast<CC*>(node);
		}
	}
	
	template <class CC>
	void getNodePointerWithFormat( CC **nodePtr, const char *format, ... ) CC_FORMAT_PRINTF(3, 4)
	{
		va_list ap;
		va_start(ap, format);
		cocos2d::CCNode *node = vGetCCNodeWithNameFormat( format, ap );
		va_end(ap);
		if (node) {
			(*nodePtr) = dynamic_cast<CC*>(node);
		}
	}
	
	template <class CC>
	CC* getNode( std::string name )
	{
		cocos2d::CCNode *node = getCCNode(name);
		if (node) {
			return dynamic_cast<CC*>(node);
		}
		return NULL;
	}
	
	template <class CC>
	CC* getNodeWithFormat( const char *format, ... ) CC_FORMAT_PRINTF(2, 3)
	{
		va_list ap;
		va_start(ap, format);
		cocos2d::CCNode *node = vGetCCNodeWithNameFormat(format, ap);
		va_end(ap);
		if (node) {
			return dynamic_cast<CC*>(node);
		}
		return NULL;
	}

	
#pragma mark Locking
// Used to batch enable/disable elements with actions on a widget (menu items and flow layouts)
public:
	void setLock( bool lock );
	void addLockIgnoreName( std::string name );
	void setLockIgnoreNames( std::vector<std::string> names );
	void clearLockIgnoreList();
};

#endif /* defined(__KnightKingdom__NCLWidget__) */
