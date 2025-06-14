#include "SaveSystem.hpp"
#include "../Macros.hpp"
#include <Geode/Geode.hpp>


bool SaveSystem::writefile(std::string filename, matjson::Value data)
{
    geode::prelude::Mod::get()->setSavedValue<matjson::Value>(filename, data);
    return true;
}
matjson::Value SaveSystem::loadfile(std::string filename)
{
    
    return geode::prelude::Mod::get()->getSavedValue<matjson::Value>(filename);
} 
