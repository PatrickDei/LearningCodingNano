//
//  StarryParticles.cpp
//  NanoGirl
//
//  Created by Saša Stanko on 28/02/2014.
//
//

#include "StarryParticles.h"

using namespace std;
using namespace cocos2d;

StarryParticles *StarryParticles::create(cocos2d::CCSize contentSize)
{
    StarryParticles *node = new StarryParticles();
    if(node && node->init(contentSize)) {
        node->autorelease();
        return node;
    } else {
        delete node;
        return nullptr;
    }
}

StarryParticles::~StarryParticles() {}

bool StarryParticles::init(cocos2d::CCSize contentSize)
{
    if(!CCNode::init())
        return false;

    distribution = normal_distribution<float>(0.0f, 2.0f);

    this->setContentSize(contentSize);
    this->setAnchorPoint(CCPoint(0.5f, 0.5f));
    return true;
}

void StarryParticles::onEnter()
{
    CCNode::onEnter();
    activaTime = 0.0f;
    scheduleUpdate();
}

void StarryParticles::onExit()
{
    CCNode::onExit();
    unscheduleUpdate();
}

void StarryParticles::update(float dt)
{
    activaTime += dt;

    for(; activaTime >= generationTime; activaTime -= generationTime)
        if(aliveParticles < maxParticles)
            addParticle();
}

void StarryParticles::addParticle()
{
    float particleLifespan = lifespan + CCRANDOM_MINUS1_1() * lifespanVariance;
    float particleLifespanPeak = lifespanPeak * (particleLifespan / lifespan);

    CCSprite *particle = CCSprite::create("star_particle2.png");

    // position
    float x = clampf(distribution(generator), -4.0f, 4.0f) / 8.0f + 0.5f;
    float y = clampf(distribution(generator), -4.0f, 4.0f) / 8.0f + 0.5f;
    CCPoint position = ccpCompMult(CCPoint(x, y), getContentSize());
    particle->setPosition(position);

    // opacity
    particle->setOpacity(255 * opacityStart);
    CCSequence *opacityAction =
        CCSequence::create(CCFadeTo::create(particleLifespanPeak, 255 * opacityPeak),
                           CCFadeTo::create(particleLifespan - particleLifespanPeak, 255 * opacityEnd), nullptr);

    // scale
    particle->setScale(scaleStart);
    CCSequence *scaleAction =
        CCSequence::create(CCScaleTo::create(particleLifespanPeak, scalePeak),
                           CCScaleTo::create(particleLifespan - particleLifespanPeak, scaleEnd), nullptr);

    // adding particle
    addChild(particle);
    particle->runAction(
        CCSequence::create(CCSpawn::create(opacityAction, scaleAction, nullptr),
                           CCCallFuncN::create(this, callfuncN_selector(StarryParticles::removeParticle)), nullptr));

    ++aliveParticles;
}

void StarryParticles::removeParticle(cocos2d::CCSprite *particle)
{
    particle->removeFromParent();
    --aliveParticles;
}
