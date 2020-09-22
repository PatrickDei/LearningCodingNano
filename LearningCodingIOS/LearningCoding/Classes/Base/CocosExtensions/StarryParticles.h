//
//  StarryParticles.h
//  NanoGirl
//
//  Created by Saša Stanko on 28/02/2014.
//
//

#ifndef __NanoGirl__StarryParticles__
#define __NanoGirl__StarryParticles__

#include "cocos2d.h"
#include <random>

// simple particle system with stars
//
// each setting has 3 variants: start, peak, end
//   particle is created with start settings,
//   reaches peak settings at lifespanPeak seconds,
//   and ends with end settings
//
// lifespanPeek is changed the same way lifespan is, i.e.
//   let lifespan = 1.0, lifespanPeak = 0.4, lifespanVariance = 1
//   if particle is generated with lifespan 1.8
//   then lifespanPeak for that particle will be 0.72 (0.4*1.8/1.0)
//
// every generationTime seconds one particle is generated as long as
// their number does not exceed maxParticles

class StarryParticles : public cocos2d::CCNode
{
  public:
    static StarryParticles *create(cocos2d::CCSize contentSize);
    ~StarryParticles();

    // properties
    int maxParticles = 3;
    float generationTime = 0.45f;

    float lifespan = 0.6667f;
    float lifespanPeak = 0.3f;
    float lifespanVariance = 0.1f;

    float opacityStart = 0.0f;
    float opacityPeak = 1.0f;
    float opacityEnd = 0.0f;

    float scaleStart = 0.5f;
    float scalePeak = 0.8f;
    float scaleEnd = 0.8f;

  protected:
    virtual bool init(cocos2d::CCSize contentSize);
    virtual void onEnter();
    virtual void onExit();
    virtual void update(float dt);

    void addParticle();
    void removeParticle(cocos2d::CCSprite *particle);

    float activaTime = 0.0f;
    int aliveParticles = 0;

    std::default_random_engine generator;
    std::normal_distribution<float> distribution;
};

#endif /* defined(__NanoGirl__StarryParticles__) */
