#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/web.hpp>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
using namespace geode::prelude;

class AutoScaleCCLabelBMFont : public cocos2d::CCLabelBMFont
{
public:
	float startsize = .5f;

	static AutoScaleCCLabelBMFont *create(const char *str, const char *fntFile, float width, float height)
	{
		AutoScaleCCLabelBMFont *label = new AutoScaleCCLabelBMFont();
		if (label && label->initWithString(str, fntFile, width, kCCTextAlignmentLeft, cocos2d::CCPointZero))
		{
			label->autorelease();
			label->maxHeight = height;
			return label;
		}
		CC_SAFE_DELETE(label);
		return nullptr;
	}

	virtual void setString(const char *labelText) override
	{
		CCLabelBMFont::setString(labelText);
		float scaleY = maxHeight / this->getContentHeight();
		if (scaleY < 1)
		{
			this->setScale(scaleY * startsize);
		}
		else
		{
			this->setScale(startsize);
		}

		scaleY = maxHeight / this->getContentHeight();
		if (scaleY < 1)
		{
			this->setScale(scaleY * startsize);
		}
	}

protected:
	float maxHeight;
};

class CCImageSelectNode : public CCLayerColor
{
protected:
		Ref<CCSprite> arrowrefen;
		CCMenuItemSpriteExtra* m_confirm;
public:
	CCLabelBMFont *Text;
	CCMenu *_Apply_Menu;
	CCMenu *m_Icon;
	std::string m_name = "";
	std::function<void()> selectionobject;
	std::function<void()> m_garbcan;
	std::filesystem::path m_filePath;

	std::string getName()
	{
		return m_name;
	};
	void selected(CCObject *)
	{
		if (selectionobject)
		{
			selectionobject();
		}
	}
	void canhit(CCObject *)
	{
		if (m_garbcan)
		{
			m_garbcan();
		}
	}
	void CreateArrow()
	{
		arrowrefen = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
		arrowrefen->setPosition(m_confirm->getContentSize() / 2.f - ccp(40, 0));
		arrowrefen->setRotation(0);
		arrowrefen->setScale(0.5);
		arrowrefen->runAction(CCRepeatForever::create(
			CCSequence::createWithTwoActions(
				CCMoveBy::create(0.3f, ccp(15, 0)),
				CCMoveBy::create(0.3f, ccp(-15.f, 0)))));
		arrowrefen->setZOrder(12);
		m_confirm->addChild(arrowrefen);
	};

	bool init(matjson::Value x)
	{
		if (!CCLayerColor::init())
			return false;

		
		if (!x.isString() || !std::filesystem::is_regular_file(x.asString().unwrap()))
		{
			return true;
		}
		std::filesystem::path file(x.asString().unwrap());
		this->setContentSize(ccp(390, 35));
		this->setAnchorPoint(ccp(0, 1));
		this->setPositionY(207);
		this->setOpacity(100);
		m_filePath = file;
		m_name = m_filePath.filename().string().c_str();
		Text = CCLabelBMFont::create(m_name.c_str(), "bigFont.fnt",350,CCTextAlignment::kCCTextAlignmentLeft);
        Text->limitLabelWidth(150, 1, .001f);
        Text->setAnchorPoint({ .5f, .5f });
		Text->setID("name-label");
		Text->setLayoutOptions(AxisLayoutOptions::create()->setScalePriority(1));
		this->addChildAtPosition(Text, Anchor::Left, ccp(0, 0.5), ccp(0, 0.5f));

		CCLayerGradient *gradient = CCLayerGradient::create(ccc4(0, 0, 0, 100), ccc4(0, 0, 0, 100));
		gradient->setContentSize(this->getContentSize());
		gradient->setZOrder(-3);
		gradient->setVector(ccp(90, 0));
		this->addChild(gradient);
		this->setOpacity(0);
		m_confirm = CCMenuItemSpriteExtra::create(ButtonSprite::create("Set", 40.f, true, "bigFont.fnt", "GJ_button_01.png", 20.f, 1.0f), this, menu_selector(CCImageSelectNode::selected));

		_Apply_Menu = CCMenu::create();
		_Apply_Menu->setID("apply");
		_Apply_Menu->ignoreAnchorPointForPosition(false);
		_Apply_Menu->addChild(m_confirm);
		_Apply_Menu->setLayout(RowLayout::create()
								   ->setAxisAlignment(AxisAlignment::Start)
								   ->setCrossAxisLineAlignment(AxisAlignment::Start)
								   ->setCrossAxisAlignment(AxisAlignment::Start));
		_Apply_Menu->setContentSize(m_confirm->getContentSize());
		_Apply_Menu->setPosition(ccp(this->getContentSize().width - m_confirm->getContentSize().width / 1.5, this->getContentSize().height / 2));
		_Apply_Menu->updateLayout();
		this->addChild(_Apply_Menu);

		auto trashSpr = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
		trashSpr->setScale(0.75);
        auto trashBtn = CCMenuItemSpriteExtra::create(
            trashSpr, this, menu_selector(CCImageSelectNode::canhit)
        );
		m_Icon = CCMenu::create();
		m_Icon->setID("Other-buttons");
		m_Icon->addChild(trashBtn);
		m_Icon->ignoreAnchorPointForPosition(false);
		m_Icon->setLayout(RowLayout::create()
								   ->setAxisAlignment(AxisAlignment::Start)
								   ->setCrossAxisLineAlignment(AxisAlignment::Start)
								   ->setCrossAxisAlignment(AxisAlignment::Start));
		m_Icon->setContentSize(m_confirm->getContentSize());
		m_Icon->setPosition(ccp(this->getContentSize().width - (_Apply_Menu->getContentSize().width + m_Icon->getContentSize().width), this->getContentSize().height / 2));
		m_Icon->updateLayout();
		this->addChild(m_Icon);

		auto LazySprite = LazySprite::create({35.f, 35.f}, true);
		LazySprite->ignoreAnchorPointForPosition(false);
		LazySprite->setAutoResize(true);
		LazySprite->loadFromFile(m_filePath);
		LazySprite->setPosition(ccp(this->getContentSize().width - m_confirm->getContentSize().width * 3.5, this->getContentSize().height / 2));
		this->addChild(LazySprite);
		_Apply_Menu->setAnchorPoint({0.5, 0.5});
		return true;
	}
	static CCImageSelectNode *create(matjson::Value x)
	{
		CCImageSelectNode *pRet = new CCImageSelectNode();
		if (pRet && pRet->init(x))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			return nullptr;
		}
	};
};