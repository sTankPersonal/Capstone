#pragma once

#include <chrono>
#include <format>
#include <string>
#include <string_view>

inline std::string dateToStr(const std::chrono::year_month_day& d) {
    return std::format("{:%Y-%m-%d}", d);
}

inline std::chrono::year_month_day dateFromStr(std::string_view s) {
    using namespace std::chrono;
    return year_month_day{
        year{std::stoi(std::string(s.substr(0, 4)))},
        month{static_cast<unsigned>(std::stoi(std::string(s.substr(5, 2))))},
        day{static_cast<unsigned>(std::stoi(std::string(s.substr(8, 2))))}
    };
}
