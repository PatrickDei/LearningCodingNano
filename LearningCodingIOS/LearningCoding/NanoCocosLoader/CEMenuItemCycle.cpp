//
//  CEMenuItemCycle.cpp
//  NanoEditor
//
//  Created by Bruno on 10/22/13.
//
//

#include "CEMenuItemCycle.h"

using namespace cocos2d;

bool CEMenuItemCycle::init(){
	if( CCMenuItem::init() ){
		selectedItem = NULL;
		return true;
	}
	return false;
}

void CEMenuItemCycle::setContentSize( cocos2d::CCSize size ){
	noItemSize = size;
	if( !selectedItem )
	{
		CCMenuItem::setContentSize(size);
	}
}

CCMenuItem* CEMenuItemCycle::findNextMenuItem()
{
	int startIndex = -1;
	int childrenCount = getChildrenCount();
	if( selectedItem )
	{
		startIndex = m_pChildren->indexOfObject(selectedItem);
		if( startIndex == CC_INVALID_INDEX )
		{
			return NULL;
		}
	}
	int count = childrenCount;
	while (count > 0) {
		startIndex = (startIndex+1) % childrenCount;
		CCNode *potentialNode = (CCNode*)m_pChildren->objectAtIndex(startIndex);
		if( dynamic_cast<CCMenuItem*>(potentialNode) ){
			return (CCMenuItem*)potentialNode;
		}
		count--;
	}
	return NULL;
}

void CEMenuItemCycle::refreshSelectedItem(){
	selectedItem = findNextMenuItem();
	if( selectedItem )
	{
		CCMenuItem::setContentSize(selectedItem->getContentSize());
		selectedItem->setVisible(true);
	}
	int childrenCount = getChildrenCount();
	for (int i = 0; i<childrenCount; i++) {
		CCNode *item = (CCNode*)m_pChildren->objectAtIndex(i);
		if( dynamic_cast<CCMenuItem*>(item) && item != selectedItem ){
			item->setVisible(false);
		}
	}
}

void CEMenuItemCycle::activate(){
	if( m_bEnabled ){
		refreshSelectedItem();
		CCMenuItem::activate();
	}
}

void CEMenuItemCycle::selected()
{
	CCMenuItem::selected();
	if( selectedItem )
		selectedItem->selected();
}

void CEMenuItemCycle::unselected()
{
	CCMenuItem::unselected();
	if( selectedItem )
		selectedItem->unselected();
}

void CEMenuItemCycle::addChild(CCNode* child, int zOrder, int tag)
{
	CCMenuItem::addChild(child, zOrder, tag);
	if( dynamic_cast<CCMenuItem*>(child)){
		if( selectedItem == NULL ){
			refreshSelectedItem();
		}else{
			child->setVisible(false);
		}
	}
}

void CEMenuItemCycle::removeChild(CCNode* child, bool cleanup)
{
	if( child == selectedItem )
	{
		refreshSelectedItem();
	}
	CCMenuItem::removeChild(child, cleanup);
	if( child == selectedItem )
	{
		refreshSelectedItem();
	}
}

void CEMenuItemCycle::removeAllChildrenWithCleanup(bool cleanup)
{
	selectedItem = NULL;
	CCMenuItem::removeAllChildrenWithCleanup(cleanup);
	refreshSelectedItem();
}

CCMenuItem* CEMenuItemCycle::getSelectedItem()
{
	return selectedItem;
}

void CEMenuItemCycle::selectItemWithTag( int tag )
{
	CCMenuItem *newItem = dynamic_cast<CCMenuItem*>( getChildByTag(tag) );
	if( newItem )
	{
		if( selectedItem )
		{
			selectedItem->setVisible(false);
		}
		selectedItem = newItem;
		selectedItem->setVisible(true);
	}
}

void CEMenuItemCycle::selectItemAtIndex( int index )
{
	int i = 0;
	int childrenCount = getChildrenCount();
	if( index > childrenCount )
		return;
	while (index > 0 && i < childrenCount ) {
		CCMenuItem *newItem =  dynamic_cast<CCMenuItem*>( m_pChildren->objectAtIndex(i) );
		if( newItem )
		{
			if( index == 0 )
			{
				if( selectedItem )
				{
					selectedItem->setVisible(false);
				}
				selectedItem = newItem;
				selectedItem->setVisible(true);
				return;
			}
			index--;
		}
		i++;
	}
}
