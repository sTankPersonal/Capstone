#pragma once
#include "IDto.h"
#include "EmploymentStatus.h"
#include <string>
#include "crow.h"

class EmploymentStatusDto : public IDto {
    std::string id_;
    std::string value_;
    std::string createdAt_;

public:
    explicit EmploymentStatusDto(const EmploymentStatus& status);
    explicit operator crow::json::wvalue() const override;
};
