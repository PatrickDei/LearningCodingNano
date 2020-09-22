//
//  NCLNodeFactory.h
//  NCLCpp
//
//  Created by Bruno on 6/24/13.
//
//

#ifndef __NCLCpp__NCLNodeFactory__
#define __NCLCpp__NCLNodeFactory__

#define ANDROID_COMPATIBILITY

namespace cocos2d{
	namespace extension{
		class CCScale9Sprite;
	}
	class CCSprite;
	class CCDictionary;
	class CCNode;
	class CCRGBAProtocol;
	class CCLabelTTF;
	class CCLabelBMFont;
	class CCProgressTimer;
	class CCParticleSystem;
	class CCMenu;
	class CCMenuItem;
	class CCMenuItemSprite;
	class CCMenuItemImage;
	class CCMenuItemLabel;
}

class CEFlowLayout;
class NCLWidget;
struct NCLLoadingSettings;
class CEFlowLayoutDataSource;
class CEFlowLayoutDelegate;

class NCLNodeFactory {
private:
	static void loadSprite( cocos2d::CCSprite* node, cocos2d::CCDictionary* dictionary, float scale, float imageScale, bool findSpriteFrame, cocos2d::CCSize parentSize );
	static void loadSprite9( cocos2d::extension::CCScale9Sprite* node, cocos2d::CCDictionary* dictionary, float scale, float imageScale, bool findSpriteFrame, cocos2d::CCSize parentSize );
	static void loadNode( cocos2d::CCNode* node, cocos2d::CCDictionary* dictionary, float scale, cocos2d::CCSize parentSize );
	static void loadNodeRGBA( cocos2d::CCRGBAProtocol* node, cocos2d::CCDictionary* dictionary );
	static void loadLabelTTF( cocos2d::CCLabelTTF *node, cocos2d::CCDictionary *dictionary, float scale, NCLWidget* manager, cocos2d::CCSize parentSize );
	static void loadLabelBMF( cocos2d::CCLabelBMFont *node, cocos2d::CCDictionary *dictionary, float scale, NCLWidget* manager );
	static void loadProgressTimer( cocos2d::CCProgressTimer *node, cocos2d::CCDictionary *dictionary );
	static void loadParticleSystemQuad( cocos2d::CCParticleSystemQuad *node, cocos2d::CCDictionary *dictionary );
	static void loadMenu( cocos2d::CCMenu *node, cocos2d::CCDictionary* dictionary );
	static void loadMenuItem( cocos2d::CCMenuItem *node, cocos2d::CCDictionary* dictionary, const NCLLoadingSettings& settings, float scale, cocos2d::CCSize parentSize  );
	static void loadMenuSprite( cocos2d::CCMenuItemSprite *node );
	static void loadMenuImage( cocos2d::CCMenuItemImage *node, cocos2d::CCDictionary* dictionary );
	static void loadMenuImageSpecials( cocos2d::CCMenuItemImage *node, NCLWidget* manager );
	static void loadMenuLabel( cocos2d::CCMenuItemLabel *node );
	static void loadFlowLayout( CEFlowLayout* node, cocos2d::CCDictionary* dictionary, float scale, CEFlowLayoutDataSource *dataSource, CEFlowLayoutDelegate* delegate, cocos2d::CCSize parentSize );
	static void loadContentSize( cocos2d::CCNode* node, cocos2d::CCDictionary* dict, float scale, cocos2d::CCSize parentSize );
	
public:
	// Creates a new CCNode, fills owner address and clone book if necessary
	static cocos2d::CCNode* createFromDictionary( cocos2d::CCDictionary* dictionary, NCLWidget* owner = NULL, cocos2d::CCNode* parent = nullptr, bool putIntoAddressBook = true, float parentCSFactor = 1.0 );
    static void loadPosition(cocos2d::CCNode *node, cocos2d::CCDictionary *dictionary, float scale, cocos2d::CCSize parentSize);
};

void onlyParentScaleMultiply( cocos2d::CCNode* node, float factor, bool skipFirst = false);

#endif /* defined(__NCLCpp__NCLNodeFactory__) */
