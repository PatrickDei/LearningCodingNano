/*******************************************************************************
 * Copyright (c) 2013, Esoteric Software
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include <spine/spine-cocos2dx.h>
#include <spine/extension.h>

USING_NS_CC;

GLuint wrap (spAtlasWrap wrap) {
    return wrap == SP_ATLAS_CLAMPTOEDGE ? GL_CLAMP_TO_EDGE : GL_REPEAT;
}

GLuint filter (spAtlasFilter filter) {
    switch (filter) {
        case SP_ATLAS_NEAREST:
            return GL_NEAREST;
        case SP_ATLAS_LINEAR:
            return GL_LINEAR;
        case SP_ATLAS_MIPMAP:
            return GL_LINEAR_MIPMAP_LINEAR;
        case SP_ATLAS_MIPMAP_NEAREST_NEAREST:
            return GL_NEAREST_MIPMAP_NEAREST;
        case SP_ATLAS_MIPMAP_LINEAR_NEAREST:
            return GL_LINEAR_MIPMAP_NEAREST;
        case SP_ATLAS_MIPMAP_NEAREST_LINEAR:
            return GL_NEAREST_MIPMAP_LINEAR;
        case SP_ATLAS_MIPMAP_LINEAR_LINEAR:
            return GL_LINEAR_MIPMAP_LINEAR;
        case SP_ATLAS_UNKNOWN_FILTER:
            break;
    }
    return GL_LINEAR;
}

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path) {
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(path);
    texture->retain();
    
    ccTexParams textureParams = {filter(self->minFilter), filter(self->magFilter), wrap(self->uWrap), wrap(self->vWrap)};
    texture->setTexParameters(&textureParams);
    
    self->rendererObject = texture;
    self->width = texture->getPixelsWide();
    self->height = texture->getPixelsHigh();
}

void _spAtlasPage_disposeTexture (spAtlasPage* self) {
    ((CCTexture2D*)self->rendererObject)->release();
}

char* _spUtil_readFile (const char* path, int* length) {
    unsigned long size;
    char* data = reinterpret_cast<char*>(CCFileUtils::sharedFileUtils()->getFileData(
                                                                                     CCFileUtils::sharedFileUtils()->fullPathForFilename(path).c_str(), "r", &size));
    *length = size;
    return data;
}
