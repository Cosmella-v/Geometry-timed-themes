#include <Geode/Geode.hpp>
#include <Geode/modify/CCSprite.hpp>
#include "CCImageSprite.hpp"

using namespace geode::prelude;

CCTexture2D* CCImageSprite::loadTexture(const std::string &imagePath)
{
    auto fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(imagePath.c_str(), false);
    auto image = new CCImage();
    if (!image->initWithImageFile(fullPath.c_str(), cocos2d::CCImage::kFmtUnKnown))
    {
        geode::prelude::log::error("Failed to load image: {}", fullPath.c_str());
        delete image;
        return nullptr;
    }
    CCTexture2D *image2D = new CCTexture2D();
    if (!image2D->initWithImage(image))
    {
        geode::prelude::log::error("Failed to create texture from image.");
        CC_SAFE_DELETE(image);
        CC_SAFE_DELETE(image2D);
        return nullptr;
    }
    CC_SAFE_DELETE(image);
    this->texture = image2D;
    return image2D;
}

CCImageSprite *CCImageSprite::createWithImage(const std::string &path)
{
    auto sprite = new CCImageSprite();
    if (sprite && sprite->loadTexture(path) && sprite->initWithTexture(sprite->texture))
    {
        sprite->autorelease();
        return sprite;
    }

    CC_SAFE_DELETE(sprite);
    return nullptr;
}
