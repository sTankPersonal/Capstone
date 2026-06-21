#pragma once
#include "PfcDetailedCategoryId.h"

struct GetPfcDetailedCategoryQuery {
    PfcDetailedCategoryId id;
    explicit GetPfcDetailedCategoryQuery(PfcDetailedCategoryId id) : id(std::move(id)) {}
};
