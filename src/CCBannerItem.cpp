#include "CCBannerItem.hpp"
#include "Macros.hpp"
#include "popups/CCSelectImagePopup.hpp"
#include <Geode/modify/CCMenuItemSpriteExtra.hpp>

using namespace geode::prelude;

void CCBannerItem::activate()
{
    CCMenuItemSpriteExtra::activate();
    CCSelectImagePopup *popup = CCSelectImagePopup::create(this);
    popup->m_noElasticity = false;
    popup->show();
}
bool CCBannerItem::changeimage(std::filesystem::path path)
{
    auto oldls = m_lazysprite;
    m_lazysprite = LazySprite::create(m_size, true);
    m_lazysprite->setLoadCallback([=](Result<> res)
                                  {
						if (res) {
							log::info("Sprite loaded successfully!");
						} else {
							log::error("Failed Getting current sprite: {}", res.unwrapErr());
							initWithFile(this->m_lazysprite,"GJ_square05.png")
						} });
    m_lazysprite->setAutoResize(true);
    m_lazysprite->loadFromFile(path);
    m_lazysprite->setPosition(oldls->getPosition());
    m_lazysprite->setAnchorPoint(oldls->getAnchorPoint());
    oldls->removeFromParentAndCleanup(true);
    this->m_pNormalImage = (m_lazysprite);
    this->addChild(this->m_pNormalImage);
    return true;
}
bool CCBannerItem::pre_init(CCSize size, std::string setting)
{
    // oh i know what to do i am so smart!1!
    m_size = size;
    m_settingName = setting;
    m_lazysprite = LazySprite::create(m_size, true);
    m_lazysprite->setAutoResize(true);
    m_lazysprite->setPosition({40,40});
    m_lazysprite->setAnchorPoint({0.5,0.5});
    std::filesystem::path poster(geode::prelude::Mod::get()->getSavedValue<std::string>(setting));
    if (std::filesystem::is_regular_file(poster))
    {
        m_lazysprite->setLoadCallback([=](Result<> res)
                                      {
						if (res) {
							log::info("Sprite loaded successfully!");
						} else {
							log::error("Failed Getting current sprite: {}", res.unwrapErr());
							initWithFile(this->m_lazysprite,"GJ_square05.png")
						} });
        m_lazysprite->loadFromFile(poster);
    }
    else
    {
        initWithFile(m_lazysprite, "GJ_square05.png")
    }
    return m_lazysprite != nullptr;
};

bool CCBannerItem::save(std::filesystem::path path)
{
    geode::prelude::Mod::get()->setSavedValue<std::string>(m_settingName, path.string());
    return true;
}
CCBannerItem *CCBannerItem::create(std::string settingName, CCSize size)
{
    auto sprite = new CCBannerItem();
    // prefix
    if (sprite && sprite->pre_init(size, settingName) && sprite->init(sprite->m_lazysprite, sprite->m_lazysprite, sprite->m_lazysprite, nullptr))
    {
        // postfix
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}