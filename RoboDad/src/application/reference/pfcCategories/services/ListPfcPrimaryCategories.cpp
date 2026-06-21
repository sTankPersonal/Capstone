#include "application/references/pfcCategories/services/ListPfcPrimaryCategories.h"

ListPfcPrimaryCategories::ListPfcPrimaryCategories(IPfcPrimaryCategoryRepository& repo) : repo_(repo) {}

std::vector<PfcPrimaryCategoryDto> ListPfcPrimaryCategories::execute(const ListPfcPrimaryCategoriesQuery&) {
    std::vector<PfcPrimaryCategoryDto> dtos;
    for (const auto& cat : repo_.findAll())
        dtos.emplace_back(cat);
    return dtos;
}
