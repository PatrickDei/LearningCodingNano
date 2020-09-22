
#include "BlurSprite.h"
#include "NanoExtensions.h"

BlurSprite::~BlurSprite() {
}

BlurSprite* BlurSprite::create(const char *pszFileName) {
    BlurSprite* pRet = new (std::nothrow) BlurSprite();
    if (pRet && pRet->initWithFile(pszFileName)) {
        pRet->autorelease();
    } else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool BlurSprite::initWithTexture(CCTexture2D* texture, const CCRect& rect) {
    _blurRadius = 0;
    if (CCSprite::initWithTexture(texture, rect)) {
        initGLProgram();
        return true;
    }
    return false;
}

void BlurSprite::initGLProgram() {
    CCGLProgram *program = getBlurShader();

    program->setUniformsForBuiltins();
    m_uniformblurRadius = glGetUniformLocation(program->getProgram(), "blurRadius");
    m_uniformResolution = glGetUniformLocation(program->getProgram(), "resolution");
    m_uniformsampleNum = glGetUniformLocation(program->getProgram(), "sampleNum");
    
    program->setUniformLocationWith2f(m_uniformResolution, getTexture()->getContentSizeInPixels().width, getTexture()->getContentSizeInPixels().height);
    program->setUniformLocationWith1f(m_uniformblurRadius, _blurRadius);
    program->setUniformLocationWith1f(m_uniformsampleNum, _blurSampleNum);
    
    setShaderProgram(program);
}

void BlurSprite::setBlurRadius(float radius) {
    _blurRadius = radius;
    getShaderProgram()->setUniformLocationWith1f(m_uniformblurRadius, _blurRadius);
}

void BlurSprite::setBlurSampleNum(float num) {
    _blurSampleNum = num;
    getShaderProgram()->setUniformLocationWith1f(m_uniformsampleNum, _blurSampleNum);
}
