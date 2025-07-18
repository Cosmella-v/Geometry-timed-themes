// geode make that warning disappear
#pragma once
#include <Geode/Geode.hpp>
class SaveSystem
{
public:
    bool writefile(std::string filename, matjson::Value data);
    matjson::Value loadfile(std::string filename);
    matjson::Value DefaultPaths = matjson::Value(std::vector<matjson::Value>{ 
        // @geode-ignore(unknown-resource)
        geode::prelude::Mod::get()->getResourcesDir() / "Transflag.png",
        // @geode-ignore(unknown-resource)
        geode::prelude::Mod::get()->getResourcesDir() / "BiFlag.png",
        // @geode-ignore(unknown-resource)
        geode::prelude::Mod::get()->getResourcesDir() / "GayFlag.png"
    }
    );
};
static SaveSystem* SaveSystem_class = new SaveSystem; 