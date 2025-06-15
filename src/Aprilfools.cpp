#include "time.hpp"
#include "geodehookAdditon.hpp"
#include "ImageItems/SaveSystem.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/Modify.hpp>
#include "dvd.hpp"
#include <random>
#include <Geode/modify/CreatorLayer.hpp>

using namespace geode::prelude;

class $modify(FoolsLayer, MenuLayer)
{
	static void onModify(auto &self)
	{
		auto onCreator = self.getHook("MenuLayer::onCreator").unwrapOrDefault();
		if (onCreator)
		{
			(void)onCreator->setPriority(Priority::Last);
			Hooks::items.emplace("RobtopPremium", onCreator);
			(void)onCreator->setAutoEnable(false);
			(void)onCreator->disable(); // i'm assume it doesn't but just in case
		};
		auto init = self.getHook("MenuLayer::init").unwrapOrDefault();
		if (init)
		{
			(void)init->setPriority(Priority::First);
			Hooks::items.emplace("DVD", init);
			(void)init->setAutoEnable(false);
			(void)init->disable(); // i'm assume it doesn't but just in case
		};
	}
	virtual bool init()
	{
		if (!MenuLayer::init())
			return false;

		Dvd* Dvd = Dvd::create("dvd.png"_spr);
		Dvd->setZOrder(30);
		Dvd->setID("AprilFools-Dvd"_spr);
		this->addChild(Dvd);
		return true;
	};
	void onCreator(CCObject *Sender)
	{
		if (Sender != nullptr)
		{
			if (!geode::prelude::Mod::get()->getSavedValue<bool>("AprilFoolsAccountCreation"))
			{
				createQuickPopup(
					"RobTop Premium Required",
					"Please <cr>subscribe</c> to RobTop Premium to continue using Geometry Dash <cy>PRO</c>.",
					"Cancel", "Subscribe",
					[this](auto _this, bool btn1)
					{
						if (btn1)
						{
							MenuLayer::onCreator(nullptr);
							geode::prelude::Mod::get()->setSavedValue<bool>("AprilFoolsAccountCreation", true);
						}
					},
					true);
			}
			else
			{
				createQuickPopup(
					"Login to RobTop Premium",
					"Please <cr>log in</c> to your RobTop Premium account to continue using Geometry Dash <cy>PRO</c>.",
					"Cancel", "Login",
					[this](auto _this, bool btn1)
					{
						if (btn1)
						{
							MenuLayer::onCreator(nullptr);
						}
					},
					true);
			}
		}
		else
		{
			MenuLayer::onCreator(nullptr);
		}
	}
};
int foolsCooldown = 15;
bool makeplayerseemcrazy = false;
class $modify(FoolsCreatorLayer, CreatorLayer)
{
	struct Fields
	{
		bool ResetCrazyValue = false;
	};
	static void onModify(auto &self)
	{
		auto init = self.getHook("CreatorLayer::init").unwrapOrDefault();
		if (init)
		{
			(void)init->setPriority(Priority::Last);
			Hooks::items.emplace("WrongSpots", init);
			(void)init->setAutoEnable(false);
			(void)init->disable(); // i'm assume it doesn't but just in case
		};
	}
	void onMyLevels_NoUnstable(CCObject *sender)
	{
		if (this->m_fields->ResetCrazyValue)
		{
			makeplayerseemcrazy = false;
			CCMenu *menu = typeinfo_cast<CCMenu *>(this->getChildByID("creator-buttons-menu"));
			CCMenuItemSpriteExtra *CreateButton = typeinfo_cast<CCMenuItemSpriteExtra *>(menu->getChildByID("create-button"));
			CCMenuItemSpriteExtra *SavedButton = typeinfo_cast<CCMenuItemSpriteExtra *>(menu->getChildByID("saved-button"));
			if (CreateButton && SavedButton)
			{
				this->m_fields->ResetCrazyValue = false;
				int zOrder = SavedButton->getZOrder();
				SavedButton->setZOrder(CreateButton->getZOrder());
				CreateButton->setZOrder(zOrder);
			}
			CreatorLayer::onSavedLevels(sender);
			return menu->updateLayout();
		}
		CreatorLayer::onMyLevels(sender);
	};
	void onSavedLevels_NoUnstable(CCObject *sender)
	{
		if (this->m_fields->ResetCrazyValue)
		{
			makeplayerseemcrazy = false;
			CCMenu *menu = typeinfo_cast<CCMenu *>(this->getChildByID("creator-buttons-menu"));
			CCMenuItemSpriteExtra *CreateButton = typeinfo_cast<CCMenuItemSpriteExtra *>(menu->getChildByID("create-button"));
			CCMenuItemSpriteExtra *SavedButton = typeinfo_cast<CCMenuItemSpriteExtra *>(menu->getChildByID("saved-button"));
			if (CreateButton && SavedButton)
			{
				this->m_fields->ResetCrazyValue = false;
				int zOrder = SavedButton->getZOrder();
				SavedButton->setZOrder(CreateButton->getZOrder());
				CreateButton->setZOrder(zOrder);
			}
			CreatorLayer::onMyLevels(sender);
			return menu->updateLayout();
		}
		CreatorLayer::onSavedLevels(sender);
	}
	bool init()
	{
		if (!CreatorLayer::init())
			return false;

		CCMenu *menu = typeinfo_cast<CCMenu *>(this->getChildByID("creator-buttons-menu"));
		if (menu)
		{
			auto children = menu->getChildren();
			int i = 0;
			for (CCNode *item : CCArrayExt<CCNode *>(children))
			{
				item->setZOrder(i);
				i += 1;
			}
			CCMenuItemSpriteExtra *CreateButton = typeinfo_cast<CCMenuItemSpriteExtra *>(menu->getChildByID("create-button"));
			CCMenuItemSpriteExtra *SavedButton = typeinfo_cast<CCMenuItemSpriteExtra *>(menu->getChildByID("saved-button"));
			foolsCooldown--;
			if (CreateButton && SavedButton && !makeplayerseemcrazy && foolsCooldown < 0)
			{
				this->m_fields->ResetCrazyValue = true;
				int zOrder = SavedButton->getZOrder();
				SavedButton->setZOrder(CreateButton->getZOrder());
				CreateButton->setZOrder(zOrder);
				// hooking these are unstable
				CreateButton->m_pfnSelector = menu_selector(FoolsCreatorLayer::onMyLevels_NoUnstable);
				SavedButton->m_pfnSelector = menu_selector(FoolsCreatorLayer::onSavedLevels_NoUnstable);
				foolsCooldown = 20;
			}
			CCMenuItemSpriteExtra *DailyButton = typeinfo_cast<CCMenuItemSpriteExtra *>(menu->getChildByID("daily-button"));
			CCMenuItemSpriteExtra *WeeklyButton = typeinfo_cast<CCMenuItemSpriteExtra *>(menu->getChildByID("weekly-button"));
			if (DailyButton && WeeklyButton)
			{
				int zOrder = WeeklyButton->getZOrder();
				WeeklyButton->setZOrder(DailyButton->getZOrder());
				DailyButton->setZOrder(zOrder);
			}
			menu->updateLayout();
		}
		return true;
	}
};
