#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;

class Hooks
{
public:
    static inline std::unordered_multimap<std::string, Hook*> items;
    static inline std::string lastid = "";
    static void EnableCertainHooks(std::string name) {
        lastid = name;
        for (const auto& [id, hook] : items) {
           if (id == name) {
                if (hook->isEnabled()) continue;
                (void)hook->enable();
           } else {
            if (hook->isEnabled()) (void)hook->disable();
           }
        }
    }
};



