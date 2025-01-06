#include <Geode/Geode.hpp>

using namespace geode::prelude;
#include "time.hpp"
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CCScheduler.hpp>

$on_mod(Loaded) {
	timeUtil::updateTime();
}

class $modify(MyCCScheduler, CCScheduler) {
	static void onModify(auto& self) {
		(void) self.setHookPriority("CCScheduler::update", 1);
	}
	void update(float dt) {
		CCScheduler::update(dt);
		timeUtil::updateTime(); // update time every fram
	}
};


class $modify (MenuLayer)
{
    virtual bool init()
    {
        if (!MenuLayer::init())
            return false;
           
		   if (timeUtil::chimas && (Mod::get()->getSettingValue<bool>("snow")) ) {
		    auto snow = CCParticleSnow::create();
            snow->setID("snow"_spr);
            this->addChild(snow, 420);
		   }

        return true;
    }
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
		CCSprite* sp = this->m_iconSprite;
		this->m_fields->m_jollyhat->setParent(sp);
		this->m_fields->m_jollyhat->setAnchorPoint({0.05,0});
		this->m_fields->m_jollyhat->setScale(0.8);
		this->m_fields->m_jollyhat->setVisible(timeUtil::JollyHats);
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
	CCSprite* createjoll() {
		CCSprite* sp = this->m_iconSprite;
		CCSprite* re =  CCSprite::create("JollyHat.png"_spr);
		re->setAnchorPoint({0.05,0});
		re->setScale(0.8);
		re->setPosition(ccp(-8,sp->getContentHeight() / 2) );
		re->setVisible(timeUtil::JollyHats);
		return re;
	}
	bool init(int p0, int p1, GJBaseGameLayer* p2, CCLayer* p3, bool p4) {
		bool in = PlayerObject::init(p0,p1,p2,p3,p4);
		if (!in) {
			return in;
		};
		CCSprite* sp = this->m_iconSprite;
		CCSpriteBatchNode* sp223 = this->m_robotBatchNode;
		CCSpriteBatchNode* sp23 = this->m_spiderBatchNode;
		this->m_fields->m_jollyhat = createjoll();
		sp->addChild(this->m_fields->m_jollyhat);
		return in;
	};
};