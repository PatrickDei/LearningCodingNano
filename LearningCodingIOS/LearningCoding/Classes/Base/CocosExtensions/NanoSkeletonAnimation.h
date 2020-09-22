//
//  NanoSkeletonAnimation.h
//  LearningCoding
//
//  Created by Lana Lisjak on 08/03/2019.
//

#ifndef NanoSkeletonAnimation_h
#define NanoSkeletonAnimation_h

#include <stdio.h>
#include <cocos2d.h>
#include <spine/spine-cocos2dx.h>

using namespace spine;

class NanoSkeletonAnimation : public CCSkeletonAnimation
{
  public:
    NanoSkeletonAnimation(spSkeletonData *skeletonData);

    void initialSkeletonUpdate();

    bool loadImage(std::string imageName, std::string slotName, std::string attachmentName, spSkin *skin);
    bool loadImageToSkin(std::string imageName, std::string slotName, std::string attachmentName);

    void addEventListener(std::function<void(int trackIndex, spEvent *event)> listener);
    void addAnimationCompletedListener(std::function<void(int trackIndex, int loopCount)> listener);
    void addAnimationEndListener(std::function<void(int trackIndex)> listener);

    bool runAnimation(const char *animationName, bool loop = false);
    bool addAnimation(const char *animationName, bool loop = false);
    bool stopAnimation();
    float getAnimationDuration();
    bool isAnimationCompleted();
    cocos2d::CCString *getCurrentAnimation();

    cocos2d::CCRect getActiveRect();
    cocos2d::CCRect getRectForSlot(const char *slotName);
    cocos2d::CCPoint getCenterForSlot(const char *slotName);

    cocos2d::CCArray *getAnimations();
    void setMix(const char *fromAnimation, const char *toAnimation, float duration);

    void setFlipX(bool flip);
    bool isFlipX();
    void setFlipY(bool flip);
    bool isFlipY();

    void removeAttachment(const char *slotName);
    void setSlotColor(int slotIndex, GLubyte r, GLubyte g, GLubyte b);

    void setNewAttachmentByName(spAtlasRegion *region, const char *attachmentName, const char *slotName,
                                spSkin *skin = nullptr);
    void setNewAttachmentByBone(spAtlasRegion *region, const char *boneName, const char *slotName,
                                spSkin *skin = nullptr);
    void setNewAttachment(spAttachment *attachment, const char *slotName);

  protected:
    // skeletonNode is updated for each frame, except for the first
    // call this function to update skeleton when Model is created

    spRegionAttachment *createRegionAttachment(spAtlasRegion *region, spRegionAttachment *oldAtt);

    spMeshAttachment *createMeshAttachment(spAtlasRegion *region, spMeshAttachment *oldAtt);

    spWeightedMeshAttachment *createWeightedMeshAttachment(spAtlasRegion *region, spWeightedMeshAttachment *oldAtt);

    void setNewAttachment(spAtlasRegion *region, const char *oldSlotName, const char *oldAttachmentName,
                          const char *newSlotName, spSkin *skin = nullptr);

    spSkin *additionalSkin = nullptr;
};

#endif /* NanoSkeletonAnimation_h */
