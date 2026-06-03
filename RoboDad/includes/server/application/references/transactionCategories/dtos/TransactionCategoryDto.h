#pragma once
#include "IDto.h"
#include "TransactionCategory.h"
#include <string>
#include "crow.h"

class TransactionCategoryDto : public IDto {
    std::string id_;
    std::string value_;
    std::string createdAt_;

public:
    explicit TransactionCategoryDto(const TransactionCategory& category);
    explicit operator crow::json::wvalue() const override;
};
