#pragma once
#include "PfcPrimaryCategoryId.h"

struct GetPfcPrimaryCategoryQuery {
    PfcPrimaryCategoryId id;
    explicit GetPfcPrimaryCategoryQuery(PfcPrimaryCategoryId id) : id(std::move(id)) {}
};
