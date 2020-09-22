//
//  AnimatedLabelTTF.cpp
//  Hollywood
//
//  Created by Saša Stanko on 13/05/2014.
//
//

#include "AnimatedLabelTTF.h"
#include "IosFunctions.h"

using namespace cocos2d;
using namespace std;

AnimatedLabelTTF *AnimatedLabelTTF::create(const char *string, const char *fontName, float fontSize, CCSize dimensions,
                                           CCTextAlignment hAlignment, CCVerticalTextAlignment vAlignment, bool animate)
{
    AnimatedLabelTTF *label = new AnimatedLabelTTF();
    if(label && label->init(string, fontName, fontSize, dimensions, hAlignment, vAlignment, animate)) {
        label->autorelease();
        return label;
    } else {
        delete label;
        return nullptr;
    }
}

bool AnimatedLabelTTF::init(const char *string, const char *fontName, float fontSize, CCSize dimensions,
                            CCTextAlignment hAlignment, CCVerticalTextAlignment vAlignment, bool animate)
{
    if(!CCLabelTTF::initWithString(string, fontName, fontSize, dimensions, hAlignment, vAlignment))
        return false;
     this->animate = animate;
    return true;
}

void AnimatedLabelTTF::setString(const char *label)
{
    originalText = text = label;
    prepareText();
}

void AnimatedLabelTTF::run(std::function<void()> animationCompletedFunc, bool fromStart)
{
    if(fromStart) {
        timePassed = 0.0f;
        CCLabelTTF::setString("");
    }
    this->animationCompletedFunc = animationCompletedFunc;
    if(animate){
        //Google featuring, here we come!
        if (strcmp(IosFunctions::getDeviceName().c_str(), "Nexus 9") == 0)  {
            schedule(schedule_selector(AnimatedLabelTTF::updateText), 0.07f);
        }
        else {
            schedule(schedule_selector(AnimatedLabelTTF::updateText));
        }
    }
    else{
        finish();
    }
}

void AnimatedLabelTTF::finish()
{
    timePassed = text.size() / lettersPerSecond;
    CCLabelTTF::setString(text.c_str());
    unschedule(schedule_selector(AnimatedLabelTTF::updateText));
    if(animationCompletedFunc) {
        animationCompletedFunc();
        animationCompletedFunc = nullptr;
    }
}

void AnimatedLabelTTF::prepareText()
{
    string text_before = getString();

    {
        size_t i = 0;
        vector<size_t> blanks;
        while((i = text.find_first_of(' ', i + 1)) != string::npos)
            blanks.push_back(i);
        blanks.push_back(text.size());

        if(blanks.size() > 1) {
            CCLabelTTF::setString(text.substr(0, blanks[0]).c_str());
            float height = getTexture()->getContentSize().height;
            for(size_t i = 1, size = blanks.size(); i < size; ++i) {
                CCLabelTTF::setString(text.substr(0, blanks[i]).c_str());
                float newHeight = getTexture()->getContentSize().height;
                if(newHeight > height + 0.9f * m_fFontSize) {
                    height = newHeight;
                    text[blanks[i - 1]] = '\n';
                }
            }
        }
    }

    CCLabelTTF::setString(text.c_str());
    finalContentSize = getContentSize();
    CCLabelTTF::setString(text_before.c_str());
}

void AnimatedLabelTTF::updateText(float dt)
{
    timePassed +=  dt;
    size_t count = std::min(text.size(), (size_t)(lettersPerSecond * timePassed ));

    if(count < text.size())
        CCLabelTTF::setString(text.substr(0, count).c_str());
    else
        finish();
}
