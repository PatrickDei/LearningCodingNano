//
//  AtlasCache.cpp
//  KnightKingdom
//
//  Created by Bruno on 3/10/14.
//
//

#include "AtlasCache.h"

using namespace std;
using namespace cocos2d;

AtlasCache *AtlasCache::singleton = nullptr;

AtlasCache *AtlasCache::getSingleton()
{
    if(!singleton) {
        singleton = new AtlasCache();
    }
    return singleton;
}

AtlasCache::AtlasCache()
{
    cache = map<string, spAtlas *>();
    atlasCount = map<string, int>();
}

spAtlas *AtlasCache::mGetAtlas(std::string name)
{
    spAtlas *existing = cache[name];
    if(!existing) {
        CCLOG("Creating atlas %s", name.c_str());
        existing = spAtlas_createFromFile(name.c_str(), nullptr);
        cache[name] = existing;
        if(existing == nullptr) {
            CCLOG("Missing atlas %s", name.c_str());
        }
    }
    if(existing) {
        atlasCount[name] = atlasCount[name] + 1;
    }
    return existing;
}

spAtlas *AtlasCache::mGetAtlasFromImage(std::string name)
{
    spAtlas *existing = cache[name];
    if(!existing) {
        CCLOG("Creating atlas from image %s", name.c_str());
        if(CCTexture2D *tex = CCTextureCache::sharedTextureCache()->addImage(name.c_str())) {
            char buffer[256];
            sprintf(buffer, "%s\nformat: RGBA8888\nfilter: Linear,Linear\nrepeat: "
                            "none\n%s\n  rotate: false\n  xy: 0, 0\n  size: %d, %d\n "
                            " orig: %d, %d\n  offset: 0, 0\n  index: -1",
                    name.c_str(), name.c_str(), tex->getPixelsWide(), tex->getPixelsHigh(), tex->getPixelsWide(),
                    tex->getPixelsHigh());

            existing = spAtlas_create(buffer, strlen(buffer), "", nullptr);
            cache[name] = existing;
        }
    }
    if(existing) {
        atlasCount[name] = atlasCount[name] + 1;
    }
    return existing;
}

void AtlasCache::mDisposeOfAtlas(std::string name)
{
    int prevCount = atlasCount[name];
    prevCount--;
    if(prevCount == 0) {
        CCLOG("Removing atlas %s", name.c_str());
        spAtlas *toRemove = cache[name];
        cache.erase(name);
        spAtlas_dispose(toRemove);
    } else if(prevCount < 0) {
        CCLog("Atlas cache error (%s)!", name.c_str());
        prevCount = 0;
    }
    atlasCount[name] = prevCount;
}

void AtlasCache::mDisposeOfAtlas(spAtlas *atlas)
{
    std::string name = "";
    for(auto cacheEl : cache) {
        if(cacheEl.second == atlas) {
            name = cacheEl.first;
            break;
        }
    }
    if(name.length() > 0) {
        mDisposeOfAtlas(name);
    } else {
        CCLog("Atlas not in cache.");
    }
}

spAtlas *AtlasCache::getAtlas(std::string name) { return getSingleton()->mGetAtlas(name); }

spAtlas *AtlasCache::getAtlasFromImage(std::string name) { return getSingleton()->mGetAtlasFromImage(name); }

void AtlasCache::disposeOfAtlas(std::string name) { getSingleton()->mDisposeOfAtlas(name); }

void AtlasCache::disposeOfAtlas(spAtlas *atlas) { getSingleton()->mDisposeOfAtlas(atlas); }

void AtlasCache::clearCache(){
    getSingleton()->cache.clear();
    getSingleton()->atlasCount.clear();
}
