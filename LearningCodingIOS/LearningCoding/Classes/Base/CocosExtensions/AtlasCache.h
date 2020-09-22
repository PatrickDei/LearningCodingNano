//
//  AtlasCache.h
//  KnightKingdom
//
//  Created by Bruno on 3/10/14.
//
//

#ifndef __KnightKingdom__AtlasCache__
#define __KnightKingdom__AtlasCache__

#include "cocos2d.h"
#include "spine/spine-cocos2dx.h"
#include "spine/spine.h"
#include <iostream>
#include <map>

class AtlasCache
{
  private:
    std::map<std::string, spAtlas *> cache;
    std::map<std::string, int> atlasCount;

    AtlasCache();
    static AtlasCache *singleton;
    static AtlasCache *getSingleton();
    spAtlas *mGetAtlas(std::string name);
    spAtlas *mGetAtlasFromImage(std::string name);
    void mDisposeOfAtlas(std::string name);
    void mDisposeOfAtlas(spAtlas *atlas);

  public:
    static spAtlas *getAtlas(std::string name);
    static spAtlas *getAtlasFromImage(std::string name);
    static void disposeOfAtlas(std::string name);
    static void disposeOfAtlas(spAtlas *atlas);
    static void clearCache();
};

#endif /* defined(__KnightKingdom__AtlasCache__) */
