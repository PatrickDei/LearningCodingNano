//
//  NCLWidget.cpp
//  KnightKingdom
//
//  Created by Bruno Mikus on 26/09/14.
//
//

#include "NCLWidget.h"
#include "NCLNodeFactory.h"
#include "NCLResponder.h"
#include "CEFlowLayout.h"
#include "NCLWidgetCache.h"
#include "NCLWidgetNode.h"

#define MAX_NAME_LEN 128

using namespace cocos2d;
using namespace std;

#pragma mark - NCLWidget

NCLWidget::NCLWidget( const char* plistFile, const NCLLoadingSettings& settings )
{
	initWithFile( plistFile, settings );
}

NCLWidget::NCLWidget( const char* plistFile, NCLResponder* responder, CEFlowLayoutDataSource* tableDataSource, CEFlowLayoutDelegate* tableDelegate )
{
	NCLLoadingSettings settings;
	settings.responder = responder;
	settings.flowDataSource = tableDataSource;
	settings.flowDelegate = tableDelegate;
	settings.deviceScale = NCLDeviceInfo::getDeviceScale();
	settings.fontScale = settings.deviceScale;
	settings.imageScale = settings.deviceScale;
	initWithFile(plistFile, settings);
}

NCLWidget::NCLWidget( cocos2d::CCDictionary* dictionary, const NCLLoadingSettings& settings )
{
	initWithDictionary(dictionary, settings);
}

NCLWidget::NCLWidget()
{
}

NCLWidget::~NCLWidget()
{
	unload();
	specials.clear();
	CC_SAFE_RELEASE(originalPlist);
}

void NCLWidget::initWithFile( const char *plistFile, const NCLLoadingSettings& settings )
{
	string fileName = plistFile;
	if( settings.addExtension )
	{
		fileName = NCLDeviceInfo::appendExtensionToPath(fileName);
	}
	
	/* LOAD PLIST */
	CCDictionary* plistDictionary = CCDictionary::createWithContentsOfFile(fileName.c_str());
	
	initWithDictionary( plistDictionary, settings );
}

void NCLWidget::initWithDictionary( cocos2d::CCDictionary *data, const NCLLoadingSettings& _settings )
{
	setSettings(_settings);
	isLocked = false;
	
	if( specials.size() == 0 )
	{
		specials["_n"] = std::vector<CCNode*>();
		specials["_s"] = std::vector<CCNode*>();
		specials["_d"] = std::vector<CCNode*>();
	}
	
	/* LOAD PLIST */
	originalPlist = data;
	CC_SAFE_RETAIN(originalPlist);
	
	float factor = 1.0;	// TODO: convert to new scheme with better old-menu compatibility
#ifdef ANDROID_COMPATIBILITY
	CCDictionary* settingsDict = (CCDictionary*)originalPlist->objectForKey("settings");
	if( settingsDict )
	{
		CCString* versionStr = (CCString*)settingsDict->objectForKey("version");
		if( !versionStr )
		{
			CCString* retinaStr = (CCString*)settingsDict->objectForKey("retina");
			bool isRetina = retinaStr ? retinaStr->boolValue() : false;
			
			CCString* deviceStr = (CCString*)settingsDict->objectForKey("device");
			if( deviceStr ){
				if(((deviceStr->compare("iPhone") == 0 && isRetina) || deviceStr->compare("iPhone5") == 0 ) )
					factor *= 2.0;
				
				//if( deviceStr->compare("iPad") == 0 )
				//	factor *= 0.5;
			}
		}
	}
#endif
	
	settings.deviceScale *= factor;
	settings.fontScale *= factor;
	
	if( settings.doLoad ){
		reload();
	}
}


#pragma mark Factory support

void NCLWidget::addToAddressBook(CCNode *node, string name)
{
	if( NULL != node && name.length() > 0 )
	{
		auto foundSpecialIt = specials.find(name);
		if( foundSpecialIt != specials.end() )
		{
			foundSpecialIt->second.push_back(node);
		}
		else
		{
			addressBook[name] = node;
		}
	}
}

void NCLWidget::addMenuItem(cocos2d::CCMenuItem *item)
{
	menuItems.push_back(item);
}

void NCLWidget::addFlowLayout(CEFlowLayout *layout)
{
	flowLayouts.push_back(layout);
}

void NCLWidget::addWidgetNode(NCLWidgetNode *widget)
{
	internalWidgets.push_back(widget);
}

void NCLWidget::addToCloneBook(CCDictionary *cloneDict, string name )
{
	if( NULL != cloneDict && name.length() > 0 )
	{
		auto foundSpecialIt = specials.find(name);
		if( foundSpecialIt == specials.end() )
		{
			cloneBook[name] = cloneDict;
		}
	}
}

string NCLWidget::nameForNode(CCNode *node)
{
	map<string,CCNode*>::iterator target = addressBook.begin();
	while (target != addressBook.end() && target->second != node )
	{
		target++;
	}
	if( target != addressBook.end() )
	{
		return target->first;
	}
	return "";
}

bool NCLWidget::isSpecialyNormal( cocos2d::CCNode* node )
{
	auto normals = specials["_n"];
	return find( normals.begin(), normals.end(), node ) != normals.end();
}

bool NCLWidget::isSpecialySelected( cocos2d::CCNode* node )
{
	auto selecteds = specials["_s"];
	return find( selecteds.begin(), selecteds.end(), node ) != selecteds.end();
}

bool NCLWidget::isSpecialyDisabled( cocos2d::CCNode* node )
{
	auto disableds = specials["_d"];
	return find( disableds.begin(), disableds.end(), node ) != disableds.end();
}


#pragma mark Loading and setup

void NCLWidget::removeAllCocosChildren()
{
	for( int i=0; i<cocosChildren.size(); i++ ){
		cocosChildren[i]->release();
	}
	cocosChildren.clear();
}

CCDictionary* NCLWidget::findNodeDictionary( CCArray* children, CCString* name )
{
	if( !children )
		return nullptr;
	
	CCDictionary* childDict = nullptr;
	CCString* childName = nullptr;
	CCArray* grandChildren = nullptr;
	CCDictionary* foundDict = nullptr;
	for( int i = 0; i < children->data->num; i++ )
	{
		childDict = static_cast<CCDictionary*>(children->data->arr[i]);
		childName = static_cast<CCString*>(	childDict->objectForKey("name") );
		if( childName != nullptr && strcmp( childName->getCString(), name->getCString()) == 0 )
		{
			foundDict = childDict;
			break;
		}
		else
		{
			grandChildren = static_cast<CCArray*>(childDict->objectForKey("children"));
			if( grandChildren )
			{
				foundDict = NCLWidget::findNodeDictionary(grandChildren, name);
				if( foundDict )
				{
					break;
				}
			}
		}
	}
	return foundDict;
}

cocos2d::CCNode* NCLWidget::createNode( cocos2d::CCDictionary* cDict, cocos2d::CCNode* parent, bool putIntoAddressBook )
{
	return NCLNodeFactory::createFromDictionary(cDict, this, parent, putIntoAddressBook);
}

void NCLWidget::changeOriginalDictProperty(CCString *nodeName, CCString *property, CCString *value)
{
	if( !originalPlist )
		return;
	
	CCArray *children = (CCArray *)originalPlist->objectForKey("children");
	if( children )
	{
		CCDictionary* nodeDict = NCLWidget::findNodeDictionary(children, nodeName);
		if( nodeDict )
		{
			nodeDict->setObject(value, property->getCString());
		}
	}
}

void NCLWidget::changeOriginalDictProperty( const std::string& nodeName, const std::string& property, const std::string& value )
{
	changeOriginalDictProperty(CCString::create(nodeName), CCString::create(property), CCString::create(value));
}

void NCLWidget::cocosizeInto(CCNode* target)
{
    CCArray *children = (CCArray *)originalPlist->objectForKey("children");
	vector<CCNode*>::iterator childIt = cocosChildren.begin();
    int index = 0;
	while (childIt != cocosChildren.end())
	{
        CCDictionary *childDictionary = (CCDictionary*)(children->objectAtIndex(index));
		(*childIt)->removeFromParentAndCleanup(false);
		target->addChild( *childIt );
        
        NCLNodeFactory::loadPosition((*childIt), childDictionary, settings.deviceScale, target->getContentSize());
        index++;
		childIt++;
	}
}

CCNode* NCLWidget::clone(string name)
{
	map<string, CCDictionary*>::iterator cloneIt = cloneBook.find(name);
	CCNode* child = NULL;
	if( cloneIt != cloneBook.end() )
	{
		child = createNode(cloneIt->second, nullptr, false);
	}
	return child;
}

CCNode* NCLWidget::detachAndClone( string name )
{
	CCNode *existingChild = getNode<CCNode>(name);
	CCNode *parent = NULL;
	if( existingChild )
	{
		auto childInRootIt = find(cocosChildren.begin(), cocosChildren.end(), existingChild);
		if( childInRootIt != cocosChildren.end() )
		{
			cocosChildren.erase(childInRootIt);
		}
		else
		{
			parent = existingChild->getParent();
		}
	}
	
	map<string, CCDictionary*>::iterator cloneIt = cloneBook.find(name);
	CCNode* child = NULL;
	if( cloneIt != cloneBook.end() )
	{
		child = createNode(cloneIt->second, parent, true);
		// If a node was root, autorelease it since it was not autoreleased after beeing created
		// If it had a parent, we need to retain it to maintain it, and we need an autorelease to clear that retain
		// It is supposed that existingChild will be added soon after detachAndClone was called
		existingChild->autorelease();
		if( !parent )
		{	// ROOT node, NOTE: it can have a parent, but we do not cere since it was not created as NCL
			cocosChildren.push_back(child);
		}
		else
		{
			existingChild->retain();
			existingChild->removeFromParentAndCleanup(true);
			parent->addChild(child);
			child->release();
		}
	}
	return existingChild;
}

bool NCLWidget::isLoaded()
{
	return cocosChildren.size() != 0;
}

const NCLLoadingSettings& NCLWidget::getSettings()
{
	return settings;
}

void NCLWidget::setSettings( const NCLLoadingSettings& newSettings )
{
	settings = newSettings;
}

void NCLWidget::addSelector(std::string handleName, cocos2d::CCObject *target, cocos2d::SEL_MenuHandler handle)
{
	settings.addSelector( handleName, target, handle );
}

void NCLWidget::setResponder(NCLResponder *responder)
{
	settings.responder = responder;
}

void NCLWidget::reload()
{
	unload();
	if( !originalPlist )
		return;
	
	settings.doLoad = true;

	CCArray *children = (CCArray *)originalPlist->objectForKey("children");
	if( NULL != children ){
		int length = children->count();
		for( int i=0; i<length; i++ ){
			CCDictionary *childDictionary = (CCDictionary*)(children->objectAtIndex(i));
			CCNode* newChild = createNode(childDictionary, nullptr, true);
			cocosChildren.push_back(newChild);
		}
	}
}

void NCLWidget::unload()
{
	removeAllCocosChildren();
	addressBook.clear();
	for(auto& pair : specials )
	{
		pair.second.clear();
	}
	cloneBook.clear();
	menuItems.clear();
	flowLayouts.clear();
	lockIgnoreList.clear();
	internalWidgets.clear();
}

void NCLWidget::sendNodeToWidgetCache( const std::string& nodeName )
{
	if( !originalPlist )
		return;
	
	CCString* ccName = CCString::create( nodeName );
	CCArray *children = (CCArray *)originalPlist->objectForKey("children");
	if( children )
	{
		CCDictionary* nodeDict = NCLWidget::findNodeDictionary(children, ccName);
		if( nodeDict )
		{
			NCLWidgetCache::get()->addWidget(nodeDict, &settings, nodeName.c_str());
		}
	}
}

#pragma mark Node search

cocos2d::CCNode* NCLWidget::vGetCCNodeWithNameFormat( const char *format, va_list ap )
{
	CCNode *node = NULL;
	char* pBuf = (char*)malloc(MAX_NAME_LEN);
	if (pBuf != NULL)
	{
		vsnprintf(pBuf, MAX_NAME_LEN, format, ap);
		node = getCCNode(pBuf);
		free(pBuf);
	}
	
	return node;
}

cocos2d::CCNode* NCLWidget::getCCNodeWithNameFormat( const char *format, ... )
{
	va_list ap;
	va_start(ap, format);
	CCNode *node = vGetCCNodeWithNameFormat(format, ap);
	va_end(ap);
	
	return node;
}

CCNode* NCLWidget::getCCNode( string name)
{
	map<string,CCNode*>::iterator target = addressBook.find(name);
	if (target != addressBook.end())
	{
		return target->second;
	}
	CCNode* found = nullptr;
	for( NCLWidgetNode* widget : internalWidgets )
	{
		found = widget->getCCNode(name);
		if( found )
			break;
	}
	return found;
}

const std::vector<cocos2d::CCNode*>& NCLWidget::getAllChildren()
{
	return cocosChildren;
}


#pragma mark Locking

void NCLWidget::setLock( bool lock )
{
	if( (!isLocked && lock) || (isLocked && !lock) )
	{
		isLocked = lock;
		for( auto mItem : menuItems )
		{
			mItem->setEnabled(!isLocked);
		}
		for( auto fLayout : flowLayouts )
		{
			fLayout->setLock(isLocked);
		}
		lockIgnoreList.clear();
	}
}

void NCLWidget::addLockIgnoreName(std::string name){
	if( isLocked )
	{
		CCNode* node = getCCNode(name);
		if( node && find(menuItems.begin(), menuItems.end(), (CCMenuItem*)node) != menuItems.end() )
		{
			((CCMenuItem*)node)->setEnabled(true);
		}
	}
	lockIgnoreList.push_back(name);
}

void NCLWidget::setLockIgnoreNames(std::vector<std::string> names)
{
	clearLockIgnoreList();
	for (auto name : names)
	{
		addLockIgnoreName(name);
	}
}

void NCLWidget::clearLockIgnoreList()
{
	if( isLocked )
	{
		for (auto name : lockIgnoreList)
		{
			CCNode* node = getCCNode(name);
			if( node && find(menuItems.begin(), menuItems.end(), (CCMenuItem*)node) != menuItems.end() )
			{
				((CCMenuItem*)node)->setEnabled(false);
			}
		}
	}
	lockIgnoreList.clear();
}
