
#ifndef __BlurSprite__
#define __BlurSprite__

#include "cocos2d.h"
using namespace cocos2d;

//blur shader is memory heavy
//use it on static elements and simple menus
class BlurSprite : public CCSprite
{
  public:
    ~BlurSprite();
    bool initWithTexture(CCTexture2D *texture, const CCRect &rect);
    void initGLProgram();
    static BlurSprite *create(const char *pszFileName);
    void setBlurRadius(float radius);
    void setBlurSampleNum(float num);

  protected:
    float _blurRadius;
    float _blurSampleNum;
    GLint m_uniformblurRadius;
    GLint m_uniformResolution;
    GLint m_uniformsampleNum;
};

#endif /* defined(__BlurSprite__) */
