#pragma once
#include <Geode/Geode.hpp>

#define M_get(type, key_name) geode::prelude::Mod::get()->getSettingValue<type>(key_name)

class LambdaAction : public cocos2d::CCActionInstant {
public:
    static LambdaAction* create(const std::function<void()>& func) {
        LambdaAction* ret = new LambdaAction();
        if (ret && ret->initWithFunction(func)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    bool initWithFunction(const std::function<void()>& func) {
        m_function = func;
        return true;
    }

    virtual void update(float time) override {
        if (m_function) m_function();
    }

private:
    std::function<void()> m_function;
};

