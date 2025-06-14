#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/CCMenuItemSpriteExtra.hpp>

using namespace geode::prelude;

class CCBannerItem : public CCMenuItemSpriteExtra {
    protected:
            LazySprite* m_lazysprite;
            CCSize m_size;
            std::string m_settingName;
    public:
        Ref<CCSprite> arrowref;
        CCLayerColor* m_Overlay;
        bool ShowUserHow = false;
        bool pre_init(CCSize size, std::string setting);
        bool changeimage(std::filesystem::path path);
        bool save(std::filesystem::path path);
        static CCBannerItem* create(std::string settingName, CCSize size);
        virtual void activate();
};