#pragma once
#include "UserId.h"
#include <string>

struct ImportPlaidTransactionsCommand {
    UserId userId;
    std::string accessToken;
    std::string startDate;
    std::string endDate;
    int count;

    explicit ImportPlaidTransactionsCommand(UserId userId, std::string accessToken, std::string startDate, std::string endDate, int count = 100)
        : userId(std::move(userId)), accessToken(std::move(accessToken)), startDate(std::move(startDate)), endDate(std::move(endDate)), count(count) {}
};
