//
//  SpriteHole.cpp
//  Sanji
//
//  Created by midchow on 2014. 12. 18..
//
//

#include "SpriteHole.h"

USING_NS_CC;

SpriteHole::~SpriteHole()
{
}

SpriteHole* SpriteHole::create(const char *pszFileName)
{
    SpriteHole* pRet = new (std::nothrow) SpriteHole();
    if (pRet && pRet->initWithFile(pszFileName))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool SpriteHole::initWithTexture(Texture2D* texture, const Rect& rect)
{
    if( Sprite::initWithTexture(texture, rect) )
    {
#if CC_ENABLE_CACHE_TEXTURE_DATA
        auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event){
            setGLProgram(nullptr);
            initGLProgram();
        });
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif
        
        initGLProgram();
        
        return true;
    }
    
    return false;
}

void SpriteHole::initGLProgram()
{
    GLchar * fragSource = (GLchar*) String::createWithContentsOfFile(
                             FileUtils::getInstance()->fullPathForFilename("Shaders/hole.fsh").c_str())->getCString();
    auto program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource);
    
    auto glProgramState = GLProgramState::getOrCreateWithGLProgram(program);
    setGLProgramState(glProgramState);
    
    size = getTexture()->getContentSizeInPixels();
    _holeAlpha = 0;

    getGLProgramState()->setUniformFloat("holeRadius", 0.5*0.5);
    getGLProgramState()->setUniformVec2("centerPos", Vec2(0.5, 0.5));
    getGLProgramState()->setUniformVec4("inLineColor", Vec4(0.58823529411, 0.22352941176, 0.72549019607, 1)); // 150, 57, 185
    getGLProgramState()->setUniformVec4("outLineColor", Vec4(0.81960784313, 0.41960784313, 0.9725490196, 1)); // 209, 107, 248
    getGLProgramState()->setUniformFloat("holeAlpha", _holeAlpha);
}

void SpriteHole::setHoleAlpha(float a)
{
    _holeAlpha = a;
    getGLProgramState()->setUniformFloat("holeAlpha", _holeAlpha);
}