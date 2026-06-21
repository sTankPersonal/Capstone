#pragma once
#include "PfcPrimaryCategoryId.h"
#include <optional>

struct ListPfcDetailedCategoriesQuery {
    std::optional<PfcPrimaryCategoryId> primaryId;

    ListPfcDetailedCategoriesQuery() = default;
    explicit ListPfcDetailedCategoriesQuery(PfcPrimaryCategoryId primaryId)
        : primaryId(std::move(primaryId)) {}
};
