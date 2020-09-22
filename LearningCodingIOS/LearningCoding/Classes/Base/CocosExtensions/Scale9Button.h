//
//  Scale9Button.h
//  Hollywood
//
//  Created by Saša Stanko on 14/07/2014.
//
//

#ifndef __Hollywood__Scale9Button__
#define __Hollywood__Scale9Button__

#include "cocos2d.h"
namespace cocos2d
{
namespace extension
{
class CCScale9Sprite;
}
};

class Scale9Button : public cocos2d::CCMenuItem
{
  public:
    virtual bool init(const char *pinName, cocos2d::CCObject *rec, cocos2d::SEL_MenuHandler selector);
    virtual bool init(const char *pinName, const char *selectedPinName, cocos2d::CCObject *rec,
                      cocos2d::SEL_MenuHandler selector);
    virtual bool init(const char *pinName, const char *selectedPinName, const char *disabledPinName,
                      cocos2d::CCObject *rec, cocos2d::SEL_MenuHandler selector);
    virtual void setContentSize(const cocos2d::CCSize &size);

    virtual void selected();
    virtual void unselected();
    virtual void setEnabled(bool value);
    
    virtual void setColor(const cocos2d::ccColor3B& color);
    virtual void setColorSelected(const cocos2d::ccColor3B& color);

  protected:
    cocos2d::extension::CCScale9Sprite *back;
    cocos2d::extension::CCScale9Sprite *front = nullptr;
    cocos2d::extension::CCScale9Sprite *front_selected = nullptr;
    cocos2d::extension::CCScale9Sprite *front_disabled = nullptr;
};

#endif /* defined(__Hollywood__Scale9Button__) */
