#pragma once
#include <string>

struct PlaidTransactionData {
    double      amount;
    std::string description;
    std::string date;         // ISO 8601 "YYYY-MM-DD"
    std::string category;     // primary Plaid category name
    std::string currencyCode; // e.g. "USD"
};
