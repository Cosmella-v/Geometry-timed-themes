#pragma once
#include <chrono>
#include <ctime>
#include "Macros.hpp"

class time
{
public:
    static std::unordered_map<std::string, bool> Enabled;
    static bool IsEnabled(std::string Time)
    {
        if (M_get(bool, Time))
        {
            return Enabled.find(Time) != Enabled.end() && Enabled[Time] || false;
        }
        else
        {
            return false;
        }
    }
    static void updateTime()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm *localTime = std::localtime(&currentTime);

        int month = localTime->tm_mon + 1;
        int day = localTime->tm_mday;
        geode::log::debug("month {}", month);

        if (M_get(bool, "Force-Winter"))
        {
            month = 12;
        };
        // winter
        Enabled["Winter"] = (month == 12 || month == 11);
        Enabled["WinterHats"] = Enabled["Winter"];
        Enabled["Menu-Snow"] = Enabled["Winter"];

        if (M_get(bool, "Force-Pride-mode"))
        {
            month = 6;
        };

        // Pride Mode
        Enabled["Force-Pride-mode"] = month == 6;
        Enabled["Pride-Icon"] = Enabled["Force-Pride-mode"];
    };
};