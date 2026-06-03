#pragma once

#include <string>

struct PlaidTransactionData {
    double      amount;
    std::string description;
    std::string date;
    std::string category;
    std::string currencyCode;
};
