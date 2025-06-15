#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

CCPoint Location = ccp(0,0);
CCPoint velocity = ccp(120, 120);

bool defData = true;
class Dvd : public CCNode
{
protected:
    CCSprite *dvdSprite;
    CCSize screenSize;
    CCDirector* shared;

public:
    static Dvd *create(std::string spr)
    {
        auto ret = new Dvd();
        if (ret && ret->init(spr))
        {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init(std::string spr)
    {
        if (!CCNode::init())
            return false;

        shared = CCDirector::sharedDirector();
        screenSize = shared->getWinSize();

        dvdSprite = CCSprite::create(spr.c_str());
        this->addChild(dvdSprite);

        if (defData) {
            Location = ccp(screenSize.width / 2, screenSize.height / 2);
            defData = false;
        };

        dvdSprite->setPosition(Location);

        this->scheduleUpdate();

        return true;
    }

    void update(float dt) override
    {
        auto pos = dvdSprite->getPosition();

        screenSize = shared->getWinSize(); // user may change size

        pos += velocity * dt;

        if (pos.x <= 0 + dvdSprite->getContentSize().width / 2 && velocity.x < 0 ||
            pos.x >= screenSize.width - dvdSprite->getContentSize().width / 2 && velocity.x > 0)
        {
            velocity.x = -velocity.x;
        }

        if (pos.y <= 0 + dvdSprite->getContentSize().height / 2 && velocity.y < 0 ||
            pos.y >= screenSize.height - dvdSprite->getContentSize().height / 2 && velocity.y > 0)
        {
            velocity.y = -velocity.y;
        }

        dvdSprite->setPosition(pos);
        Location = pos;
    }
};
