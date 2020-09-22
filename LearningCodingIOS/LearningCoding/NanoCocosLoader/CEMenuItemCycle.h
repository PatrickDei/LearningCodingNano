//
//  CEMenuItemCycle.h
//  NanoEditor
//
//  Created by Bruno on 10/22/13.
//
//

#ifndef __NanoEditor__CEMenuItemCycle__
#define __NanoEditor__CEMenuItemCycle__

#include "cocos2d.h"

class CEMenuItemCycle : public cocos2d::CCMenuItem {
	cocos2d::CCMenuItem *selectedItem;
	cocos2d::CCSize noItemSize;
	
	CCMenuItem* findNextMenuItem();
	void refreshSelectedItem();
	
public:
	virtual bool init();
	
	virtual void setContentSize( cocos2d::CCSize size );
	
	virtual void activate();
	virtual void selected();
	virtual void unselected();
	virtual void addChild(CCNode* child, int zOrder, int tag);
	virtual void removeChild(CCNode* child, bool cleanup);
	virtual void removeAllChildrenWithCleanup(bool cleanup);
	
	cocos2d::CCMenuItem* getSelectedItem();
	void selectItemWithTag( int tag );
	void selectItemAtIndex( int index );
};

#endif /* defined(__NanoEditor__CEMenuItemCycle__) */
