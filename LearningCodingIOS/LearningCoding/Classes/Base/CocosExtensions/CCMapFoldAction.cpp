//
//  CCMapFoldAction.cpp
//  Hollywood
//
//  Created by Saša Stanko on 21/08/2014.
//
//

#include "CCMapFoldAction.h"
using namespace cocos2d;

CCMapFoldAction *CCMapFoldAction::create(float duration, const std::vector<float> &folds)
{
    auto action = new CCMapFoldAction();
    if(action && action->initWithDuration(duration, folds))
        action->autorelease();
    else
        CC_SAFE_RELEASE_NULL(action);
    return action;
}

bool CCMapFoldAction::initWithDuration(float duration, const std::vector<float> &folds)
{
    if(!CCActionInterval::initWithDuration(duration))
        return false;
    this->folds = folds;
    return true;
}

void CCMapFoldAction::startWithTarget(CCNode *target)
{
    CCActionInterval::startWithTarget(target);
    this->target = static_cast<CCMapFoldSprite *>(target);
    std::vector<float> inner_folds(folds.begin() + 1, folds.end() - 1);
    this->target->setFolds(inner_folds);
    original_lx = this->target->vertices[0].vertices.x;
    original_rx = this->target->vertices[this->target->v / 2 - 1].vertices.x;
    fold_lx = folds[1];
    fold_rx = folds[folds.size() - 1];
    to_rx = folds[folds.size() - 3];
}

void CCMapFoldAction::update(float time)
{
    if(time <= 0.5f) {
        updateFold1(2 * time);
    } else {
        if(current_fold == 1) {
            updateFold1(1.0f);
            prepareFold2();
            current_fold = 2;
        }

        updateFold2(2 * (time - 0.5f));
    }
}

void CCMapFoldAction::updateFold1(float time)
{
    int v = target->v;
    float tt = 2.0f * time;

    // rotates the leftmost quad under the map
    float lx = (1 - tt) * original_lx + tt * fold_lx;
    target->vertices[0].vertices.x = lx;
    target->vertices[0].vertices.z = (fold_lx - original_lx) * sinf(-time * M_PI);
    target->vertices[v - 1].vertices.x = lx;
    target->vertices[v - 1].vertices.z = (fold_lx - original_lx) * sinf(-time * M_PI);

    // rotates the rightmost quad over the map
    float rx = (1 - time) * original_rx + time * to_rx;
    target->vertices[v / 2 - 1].vertices.x = rx;
    target->vertices[v / 2 - 1].vertices.z = (original_rx - fold_rx) * sinf(time * M_PI);
    target->vertices[v / 2].vertices.x = rx;
    target->vertices[v / 2].vertices.z = (original_rx - fold_rx) * sinf(time * M_PI);
}

void CCMapFoldAction::prepareFold2()
{
    // removing first and second-to-last quad
    int n = target->n;
    GLushort *indices = target->indices;
    for(int i = 6; i < 6 * n; ++i)
        indices[i - 6] = indices[i];
    for(int i = 6 * (n - 1); i < 6 * n; ++i)
        indices[i - 12] = indices[i];
    target->n -= 2;

    // setting variables needed for animation
    original_lx = fold_lx;
    fold_lx = folds[2];
    original_rx = folds[folds.size() - 2];
}

void CCMapFoldAction::updateFold2(float time)
{
    int v = target->v;
    float tt = 2.0f * time;

    // rotates the next to leftmost quad under the map
    float lx = (1 - tt) * original_lx + tt * fold_lx;
    target->vertices[1].vertices.x = lx;
    target->vertices[1].vertices.z = (fold_lx - original_lx) * sinf(-time * M_PI);
    target->vertices[v - 2].vertices.x = lx;
    target->vertices[v - 2].vertices.z = (fold_lx - original_lx) * sinf(-time * M_PI);
}

CCMapFoldSprite *CCMapFoldSprite::create(CCTexture2D *texture)
{
    auto sprite = new CCMapFoldSprite();
    if(sprite && sprite->initWithTexture(texture))
        sprite->autorelease();
    else
        CC_SAFE_RELEASE_NULL(sprite);
    return sprite;
}

bool CCMapFoldSprite::initWithTexture(CCTexture2D *texture, const CCRect &rect, bool rotated)
{
    if(!CCSprite::initWithTexture(texture, rect, rotated))
        return false;

    setFolds(std::vector<float>());
    return true;
}

bool CCMapFoldSprite::initWithTexture(CCTexture2D *texture) { return CCSprite::initWithTexture(texture); }

CCMapFoldSprite::~CCMapFoldSprite()
{
    delete vertices;
    delete indices;
}

void CCMapFoldSprite::setTextureRect(const CCRect &rect, bool rotated, const CCSize &untrimmedSize)
{
    CCSprite::setTextureRect(rect, rotated, untrimmedSize);
    setVertices();
}

void CCMapFoldSprite::draw()
{
    CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");

    CCAssert(!m_pobBatchNode, "If CCSprite is being rendered by "
                              "CCSpriteBatchNode, CCSprite#draw SHOULD NOT be "
                              "called");

    CC_NODE_DRAW_SETUP();

    ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);

    ccGLBindTexture2D(m_pobTexture->getName());
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);

#define kVertexSize sizeof(ccV3F_C4B_T2F)
    long offset = (long)vertices;

    // vertex
    int diff = offsetof(ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kVertexSize, (void *)(offset + diff));

    // texCoods
    diff = offsetof(ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kVertexSize, (void *)(offset + diff));

    // color
    diff = offsetof(ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kVertexSize, (void *)(offset + diff));

    glDrawElements(GL_TRIANGLES, (GLsizei)n * 6, GL_UNSIGNED_SHORT, indices);

    CHECK_GL_ERROR_DEBUG();

    CC_INCREMENT_GL_DRAWS(1);

    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
}

void CCMapFoldSprite::setFolds(const std::vector<float> &folds)
{
    this->folds = folds;
    v = 2 * (2 + folds.size());
    n = v / 2 - 1;

    delete[] vertices;
    delete[] indices;
    vertices = new ccV3F_C4B_T2F[v];
    indices = new GLushort[6 * n];

    setVertices();
    setIndices();
}

void CCMapFoldSprite::setVertices()
{
    if(!vertices)
        return;

    vertices[0] = m_sQuad.bl;
    vertices[v / 2 - 1] = m_sQuad.br;
    vertices[v / 2] = m_sQuad.tr;
    vertices[v - 1] = m_sQuad.tl;

    for(int i = 1; i + 1 < v / 2; ++i) {
        float u = folds[i - 1] / m_obContentSize.width;
        vertices[i] = vertices[0];
        vertices[i].vertices.x = (1 - u) * vertices[0].vertices.x + u * vertices[v / 2 - 1].vertices.x;
        vertices[i].texCoords.u = (1 - u) * vertices[0].texCoords.u + u * vertices[v / 2 - 1].texCoords.u;
        vertices[v - 1 - i] = vertices[v - 1];
        vertices[v - 1 - i].vertices.x = (1 - u) * vertices[v - 1].vertices.x + u * vertices[v / 2].vertices.x;
        vertices[v - 1 - i].texCoords.u = (1 - u) * vertices[v - 1].texCoords.u + u * vertices[v / 2].texCoords.u;
    }
}

void CCMapFoldSprite::setIndices()
{
    if(!indices)
        return;

    for(int i = 0; i + 1 < v / 2; ++i) {
        indices[6 * i + 0] = i;
        indices[6 * i + 1] = i + 1;
        indices[6 * i + 2] = v - 1 - i;

        indices[6 * i + 3] = v - 1 - i;
        indices[6 * i + 4] = i + 1;
        indices[6 * i + 5] = v - 2 - i;
    }
}
