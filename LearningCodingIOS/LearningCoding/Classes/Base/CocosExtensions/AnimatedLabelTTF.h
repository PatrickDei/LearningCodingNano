//
//  AnimatedLabelTTF.h
//  Hollywood
//
//  Created by Saša Stanko on 13/05/2014.
//
//

#ifndef __Hollywood__AnimatedLabelTTF__
#define __Hollywood__AnimatedLabelTTF__

#include "cocos2d.h"
#include <string>

class AnimatedLabelTTF : public cocos2d::CCLabelTTF
{
  public:
    static AnimatedLabelTTF *create(const char *string, const char *fontName, float fontSize,
                                    cocos2d::CCSize dimensions = cocos2d::CCSizeZero,
                                    cocos2d::CCTextAlignment hAlignment = cocos2d::kCCTextAlignmentCenter,
                                    cocos2d::CCVerticalTextAlignment vAlignment = cocos2d::kCCVerticalTextAlignmentTop, bool animate = true);

    virtual bool init(const char *string, const char *fontName, float fontSize,
                      cocos2d::CCSize dimensions = cocos2d::CCSizeZero,
                      cocos2d::CCTextAlignment hAlignment = cocos2d::kCCTextAlignmentCenter,
                      cocos2d::CCVerticalTextAlignment vAlignment = cocos2d::kCCVerticalTextAlignmentTop, bool animate = true);

    virtual void setString(const char *label);

    void run(std::function<void()> animationCompletedFunc = nullptr, bool fromStart = true);
    void finish();

    std::string getOriginalText() { return originalText; }
    cocos2d::CCSize getFinalContentSize() { return finalContentSize; }

  protected:
    void prepareText();
    void updateText(float dt);

    float lettersPerSecond = 40;
    float timePassed = 0.0f;
    std::string originalText;
    std::string text;
    std::function<void()> animationCompletedFunc = nullptr;
    cocos2d::CCSize finalContentSize;
    bool animate = true;
};

#endif /* defined(__Hollywood__AnimatedLabelTTF__) */
