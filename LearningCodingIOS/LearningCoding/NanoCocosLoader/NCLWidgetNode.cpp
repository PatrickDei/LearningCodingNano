//
//  NCLWidgetNode.cpp
//  KnightKingdom
//
//  Created by Bruno Mikus on 19/11/14.
//
//

#include "NCLWidgetNode.h"
#include "NCLNodeFactory.h"


NCLWidgetNode::NCLWidgetNode() : NCLWidget(), cocos2d::CCNodeRGBA()
{
	
}

NCLWidgetNode::~NCLWidgetNode()
{
	
}

NCLWidgetNode* NCLWidgetNode::create( const char* plistFile, const NCLLoadingSettings& settings )
{
	NCLWidgetNode* wNode = new NCLWidgetNode();
	wNode->initWithFile(plistFile, settings);
	wNode->init();
	wNode->autorelease();
	return wNode;
}

NCLWidgetNode* NCLWidgetNode::create( cocos2d::CCDictionary* dictionary, const NCLLoadingSettings& settings )
{
	NCLWidgetNode* wNode = new NCLWidgetNode();
	wNode->initWithDictionary(dictionary, settings);
	wNode->init();
	wNode->autorelease();
	return wNode;
}

NCLWidgetNode* NCLWidgetNode::create()
{
	NCLWidgetNode* wNode = new NCLWidgetNode();
	wNode->init();
	wNode->autorelease();
	return wNode;
}

cocos2d::CCNode* NCLWidgetNode::createNode( cocos2d::CCDictionary* cDict, cocos2d::CCNode* parent, bool putIntoAddressBook )
{
	return NCLNodeFactory::createFromDictionary(cDict, this, parent ? parent : this, putIntoAddressBook);
}