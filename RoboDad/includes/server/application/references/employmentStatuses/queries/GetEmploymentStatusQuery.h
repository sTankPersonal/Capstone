#pragma once
#include "EmploymentStatusId.h"

struct GetEmploymentStatusQuery {
    EmploymentStatusId id;

    explicit GetEmploymentStatusQuery(EmploymentStatusId id)
        : id(std::move(id)) {}
};
