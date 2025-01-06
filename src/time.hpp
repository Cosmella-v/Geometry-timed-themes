#pragma once
#include <chrono>
#include <ctime>
class timeUtil {
public:
    static inline bool JollyHats = false;
    static inline bool chimas = false;

    static void updateTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm* localTime = std::localtime(&currentTime);

        int month = localTime->tm_mon + 1;
        int day = localTime->tm_mday;

        //log::debug("the current month is {}",month);
            if (Mod::get()->getSettingValue<bool>("Winter")) {
                month = 12;
            };
            
            if (Mod::get()->getSettingValue<bool>("hats")) {
                JollyHats = (month == 12);
            }

            chimas = (month == 12 || month == 11);
        };
};