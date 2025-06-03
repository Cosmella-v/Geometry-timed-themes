#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/CCSprite.hpp>

using namespace geode::prelude;

class CCImageSprite : public CCSprite
{
protected:
    CCTexture2D *texture;
    CCTexture2D *loadTexture(const std::string &imagePath);

public:
    static CCImageSprite *createWithImage(const std::string &path);
};
