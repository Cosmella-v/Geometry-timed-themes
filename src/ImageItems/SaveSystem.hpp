#pragma once
#include <Geode/Geode.hpp>

class SaveSystem
{
public:
    bool writefile(std::string filename, matjson::Value data);
    matjson::Value loadfile(std::string filename);
    matjson::Value DefaultPaths = matjson::Value(std::vector<matjson::Value>{ 
        geode::prelude::Mod::get()->getResourcesDir() / "Transflag.png",
        geode::prelude::Mod::get()->getResourcesDir() / "BiFlag.png",
        geode::prelude::Mod::get()->getResourcesDir() / "GayFlag.png"
    }
    );
};
static SaveSystem* SaveSystem_class = new SaveSystem; 