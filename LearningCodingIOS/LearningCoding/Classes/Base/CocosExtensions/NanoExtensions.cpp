//
//  NanoExtensions.cpp
//  NanoGirl
//
//  Created by Luka Piljek on 7/15/13.
//
//

#include "NanoExtensions.h"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctype.h>
#include "stdio.h"
#include <stdlib.h>
using namespace std;

//#include "GameState.h"
//#include "HudLayer.h"
#include "CCControlUtils.h"
#include "CCJSONConverter.h"
//#include "Constants.h"
//#include "HudHeader.h"
//#include "SoundManager.h"
//#include "GameState.h"
//#include "GameProfile.h"
//#include "InAppProfile.h"
//#include "TicketController.h"
//#include "InAppPurchaseManager.h"

using namespace cocos2d::extension;

// modify in CCNative_objc.mm to prevent freeze when multiple alerts come up :

/*- (void)createAlertView:(NSString *)title
andMessage:(NSString *)message
andCancelButtonTitle:(NSString *)cancelButtonTitle
{
    if (alertView_)
    {
        // NanoExtensions : cancel visible alert view
        // CCLOG("[CCNative_objc] ERR, createAlertView() alert view already
exists");
        [self cancelAlertView];
        //return;
    }

    CCLOG("[CCNative_objc] createAlertView() title: %s, message: %s,
cancelButtonTitle: %s",
          utf8cstr(title), utf8cstr(message), utf8cstr(cancelButtonTitle));
    alertView_ = [[UIAlertView alloc] initWithTitle:title
                                            message:message
                                           delegate:self
                                  cancelButtonTitle:cancelButtonTitle
                                  otherButtonTitles:nil];
}*/

#pragma mark - CCDirector

void CCDirector::drawScene(void)
{
    // calculate "global" dt
    calculateDeltaTime();

    // tick before glClear: issue #533
    if (!m_bPaused) {
        m_pScheduler->update(m_fDeltaTime);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* to avoid flickr, nextScene MUST be here: after tick and before draw.
     XXX: Which bug is this one. It seems that it can't be reproduced with v0.9 */
    if (m_pNextScene) {
        setNextScene();
    }

    kmGLPushMatrix();

    // draw the scene
    if (m_pRunningScene) {
        // NanoExtensions - added for touches propagation
        CCNode::absoluteZCounter = 0;

        m_pRunningScene->visit();

        CCNode::absoluteZDirty = false;
    }

    // draw the notifications node
    if (m_pNotificationNode) {
        m_pNotificationNode->visit();
    }

    if (m_bDisplayStats) {
        showStats();
    }

    kmGLPopMatrix();

    m_uTotalFrames++;

    // swap buffers
    if (m_pobOpenGLView) {
        m_pobOpenGLView->swapBuffers();
    }

    if (m_bDisplayStats) {
        calculateMPF();
    }
}

void CCDirector::runWithScene(CCScene *pScene)
{
    CCAssert(pScene != NULL, "This command can only be used to start the "
                             "CCDirector. There is already a scene present.");
    CCAssert(m_pRunningScene == NULL, "m_pRunningScene should be null");

    // fix splash screen transition
    //    kmGLPushMatrix();
    //    pScene->visit();
    //    kmGLPopMatrix();
    //    if (m_pobOpenGLView)
    //    {
    //        m_pobOpenGLView->swapBuffers();
    //    }

    pushScene(pScene);
    startAnimation();
}

#pragma mark - CCNode

int CCNode::absoluteZCounter = 0;
bool CCNode::absoluteZDirty = true;

void CCNode::insertChild(CCNode *child, int z)
{
    m_bReorderChildDirty = true;
    absoluteZDirty = true;
    ccArrayAppendObjectWithResize(m_pChildren->data, child);
    child->_setZOrder(z);
}

void CCNode::detachChild(CCNode *child, bool doCleanup)
{
    // IMPORTANT:
    //  -1st do onExit
    //  -2nd cleanup
    if (m_bRunning) {
        child->onExitTransitionDidStart();
        child->onExit();
    }

    // If you don't do cleanup, the child's actions will not get removed and the
    // its scheduledSelectors_ dict will not get released!
    if (doCleanup) {
        child->cleanup();
    }

    // set parent nil at the end
    child->setParent(NULL);

    m_pChildren->removeObject(child);

    absoluteZDirty = true;
}

void CCNode::beforeDraw() {}

void CCNode::afterDraw() {}

void CCNode::visit()
{
    // quick return if not visible. children won't be drawn.
    if (!m_bVisible) {
        return;
    }
    kmGLPushMatrix();

    if (m_pGrid && m_pGrid->isActive()) {
        m_pGrid->beforeDraw();
    }

    this->transform();
    this->beforeDraw();

    CCNode *pNode = NULL;
    unsigned int i = 0;

    if (m_pChildren && m_pChildren->count() > 0) {
        sortAllChildren();
        // draw children zOrder < 0
        ccArray *arrayData = m_pChildren->data;
        for (; i < arrayData->num; i++) {
            pNode = (CCNode *)arrayData->arr[i];

            if (pNode && pNode->m_nZOrder < 0) {
                pNode->visit();
            } else {
                break;
            }
        }
        // self draw
        this->draw();

        // NanoExtensions - added for touches propagation
        if (absoluteZDirty) {
            if (CCLayer *layer = dynamic_cast<CCLayer *>(this)) {
                layer->setTouchPriority(-absoluteZCounter);
                absoluteZCounter++;
            }
        }
        //

        for (; i < arrayData->num; i++) {
            pNode = (CCNode *)arrayData->arr[i];
            if (pNode) {
                pNode->visit();
            }
        }
    } else {
        this->draw();
    }

    this->afterDraw();
    // reset for next frame
    m_uOrderOfArrival = 0;

    if (m_pGrid && m_pGrid->isActive()) {
        m_pGrid->afterDraw(this);
    }

    kmGLPopMatrix();
}

void CCNode::setVisible(bool var)
{
    m_bVisible = var;
    absoluteZDirty = true;
}

bool CCNode::isPointInRect(const cocos2d::CCPoint &point)
{
    CCPoint local = convertToNodeSpace(point);
    CCRect r = CCRectMake(0.0f, 0.0f, m_obContentSize.width, m_obContentSize.height);

    if (r.containsPoint(local))
        return true;

    return false;
}

bool CCNode::isBeingTouched(const cocos2d::CCTouch *touch) { return isPointInRect(touch->getLocation()); }

void CCNode::loadFromDict(CCDictionary *dict, float scaleFactor)
{
    float x = dict->floatForKey("x", m_obPosition.x), y = dict->floatForKey("y", m_obPosition.y); // LEGACY
    CCNode::setPosition(dict->floatForKey("positionX", x) * scaleFactor,
                        dict->floatForKey("positionY", y) * scaleFactor);
    setScaleX(dict->floatForKey("scaleX", m_fScaleX));
    setScaleY(dict->floatForKey("scaleY", m_fScaleY));
    setSkewX(dict->floatForKey("skewX", m_fSkewX));
    setSkewY(dict->floatForKey("skewY", m_fSkewY));
    setContentSize(CCSize(dict->floatForKey("contSizeX", m_obContentSize.width) * scaleFactor,
                          dict->floatForKey("contSizeY", m_obContentSize.height) * scaleFactor));
    setAnchorPoint(CCPoint(dict->floatForKey("anchorX", 0.0f), dict->floatForKey("anchorY", 0.0f)));
    setRotation(dict->floatForKey("rotation", m_fRotationX));
    setZOrder(dict->intForKey("zOrder", m_nZOrder));
    setVertexZ(dict->floatForKey("vertexZ", m_fVertexZ));
}

void CCNode::removeSoundInUserDictionary()
{
    if (getUserDictionary()) {
        getUserDictionary()->removeObjectForKey("soundName");
    }
}

void CCNode::setSoundInUserDictionary(std::string soundName)
{
    if (!soundName.empty()) {
        if (getUserDictionary()) {
            getUserDictionary()->setObject(CCString::create(soundName), "soundName");
        }
    } else {
        removeSoundInUserDictionary();
    }
}

CCDictionary *CCNode::getUserDictionary()
{
    if (!m_pUserObject) {
        m_pUserObject = CCDictionary::create();
        m_pUserObject->retain();
    }

    return dynamic_cast<CCDictionary *>(m_pUserObject);
}

CCPoint CCNode::convertToOtherNodeSpace(CCNode *othenNode, CCPoint nodePoint)
{
    CCPoint worldPoint = convertToWorldSpace(nodePoint);
    return othenNode->convertToNodeSpace(worldPoint);
}

CCPoint CCNode::getWorldPosition() { return convertToWorldSpace(m_obAnchorPointInPoints); }

CCPoint CCNode::getWorldScale()
{
    float worldScaleX = m_fScaleX;
    float worldScaleY = m_fScaleY;
    CCNode *aParent = m_pParent;

    while (aParent) {
        worldScaleX *= aParent->m_fScaleX;
        worldScaleY *= aParent->m_fScaleY;
        aParent = aParent->getParent();
    }

    return ccp(worldScaleX, worldScaleY);
}

CCRect CCNode::getScreenSpaceBB()
{
    CCRect rect = CCRectMake(0, 0, m_obContentSize.width, m_obContentSize.height);
    return CCRectApplyAffineTransform(rect, nodeToWorldTransform());
}

CCRect CCNode::enclosingBox()
{
    CCRect enclosingRect = this->boundingBox();
    auto t = this->nodeToParentTransform();
    CCARRAY_FOREACH_TYPE(this->getChildren(), CCNode, child)
    {
        if (child->isVisible()) {
            CCRect rect = child->enclosingBox();
            auto bl = CCPointApplyAffineTransform(rect.origin, t);
            auto tr = CCPointApplyAffineTransform(rect.origin + rect.size, t);
            rect = CCRect(bl.x, bl.y, (tr - bl).x, (tr - bl).y);
            enclosingRect = CCControlUtils::CCRectUnion(enclosingRect, rect);
        }
    }
    return enclosingRect;
}

void CCNode::scaleToSize(CCSize size)
{
    setScale(size.width / m_obContentSize.width, size.height / m_obContentSize.height);
}

CCRenderTexture *CCNode::renderToTextureTransform(const CCSize &renderSize, const CCPoint &offset, float scale,
                                                  float rotation)
{
    CCPoint oldPos = m_obPosition;
    CCPoint oldAnchor = m_obAnchorPoint;
    CCPoint oldScale = CCPoint(m_fScaleX, m_fScaleY);
    CCPoint oldRotation = CCPoint(m_fRotationX, m_fRotationY);
    CCPoint oldSkew = CCPoint(m_fSkewX, m_fSkewY);

    setPosition(offset);
    setAnchorPoint(CCPointZero);
    setScaleX(scale);
    setScaleY(scale);
    setRotationX(rotation);
    setRotationY(0.0f);
    setSkewX(0.0f);
    setSkewY(0.0f);

    CCNode *tempParent = m_pParent;
    m_pParent = nullptr;

    CCRenderTexture *rt = CCRenderTexture::create(renderSize.width, renderSize.height);
    rt->beginWithClear(0, 0, 0, 0);
    visit();
    rt->end();

    m_pParent = tempParent;
    setPosition(oldPos);
    setAnchorPoint(oldAnchor);
    setScaleX(oldScale.x);
    setScaleY(oldScale.y);
    setRotationX(oldRotation.x);
    setRotationY(oldRotation.y);
    setSkewX(oldSkew.x);
    setSkewY(oldSkew.y);

    return rt;
}

CCRenderTexture *CCNode::renderToTexture(const CCPoint &renderOrigin, const CCSize &renderSize,
                                         const ccColor4F &backgroundColor)
{
    CCPoint oldPos = m_obPosition;
    setPosition(oldPos - renderOrigin);
    CCNode *tempParent = m_pParent;
    m_pParent = nullptr;

    CCRenderTexture *rt = CCRenderTexture::create(renderSize.width, renderSize.height, kTexture2DPixelFormat_RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);
    rt->beginWithClear(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    visit();
    rt->end();

    m_pParent = tempParent;
    setPosition(oldPos);

    return rt;
}

CCRenderTexture *CCNode::renderToTextureAutoFit(const ccColor4F &backgroundColor, int leftBorder, int rightBorder,
                                                int upBorder, int downBorder)
{
    CCRect bb = boundingBox();
    bb.origin = ccp(bb.origin.x - leftBorder, bb.origin.y - downBorder);
    bb.size = ccp(bb.size.width + rightBorder, bb.size.height + upBorder);
    return renderToTexture(bb.origin, bb.size, backgroundColor);
}

CCRenderTexture *CCNode::renderToTextureAutoFitToSize(const CCSize &renderSize, const ccColor4F &backgroundColor,
                                                      int leftBorder, int rightBorder, int upBorder, int downBorder)
{
    CCSize sizeWithoutPadding = renderSize - CCSize(leftBorder + rightBorder, downBorder + upBorder);
    CCRect bb = boundingBox();

    CCPoint oldScale = CCPoint(m_fScaleX, m_fScaleY);

    setScaleX(getScaleX() * sizeWithoutPadding.width / bb.size.width);
    setScaleY(getScaleY() * sizeWithoutPadding.height / bb.size.height);

    CCRenderTexture *rt = renderToTextureAutoFit(backgroundColor, leftBorder, rightBorder, upBorder, downBorder);

    setScaleX(oldScale.x);
    setScaleY(oldScale.y);

    return rt;
}

ccColor4B CCNode::getColorAtPoint(const cocos2d::CCPoint &touchLocation)
{
    ccColor4B color;
    CCPoint point = convertToNodeSpace(touchLocation);

    CCPoint oldPos = m_obPosition;
    CCPoint oldAnchor = m_obAnchorPoint;
    CCPoint oldScale = CCPoint(m_fScaleX, m_fScaleY);
    CCPoint oldRotation = CCPoint(m_fRotationX, m_fRotationY);
    CCPoint oldSkew = CCPoint(m_fSkewX, m_fSkewY);

    setPosition(CCPoint(-point.x, -point.y));
    setAnchorPoint(CCPointZero);
    setScaleX(1.0f);
    setScaleY(1.0f);
    setRotationX(0.0f);
    setRotationY(0.0f);
    setSkewX(0.0f);
    setSkewY(0.0f);

    CCRenderTexture *rt = CCRenderTexture::create(32, 32);
    rt->beginWithClear(0, 0, 0, 0);
    visit();
    glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &color);
    rt->end();

    setPosition(oldPos);
    setAnchorPoint(oldAnchor);
    setScaleX(oldScale.x);
    setScaleY(oldScale.y);
    setRotationX(oldRotation.x);
    setRotationY(oldRotation.y);
    setSkewX(oldSkew.x);
    setSkewY(oldSkew.y);

    return color;
}

bool CCNode::isPointNonTransparent(const cocos2d::CCPoint &touchLocation)
{
    return getColorAtPoint(touchLocation).a != 0;
}

// lambda scheduleri
// propagacija toucheva
// pomoćne metode za skaliranje noda i za dodavanje djece na relativnu poziciju

#pragma mark - CCNodeRGBA

void CCNodeRGBA::loadFromDict(CCDictionary *dict, float scaleFactor)
{
    CCNode::loadFromDict(dict, scaleFactor);
    // setCascadeColorEnabled(true);
    // setCascadeOpacityEnabled(true);

    setColor(ccc3(dict->intForKey("colorR", 255), dict->intForKey("colorG", 255), dict->intForKey("colorB", 255)));
    setOpacity(dict->intForKey("opacity", 255));
}

#pragma mark - CCSprite

CCSprite *CCSprite::createWithSpriteFrameOrFile(const char *pszSpriteFrame)
{
    CCSprite *pobSprite = new CCSprite();
    if (pobSprite && pobSprite->initWithSpriteFrameOrFile(pszSpriteFrame)) {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

bool CCSprite::initWithSpriteFrameOrFile(const char *pszFrameName)
{
    CCAssert(pszFrameName != NULL, "");

    if (CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszFrameName)) {
        return initWithSpriteFrame(pFrame);
    } else {
        return initWithFile(pszFrameName);
    }
}

CCSprite *CCSprite::createWithSizeAndColor(CCSize size, ccColor3B color, GLubyte opacity)
{
    CCSprite *pobSprite = new CCSprite();
    if (pobSprite && pobSprite->initWithSizeAndColor(size, color, opacity)) {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

bool CCSprite::initWithSizeAndColor(CCSize size, ccColor3B color, GLubyte opacity)
{
    if (initWithTexture(NULL, CCRect(0.0f, 0.0f, size.width, size.height))) {
        setColor(color);
        setOpacity(opacity);
        return true;
    } else {
        return false;
    }
}

void CCSprite::loadFromDict(CCDictionary *dict, float scaleFactor)
{
    CCNodeRGBA::loadFromDict(dict, scaleFactor);

    //    if (CCString *imgName = (CCString *)dict->objectForKey("fileName")) {
    //        CCTexture2D *pTexture =
    // CCTextureCache::sharedTextureCache()->addImage(imgName->getCString());
    //        setTexture(pTexture);
    //    }

    setFlipX(dict->intForKey("flipX") > 0);
    setFlipY(dict->intForKey("flipY") > 0);
}

bool CCSprite::isBeingTouchedNonTransparent(const cocos2d::CCTouch *touch)
{
    CCPoint touchLocation = touch->getLocation();

    if (isPointInRect(touchLocation) && isPointNonTransparent(touchLocation))
        return true;

    return false;
}
// color sprite
// pamtiti ime texture?
// shader sprite?

#pragma mark - CCMenuItem

void CCMenuItem::activate()
{
    if (m_bEnabled) {
        if (m_pListener && m_pfnSelector) {
            m_pfnSelector(m_pListener, this);
            //CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationCCMenuItemActivated, this);
            CCDictionary *userDict = getUserDictionary();
            //std::string soundName = kEffectClick;
            if (userDict) {
                if (auto soudCCString = dynamic_cast<CCString *>(userDict->objectForKey("soundName"))) {
                    //soundName = soudCCString->getCString();
                }
            }
            //SoundManager::get()->playEffect(soundName.c_str());
        }

        if (kScriptTypeNone != m_eScriptType) {
            CCScriptEngineManager::sharedManager()->getScriptEngine()->executeMenuItemEvent(this);
        }
    }
}
// tutorial enabled
// touch area (content, texture rect, non-transparent image)
// re-enable button with delay
// bounce?

#pragma mark - CCMenuItemSprite

void CCMenuItemSprite::loadFromDict(CCDictionary *dict, float scaleFactor)
{
    CCNodeRGBA::loadFromDict(dict, scaleFactor);

    //    if (CCString *imgName = (CCString *)dict->objectForKey("fileName")) {
    //        CCSprite *imageSprite =CCSprite::create(imgName->getCString());
    //        imageSprite->setFlipX(dict->intForKey("flipX") > 0);
    //        imageSprite->setFlipY(dict->intForKey("flipY") > 0);
    //        setNormalImage(imageSprite);
    //    }
    //    if (CCString *imgName = (CCString *)dict->objectForKey("selectedImage"))
    //{
    //        CCSprite *imageSprite =CCSprite::create(imgName->getCString());
    //        imageSprite->setFlipX(dict->intForKey("flipX") > 0);
    //        imageSprite->setFlipY(dict->intForKey("flipY") > 0);
    //        setNormalImage(imageSprite);
    //    }
    //    if (CCString *imgName = (CCString *)dict->objectForKey("disabledImage"))
    //{
    //        CCSprite *imageSprite =CCSprite::create(imgName->getCString());
    //        imageSprite->setFlipX(dict->intForKey("flipX") > 0);
    //        imageSprite->setFlipY(dict->intForKey("flipY") > 0);
    //        setNormalImage(imageSprite);
    //    }
}

bool CCMenuItemSprite::isBeingTouchedNonTransparent(const cocos2d::CCTouch *touch)
{
    CCPoint touchLocation = touch->getLocation();

    CCNode *activeSprite = nullptr;
    if (!m_bEnabled && m_pDisabledImage != NULL) {
        activeSprite = m_pDisabledImage;
    } else if (m_bSelected && m_pSelectedImage != NULL) {
        activeSprite = m_pSelectedImage;
    } else if (m_pNormalImage != NULL) {
        activeSprite = m_pNormalImage;
    }

    if (activeSprite && isPointInRect(touchLocation) && activeSprite->isPointNonTransparent(touchLocation))
        return true;

    return false;
}

void CCMenuItemSprite::swapNormalAndSelectedImages()
{
    CCNode *temp = m_pNormalImage;
    m_pNormalImage = m_pSelectedImage;
    m_pSelectedImage = temp;
}
// init with sprite frame

void CCMenuItemSprite::swapNormalAndDisabledImages()
{
    CCNode *temp = m_pNormalImage;
    m_pNormalImage = m_pDisabledImage;
    m_pDisabledImage = temp;
}

#pragma mark - CCMenu
// CCMenu now remembers item selected state when moving

bool CCMenu::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    CC_UNUSED_PARAM(event);
    if (m_eState != kCCMenuStateWaiting || !m_bVisible || !m_bEnabled) {
        return false;
    }

    for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent()) {
        if (c->isVisible() == false) {
            return false;
        }
    }

    m_pSelectedItem = this->itemForTouch(touch);
    if (m_pSelectedItem) {
        selectedItemInitialSelected = m_pSelectedItem->isSelected();
        m_eState = kCCMenuStateTrackingTouch;
        m_pSelectedItem->selected();
        return true;
    }
    return false;
}

void CCMenu::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchEnded] -- invalid state");
    if (m_pSelectedItem) {
        if (!selectedItemInitialSelected)
            m_pSelectedItem->unselected();

        m_pSelectedItem->activate();
    }
    m_eState = kCCMenuStateWaiting;
    m_pSelectedItem = NULL;
}

void CCMenu::ccTouchCancelled(CCTouch *touch, CCEvent *event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchCancelled] -- invalid state");
    if (m_pSelectedItem) {
        if (!selectedItemInitialSelected)
            m_pSelectedItem->unselected();
    }
    m_eState = kCCMenuStateWaiting;
    m_pSelectedItem = NULL;
}

void CCMenu::ccTouchMoved(CCTouch *touch, CCEvent *event)
{
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchMoved] -- invalid state");
    CCMenuItem *currentItem = this->itemForTouch(touch);
    if (currentItem != m_pSelectedItem) {
        if (m_pSelectedItem) {
            if (!selectedItemInitialSelected)
                m_pSelectedItem->unselected();
        }
        m_pSelectedItem = currentItem;
        if (m_pSelectedItem) {
            selectedItemInitialSelected = m_pSelectedItem->isSelected();
            m_pSelectedItem->selected();
        }
    }
}

void CCMenu::registerWithTouchDispatcher()
{
    m_eState = kCCMenuStateWaiting;

    if (m_pSelectedItem) {
        if (!selectedItemInitialSelected)
            m_pSelectedItem->unselected();
    }

    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, this->getTouchPriority(), true);
}

void CCMenu::removeAllChildrenWithCleanup(bool cleanup)
{
    m_pSelectedItem = NULL;

    CCNode::removeAllChildrenWithCleanup(cleanup);
}

#pragma mark - CCLabelTTF

bool CCLabelTTF::autoFit = true;

// function decreases label font size until new label (height) fits predefined
// dimensions
void CCLabelTTF::fitLabelToDimensions()
{
    if (m_string.length() == 0)
        return;

    float fontSize = m_fFontSize;
    float targetHeight = m_obContentSize.height;
    const float minFontSize = 5.0f;

    // temporary label used for calculating new label dimensions (height)
    // during this process autoFit is disabled
    bool oldAutoFit = CCLabelTTF::autoFit;
    CCLabelTTF::autoFit = false;
    CCLabelTTF *tempLabel = CCLabelTTF::create(m_string.c_str(), m_pFontName->c_str(), m_fFontSize,
                                               CCSize(m_tDimensions.width, 0), m_hAlignment, m_vAlignment);

    bool shouldChange = false;
    while (tempLabel->getContentSize().height > targetHeight && fontSize > minFontSize) {
        fontSize = tempLabel->getFontSize() - 1;
        tempLabel->setFontSize(fontSize);
        shouldChange = true;
    }

    if (shouldChange)
        setFontSize(fontSize);

    CCLabelTTF::autoFit = oldAutoFit;
}

void CCLabelTTF::setString(const char *string)
{
    CCAssert(string != NULL, "Invalid string");

    if (m_string.compare(string)) {
        m_string = string;

        this->updateTexture();

        if (autoFit)
            fitLabelToDimensions();
    }
}

void CCLabelTTF::setStringWithFormat(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    char *pBuf = (char *)malloc(MAX_STRING_LEN);
    if (pBuf != NULL) {
        vsnprintf(pBuf, MAX_STRING_LEN, format, ap);
        setString(pBuf);
        free(pBuf);
    }

    va_end(ap);
}

void CCLabelTTF::setValue(int number) { setStringWithFormat("%d", number); }

void CCLabelTTF::setValue(float number) { setStringWithFormat("%f", number); }

void CCLabelTTF::setValue(double number) { setStringWithFormat("%f", number); }

void CCLabelTTF::setShortString(std::string text)
{
    string lastString;

    if (!textFitToLabel(text)) {
        for (int i = 0; i < text.size(); i++) {
            if (!isalnum(text[i])) {
                string s = text.substr(0, i) + "...";

                if (textFitToLabel(s)) {
                    lastString = s;
                } else {
                    break;
                }
            }
        }
    } else {
        lastString = text;
    }

    setString(lastString.c_str());
}

bool CCLabelTTF::textFitToLabel(std::string text)
{
    CCLabelTTF *templateLabel = CCLabelTTF::create(text.c_str(), getFontName(), getFontSize(),
                                                   CCSize(getDimensions().width, 0), getHorizontalAlignment());
    return templateLabel->boundingBox().size.height < boundingBox().size.height;
}

#pragma mark - CCLabelBMFont

void CCLabelBMFont::setStringWithFormat(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    char *pBuf = (char *)malloc(MAX_STRING_LEN);
    if (pBuf != NULL) {
        vsnprintf(pBuf, MAX_STRING_LEN, format, ap);
        setString(pBuf);
        free(pBuf);
    }

    va_end(ap);
}

#pragma mark - CCProgresTo

void CCProgressTo::startWithTarget(CCNode *pTarget) // progress always starts from where it was
{
    CCActionInterval::startWithTarget(pTarget);
    m_fFrom = ((CCProgressTimer *)(pTarget))->getPercentage();
}

#pragma mark - CCAnimation

CCAnimation *CCAnimation::create(CCDictionary *dict)
{
    CCAnimation *pAnimation = new CCAnimation();
    pAnimation->initWithDict(dict);
    pAnimation->autorelease();
    return pAnimation;
}

bool CCAnimation::initWithDict(CCDictionary *dict)
{
    if (CCArray *frameArray = dict->arrayForKey("frames")) {
        CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();

        const CCString *loops = dict->valueForKey("loops");
        m_uLoops = 0 != loops->length() ? loops->intValue() : 1;
        m_bRestoreOriginalFrame = dict->valueForKey("restoreOriginalFrame")->boolValue();
        m_fDelayPerUnit = dict->floatForKey("delayPerUnit");

        // Array of AnimationFrames
        CCArray *array = CCArray::createWithCapacity(frameArray->count());

        CCARRAY_FOREACH_TYPE(frameArray, CCDictionary, entry)
        {
            const char *spriteFrameName = entry->valueForKey("spriteframe")->getCString();
            CCSpriteFrame *spriteFrame = frameCache->spriteFrameByName(spriteFrameName);

            if (!spriteFrame) {
                CCLOG("CCAnimation refers to frame '%s' which is not currently in the "
                      "CCSpriteFrameCache. This frame will not be added to the "
                      "animation.",
                      spriteFrameName);
                continue;
            }

            float delayUnits = entry->valueForKey("delayUnits")->floatValue();
            CCDictionary *userInfo = entry->dictForKey("notification");

            CCAnimationFrame *animFrame = new CCAnimationFrame();
            animFrame->initWithSpriteFrame(spriteFrame, delayUnits, userInfo);

            array->addObject(animFrame);
            m_fTotalDelayUnits += animFrame->getDelayUnits();

            animFrame->release();
        }

        setFrames(array);
        return true;
    } else {
        return false;
    }
}

CCAnimation *CCAnimation::createFromFile(std::string fileName)
{
    CCAnimation *pAnimation = new CCAnimation();
    pAnimation->initWithFile(fileName);
    pAnimation->autorelease();
    return pAnimation;
}

bool CCAnimation::initWithFile(std::string fileName)
{
    CCDictionary *dict = CCDictionary::createWithContentsOfFile(fileName.c_str());

    if (dict) {
        std::string atlas = dict->valueForKey("atlas")->getCString();
        int count = dict->intForKey("frameCount");
        std::string format = dict->valueForKey("format")->getCString();
        float delay = dict->floatForKey("delay");
        if (delay == 0) {
            delay = 1 / (float)30;
        }

        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(atlas.c_str());

        m_uLoops = 1;
        m_fDelayPerUnit = delay;

        // Array of AnimationFrames
        CCArray *array = CCArray::createWithCapacity(count);

        for (int i = 0; i < count; i++) {
            std::string spriteFrameName = CCString::createWithFormat(format.c_str(), i)->getCString();

            CCSpriteFrame *spriteFrame =
                CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteFrameName.c_str());

            if (!spriteFrame) {
                CCLOG("CCAnimation refers to frame '%s' which is not currently in the "
                      "CCSpriteFrameCache. This frame will not be added to the "
                      "animation.",
                      spriteFrameName.c_str());
                continue;
            }

            CCAnimationFrame *animFrame = new CCAnimationFrame();
            animFrame->initWithSpriteFrame(spriteFrame, 1, NULL);

            array->addObject(animFrame);
            m_fTotalDelayUnits++;

            animFrame->release();
        }

        setFrames(array);

        return true;
    } else {
        return false;
    }
}

#pragma mark - CCArray

void CCArray::addObject(int number)
{
    CCString *ccNumber = CCString::createWithFormat("%d", number);
    CCArray::addObject(ccNumber);
}

void CCArray::addObject(float number)
{
    CCString *ccNumber = CCString::createWithFormat("%f", number);
    CCArray::addObject(ccNumber);
}

void CCArray::addObject(double number)
{
    CCString *ccNumber = CCString::createWithFormat("%f", number);
    CCArray::addObject(ccNumber);
}

int CCArray::intAtIndex(unsigned int index, int defaultValue)
{
    CCString *number = dynamic_cast<CCString *>(objectAtIndex(index));
    if (number) {
        return number->intValue();
    }
    return defaultValue;
}

float CCArray::floatAtIndex(unsigned int index, float defaultValue)
{
    CCString *number = dynamic_cast<CCString *>(objectAtIndex(index));
    if (number) {
        return number->floatValue();
    }
    return defaultValue;
}

double CCArray::doubleAtIndex(unsigned int index, double defaultValue)
{
    CCString *number = dynamic_cast<CCString *>(objectAtIndex(index));
    if (number) {
        return number->doubleValue();
    }
    return defaultValue;
}

std::string CCArray::stringAtIndex(unsigned int index, std::string defaultValue)
{
    CCString *string = dynamic_cast<CCString *>(objectAtIndex(index));
    if (string) {
        return string->getCString();
    }
    return defaultValue;
}

// CODING
CODING_DEFINE(CCArray);

void CCArray::encode(Encoder &encoder)
{
    encoder.beginArray();
    CCObject *object;
    CCARRAY_FOREACH(this, object) { encoder.add(object); }
    encoder.endArray();
    encoder.addEncoded("data");
}

void CCArray::initWithDecoder(Decoder &decoder)
{
    initWithCapacity(1);

    decoder.getEncoded("data");
    while (decoder.getEncoded()) {
        CCObject *element = nullptr;
        decoder.decodeData(element);
        if (element) {
            addObject(element);
            element->release();
        }
    }
    decoder.popData();
}

CCObject *CCArray::pop()
{
    if (this->count()) {
        CCObject *aObj = this->objectAtIndex(0);
        CC_SAFE_RETAIN(aObj);
        this->removeObjectAtIndex(0);
        if (aObj)
            aObj->autorelease();

        return aObj;
    }

    return NULL;
}

// dohvaćanje objekata, vektori, boje

#pragma mark - CCDictionary

void CCDictionary::setObjectSafe(CCObject *pObject, const std::string &key)
{
    if (pObject != NULL) {
        setObject(pObject, key);
    }
}

void CCDictionary::setObjectSafe(CCObject *pObject, intptr_t key)
{
    if (pObject != NULL) {
        setObject(pObject, key);
    }
}

void CCDictionary::setObject(int number, const std::string &key)
{
    CCString *ccNumber = CCString::createWithFormat("%d", number);
    CCDictionary::setObject(ccNumber, key);
}

void CCDictionary::setObject(int number, intptr_t key)
{
    CCString *ccNumber = CCString::createWithFormat("%d", number);
    CCDictionary::setObject(ccNumber, key);
}

void CCDictionary::setObject(float number, const std::string &key)
{
    CCString *ccNumber = CCString::createWithFormat("%f", number);
    CCDictionary::setObject(ccNumber, key);
}

void CCDictionary::setObject(float number, intptr_t key)
{
    CCString *ccNumber = CCString::createWithFormat("%f", number);
    CCDictionary::setObject(ccNumber, key);
}

void CCDictionary::setObject(double number, const std::string &key)
{
    CCString *ccNumber = CCString::createWithFormat("%f", number);
    CCDictionary::setObject(ccNumber, key);
}

void CCDictionary::setObject(double number, intptr_t key)
{
    CCString *ccNumber = CCString::createWithFormat("%f", number);
    CCDictionary::setObject(ccNumber, key);
}

void CCDictionary::setObject(ccColor3B color, const std::string &key)
{
    CCString *str = CCString::createWithFormat("%d, %d, %d", color.r, color.g, color.b);
    CCDictionary::setObject(str, key);
}

void CCDictionary::setObject(ccColor4F color, const std::string &key)
{
    CCString *str = CCString::createWithFormat("%f, %f, %f, %f", color.r, color.g, color.b, color.a);
    CCDictionary::setObject(str, key);
}

void CCDictionary::setObject(CCPoint point, const std::string &key)
{
    CCString *str = CCString::createWithFormat("%f, %f", point.x, point.y);
    CCDictionary::setObject(str, key);
}

void CCDictionary::setObject(CCSize size, const std::string &key)
{
    CCString *str = CCString::createWithFormat("%f, %f", size.width, size.height);
    CCDictionary::setObject(str, key);
}

#include <sstream>

ccColor3B CCDictionary::color3BForKey(const std::string &key, ccColor3B defaultValue)
{
    CCString *str = dynamic_cast<CCString *>(objectForKey(key));
    if (str && str->length() > 0) {
        std::istringstream iss(str->m_sString);
        int r = defaultValue.r, g = defaultValue.g, b = defaultValue.b;
        sscanf(str->getCString(), "%d, %d, %d", &r, &g, &b);
        defaultValue.r = r;
        defaultValue.g = g;
        defaultValue.b = b;
    }
    return defaultValue;
}

ccColor4F CCDictionary::color4FForKey(const std::string &key, ccColor4F defaultValue)
{
    CCString *str = dynamic_cast<CCString *>(objectForKey(key));
    if (str && str->length() > 0) {
        sscanf(str->getCString(), "%f, %f, %f, %f", &defaultValue.r, &defaultValue.g, &defaultValue.b, &defaultValue.a);
    }
    return defaultValue;
}

CCPoint CCDictionary::pointForKey(const std::string &key, CCPoint defaultValue)
{
    CCString *str = dynamic_cast<CCString *>(objectForKey(key));
    if (str && str->length() > 0) {
        sscanf(str->getCString(), "%f, %f", &defaultValue.x, &defaultValue.y);
    }
    return defaultValue;
}

CCSize CCDictionary::sizeForKey(const std::string &key, CCSize defaultValue)
{
    CCString *str = dynamic_cast<CCString *>(objectForKey(key));
    if (str && str->length() > 0) {
        sscanf(str->getCString(), "%f, %f", &defaultValue.width, &defaultValue.height);
    }
    return defaultValue;
}

int CCDictionary::intForKey(const std::string &key, int defaultValue)
{
    CCString *number = dynamic_cast<CCString *>(objectForKey(key));
    if (number) {
        return number->intValue();
    }
    return defaultValue;
}

int CCDictionary::intForKey(intptr_t key, int defaultValue)
{
    CCString *number = dynamic_cast<CCString *>(objectForKey(key));
    if (number) {
        return number->intValue();
    }
    return defaultValue;
}

float CCDictionary::floatForKey(const std::string &key, float defaultValue)
{
    CCString *number = dynamic_cast<CCString *>(objectForKey(key));
    if (number) {
        return number->floatValue();
    }
    return defaultValue;
}

float CCDictionary::floatForKey(intptr_t key, float defaultValue)
{
    CCString *number = dynamic_cast<CCString *>(objectForKey(key));
    if (number) {
        return number->floatValue();
    }
    return defaultValue;
}

double CCDictionary::doubleForKey(const std::string &key, double defaultValue)
{
    CCString *number = dynamic_cast<CCString *>(objectForKey(key));
    if (number) {
        return number->doubleValue();
    }
    return defaultValue;
}

double CCDictionary::doubleForKey(intptr_t key, double defaultValue)
{
    CCString *number = dynamic_cast<CCString *>(objectForKey(key));
    if (number) {
        return number->doubleValue();
    }
    return defaultValue;
}

bool CCDictionary::boolForKey(const std::string &key, bool defaultValue)
{
    CCString *number = dynamic_cast<CCString *>(objectForKey(key));
    if (number) {
        return number->boolValue();
    }
    return defaultValue;
}

bool CCDictionary::boolForKey(intptr_t key, bool defaultValue)
{
    CCString *number = dynamic_cast<CCString *>(objectForKey(key));
    if (number) {
        return number->boolValue();
    }
    return defaultValue;
}

CCArray *CCDictionary::arrayForKey(const std::string &key, CCArray *defaultValue)
{
    CCArray *array = dynamic_cast<CCArray *>(objectForKey(key));
    if (array) {
        return array;
    }
    return defaultValue;
}

CCArray *CCDictionary::arrayForKey(intptr_t key, CCArray *defaultValue)
{
    CCArray *array = dynamic_cast<CCArray *>(objectForKey(key));
    if (array) {
        return array;
    }
    return defaultValue;
}

CCDictionary *CCDictionary::dictForKey(const std::string &key, CCDictionary *defaultValue)
{
    CCDictionary *dict = dynamic_cast<CCDictionary *>(objectForKey(key));
    if (dict) {
        return dict;
    }
    return defaultValue;
}

CCDictionary *CCDictionary::dictForKey(intptr_t key, CCDictionary *defaultValue)
{
    CCDictionary *dict = dynamic_cast<CCDictionary *>(objectForKey(key));
    if (dict) {
        return dict;
    }
    return defaultValue;
}

CCArray *CCDictionary::allObjects()
{
    int iKeyCount = this->count();
    if (iKeyCount <= 0)
        return NULL;

    CCArray *pArray = CCArray::createWithCapacity(iKeyCount);

    CCDictElement *pElement, *tmp;
    HASH_ITER(hh, m_pElements, pElement, tmp) { pArray->addObject(pElement->getObject()); }

    return pArray;
}

CCDictionary *CCDictionary::createWithContentsOfFile(const char *pFileName)
{
    CCDictionary *pRet = createWithContentsOfFileThreadSafe(pFileName);
    if (pRet)
        pRet->autorelease();
    return pRet;
}

// CODING
CODING_DEFINE(CCDictionary);

void CCDictionary::encode(Encoder &encoder)
{
    encoder.add(m_eDictType, "type");
    encoder.beginMap();
    CCDictElement *object;
    CCDICT_FOREACH(this, object)
    {
        if (m_eDictType == kCCDictInt) {
            encoder.add(object->getObject(), object->getIntKey());
        } else {
            encoder.add(object->getObject(), object->getStrKey());
        }
    }
    encoder.endMap();
    encoder.addEncoded("data");
}

void CCDictionary::initWithDecoder(Decoder &decoder)
{
    int type = 0;
    decoder.get(type, "type");
    m_eDictType = (CCDictType)type;
    decoder.getEncoded("data");

    while (decoder.getEncoded()) {
        CCObject *element = nullptr;
        if (m_eDictType == kCCDictInt) {
            int key = -1;
            decoder.getKey(key);
            decoder.decodeData(element);
            if (element) {
                setObject(element, key);
                element->release();
            }
        } else {
            std::string key;
            decoder.getKey(key);
            decoder.decodeData(element);
            if (element) {
                setObject(element, key);
                element->release();
            }
        }
    }
    decoder.popData();
}
// dohvaćanje objekata, vektori, boje

#pragma mark - CCString

bool CCString::isEqual(std::string &str) const { return m_sString.compare(str) == 0; }

bool CCString::isEqual(const char *str) const { return m_sString.compare(str) == 0; }

CODING_DEFINE(CCString);

void CCString::encode(Encoder &encoder) { encoder.add(m_sString, "data"); }

void CCString::initWithDecoder(Decoder &decoder) { decoder.get(m_sString, "data"); }

#pragma mark - CCBool
CODING_DEFINE(CCBool);

void CCBool::encode(Encoder &encoder) { encoder.add(m_bValue, "data"); }

void CCBool::initWithDecoder(Decoder &decoder) { decoder.get(m_bValue, "data"); }

#pragma mark - CCInteger
CODING_DEFINE(CCInteger);

void CCInteger::encode(Encoder &encoder) { encoder.add(m_nValue, "data"); }

void CCInteger::initWithDecoder(Decoder &decoder) { decoder.get(m_nValue, "data"); }

#pragma mark - CCFloat
CODING_DEFINE(CCFloat);

void CCFloat::encode(Encoder &encoder) { encoder.add(m_fValue, "data"); }

void CCFloat::initWithDecoder(Decoder &decoder) { decoder.get(m_fValue, "data"); }

#pragma mark - CCDouble
CODING_DEFINE(CCDouble);

void CCDouble::encode(Encoder &encoder) { encoder.add(m_dValue, "data"); }

void CCDouble::initWithDecoder(Decoder &decoder) { decoder.get(m_dValue, "data"); }

#pragma mark - CCSet
CODING_DEFINE(CCSet);

void CCSet::encode(Encoder &encoder)
{
    encoder.beginArray();
    for (CCObject *obj : *m_pSet) {
        encoder.add(obj);
    }
    encoder.endArray();
    encoder.addEncoded("data");
}

void CCSet::initWithDecoder(Decoder &decoder)
{
    decoder.getEncoded("data");
    while (decoder.getEncoded()) {
        CCObject *element = nullptr;
        decoder.decodeData(element);
        if (element) {
            addObject(element);
            element->release();
        }
    }
    decoder.popData();
}

#pragma mark - CCColor3B

bool cocos2d::operator==(const ccColor3B &first, const ccColor3B &second) { return ccc3BEqual(first, second); }

bool cocos2d::operator!=(const ccColor3B &first, const ccColor3B &second) { return !ccc3BEqual(first, second); }

#pragma mark - Shaders

CCGLProgram *getBlurShader()
{
    CCGLProgram *p = CCShaderCache::sharedShaderCache()->programForKey("blurShader");
    if (!p) {
        p = new CCGLProgram();
        GLchar *fragSource = (GLchar *)CCString::createWithContentsOfFile(
                                 CCFileUtils::sharedFileUtils()->fullPathForFilename("blurShader.fsh").c_str())
                                 ->getCString();
        p->initWithVertexShaderByteArray(ccPositionTextureColor_vert, fragSource);

        p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

        p->link();
        p->updateUniforms();

        CHECK_GL_ERROR_DEBUG();
        CCShaderCache::sharedShaderCache()->addProgram(p, "blurShader");
    }
    return p;
}

#pragma mark - Helper functions

//void buyIfPossible(float priceTickets, float priceGems, std::function<void()> yesFunction,
//                   std::function<void()> noFunction, CCNode *buyButton, const char *soundEffect, bool showOneTimeOffer)
//{
//    GameState *gs = GameState::get();
//
//    int playerTickets = gs->getPlayerTickets();
//    int playerGems = gs->getPlayerGems();
//
//    if (priceTickets <= playerTickets && priceGems <= playerGems) {
//
//        if (GameState::get()->getDaysPlayed() == 0 && priceTickets > 0) {
//            GameState::get()->addTicketsSpentFirstDay(priceTickets);
//        }
//
//        TicketController::get()->addPlayerTickets(-priceTickets);
//        gs->setPlayerGems(playerGems - priceGems);
//
//        if (yesFunction) {
//            yesFunction();
//        }
//
//        if (soundEffect) {
//            SoundManager::get()->playEffect(soundEffect);
//        }
//    } else {
//        auto func = [=]() {
//            buyIfPossible(priceTickets, priceGems, yesFunction, noFunction, buyButton, soundEffect, showOneTimeOffer);
//        };
//        if (noFunction) {
//            noFunction();
//        }
//        // not enough money/gems
//        if (priceGems > playerGems) {
//            HudLayer::get()->getHudHeader()->showGems();
//            bool showInappMenu = Parameters::getParameters()->boolForKey("inGameInAppShowMenu", false);
//            if (showInappMenu) {
//                HudLayer::get()->getHudHeader()->showInAppMenu(InAppTab::GemsTab, func, nullptr);
//            } else {
//                HudLayer::get()->showInAppQuickPurchaseMenu(
//                    getInGameInAppPurchase(priceGems - playerGems, showOneTimeOffer), priceGems, func, nullptr,
//                    buyButton, InAppPurchaseManager::showOneTimeOffer() && showOneTimeOffer);
//            }
//        } else if (priceTickets > playerTickets) {
//            HudLayer::get()->getHudHeader()->showTickets();
//            HudLayer::get()->getHudHeader()->showInAppMenu(InAppTab::TicketsTab, func, nullptr);
//        }
//    }
//}
//
//std::string getInGameInAppPurchase(int gemsAmount, bool showOneTimeOffer)
//{
//    CCDictionary *showPackageDict = Parameters::getParameters()->dictForKey("inGameInAppShowPackage");
//    bool showPackageEnabled = false;
//    std::string packageId = "";
//    if (showPackageDict) {
//        showPackageEnabled = showPackageDict->boolForKey("enabled", false);
//        if (InAppPurchaseManager::showOneTimeOffer() && showOneTimeOffer) {
//            packageId = InAppPurchaseManager::getOneTimeOfferPackageId();
//        } else {
//            CCString *packageIdCCString = dynamic_cast<CCString *>(showPackageDict->objectForKey("packageId"));
//            packageId = std::string(packageIdCCString->getCString());
//        }
//    }
//
//    if (!showPackageEnabled) {
//        packageId = getNereestInAppPurchase(gemsAmount);
//    }
//    return packageId;
//}
//
//std::string getNereestInAppPurchase(int gemsAmount)
//{
//    std::string productId = "";
//    float nearestDist = INFINITY;
//    std::vector<Profile *> inAppData(GameProfile::get()->getAllActiveGemInApps());
//
//    for (int i = 0; i < inAppData.size(); i++) {
//        InAppProfile *profile = (InAppProfile *)(inAppData[i]);
//        float distance = profile->getGain() - gemsAmount;
//        if (distance > 0 && distance < nearestDist) {
//            nearestDist = distance;
//            productId = profile->getProductId()->getCString();
//        }
//    }
//    return productId;
//}

int compareVersions(const std::string &firstVersion, const std::string &secondVersion)
{
    // -1 -> firstVersion < secondVersion
    // 0 -> firstVersion = secondVersion
    // 1 -> firstVersion > secondVersion

    // check for empty strings
    if (firstVersion.length() == 0 && secondVersion.length() > 0)
        return -1;
    else if (secondVersion.length() == 0 && firstVersion.length() > 0)
        return 1;
    else if (firstVersion.length() == 0 && secondVersion.length() == 0)
        return 0;

    std::istringstream s1(firstVersion);
    std::istringstream s2(secondVersion);
    std::string item1;
    std::string item2;

    // compare numbers (between .) from left to right (1.2.3 - 1.2.4.1)
    while (true) {
        getline(s1, item1, '.');
        getline(s2, item2, '.');

        int num1, num2;

        try {
            num1 = stoi(item1);
        } catch (...) {
            num1 = 0;
        }

        try {
            num2 = stoi(item2);
        } catch (...) {
            num2 = 0;
        }

        if (s1 && !s2)
            return 1;
        else if (!s1 && s2)
            return -1;
        else if (num1 > num2)
            return 1;
        else if (num2 > num1)
            return -1;
        else if (!s1 && !s2)
            return 0;
    }

    return 0;
}

std::string stringFromInt(int number)
{
    std::ostringstream ss;

    std::locale comma_locale(std::locale(), new CommaNumpunct());
    ss.imbue(comma_locale);
    ss.precision(2);
    ss.setf(ios::fixed);

    ss << number;

    return ss.str();
}

std::string stringFromFloat(float number)
{
    std::ostringstream ss;

    std::locale comma_locale(std::locale(), new CommaNumpunct());
    ss.imbue(comma_locale);
    ss.precision(2);
    ss.setf(ios::fixed);

    ss << number;

    return ss.str();
}

std::string prettyStringFromInt(int number, int normalLimit)
{
    int absNum = abs(number);
    if (absNum <= normalLimit)
        return stringFromInt(number);

    float displayNumber = absNum >= 1000 ? absNum >= 1000000 ? number / 1000000.0f : number / 1000.0f : number;
    if (fabs(displayNumber) < 1 || fabs(displayNumber) > 100)
        return stringFromInt(static_cast<int>(displayNumber)) + (absNum >= 1000 ? absNum >= 1000000 ? "M" : "k" : "");
    else
        return stringFromFloat(displayNumber) + (absNum >= 1000 ? absNum >= 1000000 ? "M" : "k" : "");
}

string clockStringFromSeconds(double seconds)
{
    int timeSeconds = round(seconds);
    int s = timeSeconds % 60;
    int m = (timeSeconds / 60) % 60;
    int h = timeSeconds / 3600;

    char buffer[16];
    if (h > 0)
        sprintf(buffer, "%d:%02d:%02d", h, m, s);
    else
        sprintf(buffer, "%d:%02d", m, s);

    return string(buffer);
}

string clockStringFromSecondsShort(double seconds)
{
    int timeSeconds = round(seconds);
    int s = timeSeconds % 60;
    int m = (timeSeconds / 60) % 60;
    int h = timeSeconds / 3600;

    char buffer[16];
    if (h > 0)
        sprintf(buffer, "%d:%02d", h, m);
    else
        sprintf(buffer, "%d:%02d", m, s);

    return string(buffer);
}

string timeStringFromSeconds(double seconds, bool hideLoweTime)
{
    int timeSeconds = round(seconds);

    int hiTime, loTime;
    string hiString, loString;

    if (timeSeconds < HOUR) {
        hiTime = timeSeconds / 60;
        loTime = timeSeconds % 60;

        hiString = " m";
        loString = " s";
    } else if (timeSeconds < DAY) {
        hiTime = timeSeconds / HOUR;
        loTime = (timeSeconds % HOUR) / 60;

        hiString = " h";
        loString = " m";
    } else {
        hiTime = timeSeconds / DAY;
        loTime = (timeSeconds % DAY) / HOUR;

        if (hiTime == 1)
            hiString = " day";
        else
            hiString = " days";

        loString = " h";
    }

    char buffer[16];
    sprintf(buffer, "%02d", loTime);
    loString = string(buffer) + loString;

    string timeString;
    if (timeSeconds < MINUTE) {
        timeString = loString;
    } else if (loTime == 0 && hideLoweTime) { /* && loString != "sec"*/
        timeString = to_string(hiTime) + hiString;
    } else {
        timeString = to_string(hiTime) + hiString + " " + loString;
    }

    return timeString;
}

string timeStringFromTimestamp(int timestamp)
{
    const time_t rawtime = (const time_t)timestamp;

    struct tm *dt;
    char timestr[30];

    dt = localtime(&rawtime);
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", dt);
    return std::string(timestr);
}

string timeStringFromTimestampUTC(int timestamp)
{
    const time_t rawtime = (const time_t)timestamp;

    struct tm *dt;
    char timestr[30];

    dt = gmtime(&rawtime);
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", dt);
    return std::string(timestr);
}

string dateStringFromTimestamp(int timestamp)
{
    const time_t rawtime = (const time_t)timestamp;

    struct tm *dt;
    char timestr[30];

    dt = gmtime(&rawtime);
    strftime(timestr, sizeof(timestr), "%d. %m. %Y.", dt);
    return std::string(timestr);
}

int minutesInDayFromTimestamp(int timestamp)
{
    const time_t rawtime = (const time_t)timestamp;

    struct tm *dt;

    dt = localtime(&rawtime);
    return dt->tm_hour * MINUTE + dt->tm_min;
}

bool checkIfSameDate(int timestamp1, int timestamp2) { return timestamp1 % DAY == timestamp2 % DAY; }

bool stringCaseInsensitiveIsEqual(string strFirst, string strSecond)
{
    transform(strFirst.begin(), strFirst.end(), strFirst.begin(), ::tolower);
    transform(strSecond.begin(), strSecond.end(), strSecond.begin(), ::tolower);

    if (strFirst == strSecond)
        return true;
    else
        return false;
}

bool stringReplace(std::string &str, const std::string &from, const std::string &to)
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void printCCDictionary(std::string key, cocos2d::CCDictionary *dict)
{
    char *jsonDict = CCJSONConverter::sharedConverter()->strFrom(dict);
    CCLOG("%s: %s", key.c_str(), jsonDict);
}

#pragma mark - Reading and Writing to a File
void storeStringToFile(std::string str, std::string full_path)
{
    std::ofstream ofs(full_path.c_str());
    ofs << str;
    ofs.close();
}

void readStringFromFile(std::string &str, std::string full_path)
{
    std::ifstream ifs(full_path.c_str());
    str.clear();
    str.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    ifs.close();
}

#pragma mark - Other

cocos2d::CCArray *mergeSortedArrays(cocos2d::CCArray *a1, cocos2d::CCArray *a2, pLogComparison comparison)
{
    int i1 = 0, i2 = 0;
    int size1 = a1->count();
    int size2 = a2->count();
    CCArray *result = CCArray::createWithCapacity(size1 + size2);

    for (int i = 0; i < (size1 + size2); ++i) {
        CCDictionary *dict;
        if (i1 == size1) {
            dict = (CCDictionary *)a2->objectAtIndex(i2);
            ++i2;
        } else if (i2 == size2) {
            dict = (CCDictionary *)a1->objectAtIndex(i1);
            ++i1;
        } else {
            if (comparison((CCDictionary *)a1->objectAtIndex(i1), (CCDictionary *)a2->objectAtIndex(i2))) {
                dict = (CCDictionary *)a1->objectAtIndex(i1);
                ++i1;
            } else {
                dict = (CCDictionary *)a2->objectAtIndex(i2);
                ++i2;
            }
        }

        result->insertObject(dict, i);
    }

    return result;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

//bool isCountryUS(std::string countryCode)
//{
//    const CCString *usCountryCode = Parameters::getParameters()->valueForKey("countryCodeUS");
//    return usCountryCode->m_sString == countryCode;
//}

//bool isCountryEU(std::string countryCode)
//{
//    CCArray *countryCodes = Parameters::getParameters()->arrayForKey("countryCodesEU");
//    CCARRAY_FOREACH_TYPE(countryCodes, CCString, aCode)
//    {
//        if (aCode->isEqual(countryCode)) {
//            return true;
//        }
//    }
//    return false;
//}

CCSprite *CCSprite::createSpriteWithAnchorPointAndScale(std::string name, CCPoint anchorPint, float scaleX,
                                                        float scaleY)
{
    CCSprite *image = CCSprite::create(name.c_str());
    if (image) {
        image->setAnchorPoint(anchorPint);
        image->setScaleX(scaleX);
        image->setScaleY(scaleY);
        return image;
    } else {
        return nullptr;
    }
}

CCSprite *CCSprite::maskToSpriteWhite(CCSprite *mask)
{
    CCRenderTexture *rt = CCRenderTexture::create(mask->getContentSize().width, mask->getContentSize().height);
    float scaleX = mask->getContentSize().width / getContentSize().width;
    float scaleY = mask->getContentSize().height / getContentSize().height;
    float scale = fmax(scaleX, scaleY);
    setScale(scale);
    setZOrder(0);
    mask->setPosition(ccp(mask->getContentSize().width / 2, mask->getContentSize().height / 2));
    setPosition(ccp(getContentSize().width / 2 * scale, getContentSize().height / 2 * scale));

    mask->setBlendFunc((ccBlendFunc){GL_ONE, GL_ZERO});
    setBlendFunc((ccBlendFunc){GL_DST_ALPHA, GL_ZERO});
    rt->beginWithClear(0, 0, 0, 0);
    mask->visit();
    visit();
    rt->end();

    CCSprite *retval = CCSprite::createWithTexture(rt->getSprite()->getTexture());
    retval->setScaleY(-1);
    retval->setAnchorPoint(ccp(0, 1));
    return retval;
}
