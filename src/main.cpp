#include "time.hpp"
#include "CCBannerItem.hpp"
#include "geodehookAdditon.hpp"
#include "ImageItems/SaveSystem.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/Modify.hpp>
#include <random>
#include <Geode/modify/CreatorLayer.hpp>

using namespace geode::prelude;

$on_mod(Loaded)
{

	time::updateTime();
	// auto obj = matjson::makeObject({std::make_pair("Paths", matjson::Value(std::vector<matjson::Value>{
	//															matjson::Value("C:/Users/momo2/Downloads/Yipeee.png")}))});
	// SaveSystem_class->writefile("SavedImages", obj);
}

class $modify(CustomLayer, MenuLayer)
{
	virtual bool init()
	{
		if (!MenuLayer::init())
			return false;

		time::updateTime(); // update the time before all the menu stuff

		// snow check
		if (time::IsEnabled("Menu-Snow"))
		{
			// other mods adding snow
			if (!this->getChildByType<CCParticleSnow>(0))
			{
				auto snow = CCParticleSnow::create();
				snow->setID("snow"_spr);
				this->addChild(snow, 420);
			}
		}
		// pride icon
		if (time::IsEnabled("Pride-Icon"))
		{
			// do i exist?
			if (auto geodebtn = this->getChildByIDRecursive("geode.loader/geode-button"))
			{
				// covert this to my fav type
				CCMenuItemSpriteExtra *geodeButton = typeinfo_cast<CCMenuItemSpriteExtra *>(
					geodebtn);
				// no crashing plz thanks
				if (geodeButton->m_pNormalImage)
				{
					CCSprite *Geode_2025 = CCSprite::create("Geode-2025.png"_spr);
					Geode_2025->setScale(0.2);
					Geode_2025->setPositionX(geodeButton->m_pNormalImage->getPositionX());
					Geode_2025->setPositionY(geodeButton->m_pNormalImage->getPositionY() * 1.05);
					geodeButton->m_pNormalImage->addChild(Geode_2025, 69);
				}
			}
		}
		if (time::IsEnabled("Pride-Icon") && M_get(bool, "Posters") || M_get(bool, "Force-Posters"))
		{
			CCMenu *Banners = CCMenu::create();
			Banners->setPosition({0, 0});
			Banners->setID("Banners"_spr);
			CCSize BannerArea = Banners->getContentSize();
			this->addChild(Banners, 10);
			CCBannerItem *Banner1 = CCBannerItem::create("Banner1", {80.f, 80.f});
			Banner1->setPosition({(Banner1->getContentWidth() * Banner1->m_scaleMultiplier) / 2, Banner1->getContentHeight() / 2 + BannerArea.height / 2});
			Banners->addChild(Banner1, 5);
			if (!geode::prelude::Mod::get()->getSavedValue<bool>("Pride_HowtoUsePoster"))
			{
				Banner1->ShowUserHow = true;
				Loader::get()->queueInMainThread([=]
												 {  
							Banner1->m_Overlay = CCLayerColor::create({0, 0, 0, 150});
							Banner1->m_Overlay->setOpacity(0);
							Banner1->m_Overlay->runAction(CCFadeTo::create(.25f, 185));
							Banner1->m_Overlay->setZOrder(9);
							Banner1->m_Overlay->setID("overlay"_spr);
							this->addChild(Banner1->m_Overlay);
							geode::prelude::Mod::get()->setSavedValue<bool>("Pride_HowtoUsePoster", true);
							if (Banner1->ShowUserHow) {
								Banner1->arrowref = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
								Banner1->arrowref->setPosition(Banner1->getContentSize() / 2.f + ccp(40, 0));
								Banner1->arrowref->setRotation(180);
								Banner1->arrowref->runAction(CCRepeatForever::create(
									CCSequence::createWithTwoActions(
										CCMoveBy::create(0.3f, ccp(15, 0)),
										CCMoveBy::create(0.3f, ccp(-15.f, 0))
									)
								));
								Banner1->arrowref->setZOrder(12);
								Banner1->addChild(Banner1->arrowref);
							} });
			};
		}

		return true;
	}
};
