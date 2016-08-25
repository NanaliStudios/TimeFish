//
//  SpriteFlow.cpp
//  Sanji
//
//  Created by midchow on 2014. 11. 6..
//
//

#include "SpriteFlow.h"

USING_NS_CC;

SpriteFlow::~SpriteFlow()
{
}

SpriteFlow* SpriteFlow::create(const char *pszFileName)
{
    SpriteFlow* pRet = new (std::nothrow) SpriteFlow();
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

bool SpriteFlow::initWithTexture(Texture2D* texture, const Rect& rect)
{
    _flowCountX = 0;
    _flowCountY = 0;
    deltaX = 0;
    deltaY = 0;

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

void SpriteFlow::tick(float dt)
{
    auto size = getTexture()->getContentSizeInPixels();

    // X-axis
    _flowCountX += deltaX;
    if (_flowCountX >  size.width) {
        _flowCountX = 0;
    }
    else if (_flowCountX <= 0) {
        _flowCountX = size.width;
    }

    // Y-axis
    _flowCountY += deltaY;
    if (_flowCountY >  size.height) {
        _flowCountY = 0;
    }
    else if (_flowCountY <= 0) {
        _flowCountY = size.height;
    }

    // set flow control
    setFlowCountXY(_flowCountX/size.width, _flowCountY/size.height);
}

void SpriteFlow::initGLProgram()
{
    GLchar * fragSource = (GLchar*) String::createWithContentsOfFile(
                            FileUtils::getInstance()->fullPathForFilename("Shaders/flow.fsh").c_str())->getCString();
    auto program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource);
    
    auto glProgramState = GLProgramState::getOrCreateWithGLProgram(program);
    setGLProgramState(glProgramState);
    
    auto size = getTexture()->getContentSizeInPixels();
    getGLProgramState()->setUniformFloat("flowCountX", _flowCountX);
    getGLProgramState()->setUniformFloat("flowCountY", _flowCountY);

    schedule(CC_SCHEDULE_SELECTOR(SpriteFlow::tick));
}

void SpriteFlow::setFlowCountXY(float numX, float numY)
{
    getGLProgramState()->setUniformFloat("flowCountX", numX);
    getGLProgramState()->setUniformFloat("flowCountY", numY);
}

void SpriteFlow::setDelta(Vec2 delta)
{
    deltaX = delta.x;
    deltaY = delta.y;
}
