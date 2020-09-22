//
//  NCLNodeFactory.cpp
//  NCLCpp
//
//  Created by Bruno on 6/24/13.
//
//

#include "cocos2d.h"
#include "NCLNodeFactory.h"
#include "NCLResponder.h"
#include "NCLUtils.h"
#include "ccUtils.h"
#include "CEMenuItemCycle.h"
#include "CCScale9Sprite.h"
#include "CEFlowLayout.h"
#include "NCLDeviceNodeManager.h"
#include "NCLWidgetCache.h"
#include "NCLLocalizer.h"
#include "NCLWidgetNode.h"

#define MENU_ITEM_SCALE_COMPATIBILITY 0

#define NCL_NODE_DEFAULT_POSITION CCPoint(0.0, 0.0)
#define NCL_NODE_DEFAULT_ANCHOR CCPoint(0.0, 0.0)
#define NCL_NODE_DEFAULT_SCALE 1.0
#define NCL_NODE_DEFAULT_SKEW 0.0
#define NCL_NODE_DEFAULT_ANGLE 0.0
#define NCL_NODE_DEFAULT_TAG 0
#define NCL_NODE_DEFAULT_VISIBLE true
#define NCL_NODE_DEFAULT_CONTENT_SIZE CCSize(0, 0)

#define NCL_RGBANODE_DEFAULT_OPACITY 255
#define NCL_RGBANODE_DEFAULT_COLOR ccc3(255, 255, 255)
#define NCL_RGBANODE_DEFAULT_CASCADE true

#define NCL_LABELTTF_DEFAULT_DIMENSIONS CCSize(0, 0)
#define NCL_LABELTTF_DEFAULT_H_ALIGN kCCTextAlignmentLeft
#define NCL_LABELTTF_DEFAULT_LINE_BREAK kCCLineBreakModeWordWrap
#define NCL_LABELTTF_DEFAULT_V_ALIGN kCCVerticalTextAlignmentTop
#define NCL_LABELTTF_DEFAULT_FONT_NAME "Verdana"
#define NCL_LABELTTF_DEFAULT_FONT_SIZE 12
#define NCL_LABELTTF_DEFAULT_FONT_SIZE_PAD 16

#define NCL_LABELBM_DEFAULT_FONT "Myriad_Pro_White_Regular_13.fnt"
#define NCL_LABELBM_DEFAULT_WIDTH kCCLabelAutomaticWidth
#define NCL_LABELBM_DEFAULT_ALIGN kCCTextAlignmentLeft
#define NCL_LABELBM_DEFAULT_OFFSET CCPoint(0, 0)

#define NCL_PROGBAR_DEFAULT_MIDPOINT CCPoint(0, 0)
#define NCL_PROGBAR_DEFAULT_BAR_CHANGE_RATE CCPoint(1, 1)
#define NCL_PROGBAR_DEFAULT_PERCENTAGE 100
#define NCL_PROGBAR_DEFAULT_REVERSE false

#define NCL_FLOW_VIEW_SIZE CCSize(100, 100)
#define NCL_FLOW_PAGE_RATIO 0.5
#define NCL_FLOW_PAGE_SPEED CCPoint(300, 300)
#define NCL_FLOW_SCROLL_SENSITIVITY 4
#define NCL_FLOW_AUTOSCROLL NO
#define NCL_FLOW_SCROLLSTOP 0.5
#define NCL_FLOW_SCROLL_SMOOTHNESS 0.5
#define NCL_FLOW_EASING true
#define NCL_FLOW_SNAP true
#define NCL_FLOW_TOUCH_RADIUS 12

#define NCL_MENU_DEFAULT_ENABLED true

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace NCLUtils;

void NCLNodeFactory::loadSprite(CCSprite *node, CCDictionary *dictionary, float scale, float imageScale,
                                bool findSpriteFrame, cocos2d::CCSize parentSize)
{
    CCString *imageName = (CCString *)dictionary->objectForKey("image");
    if (NULL != imageName) {
        bool didLoad = false;
        if (findSpriteFrame) {
            CCSpriteFrame *frame =
                CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(imageName->getCString());
            if (frame) {
                node->setDisplayFrame(frame);
                didLoad = true;
            }
        }

        if (!didLoad) {
            CCTexture2D *newImage = CCTextureCache::sharedTextureCache()->addImage(imageName->getCString());
            if (newImage) {
                node->setTexture(newImage);
                CCRect textureRect = CCRect();
                textureRect.size = newImage->getContentSize();
                node->setTextureRect(textureRect);
                node->setContentSize(textureRect.size);
            }
        }
    }

    CCString *texMag = (CCString *)dictionary->objectForKey("texMag");
    CCString *texMin = (CCString *)dictionary->objectForKey("texMin");
    CCString *texWrapS = (CCString *)dictionary->objectForKey("texWrapS");
    CCString *texWrapT = (CCString *)dictionary->objectForKey("texWrapT");

    if ((texMag || texMin || texWrapS || texWrapT) && !false) {
        CCTexture2D *texture = node->getTexture();
        ccGLBindTexture2D(texture->getName());
        GLint tParam[4];
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &tParam[0]);
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &tParam[1]);
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &tParam[2]);
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &tParam[3]);
        ccTexParams params;
        params.magFilter = tParam[0];
        params.minFilter = tParam[1];
        params.wrapS = tParam[2];
        params.wrapT = tParam[3];

        if (texMag) {
            params.magFilter = intFromString(texMag);
        }
        if (texMin) {
            params.minFilter = intFromString(texMin);
        }
        if (texWrapS) {
            params.wrapS = intFromString(texWrapS);
        }
        if (texWrapT) {
            params.wrapT = intFromString(texWrapT);
        }

        if ((texture->getPixelsWide() == ccNextPOT(texture->getPixelsWide()) || params.wrapS == GL_CLAMP_TO_EDGE) &&
            (texture->getPixelsHigh() == ccNextPOT(texture->getPixelsHigh()) || params.wrapT == GL_CLAMP_TO_EDGE)) {
            texture->setTexParameters(&params);

            loadContentSize(node, dictionary, scale, parentSize);
            CCRect rect;
            rect.origin = CCPoint(0, 0);
            rect.size = node->getContentSize();
            node->setTextureRect(rect);
        }

    } else {
#ifndef ANDROID_COMPATIBILITY
        loadContentSize(node, dictionary, scale, parentSize);
#else
        loadContentSize(node, dictionary, scale, parentSize / imageScale);

        CCTexture2D *texture = node->getTexture();
        if (!node->getContentSize().equals(texture->getContentSize())) {
            node->setContentSize(node->getContentSize());

            ccGLBindTexture2D(texture->getName());
            GLint tParam[4];
            glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &tParam[0]);
            glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &tParam[1]);
            glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &tParam[2]);
            glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &tParam[3]);
            ccTexParams params;
            params.magFilter = tParam[0];
            params.minFilter = tParam[1];
            params.wrapS = tParam[2];
            params.wrapT = tParam[3];
            texture->setTexParameters(&params);

            CCRect rect;
            rect.origin = CCPoint(0, 0);
            rect.size = node->getContentSize();
            node->setTextureRect(rect);
        }
#endif
    }

    CCString *flipX = (CCString *)dictionary->objectForKey("flipX");
    node->setFlipX(flipX ? boolFromString(flipX) : false);

    CCString *flipY = (CCString *)dictionary->objectForKey("flipY");
    node->setFlipY(flipY ? boolFromString(flipY) : false);
}

void NCLNodeFactory::loadSprite9(cocos2d::extension::CCScale9Sprite *node, cocos2d::CCDictionary *dictionary,
                                 float scale, float imageScale, bool findSpriteFrame, cocos2d::CCSize parentSize)
{
    CCString *imageName = (CCString *)dictionary->objectForKey("image");
    if (NULL != imageName) {
        CCSpriteFrame *frame = nullptr;
        if (findSpriteFrame) {
            frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(imageName->getCString());
        }

        if (!frame) {
            CCTexture2D *newImage = CCTextureCache::sharedTextureCache()->addImage(imageName->getCString());
            if (newImage) {
                CCRect rect = CCRect();
                rect.size = newImage->getContentSize();
                frame = CCSpriteFrame::create(imageName->getCString(), rect);
            }
        }
        if (frame) {
            node->setSpriteFrame(frame);
        }
    }

    loadContentSize(node, dictionary, scale, parentSize * (1 / imageScale));
    CCRect rect;
    rect.origin = CCPoint(0, 0);
    rect.size = node->getContentSize();
    node->setPreferredSize(rect.size);

    CCString *bottom = (CCString *)dictionary->objectForKey("marginBottom");
    CCString *top = (CCString *)dictionary->objectForKey("marginTop");
    CCString *left = (CCString *)dictionary->objectForKey("marginLeft");
    CCString *right = (CCString *)dictionary->objectForKey("marginRight");

    CCSize size = node->getOriginalSize();
    node->setInsetBottom((bottom ? bottom->floatValue() : 0.33333) * size.height);
    node->setInsetTop((top ? top->floatValue() : 0.33333) * size.height);
    node->setInsetLeft((left ? left->floatValue() : 0.33333) * size.width);
    node->setInsetRight((right ? right->floatValue() : 0.33333) * size.width);
}

void NCLNodeFactory::loadNode(CCNode *node, CCDictionary *dictionary, float scale, CCSize parentSize)
{

    loadPosition(node, dictionary, scale, parentSize);

    CCString *scaleX = (CCString *)dictionary->objectForKey("scaleX");
    node->setScaleX(scaleX ? floatFromString(scaleX) : NCL_NODE_DEFAULT_SCALE);

    CCString *scaleY = (CCString *)dictionary->objectForKey("scaleY");
    node->setScaleY(scaleY ? floatFromString(scaleY) : NCL_NODE_DEFAULT_SCALE);

    CCString *angle = (CCString *)dictionary->objectForKey("rotation");
    if (!angle)
        angle = (CCString *)dictionary->objectForKey("angle");
    node->setRotation(angle ? floatFromString(angle) : NCL_NODE_DEFAULT_ANGLE);

    CCString *skewX = (CCString *)dictionary->objectForKey("skewX");
    node->setSkewX(skewX ? floatFromString(skewX) : NCL_NODE_DEFAULT_SKEW);

    CCString *skewY = (CCString *)dictionary->objectForKey("skewY");
    node->setSkewY(skewY ? floatFromString(skewY) : NCL_NODE_DEFAULT_SKEW);

    CCString *anchor = (CCString *)dictionary->objectForKey("anchor");
    node->setAnchorPoint(anchor ? pointFromString(anchor) : NCL_NODE_DEFAULT_ANCHOR);

    CCString *tag = (CCString *)dictionary->objectForKey("tag");
    node->setTag(tag ? intFromString(tag) : NCL_NODE_DEFAULT_TAG);

    CCString *visible = (CCString *)dictionary->objectForKey("visible");
    node->setVisible(visible ? boolFromString(visible) : NCL_NODE_DEFAULT_VISIBLE);

    CCSize contentSize = node->getContentSize();
    if (contentSize.width == 0 && contentSize.height == 0) {
        loadContentSize(node, dictionary, scale, parentSize);
    }

    CCString *zOrder = (CCString *)dictionary->objectForKey("zOrder");
    if (zOrder) {
        node->setZOrder(intFromString(zOrder));
    }
}

void NCLNodeFactory::loadPosition(cocos2d::CCNode *node, cocos2d::CCDictionary *dictionary, float scale,
                                  cocos2d::CCSize parentSize)
{
    // new
    CCString *rPosStr = (CCString *)dictionary->objectForKey("posRel");
    CCString *aPosStr = (CCString *)dictionary->objectForKey("posAbs");
    if (rPosStr || aPosStr) {
        CCPoint rPos = sizeFromString(rPosStr);
        CCPoint aPos = sizeFromString(aPosStr);

        aPos.x = aPos.x * scale + rPos.x * parentSize.width;
        aPos.y = aPos.y * scale + rPos.y * parentSize.height;
        node->setPosition(aPos);
    }

    // old & older
    CCString *positionStr = (CCString *)dictionary->objectForKey("position");
    CCString *useRelative = (CCString *)dictionary->objectForKey("useRelative");
    CCString *posRelativeStr = (CCString *)dictionary->objectForKey("relPosState");

    if (positionStr || useRelative || posRelativeStr) {
        CCPoint position = pointFromString(positionStr, NCL_NODE_DEFAULT_POSITION);

        CCPoint relativePosState = CCPoint(0, 0);
        if (useRelative && useRelative->boolValue()) {
            relativePosState = CCPoint(1, 1);
        } else if (posRelativeStr) {
            relativePosState = pointFromString(posRelativeStr);
        }

        position.x = position.x * (relativePosState.x > 0.5 ? parentSize.width : scale);
        position.y = position.y * (relativePosState.y > 0.5 ? parentSize.height : scale);
        node->setPosition(position);
    }
}

void NCLNodeFactory::loadContentSize(cocos2d::CCNode *node, cocos2d::CCDictionary *dictionary, float scale,
                                     cocos2d::CCSize parentSize)
{
    // new
    CCString *rSizeStr = (CCString *)dictionary->objectForKey("sizeRel");
    CCString *aSizeStr = (CCString *)dictionary->objectForKey("sizeAbs");
    if (rSizeStr || aSizeStr) {
        CCSize rSize = sizeFromString(rSizeStr);
        CCSize aSize = sizeFromString(aSizeStr);

        aSize.width = aSize.width * scale + rSize.width * parentSize.width;
        aSize.height = aSize.height * scale + rSize.height * parentSize.height;
        node->setContentSize(aSize);
    }

    // old
    CCString *size = (CCString *)dictionary->objectForKey("contentSize");
    CCString *sizeRelStr = (CCString *)dictionary->objectForKey("relCSState");
    if (size || sizeRelStr) {
        CCSize newSize = size ? sizeFromString(size) : NCL_NODE_DEFAULT_CONTENT_SIZE;
        CCPoint relativeCSState = sizeRelStr ? pointFromString(sizeRelStr) : CCPoint(0, 0);

        newSize.width = newSize.width * (relativeCSState.x > 0.5 ? parentSize.width : scale);
        newSize.height = newSize.height * (relativeCSState.y > 0.5 ? parentSize.height : scale);
        node->setContentSize(newSize);
    }
}

void NCLNodeFactory::loadNodeRGBA(CCRGBAProtocol *node, CCDictionary *dictionary)
{

    if (!node->isCascadeColorEnabled()) {
        CCString *cascadeColor = (CCString *)dictionary->objectForKey("cascadeColor");
        node->setCascadeColorEnabled(cascadeColor ? boolFromString(cascadeColor) : NCL_RGBANODE_DEFAULT_CASCADE);
    }

    if (!node->isCascadeOpacityEnabled()) {
        CCString *cascadeOpacity = (CCString *)dictionary->objectForKey("cascadeOpacity");
        node->setCascadeOpacityEnabled(cascadeOpacity ? boolFromString(cascadeOpacity) : NCL_RGBANODE_DEFAULT_CASCADE);
    }

    CCString *opacity = (CCString *)dictionary->objectForKey("opacity");
    node->setOpacity(opacity ? intFromString(opacity) : NCL_RGBANODE_DEFAULT_OPACITY);

    CCString *color = (CCString *)dictionary->objectForKey("color");
    node->setColor(color ? color3FromString(color) : NCL_RGBANODE_DEFAULT_COLOR);
}

void NCLNodeFactory::loadLabelTTF(CCLabelTTF *node, CCDictionary *dictionary, float scale, NCLWidget *manager,
                                  cocos2d::CCSize parentSize)
{
    loadContentSize(node, dictionary, scale, parentSize);
    node->setDimensions(node->getContentSize());

    CCString *hAlign = (CCString *)dictionary->objectForKey("hAlignment");
    node->setHorizontalAlignment(hAlign ? horizontalAlignFromString(hAlign) : NCL_LABELTTF_DEFAULT_H_ALIGN);

    CCString *vAlign = (CCString *)dictionary->objectForKey("vAlignment");
    node->setVerticalAlignment(vAlign ? verticalAlignFromString(vAlign) : NCL_LABELTTF_DEFAULT_V_ALIGN);

    CCString *fontName = (CCString *)dictionary->objectForKey("fontName");
    node->setFontName(fontName ? fontName->getCString() : NCL_LABELTTF_DEFAULT_FONT_NAME);

    CCString *fontSize = (CCString *)dictionary->objectForKey("fontSize");
    node->setFontSize(scale * (fontSize ? floatFromString(fontSize) : NCL_LABELTTF_DEFAULT_FONT_SIZE));

    CCString *content = (CCString *)dictionary->objectForKey("content");
    std::string contentStr = content ? content->getCString() : "";
    normalizeNewline(contentStr);
    node->setString(NCLLocalizer::localizedString(contentStr).c_str());

    CCString *shadow = (CCString *)dictionary->objectForKey("shadowEnabled");
    if (shadow) {
        CCString *shadowOffset = (CCString *)dictionary->objectForKey("shadowOffset");
        CCSize offset = shadowOffset ? sizeFromString(shadowOffset) : CCSizeMake(0, 0);

        CCString *shadowBlur = (CCString *)dictionary->objectForKey("shadowBlur");
        float blur = shadowBlur ? floatFromString(shadowBlur) : 0.0f;

        CCString *shadowOpacity = (CCString *)dictionary->objectForKey("shadowOpacity");
        float opacity = shadowOpacity ? floatFromString(shadowOpacity) : 255.0;

        CCSize oldDim = node->getDimensions();
        node->setDimensions(CCSize(oldDim.width, 0));

        node->enableShadow(offset, opacity, blur);

        CCSize newDim = node->getContentSize();
        if (newDim.width > oldDim.width)
            oldDim.width = newDim.width;
        if (newDim.height > oldDim.height) {
            node->setPositionY(node->getPositionY() - (newDim.height - oldDim.height));
            oldDim.height = newDim.height;
        }
        node->setDimensions(oldDim);
    }

    /*CCString* strokeSize = (CCString*)dictionary->objectForKey("strokeWidth");
     float stroke = strokeSize ? strokeSize->doubleValue() : 0;
     if( stroke > 0 )
     {
        node->enableStroke(ccRED, stroke, true);
     }
     */
}

void NCLNodeFactory::loadLabelBMF(cocos2d::CCLabelBMFont *node, cocos2d::CCDictionary *dictionary, float scale,
                                  NCLWidget *manager)
{

    CCString *width = (CCString *)dictionary->objectForKey("width");
    node->setWidth(scale * (width ? floatFromString(width) : NCL_LABELBM_DEFAULT_WIDTH));

    CCString *align = (CCString *)dictionary->objectForKey("alignment");
    node->setAlignment(align ? (CCTextAlignment)intFromString(align) : NCL_LABELBM_DEFAULT_ALIGN);

    CCString *font = (CCString *)dictionary->objectForKey("font");
    node->setFntFile(font ? font->getCString() : NCL_LABELBM_DEFAULT_FONT);

    CCString *content = (CCString *)dictionary->objectForKey("content");
    std::string contentStr = content ? content->getCString() : "";
    node->setString(NCLLocalizer::localizedString(contentStr).c_str());
}

void NCLNodeFactory::loadProgressTimer(cocos2d::CCProgressTimer *node, cocos2d::CCDictionary *dictionary)
{

    CCString *image = (CCString *)dictionary->objectForKey("image");
    if (NULL != image) {
        CCSprite *imageSprite = CCSprite::create(image->getCString());
        node->setSprite(imageSprite);
        if (imageSprite) {
            NCLNodeFactory::loadNodeRGBA(imageSprite, dictionary);
        }
    }

    CCString *type = (CCString *)dictionary->objectForKey("type");
    std::string typeString = type->getCString();
    if (typeString.compare("radial") == 0) {
        node->setType(kCCProgressTimerTypeRadial);
    } else {
        node->setType(kCCProgressTimerTypeBar);
    }

    CCString *midPoint = (CCString *)dictionary->objectForKey("midPoint");
    node->setMidpoint(midPoint ? pointFromString(midPoint) : NCL_PROGBAR_DEFAULT_MIDPOINT);

    CCString *changeRate = (CCString *)dictionary->objectForKey("changeRate");
    node->setBarChangeRate(changeRate ? pointFromString(changeRate) : NCL_PROGBAR_DEFAULT_BAR_CHANGE_RATE);

    CCString *percentage = (CCString *)dictionary->objectForKey("percentage");
    node->setPercentage(percentage ? floatFromString(percentage) : NCL_PROGBAR_DEFAULT_PERCENTAGE);

    CCString *reverse = (CCString *)dictionary->objectForKey("reverse");
    node->setReverseProgress(reverse ? boolFromString(reverse) : NCL_PROGBAR_DEFAULT_REVERSE);
}

void NCLNodeFactory::loadParticleSystemQuad(cocos2d::CCParticleSystemQuad *node, cocos2d::CCDictionary *dictionary)
{

    CCString *file = (CCString *)dictionary->objectForKey("file");
    if (file) {
        node->initWithFile(file->getCString());
    }

    CCString *type = (CCString *)dictionary->objectForKey("posType");
    std::string typeString = type->getCString();
    if (typeString.compare("relative") == 0) {
        node->setPositionType(kCCPositionTypeRelative);
    } else if (typeString.compare("grouped") == 0) {
        node->setPositionType(kCCPositionTypeGrouped);
    } else {
        node->setPositionType(kCCPositionTypeFree);
    }
}

void NCLNodeFactory::loadMenu(cocos2d::CCMenu *node, cocos2d::CCDictionary *dictionary)
{

// Since positions of the nodes are set with the align functionality, this is actually unnecessary...
#ifdef ANDROID_COMPATIBILITY
    node->setContentSize(CCSizeZero);
    node->setAnchorPoint(CCPointZero);
    node->ignoreAnchorPointForPosition(false);
#endif
}

void NCLNodeFactory::loadMenuItem(CCMenuItem *node, CCDictionary *dictionary, const NCLLoadingSettings &settings,
                                  float scale, cocos2d::CCSize parentSize)
{

    CCString *enabled = (CCString *)dictionary->objectForKey("enabled");
    node->setEnabled(enabled ? boolFromString(enabled) : NCL_MENU_DEFAULT_ENABLED);

    CCString *selector = (CCString *)dictionary->objectForKey("selector");
    if (selector) {
        NCLSelector nSelector;
        settings.getSelector(selector->getCString(), nSelector);
        nSelector.setupMenuItem(node);
    }

    NCLNodeFactory::loadNodeRGBA(node, dictionary);

#if MENU_ITEM_SCALE_COMPATIBILITY == 0
    CCSize oldSize = node->getContentSize();
    loadContentSize(node, dictionary, scale, parentSize);
    CCSize contentSize = node->getContentSize();
    //	contentSize.width = fmaxf(contentSize.width, oldSize.width);
    //	contentSize.height = fmaxf(contentSize.height, oldSize.height);
    node->setContentSize(contentSize);
#endif
}

void NCLNodeFactory::loadMenuSprite(cocos2d::CCMenuItemSprite *node)
{

    if (!node->getNormalImage() && !node->getSelectedImage() && !node->getDisabledImage()) {
        CCArray *children = node->getChildren();
        int childCount = children->count();

        CCSprite *normalChild = childCount > 0 ? dynamic_cast<CCSprite *>(children->objectAtIndex(0)) : NULL;
        CCSprite *selectedChild = childCount > 1 ? dynamic_cast<CCSprite *>(children->objectAtIndex(1)) : NULL;
        CCSprite *disabledChild = childCount > 2 ? dynamic_cast<CCSprite *>(children->objectAtIndex(2)) : NULL;

        if (normalChild) {
            normalChild->retain();
            node->removeChild(normalChild);
            node->setNormalImage(normalChild);
            normalChild->release();
        }

        if (selectedChild) {
            selectedChild->retain();
            node->removeChild(selectedChild);
            node->setSelectedImage(selectedChild);
            selectedChild->release();
        }

        if (disabledChild) {
            disabledChild->retain();
            node->removeChild(disabledChild);
            node->setDisabledImage(disabledChild);
            disabledChild->release();
        }
    }
}

void NCLNodeFactory::loadMenuImage(cocos2d::CCMenuItemImage *node, cocos2d::CCDictionary *dictionary)
{

    CCString *normalImage = (CCString *)dictionary->objectForKey("normal");
    if (normalImage) {
        node->setNormalImage(CCSprite::create(normalImage->getCString()));
    }

    CCString *selectedImage = (CCString *)dictionary->objectForKey("selected");
    if (selectedImage) {
        node->setSelectedImage(CCSprite::create(selectedImage->getCString()));
    }

    CCString *disabledImage = (CCString *)dictionary->objectForKey("disabled");
    if (disabledImage) {
        node->setDisabledImage(CCSprite::create(disabledImage->getCString()));
    }

#ifdef MENU_ITEMS_BOUNCE
    if (normalImage && strstr(normalImage->getCString(), "button") != nullptr) {
        node->setBounceOnInteract(true);
    }
#endif
}

void NCLNodeFactory::loadMenuImageSpecials(cocos2d::CCMenuItemImage *node, NCLWidget *manager)
{
    CCArray *children = node->getChildren();
    if (children) {
        CCSize oldSize = node->getContentSize(); // save original content size so it can be restored later
        CCNode *toNormal = nullptr;
        CCNode *toSelected = nullptr;
        CCNode *toDisabled = nullptr;
        for (int i = 0; i < children->data->num; i++) {
            CCNode *child = static_cast<CCNode *>(children->data->arr[i]);
            if (manager->isSpecialyNormal(child)) {
                toNormal = child;
            } else if (manager->isSpecialySelected(child)) {
                toSelected = child;
            } else if (manager->isSpecialyDisabled(child)) {
                toDisabled = child;
            }
        }

        if (toNormal) {
            toNormal->retain();
            CCPoint anchor = toNormal->getAnchorPoint();
            node->removeChild(toNormal);
            node->setNormalImage(toNormal); // this messes up items content size
            node->setContentSize(CCSize(toNormal->getContentSize().width * toNormal->getScaleX(), // this fixes it
                                        toNormal->getContentSize().height * toNormal->getScaleY()));
            toNormal->release();
            toNormal->setAnchorPoint(anchor);
        }
        if (toSelected) {
            toSelected->retain();
            CCPoint anchor = toSelected->getAnchorPoint();
            node->removeChild(toSelected);
            node->setSelectedImage(toSelected);
            toSelected->release();
            toSelected->setAnchorPoint(anchor);
        }
        if (toDisabled) {
            toDisabled->retain();
            CCPoint anchor = toDisabled->getAnchorPoint();
            node->removeChild(toDisabled);
            node->setDisabledImage(toDisabled);
            toDisabled->release();
            toDisabled->setAnchorPoint(anchor);
        }
        if (oldSize.width > 0.0f && oldSize.height > 0.0f) { // if the content size was set manually
            node->setContentSize(oldSize);                   // restore it
        } // otherwise leave the content size copied from normal child
    }
}

void NCLNodeFactory::loadMenuLabel(cocos2d::CCMenuItemLabel *node)
{
    if (!node->getLabel() && node->getChildrenCount() > 0) {
        CCLabelTTF *labelChild = dynamic_cast<CCLabelTTF *>(node->getChildren()->objectAtIndex(0));
        if (labelChild) {
            labelChild->retain();
            node->removeChild(labelChild);
            node->setLabel(labelChild);
            labelChild->release();
        }
    }
}

void NCLNodeFactory::loadFlowLayout(CEFlowLayout *node, cocos2d::CCDictionary *dictionary, float scale,
                                    CEFlowLayoutDataSource *dataSource, CEFlowLayoutDelegate *delegate,
                                    cocos2d::CCSize parentSize)
{

    CCString *totalRows = (CCString *)dictionary->objectForKey("totalRows");
    node->setTotalRows(totalRows ? intFromString(totalRows) : 1);

    CCString *totalCols = (CCString *)dictionary->objectForKey("totalColumns");
    node->setTotalColumns(totalCols ? intFromString(totalCols) : 1);

    loadContentSize(node, dictionary, scale, parentSize);

    // Delegate and data source should be set manually

    CCString *pagingTresh = (CCString *)dictionary->objectForKey("pagingTreshold");
    node->setPagingTreshold(ccpMult(pagingTresh ? pointFromString(pagingTresh) : CCPointZero, scale));

    CCString *pageSpeed = (CCString *)dictionary->objectForKey("pageSpeed");
    node->setPageSpeed(pageSpeed ? pointFromString(pageSpeed) : NCL_FLOW_PAGE_SPEED);

    CCString *scrollSens = (CCString *)dictionary->objectForKey("scrollSensitivity");
    node->setScrollSensitivity(scrollSens ? floatFromString(scrollSens) : NCL_FLOW_SCROLL_SENSITIVITY);

    CCString *scrollSmoo = (CCString *)dictionary->objectForKey("scrollSmoothness");
    node->setScrollSensitivity(scrollSmoo ? floatFromString(scrollSmoo) : NCL_FLOW_SCROLL_SMOOTHNESS);

    CCString *autostop = (CCString *)dictionary->objectForKey("autostopDuration");
    CCString *scrollStop = (CCString *)dictionary->objectForKey("scrollStopFactor");
    node->setScrollStopFactor(autostop ? floatFromString(autostop)
                                       : (scrollStop ? floatFromString(scrollStop) : NCL_FLOW_SCROLLSTOP));

    CCString *easeLR = (CCString *)dictionary->objectForKey("easeLeftRight");
    node->setEaseLeftRight(easeLR ? boolFromString(easeLR) : NCL_FLOW_EASING);

    CCString *easeUD = (CCString *)dictionary->objectForKey("easeUpDown");
    node->setEaseUpDown(easeUD ? boolFromString(easeUD) : NCL_FLOW_EASING);

    CCString *allowTouch = (CCString *)dictionary->objectForKey("allowTouch");
    node->setTouchEnabled(allowTouch ? boolFromString(allowTouch) : true);

    CCString *allowMoveLR = (CCString *)dictionary->objectForKey("allowTableMoveLR");
    node->setAllowMoveLR(allowMoveLR ? boolFromString(allowMoveLR) : true);

    CCString *allowMoveUD = (CCString *)dictionary->objectForKey("allowTableMoveUD");
    node->setAllowMoveUD(allowMoveUD ? boolFromString(allowMoveUD) : true);

    CCString *pageSize = (CCString *)dictionary->objectForKey("pageSize");
    node->setPageSize(pageSize ? sizeFromString(pageSize) : node->getContentSize());

    CCString *touchRadius = (CCString *)dictionary->objectForKey("touchTresh");
    node->setToucRadiusTreshold(touchRadius ? floatFromString(touchRadius) : NCL_FLOW_TOUCH_RADIUS);

    CCString *swipe = (CCString *)dictionary->objectForKey("swipeAction");
    node->setSwipeAction((swipe && swipe->compare("page")) ? kSwipePaging : kSwipeScrolling);

    if (dataSource) {
        node->setDataSource(dataSource);
    }
    if (delegate) {
        node->setDelegate(delegate);
    }
}

CCNode *NCLNodeFactory::createFromDictionary(CCDictionary *dictionary, NCLWidget *owner, cocos2d::CCNode *parent,
                                             bool putIntoAddressBook, float parentCSFactor)
{
    // Create appropriate node, depending on class string and fill its values
    // Use new or Create class method?

    const NCLLoadingSettings &settings = owner->getSettings();

    float scale = settings.deviceScale;
    float fontScale = settings.fontScale;
    bool lookForSpriteFrames = settings.lookForSpriteFrames;
    CCSize parentSize;
    if (parent) {
        parentSize = parent->getContentSize() * parentCSFactor;
    } else if (!settings.parentSize.equals(cocos2d::CCSizeZero)) {
        parentSize = settings.parentSize;
    } else {
        parentSize = NCLDeviceNodeManager::getDeviceSize();
    }

    CCNode *node = NULL;
    CCString *className = (CCString *)dictionary->objectForKey("class");

    if (className->compare("Sprite") == 0) {
        // CCSprite *spriteNode = CCSprite::create();
        CCSprite *spriteNode = new CCSprite();
        spriteNode->init();
        NCLNodeFactory::loadSprite(spriteNode, dictionary, scale / settings.imageScale, settings.imageScale,
                                   lookForSpriteFrames, parentSize);
        NCLNodeFactory::loadNodeRGBA(spriteNode, dictionary);
        node = spriteNode;
    } else if (className->compare("LabelTTF") == 0) {
        // CCLabelTTF *labelTTFNode = CCLabelTTF::create();
        CCLabelTTF *labelTTFNode = new CCLabelTTF();
        labelTTFNode->init();
        NCLNodeFactory::loadLabelTTF(labelTTFNode, dictionary, fontScale, owner, parentSize);
        NCLNodeFactory::loadNodeRGBA(labelTTFNode, dictionary);
        node = labelTTFNode;
    } else if (className->compare("LabelBMFont") == 0) {
        // CCLabelBMFont *labelBMFont = CCLabelBMFont::create();
        CCLabelBMFont *labelBMFont = new CCLabelBMFont();
        labelBMFont->init();
        NCLNodeFactory::loadNodeRGBA(labelBMFont, dictionary);
        NCLNodeFactory::loadLabelBMF(labelBMFont, dictionary, scale, owner);
        node = labelBMFont;
    } else if (className->compare("ProgressTimer") == 0) {
        CCProgressTimer *progTimer = new CCProgressTimer(); // CCProgressTimer::create(NULL);
        progTimer->initWithSprite(NULL);
        NCLNodeFactory::loadProgressTimer(progTimer, dictionary);
        NCLNodeFactory::loadNodeRGBA(progTimer, dictionary);
        node = progTimer;
    } else if (className->compare("ParticleSystem") == 0) {
        CCParticleSystemQuad *particleNode = new CCParticleSystemQuad(); // CCParticleSystemQuad::create();
        particleNode->init();
        NCLNodeFactory::loadParticleSystemQuad(particleNode, dictionary);
        node = particleNode;
    } else if (className->compare("Menu") == 0) {
        CCMenu *menu = new CCMenu(); // CCMenu::create();
        menu->init();
        NCLNodeFactory::loadMenu(menu, dictionary);
        node = menu;
    } else if (className->compare("MenuItem") == 0) {
        CCMenuItem *menuItem = new CCMenuItem();
        menuItem->init();
        // menuItem->autorelease();
        NCLNodeFactory::loadMenuItem(menuItem, dictionary, settings, scale, parentSize);
        owner->addMenuItem(menuItem);
        node = menuItem;
    } else if (className->compare("MenuItemImage") == 0) {
        CCMenuItemImage *menuImage = new CCMenuItemImage();
        menuImage->init();
        // menuImage->autorelease();
        NCLNodeFactory::loadMenuImage(menuImage, dictionary);
        NCLNodeFactory::loadMenuItem(menuImage, dictionary, settings, scale, parentSize);
        owner->addMenuItem(menuImage);
        node = menuImage;
    } else if (className->compare("MenuItemSprite") == 0) {
        CCMenuItemSprite *menuSprite = new CCMenuItemSprite();
        menuSprite->init();
        // menuSprite->autorelease();
        NCLNodeFactory::loadMenuItem(menuSprite, dictionary, settings, scale, parentSize);
        owner->addMenuItem(menuSprite);
        node = menuSprite;
    } else if (className->compare("MenuItemLabel") == 0) {
        CCMenuItemLabel *menuLabel = new CCMenuItemLabel();
        menuLabel->initWithLabel(NULL, NULL, NULL);
        owner->addMenuItem(menuLabel);
        // menuLabel->autorelease();
        node = menuLabel;
    } else if (className->compare("CEFlowLayout") == 0) {
        CEFlowLayout *flowLayout = new CEFlowLayout();
        flowLayout->init(CCSize(0, 0), NULL, NULL);
        // flowLayout->autorelease();
        NCLNodeFactory::loadFlowLayout(flowLayout, dictionary, scale, settings.flowDataSource, settings.flowDelegate,
                                       parentSize);
        owner->addFlowLayout(flowLayout);
        node = flowLayout;
    } else if (className->compare("CEMenuItemCycle") == 0) {
        CEMenuItemCycle *cycleItem = new CEMenuItemCycle();
        cycleItem->init();
        NCLNodeFactory::loadMenuItem(cycleItem, dictionary, settings, scale, parentSize);
        owner->addMenuItem(cycleItem);
        node = cycleItem;
    } else if (className->compare("NodeRGBA") == 0) {
        CCNodeRGBA *rgbaItem = new CCNodeRGBA();
        rgbaItem->init();
        NCLNodeFactory::loadNodeRGBA(rgbaItem, dictionary);
        node = rgbaItem;
    } else if (className->compare("CESprite9Slice") == 0) {
        CCScale9Sprite *scale9Item = new CCScale9Sprite();
        scale9Item->init();
        NCLNodeFactory::loadSprite9(scale9Item, dictionary, scale / settings.imageScale, settings.imageScale,
                                    settings.lookForSpriteFrames, parentSize);
        NCLNodeFactory::loadNodeRGBA(scale9Item, dictionary);
        node = scale9Item;
    } else if (className->compare("CEWidget") == 0) {
        NCLWidgetNode *widget = new NCLWidgetNode();
        // widget->retain();
        widget->init();
        owner->addWidgetNode(widget);
        node = widget;
    }

    if (NULL == node) {
        node = new CCNodeRGBA();
        node->init();
    }
    NCLNodeFactory::loadNode(node, dictionary, scale, parentSize);

#ifdef MENU_ITEMS_BOUNCE
    // Force bounce after load
    CCMenuItem *mItem = nullptr;
    if ((mItem = dynamic_cast<CCMenuItem *>(node))) {
        if (mItem->getBounceOnInteract())
            mItem->setBounceOnInteract(true);
    }
#endif

    if (className->compare("CEWidget") == 0) {
        NCLWidgetNode *widget = static_cast<NCLWidgetNode *>(node);
        NCLLoadingSettings newSettings = settings;
        newSettings.suppressDeviceScale = true;
        widget->initWithDictionary(
            NCLWidgetCache::get()->getWidgetDictionary(((CCString *)dictionary->objectForKey("wName"))->getCString()),
            newSettings);
        widget->cocosizeInto(widget);
    }

    // Fill owners address books
    if (NULL != owner && true == putIntoAddressBook) {
        CCString *name = (CCString *)dictionary->objectForKey("name");
        if (NULL != name) {
            owner->addToAddressBook(node, name->getCString());
            owner->addToCloneBook(dictionary, name->getCString());
        }
    }

    float nodesCSFactor = 1.0;
    if ((className->compare("ProgressTimer") == 0 || className->compare("Sprite") == 0 ||
         className->compare("CESprite9Slice") == 0
         //|| className->compare("MenuItemImage") == 0 || className->compare("MenuItemSprite") == 0
         ) &&
        settings.imageScale != 1.0) {
        nodesCSFactor = settings.imageScale;
    }

    // Generate children
    CCArray *children = (CCArray *)dictionary->objectForKey("children");
    if (NULL != children) {
        int length = children->count();
        for (int i = 0; i < length; i++) {
            CCDictionary *childDictionary = (CCDictionary *)children->objectAtIndex(i);
            if (NULL != childDictionary) {
                CCNode *child = NCLNodeFactory::createFromDictionary(childDictionary, owner, node, putIntoAddressBook,
                                                                     nodesCSFactor);
                if (NULL != child) {
                    node->addChild(child);
                    child->release();
                }
            }
        }
        if (className->compare("MenuItemSprite") == 0) {
            CCMenuItemSprite *menuItemSprite = dynamic_cast<CCMenuItemSprite *>(node);
            if (menuItemSprite) {
                loadMenuSprite(menuItemSprite);
            }
        } else if (className->compare("MenuItemLabel") == 0) {
            CCMenuItemLabel *menuItemLabel = dynamic_cast<CCMenuItemLabel *>(node);
            if (menuItemLabel) {
                loadMenuLabel(menuItemLabel);
                loadMenuItem(menuItemLabel, dictionary, settings, scale, parentSize);
            }
        } else if (className->compare("MenuItemImage") == 0) {
            CCMenuItemImage *menuItemImage = static_cast<CCMenuItemImage *>(node);
            loadMenuImageSpecials(menuItemImage, owner);
        }
    }

    float imageScale = settings.imageScale;
    if (imageScale != 1.0) {
        if (className->compare("ProgressTimer") == 0) {
            onlyParentScaleMultiply(node, imageScale);
        }
        if (className->compare("Sprite") == 0) {
            // CCTexture2D* texture = static_cast<CCSprite*>(node)->getTexture();
            // if( texture->getContentSize().equals(node->getContentSize()) )
            onlyParentScaleMultiply(node, imageScale);
        }
        if (className->compare("CESprite9Slice") == 0) {
            onlyParentScaleMultiply(node, imageScale, true);
        }

        if (className->compare("MenuItemImage") == 0 || className->compare("MenuItemSprite") == 0) {
            CCMenuItemSprite *menuItemSprite = (CCMenuItemSprite *)node;
#if MENU_ITEM_SCALE_COMPATIBILITY == 0
            CCNode *normal = menuItemSprite->getNormalImage();
            CCNode *selected = menuItemSprite->getSelectedImage();
            CCNode *disabled = menuItemSprite->getDisabledImage();
            if (normal && !owner->isSpecialyNormal(normal)) {
                normal->setScale(normal->getScale() * imageScale);
            }
            if (selected && !owner->isSpecialySelected(selected)) {
                selected->setScale(selected->getScale() * imageScale);
            }
            if (disabled && !owner->isSpecialyDisabled(disabled)) {
                disabled->setScale(disabled->getScale() * imageScale);
            }
#else
            menuItemSprite->setScaleX(menuItemSprite->getScaleX() * imageScale);
            menuItemSprite->setScaleY(menuItemSprite->getScaleY() * imageScale);
            CCArray *children = menuItemSprite->getChildren();
            CCObject *child;
            CCARRAY_FOREACH(children, child)
            {
                if (menuItemSprite->getNormalImage() != child && menuItemSprite->getSelectedImage() != child &&
                    menuItemSprite->getDisabledImage() != child) {
                    CCNode *childNode = (CCNode *)child;
                    childNode->setScaleX(childNode->getScaleX() / imageScale);
                    childNode->setScaleY(childNode->getScaleY() / imageScale);
                    CCPoint position = childNode->getPosition();
                    position.x /= imageScale;
                    position.y /= imageScale;
                    childNode->setPosition(position);
                }
            }
#endif
        }
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (className->compare("LabelTTF") == 0) {
        node->setPositionY(node->getPositionY() - 2 * NCLDeviceInfo::getDeviceScale());
    }
#endif

    return node;
}

void onlyParentScaleMultiply(CCNode *node, float factor, bool skipFirst)
{
    node->setScaleX(node->getScaleX() * factor);
    node->setScaleY(node->getScaleY() * factor);
    float childFactor = 1 / factor;
    CCArray *children = node->getChildren();
    CCObject *child;
    CCNode *childNode;
    CCARRAY_FOREACH(children, child)
    {
        if (skipFirst) {
            skipFirst = false;
            continue;
        }
        childNode = (CCNode *)child;
        childNode->setScaleX(childNode->getScaleX() * childFactor);
        childNode->setScaleY(childNode->getScaleY() * childFactor);
        CCPoint position = childNode->getPosition();
        position.x *= childFactor;
        position.y *= childFactor;
        childNode->setPosition(position);
    }
}
