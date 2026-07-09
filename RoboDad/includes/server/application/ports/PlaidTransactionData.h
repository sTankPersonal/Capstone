#pragma once

#include <string>

struct PlaidTransactionData {
    std::string plaidTransactionId;
    double      amount;
    std::string description;
    std::string date;
    std::string category;
    std::string currencyCode;
    std::string pfcPrimary;   // personal_finance_category.primary
    std::string pfcDetailed;  // personal_finance_category.detailed
};
