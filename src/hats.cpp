#include <Geode/Geode.hpp>
#include "time.hpp"
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/SimplePlayer.hpp>

using namespace geode::prelude;

CCSprite* JollyHat(CCSprite* SpritePlacement) {
		CCSprite* Hat =  CCSprite::create("JollyHat.png"_spr);
		Hat->setAnchorPoint({0.05,0});
		Hat->setScale(0.8);
		Hat->setPosition(ccp(-8,SpritePlacement->getContentHeight() / 2) );
		return Hat;
	}

class $modify(SimplePlayer) {
	struct Fields {
		CCSprite* m_jollyhat;
	};
	void updatePlayerFrame(int p0,IconType icon) {
		SimplePlayer::updatePlayerFrame(p0,icon);
		if (!this->m_fields->m_jollyhat) {
			return;
		};
		CCSprite* sp = this->m_firstLayer;
		this->m_fields->m_jollyhat->setParent(sp);
		this->m_fields->m_jollyhat->setAnchorPoint({0.05,0});
		this->m_fields->m_jollyhat->setScale(0.8);
        this->m_fields->m_jollyhat->setVisible(true);
		this->m_fields->m_jollyhat->setPosition( ccp(-8,sp->getContentHeight() / 2) );
		switch (icon) {
    	case IconType::Ball:
			this->m_fields->m_jollyhat->setPosition(ccp(-4,sp->getContentHeight() / 2) );
			break;
		case IconType::Wave:
			this->m_fields->m_jollyhat->setVisible(false);
			break;
		case IconType::Swing:
			this->m_fields->m_jollyhat->setScale(0.75);
			this->m_fields->m_jollyhat->setPosition(ccp(0,sp->getContentHeight() / 2) );
			break;
		case IconType::Ship:
		case IconType::Jetpack:
		case IconType::Ufo:
			this->m_fields->m_jollyhat->setVisible(false);
			break;
		default:
			break;
	}

	};
	bool init(int p0) {
		bool in = SimplePlayer::init(p0);
		if (!in || !time::IsEnabled("WinterHats")) {
			return in;
		};
		CCSprite* sprite = this->m_firstLayer;
		this->m_fields->m_jollyhat = JollyHat(sprite);
		sprite->addChild(this->m_fields->m_jollyhat);
		return in;
	};
};




class $modify(PlayerObject) {
	struct Fields {
		CCSprite* m_jollyhat;
	};
	int getCurrentPlayerState() {
        if(this->isInNormalMode()) return 0;
        else if(this->m_isShip) return 1;
        else if(this->m_isBall) return 2;
        else if(this->m_isBird) return 3;
        else if(this->m_isDart) return 4;
        else if(this->m_isRobot) return 5;
        else if(this->m_isSpider) return 6;
        else if(this->m_isSwing) return 7;

        return -1;
    }
	virtual void update(float p0) {
		PlayerObject::update(p0);
        if (!this->m_fields->m_jollyhat) {
            return;
        };
		CCSprite* sp = this->m_iconSprite;
		this->m_fields->m_jollyhat->setParent(sp);
		this->m_fields->m_jollyhat->setAnchorPoint({0.05,0});
		this->m_fields->m_jollyhat->setScale(0.8);
        this->m_fields->m_jollyhat->setVisible(true);
		this->m_fields->m_jollyhat->setPosition( ccp(-8,sp->getContentHeight() / 2) );
		switch (this->getCurrentPlayerState()) {
    	case 2:
			this->m_fields->m_jollyhat->setPosition(ccp(-4,sp->getContentHeight() / 2) );
			break;
		case 4:
			this->m_fields->m_jollyhat->setVisible(false);
			break;
		case 7:
			this->m_fields->m_jollyhat->setScale(0.75);
			this->m_fields->m_jollyhat->setPosition(ccp(0,sp->getContentHeight() / 2) );
			break;
		default:
			break;
	}

	};

	bool init(int p0, int p1, GJBaseGameLayer* p2, CCLayer* p3, bool p4) {
		bool in = PlayerObject::init(p0,p1,p2,p3,p4);
		if (!in || !time::IsEnabled("WinterHats")) {
			return in;
		};
		CCSprite* sp = this->m_iconSprite;
		CCSpriteBatchNode* sp223 = this->m_robotBatchNode;
		CCSpriteBatchNode* sp23 = this->m_spiderBatchNode;
		this->m_fields->m_jollyhat = JollyHat(sp);
		sp->addChild(this->m_fields->m_jollyhat);
		return in;
	};
};