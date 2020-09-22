/******************************************************************************
 * Spine Runtimes Software License
 * Version 2.3
 *
 * Copyright (c) 2013-2015, Esoteric Software
 * All rights reserved.
 *
 * You are granted a perpetual, non-exclusive, non-sublicensable and
 * non-transferable license to use, install, execute and perform the Spine
 * Runtimes Software (the "Software") and derivative works solely for personal
 * or internal use. Without the written permission of Esoteric Software (see
 * Section 2 of the Spine Software License Agreement), you may not (a) modify,
 * translate, adapt or otherwise create derivative works, improvements of the
 * Software or develop new applications using the Software or (b) remove,
 * delete, alter or obscure any trademarks or any copyright, trademark, patent
 * or other intellectual property or proprietary rights notices on or in the
 * Software, including any copy thereof. Redistributions in binary or source
 * form must include this license and terms.
 *
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include <spine/CCSkeletonAnimation.h>
#include <spine/spine-cocos2dx.h>
#include <spine/extension.h>
#include <algorithm>

USING_NS_CC;
using std::min;
using std::max;
using std::vector;

namespace spine
{

void animationCallback(spAnimationState *state, int trackIndex, spEventType type, spEvent *event, int loopCount)
{
    ((CCSkeletonAnimation *)state->rendererObject)->onAnimationStateEvent(trackIndex, type, event, loopCount);
}

void trackEntryCallback(spAnimationState *state, int trackIndex, spEventType type, spEvent *event, int loopCount)
{
    ((CCSkeletonAnimation *)state->rendererObject)->onTrackEntryEvent(trackIndex, type, event, loopCount);
}

typedef struct _TrackEntryListeners {
    StartListener startListener;
    EndListener endListener;
    CompleteListener completeListener;
    EventListener eventListener;
} _TrackEntryListeners;

static _TrackEntryListeners *getListeners(spTrackEntry *entry)
{
    if (!entry->rendererObject) {
        entry->rendererObject = NEW(spine::_TrackEntryListeners);
        entry->listener = trackEntryCallback;
    }
    return (_TrackEntryListeners *)entry->rendererObject;
}

void disposeTrackEntry(spTrackEntry *entry)
{
    if (entry->rendererObject)
        FREE(entry->rendererObject);
    _spTrackEntry_dispose(entry);
}

//

CCSkeletonAnimation *CCSkeletonAnimation::createWithData(spSkeletonData *skeletonData)
{
    CCSkeletonAnimation *node = new CCSkeletonAnimation(skeletonData);
    node->autorelease();
    return node;
}

CCSkeletonAnimation *CCSkeletonAnimation::createWithFile(const char *skeletonDataFile, spAtlas *atlas, float scale)
{
    CCSkeletonAnimation *node = new CCSkeletonAnimation(skeletonDataFile, atlas, scale);
    node->autorelease();
    return node;
}

CCSkeletonAnimation *CCSkeletonAnimation::createWithFile(const char *skeletonDataFile, const char *atlasFile,
                                                         float scale)
{
    CCSkeletonAnimation *node = new CCSkeletonAnimation(skeletonDataFile, atlasFile, scale);
    node->autorelease();
    return node;
}

void CCSkeletonAnimation::initialize()
{
    ownsAnimationStateData = true;
    state = spAnimationState_create(spAnimationStateData_create(skeleton->data));
    state->rendererObject = this;
    state->listener = animationCallback;

    _spAnimationState *stateInternal = (_spAnimationState *)state;
    stateInternal->disposeTrackEntry = disposeTrackEntry;
}

CCSkeletonAnimation::CCSkeletonAnimation(spSkeletonData *skeletonData) : CCSkeleton(skeletonData) { initialize(); }

CCSkeletonAnimation::CCSkeletonAnimation(const char *skeletonDataFile, spAtlas *atlas, float scale)
    : CCSkeleton(skeletonDataFile, atlas, scale)
{
    initialize();
}

CCSkeletonAnimation::CCSkeletonAnimation(const char *skeletonDataFile, const char *atlasFile, float scale)
    : CCSkeleton(skeletonDataFile, atlasFile, scale)
{
    initialize();
}

CCSkeletonAnimation::~CCSkeletonAnimation()
{
    if (ownsAnimationStateData)
        spAnimationStateData_dispose(state->data);
    spAnimationState_dispose(state);
}

void CCSkeletonAnimation::update(float deltaTime)
{
    super::update(deltaTime);

    deltaTime *= timeScale;
    spAnimationState_update(state, deltaTime);
    spAnimationState_apply(state, skeleton);
    spSkeleton_updateWorldTransform(skeleton);
}

void CCSkeletonAnimation::setAnimationStateData(spAnimationStateData *stateData)
{
    CCAssert(stateData, "stateData cannot be null.");

    if (ownsAnimationStateData)
        spAnimationStateData_dispose(state->data);
    spAnimationState_dispose(state);

    ownsAnimationStateData = false;
    state = spAnimationState_create(stateData);
    state->rendererObject = this;
    state->listener = animationCallback;
}

void CCSkeletonAnimation::setMix(const char *fromAnimation, const char *toAnimation, float duration)
{
    spAnimationStateData_setMixByName(state->data, fromAnimation, toAnimation, duration);
}

spTrackEntry *CCSkeletonAnimation::setAnimation(int trackIndex, const char *name, bool loop)
{
    spAnimation *animation = spSkeletonData_findAnimation(skeleton->data, name);
    if (!animation) {
        CCLog("Spine: Animation not found: %s", name);
        return 0;
    }
    return spAnimationState_setAnimation(state, trackIndex, animation, loop);
}

spTrackEntry *CCSkeletonAnimation::addAnimation(int trackIndex, const char *name, bool loop, float delay)
{
    spAnimation *animation = spSkeletonData_findAnimation(skeleton->data, name);
    if (!animation) {
        CCLog("Spine: Animation not found: %s", name);
        return 0;
    }
    return spAnimationState_addAnimation(state, trackIndex, animation, loop, delay);
}

spTrackEntry *CCSkeletonAnimation::getCurrent(int trackIndex) { return spAnimationState_getCurrent(state, trackIndex); }

void CCSkeletonAnimation::clearTracks() { spAnimationState_clearTracks(state); }

void CCSkeletonAnimation::clearTrack(int trackIndex) { spAnimationState_clearTrack(state, trackIndex); }

void CCSkeletonAnimation::onAnimationStateEvent(int trackIndex, spEventType type, spEvent *event, int loopCount)
{
    switch (type) {
        case SP_ANIMATION_START:
            if (startListener)
                startListener(trackIndex);
            break;
        case SP_ANIMATION_END:
            if (endListener)
                endListener(trackIndex);
            break;
        case SP_ANIMATION_COMPLETE:
            if (completeListener)
                completeListener(trackIndex, loopCount);
            break;
        case SP_ANIMATION_EVENT:
            if (eventListener)
                eventListener(trackIndex, event);
            break;
    }
}

void CCSkeletonAnimation::onTrackEntryEvent(int trackIndex, spEventType type, spEvent *event, int loopCount)
{
    spTrackEntry *entry = spAnimationState_getCurrent(state, trackIndex);
    if (!entry->rendererObject)
        return;
    _TrackEntryListeners *listeners = (_TrackEntryListeners *)entry->rendererObject;
    switch (type) {
        case SP_ANIMATION_START:
            if (listeners->startListener)
                listeners->startListener(trackIndex);
            break;
        case SP_ANIMATION_END:
            if (listeners->endListener)
                listeners->endListener(trackIndex);
            break;
        case SP_ANIMATION_COMPLETE:
            if (listeners->completeListener)
                listeners->completeListener(trackIndex, loopCount);
            break;
        case SP_ANIMATION_EVENT:
            if (listeners->eventListener)
                listeners->eventListener(trackIndex, event);
            break;
    }
}

void CCSkeletonAnimation::setStartListener(spTrackEntry *entry, StartListener listener)
{
    getListeners(entry)->startListener = listener;
}

void CCSkeletonAnimation::setEndListener(spTrackEntry *entry, EndListener listener)
{
    getListeners(entry)->endListener = listener;
}

void CCSkeletonAnimation::setCompleteListener(spTrackEntry *entry, CompleteListener listener)
{
    getListeners(entry)->completeListener = listener;
}

void CCSkeletonAnimation::setEventListener(spTrackEntry *entry, spine::EventListener listener)
{
    getListeners(entry)->eventListener = listener;
}
}
