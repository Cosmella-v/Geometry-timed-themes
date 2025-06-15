#pragma once
#include <chrono>
#include <ctime>
#include "geodehookAdditon.hpp"
#include "Macros.hpp"
#include <iostream>
#include <vector>
#include <ctime>
#include <functional>
#include <random>
#include <algorithm>

struct Prank {
    std::string id;
    int year;
};

class time
{
public:
    static std::unordered_map<std::string, bool> Enabled;
    static Prank Settings_Year;
    static std::vector<Prank> allPranks;
    static void aprilFoolsMode(int year, bool HalfWay) {
        std::vector<Prank> available;
        for (const auto& prank : allPranks) {
            if (year >= prank.year)
                available.push_back(prank);
        }

        if (available.empty()) {
            return;
        }
        if (!HalfWay) {
            year-=1;
        };
        int firstYear = available.front().year;
        int offset = year - firstYear;
        int count = available.size();

        std::vector<int> indices(count);
        for (int i = 0; i < count; ++i) indices[i] = i;

        std::mt19937 rng(1337 + count);
        std::shuffle(indices.begin(), indices.end(), rng);

        int prankIndex = indices[offset % count];
        Settings_Year = available[prankIndex];
    }
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
        
        // april fools mode
        if ((month == 5 && day == 15 && M_get(bool,"AprilFools")) || M_get(bool,"Force-April-Fools")) {
                Enabled["AprilFools"] = true;
                aprilFoolsMode(localTime->tm_year, month > 5);
                if (Hooks::lastid != Settings_Year.id) {
                    Hooks::EnableCertainHooks(Settings_Year.id);
                }
        } else {
            Enabled["AprilFools"] = false;
            if (Hooks::lastid != "") {
                Hooks::EnableCertainHooks("");
            }
        };


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