//
//  Scale9Button.cpp
//  Hollywood
//
//  Created by Saša Stanko on 14/07/2014.
//
//

#include "CCScale9Sprite.h"
#include "Scale9Button.h"
using namespace cocos2d;
using namespace cocos2d::extension;

bool Scale9Button::init(const char *pinName, CCObject *rec, SEL_MenuHandler selector)
{
    return init(pinName, nullptr, nullptr, rec, selector);
}

bool Scale9Button::init(const char *pinName, const char *selectedPinName, CCObject *rec, SEL_MenuHandler selector)
{
    return init(pinName, selectedPinName, nullptr, rec, selector);
}

bool Scale9Button::init(const char *pinName, const char *selectedPinName, const char *disabledPinName, CCObject *rec,
                        SEL_MenuHandler selector)
{
    if(!CCMenuItem::initWithTarget(rec, selector))
        return false;

    front = CCScale9Sprite::create(pinName);
    front->setAnchorPoint(ccp(0.0f, 0.0f));
    front->setPosition(ccp(0.0f, 0.0f));
    front->setCascadeColorEnabled(true);
    front->setCascadeOpacityEnabled(true);
    addChild(front);
    
    back = CCScale9Sprite::create("option_02.png");
    back->setAnchorPoint(ccp(0.0f, 0.0f));
    back->setPosition(ccp(0.0f, 0.0f));
    addChild(back);

    if(selectedPinName) {
        front_selected = CCScale9Sprite::create(selectedPinName);
        front_selected->setAnchorPoint(ccp(0.0f, 0.0f));
        front_selected->setPosition(ccp(0.0f, 0.0f));
        front_selected->setCascadeColorEnabled(true);
        front_selected->setCascadeOpacityEnabled(true);
        front_selected->setVisible(false);
        addChild(front_selected);
    }

    if(disabledPinName) {
        front_disabled = CCScale9Sprite::create(disabledPinName);
        front_disabled->setAnchorPoint(ccp(0.0f, 0.0f));
        front_disabled->setPosition(ccp(0.0f, 0.0f));
        front_disabled->setCascadeColorEnabled(true);
        front_disabled->setCascadeOpacityEnabled(true);
        front_disabled->setVisible(false);
        addChild(front_disabled);
    }

    setContentSize(back->getContentSize());

    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);

    return true;
}

void Scale9Button::setContentSize(const cocos2d::CCSize &size)
{
    CCMenuItem::setContentSize(size);
    back->setContentSize(size);
    front->setContentSize(size);
    if(front_selected)
        front_selected->setContentSize(size);
    if(front_disabled)
        front_disabled->setContentSize(size);
}

void Scale9Button::selected()
{
    CCMenuItem::selected();
    if(front_selected) {
        front->setVisible(false);
        front_selected->setVisible(true);
    }
}

void Scale9Button::unselected()
{
    CCMenuItem::unselected();
    if(front_selected) {
        front->setVisible(true);
        front_selected->setVisible(false);
    }
}

void Scale9Button::setEnabled(bool value)
{
    CCMenuItem::setEnabled(value);
    if(front_disabled)
        front_disabled->setVisible(!value);
}

void Scale9Button::setColor(const cocos2d::ccColor3B& color)
{
    front->setColor(color);
}

void Scale9Button::setColorSelected(const cocos2d::ccColor3B& color)
{
    front_selected->setColor(color);
}
