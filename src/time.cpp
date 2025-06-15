#include "time.hpp"

std::unordered_map<std::string, bool> time::Enabled;
std::vector<Prank> time::allPranks = {
        //{"WrongSpots", 0},
        //{"RobtopPremium", 0},
        {"DVD",0}
    };
Prank time::Settings_Year;