#include "time.hpp"
#include "CCImageSprite.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/Modify.hpp>

using namespace geode::prelude;

bool MidnightAlpse = false;
file::FilePickOptions::Filter textFilter;
file::FilePickOptions fileOptions;

$on_mod(Loaded)
{
	textFilter.description = "Image files";
	textFilter.files = {"*.png", "*.jpeg"};
	fileOptions.filters.push_back(textFilter);
	time::updateTime();
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
					Geode_2025->setPositionY(geodeButton->m_pNormalImage->getPositionY());
					geodeButton->m_pNormalImage->addChild(Geode_2025, 5);
					auto overlay = CCLayerColor::create({0, 0, 0, 150});
					overlay->setOpacity(0);
					overlay->runAction(CCFadeTo::create(.25f, 185));
					overlay->setZOrder(500);
					overlay->setID("overlay"_spr);
					int ZOrder = geodeButton->getZOrder();
					geodeButton->setZOrder(overlay->getZOrder() + 1);
					this->addChild(overlay);
					geodeButton->runAction(CCSequence::create(
						CCDelayTime::create(0.5f),
						CCEaseElasticOut::create(CCScaleTo::create(0.75f, 2.0f), 2.0f),
						CCTintTo::create(0.15f, 246, 255, 0),
						CCTintTo::create(0.15f, 255, 255, 255),
						CCDelayTime::create(0.1f),
						CCTintTo::create(0.15f, 246, 255, 0),
						CCTintTo::create(0.15f, 255, 255, 255),
						CCEaseElasticIn::create(CCScaleTo::create(0.871, 2), 2.0f),
						LambdaAction::create([=]()
											 {  
						geodebtn->setZOrder(ZOrder);
						overlay->removeFromParentAndCleanup(true);
						file::pick(file::PickMode::OpenFile, { Mod::get()->getResourcesDir(), { textFilter } }).listen([=](Result<std::filesystem::path>* res) {
						if (res->isOk()) {
								std::filesystem::path path = res->unwrap();
								CCImageSprite* x = CCImageSprite::createWithImage(path.string());
								this->addChild(x);

							}
						}); }),
						nullptr));
				}
			};
		}

		return true;
	}
};
