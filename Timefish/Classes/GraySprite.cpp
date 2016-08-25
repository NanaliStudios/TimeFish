//
//  GraySprite.cpp
//  Timefish
//
//  Created by midchow on 2015. 7. 26..
//
//

#include "GraySprite.h"

GraySprite::GraySprite()
: _glprogramstate(nullptr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    _backgroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
                                          [this](EventCustom*)
                                          {
                                              auto glProgram = _glprogramstate->getGLProgram();
                                              glProgram->reset();
                                              glProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, _fragSource.c_str());
                                              glProgram->link();
                                              glProgram->updateUniforms();
                                          }
                                          );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif
}
GraySprite* GraySprite::create(const std::string& filename)
{
    GraySprite *sprite = new (std::nothrow) GraySprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->initGLProgram();
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool GraySprite::initWithSpriteFrame(SpriteFrame *spriteFrame)
{
    CCASSERT(spriteFrame != nullptr, "");
    
    bool bRet = initWithTexture(spriteFrame->getTexture(), spriteFrame->getRect());
    setSpriteFrame(spriteFrame);
    
    return bRet;
}

GraySprite* GraySprite::createWithSpriteFrame(SpriteFrame *spriteFrame)
{
    GraySprite *sprite = new (std::nothrow) GraySprite();
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->initGLProgram();
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

GraySprite* GraySprite::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
    CCASSERT(frame != nullptr, msg);
#endif
    
    return createWithSpriteFrame(frame);
}

void GraySprite::initGLProgram()
{
    if (false) {
        setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
    }
    else {
        auto fileUtiles = FileUtils::getInstance();
        auto fragmentFullPath = fileUtiles->fullPathForFilename("Shaders/example_greyScale.fsh");
        auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
        auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
        _fragSource = fragSource;
#endif

        _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
        _glprogramstate->retain();
        setGLProgramState(_glprogramstate);

        grayed = true;
        getGLProgramState()->setUniformFloat("u_BlendValue", 0); // zero for pure gray
        setColorRate();
    }
}

void GraySprite::setColorRate(float mult)
{
    //
    // value over 1.0 goes to white
    // value under 1.0 goes to black.
    //
    getGLProgramState()->setUniformVec3("colorRate", Vec3(0.2126, 0.7152, 0.0722) * mult);
}

void GraySprite::setGrayed()
{
    removeAllChildren();
    grayed = true;
    getGLProgramState()->setUniformFloat("u_BlendValue", 0); // zero for pure gray
}

void GraySprite::setColored()
{
    grayed = false;
    getGLProgramState()->setUniformFloat("u_BlendValue", 1); // 1 for pure color
}

GraySprite::~GraySprite()
{
    CC_SAFE_RELEASE_NULL(_glprogramstate);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    Director::getInstance()->getEventDispatcher()->removeEventListener(_backgroundListener);
#endif
}
