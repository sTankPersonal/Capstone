#include "application/references/employmentStatuses/dtos/EmploymentStatusDto.h"
#include <chrono>
#include <string>

static std::string formatDate(const std::chrono::year_month_day& d) {
    return std::to_string(int(d.year())) + "-"
         + std::to_string(unsigned(d.month())) + "-"
         + std::to_string(unsigned(d.day()));
}

EmploymentStatusDto::EmploymentStatusDto(const EmploymentStatus& status)
    : id_(status.getId().getId())
    , value_(status.getValue())
    , createdAt_(formatDate(status.getCreatedAt()))
{}

EmploymentStatusDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["id"]        = id_;
    result["value"]     = value_;
    result["createdAt"] = createdAt_;
    return result;
}
