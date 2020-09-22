//
//  CCMapFoldAction.h
//  Hollywood
//
//  Created by Saša Stanko on 21/08/2014.
//
//

#ifndef __Hollywood__CCMapFoldAction__
#define __Hollywood__CCMapFoldAction__

#include "cocos2d.h"
class CCMapFoldSprite;

// action to fold CCMapFoldSprite
class CCMapFoldAction : public cocos2d::CCActionInterval
{
  public:
    // folds are x-coordinates of places where map is folded, including left and
    // right edge
    static CCMapFoldAction *create(float duration, const std::vector<float> &folds);
    bool initWithDuration(float duration, const std::vector<float> &folds);

    virtual void startWithTarget(cocos2d::CCNode *target);
    virtual void update(float time);

  protected:
    void updateFold1(float time);
    void prepareFold2();
    void updateFold2(float time);

    CCMapFoldSprite *target;
    int current_fold = 1;
    float original_lx, original_rx;
    float fold_lx, fold_rx;
    float to_rx, aux, padding;
    std::vector<float> folds;
};

// sprite that will be folded
class CCMapFoldSprite : public cocos2d::CCSprite
{
  public:
    friend class CCMapFoldAction;

    static CCMapFoldSprite *create(cocos2d::CCTexture2D *texture);
    bool initWithTexture(cocos2d::CCTexture2D *texture, const cocos2d::CCRect &rect, bool rotated);
    bool initWithTexture(cocos2d::CCTexture2D *texture);
    virtual ~CCMapFoldSprite();

    virtual void setTextureRect(const cocos2d::CCRect &rect, bool rotated, const cocos2d::CCSize &untrimmedSize);
    virtual void draw();

  protected:
    void setFolds(const std::vector<float> &folds);
    void setVertices();
    void setIndices();

    int v = 4; // number of vertices
    int n = 1; // number of quads
    std::vector<float> folds;
    cocos2d::ccV3F_C4B_T2F *vertices = nullptr;
    GLushort *indices = nullptr;
};

#endif /* defined(__Hollywood__CCMapFoldAction__) */
