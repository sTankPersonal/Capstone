#pragma once
#include "IRepository.h"
#include "EmploymentStatus.h"
#include "EmploymentStatusId.h"
#include <optional>
#include <string>

class IEmploymentStatusRepository : public IRepository<EmploymentStatus, EmploymentStatusId> {
public:
    virtual std::optional<EmploymentStatus> findByValue(const std::string& value) = 0;
};
