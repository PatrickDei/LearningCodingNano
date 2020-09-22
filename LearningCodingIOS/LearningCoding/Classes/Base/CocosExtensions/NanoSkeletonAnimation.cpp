//
//  NanoSkeletonAnimation.cpp
//  LearningCoding
//
//  Created by Lana Lisjak on 08/03/2019.
//

#include <spine/extension.h>

#include "AtlasCache.h"
#include "NanoSkeletonAnimation.h"

using namespace std;
using namespace cocos2d;
using namespace spine;

NanoSkeletonAnimation::NanoSkeletonAnimation(spSkeletonData *skeletonData) : CCSkeletonAnimation(skeletonData)
{
    timeScale = 1.0f;
    debugBones = false;
    setPosition(ccp(100.0f, 0.0f));
    setScale(1.0f);

    initialSkeletonUpdate();
}

void NanoSkeletonAnimation::initialSkeletonUpdate() { update(0.0f); }

void NanoSkeletonAnimation::addEventListener(std::function<void(int trackIndex, spEvent *event)> listener)
{
    spTrackEntry *entry = spAnimationState_getCurrent(state, 0);
    setEventListener(entry, listener);
}

void NanoSkeletonAnimation::addAnimationCompletedListener(std::function<void(int trackIndex, int loopCount)> listener)
{
    spTrackEntry *entry = spAnimationState_getCurrent(state, 0);
    setCompleteListener(entry, listener);
}

void NanoSkeletonAnimation::addAnimationEndListener(std::function<void(int trackIndex)> listener)
{
    spTrackEntry *entry = spAnimationState_getCurrent(state, 0);
    setEndListener(entry, listener);
}

spRegionAttachment *NanoSkeletonAnimation::createRegionAttachment(spAtlasRegion *region, spRegionAttachment *oldAtt)
{
    spRegionAttachment *attachment = spRegionAttachment_create(region->name);
    attachment->rendererObject = region;
    spRegionAttachment_setUVs(attachment, region->u, region->v, region->u2, region->v2, region->rotate);
    attachment->regionOffsetX = region->offsetX;
    attachment->regionOffsetY = region->offsetY;
    attachment->regionWidth = region->width;
    attachment->regionHeight = region->height;
    attachment->regionOriginalWidth = region->originalWidth;
    attachment->regionOriginalHeight = region->originalHeight;

    attachment->x = oldAtt->x;
    attachment->y = oldAtt->y;
    attachment->rotation = oldAtt->rotation;
    attachment->width = oldAtt->width;
    attachment->height = oldAtt->height;
    spRegionAttachment_updateOffset(attachment);

    return attachment;
}

spMeshAttachment *NanoSkeletonAnimation::createMeshAttachment(spAtlasRegion *region, spMeshAttachment *oldAtt)
{
    spMeshAttachment *attachment = spMeshAttachment_create(region->name);
    MALLOC_STR(attachment->path, oldAtt->path);
    attachment->rendererObject = region;
    attachment->regionU = region->u;
    attachment->regionV = region->v;
    attachment->regionU2 = region->u2;
    attachment->regionV2 = region->v2;
    attachment->regionRotate = region->rotate;
    attachment->regionOffsetX = region->offsetX;
    attachment->regionOffsetY = region->offsetY;
    attachment->regionWidth = region->width;
    attachment->regionHeight = region->height;
    attachment->regionOriginalWidth = region->originalWidth;
    attachment->regionOriginalHeight = region->originalHeight;

    attachment->r = oldAtt->r;
    attachment->g = oldAtt->g;
    attachment->b = oldAtt->b;
    attachment->a = oldAtt->a;

    attachment->width = oldAtt->width;
    attachment->height = oldAtt->height;

    if (!oldAtt->parentMesh) {
        attachment->verticesCount = oldAtt->verticesCount;
        attachment->vertices = MALLOC(float, oldAtt->verticesCount);
        for (int i = 0; i < oldAtt->verticesCount; i++)
            attachment->vertices[i] = oldAtt->vertices[i];

        attachment->trianglesCount = oldAtt->trianglesCount;
        attachment->triangles = MALLOC(unsigned short, oldAtt->trianglesCount);
        for (int i = 0; i < oldAtt->trianglesCount; i++)
            attachment->triangles[i] = oldAtt->triangles[i];

        attachment->uvsSize = oldAtt->uvsSize;
        attachment->regionUVs = MALLOC(float, oldAtt->uvsSize);
        for (int i = 0; i < oldAtt->uvsSize; i++)
            attachment->regionUVs[i] = oldAtt->regionUVs[i];

        spMeshAttachment_updateUVs(attachment);

        attachment->hullLength = oldAtt->hullLength;

        if (oldAtt->edgesCount > 0) {
            attachment->edgesCount = oldAtt->edgesCount;
            attachment->edges = MALLOC(int, oldAtt->edgesCount);
            for (int i = 0; i < oldAtt->edgesCount; i++)
                attachment->edges[i] = oldAtt->edges[i];
        }
    }
    return attachment;
}

spWeightedMeshAttachment *NanoSkeletonAnimation::createWeightedMeshAttachment(spAtlasRegion *region,
                                                                              spWeightedMeshAttachment *oldAtt)
{
    spWeightedMeshAttachment *attachment = spWeightedMeshAttachment_create(region->name);
    MALLOC_STR(attachment->path, oldAtt->path);
    attachment->rendererObject = region;
    attachment->regionU = region->u;
    attachment->regionV = region->v;
    attachment->regionU2 = region->u2;
    attachment->regionV2 = region->v2;
    attachment->regionRotate = region->rotate;
    attachment->regionOffsetX = region->offsetX;
    attachment->regionOffsetY = region->offsetY;
    attachment->regionWidth = region->width;
    attachment->regionHeight = region->height;
    attachment->regionOriginalWidth = region->originalWidth;
    attachment->regionOriginalHeight = region->originalHeight;

    attachment->r = oldAtt->r;
    attachment->g = oldAtt->g;
    attachment->b = oldAtt->b;
    attachment->a = oldAtt->a;

    attachment->width = oldAtt->width;
    attachment->height = oldAtt->height;

    if (!oldAtt->parentMesh) {
        attachment->bonesCount = oldAtt->bonesCount;
        attachment->bones = MALLOC(int, oldAtt->bonesCount);
        for (int i = 0; i < oldAtt->bonesCount; i++)
            attachment->bones[i] = oldAtt->bones[i];

        attachment->weightsCount = oldAtt->weightsCount;
        attachment->weights = MALLOC(float, oldAtt->weightsCount);
        for (int i = 0; i < oldAtt->weightsCount; i++)
            attachment->weights[i] = oldAtt->weights[i];

        attachment->trianglesCount = oldAtt->trianglesCount;
        attachment->triangles = MALLOC(unsigned short, oldAtt->trianglesCount);
        for (int i = 0; i < oldAtt->trianglesCount; i++)
            attachment->triangles[i] = oldAtt->triangles[i];

        attachment->uvsCount = oldAtt->uvsCount;
        attachment->regionUVs = MALLOC(float, oldAtt->uvsCount);
        for (int i = 0; i < oldAtt->uvsCount; i++)
            attachment->regionUVs[i] = oldAtt->regionUVs[i];

        attachment->hullLength = oldAtt->hullLength;

        if (oldAtt->edgesCount > 0) {
            attachment->edgesCount = oldAtt->edgesCount;
            attachment->edges = MALLOC(int, oldAtt->edgesCount);
            for (int i = 0; i < oldAtt->edgesCount; i++)
                attachment->edges[i] = oldAtt->edges[i];
        }

        spWeightedMeshAttachment_updateUVs(attachment);
    }
    return attachment;
}

void NanoSkeletonAnimation::setNewAttachment(spAttachment *attachment, const char *slotName)
{
    spSlot *slot = findSlot(slotName);
    if (slot) {
        spSlot_setAttachment(slot, attachment);
    } else {
        CCLOG("MISSING SLOT: %s", slotName);
    }
}

void NanoSkeletonAnimation::setNewAttachment(spAtlasRegion *region, const char *oldSlotName,
                                             const char *oldAttachmentName, const char *newSlotName, spSkin *skin)
{
    spAttachment *oldAtt = getAttachment(oldSlotName, oldAttachmentName);

    if (oldAtt) {
        spAttachment *attachment = new spAttachment();
        if (oldAtt->type == SP_ATTACHMENT_REGION) {
            attachment = (spAttachment *)createRegionAttachment(region, (spRegionAttachment *)oldAtt);
        } else if (oldAtt->type == SP_ATTACHMENT_MESH) {
            attachment = (spAttachment *)createMeshAttachment(region, (spMeshAttachment *)oldAtt);
        } else if (oldAtt->type == SP_ATTACHMENT_WEIGHTED_MESH) {
            attachment = (spAttachment *)createWeightedMeshAttachment(region, (spWeightedMeshAttachment *)oldAtt);
        }

        setNewAttachment(attachment, newSlotName);

        if (skin) {
            int slotIndex = spSkeleton_findSlotIndex(skeleton, oldSlotName);
            spSkin_addAttachment(skin, slotIndex, oldAttachmentName, attachment);
        }

    } else {
        CCLOG("MISSING ATTACHMENT: %s , SLOT: %s", oldAttachmentName, oldSlotName);
    }
}

void NanoSkeletonAnimation::setNewAttachmentByBone(spAtlasRegion *region, const char *boneName, const char *slotName,
                                                   spSkin *skin)
{
    setNewAttachment(region, boneName, boneName, slotName, skin);
}

void NanoSkeletonAnimation::setNewAttachmentByName(spAtlasRegion *region, const char *attachmentName,
                                                   const char *slotName, spSkin *skin)
{
    setNewAttachment(region, slotName, attachmentName, slotName, skin);
}

bool NanoSkeletonAnimation::loadImage(string imageName, string slotName, string attachmentName, spSkin *skin)
{
    if (spAtlas *clothesAtlas = AtlasCache::getAtlasFromImage(imageName)) {
        for (spAtlasRegion *region = clothesAtlas->regions; region != nullptr; region = region->next) {
            if (imageName == region->name) {
                setNewAttachmentByName(region, attachmentName.c_str(), slotName.c_str(), skin);
                return true;
            }
        }
    }
    return false;
}

bool NanoSkeletonAnimation::loadImageToSkin(string imageName, string slotName, string attachmentName)
{
    spSkin *oldSkin = additionalSkin;
    additionalSkin = spSkin_create("additionalSkin");

    loadImage(imageName, slotName, attachmentName, additionalSkin);

    spSkeleton_setSkin(skeleton, additionalSkin);

    if (oldSkin) {
        spSkin_dispose(oldSkin);
    }
    return true;
}

#pragma mark - Animations

CCArray *NanoSkeletonAnimation::getAnimations()
{
    int animCount = skeleton->data->animationsCount;
    spAnimation **animations = skeleton->data->animations;

    CCArray *animArr = CCArray::createWithCapacity(animCount);
    for (int i = 0; i < animCount; ++i) {
        animArr->addObject(CCString::create(animations[i]->name));
    }

    return animArr;
}

void NanoSkeletonAnimation::setMix(const char *fromAnimation, const char *toAnimation, float duration)
{
    setMix(fromAnimation, toAnimation, duration);
}

bool NanoSkeletonAnimation::runAnimation(const char *animationName, bool loop)
{
    setAnimation(0, animationName, loop);
    initialSkeletonUpdate();
    return true;
}

bool NanoSkeletonAnimation::addAnimation(const char *animationName, bool loop)
{
    spine::CCSkeletonAnimation::addAnimation(0, animationName, loop);
    return true;
}

bool NanoSkeletonAnimation::stopAnimation()
{
    clearTrack();
    setToSetupPose();
    memcpy(skeleton->drawOrder, skeleton->slots, skeleton->slotsCount * sizeof(spSlot *));
    updateWorldTransform();
    initialSkeletonUpdate();
    return true;
}

float NanoSkeletonAnimation::getAnimationDuration()
{
    float duration = 0;
    if (getCurrent()) {
        duration = getCurrent()->animation->duration;
    }
    return duration;
}

bool NanoSkeletonAnimation::isAnimationCompleted()
{
    bool returnValue = true;
    if (state)
        returnValue = spAnimationState_getCurrent(state, 0) == 0;
    return returnValue;
}

cocos2d::CCString *NanoSkeletonAnimation::getCurrentAnimation()
{
    if (state) {
        spTrackEntry *entry = spAnimationState_getCurrent(state, 0);
        if (entry) {
            return CCString::create(entry->animation->name);
        }
    }
    return nullptr;
}

#pragma mark - Node helper methods

cocos2d::CCRect NanoSkeletonAnimation::getActiveRect()
{
    float minX = FLT_MAX, minY = FLT_MAX, maxX = -FLT_MAX, maxY = -FLT_MAX;
    for (int i = 0, n = skeleton->slotsCount; i < n; i++) {
        spSlot *slot = skeleton->slots[i];
        if (!slot->attachment || slot->attachment->type != SP_ATTACHMENT_REGION)
            continue;
        spRegionAttachment *attachment = (spRegionAttachment *)slot->attachment;
        if (attachment->rendererObject) {
            // spRegionAttachment_updateQuad(attachment, slot, &quad);

            float vertices[8];
            spRegionAttachment_computeWorldVertices(attachment, slot->bone, vertices);

            for (int i = 0; i < 8; i = i + 2) {
                minX = min(minX, vertices[i]);
                minY = min(minY, vertices[i + 1]);
                maxX = max(maxX, vertices[i]);
                maxY = max(maxY, vertices[i + 1]);
            }
        }
    }
    CCRect rect = CCRectZero;
    if (minX < maxX && minY < maxY) {
        rect = CCRectMake(getPositionX() + minX, getPositionY() + minY, maxX - minX, maxY - minY);
    }

    return rect;
}

cocos2d::CCRect NanoSkeletonAnimation::getRectForSlot(const char *slotName)
{
    float minX = FLT_MAX, minY = FLT_MAX, maxX = -FLT_MAX, maxY = -FLT_MAX;
    //    float scaleX = getScaleX();
    //    float scaleY = getScaleY();

    for (int i = 0, n = skeleton->slotsCount; i < n; i++) {
        spSlot *slot = skeleton->slots[i];
        if (!slot->attachment || slot->attachment->type != SP_ATTACHMENT_REGION)
            continue;
        if (strcmp(slot->data->name, slotName) == 0) {
            spRegionAttachment *attachment = (spRegionAttachment *)slot->attachment;
            if (attachment->rendererObject) {
                // spRegionAttachment_updateQuad(attachment, slot, &quad);

                float vertices[8];
                spRegionAttachment_computeWorldVertices(attachment, slot->bone, vertices);

                for (int i = 0; i < 8; i = i + 2) {
                    minX = min(minX, vertices[i]);
                    minY = min(minY, vertices[i + 1]);
                    maxX = max(maxX, vertices[i]);
                    maxY = max(maxY, vertices[i + 1]);
                }
            }
            break;
        }
    }
    CCRect rect = CCRectZero;
    if (minX < maxX && minY < maxY) {
        rect = CCRectMake(getPositionX() + minX, getPositionY() + minY, maxX - minX, maxY - minY);
    }

    return rect;
}

cocos2d::CCPoint NanoSkeletonAnimation::getCenterForSlot(const char *slotName)
{
    CCRect rect = getRectForSlot(slotName);
    return CCPoint(rect.origin.x + rect.size.width * 0.5f, rect.origin.y + rect.size.height * 0.5f);
}

void NanoSkeletonAnimation::setFlipX(bool flip)
{
    skeleton->flipX = flip;
    initialSkeletonUpdate();
}

bool NanoSkeletonAnimation::isFlipX() { return skeleton->flipX; }

void NanoSkeletonAnimation::setFlipY(bool flip)
{
    skeleton->flipY = flip;
    initialSkeletonUpdate();
}

bool NanoSkeletonAnimation::isFlipY() { return skeleton->flipY; }

void NanoSkeletonAnimation::removeAttachment(const char *slotName)
{
    if (spSlot *slot = findSlot(slotName)) {
        spSlot_setAttachment(slot, NULL);
    }
}

void NanoSkeletonAnimation::setSlotColor(int slotIndex, GLubyte r, GLubyte g, GLubyte b)
{
    if (slotIndex >= 0) {
        spSlot *slot = skeleton->slots[slotIndex];
        slot->r = r / 255.0f;
        slot->g = g / 255.0f;
        slot->b = b / 255.0f;
    }
}
