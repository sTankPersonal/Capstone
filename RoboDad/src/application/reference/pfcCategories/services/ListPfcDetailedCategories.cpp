#include "application/references/pfcCategories/services/ListPfcDetailedCategories.h"

ListPfcDetailedCategories::ListPfcDetailedCategories(IPfcDetailedCategoryRepository& repo) : repo_(repo) {}

std::vector<PfcDetailedCategoryDto> ListPfcDetailedCategories::execute(const ListPfcDetailedCategoriesQuery& request) {
    std::vector<PfcDetailedCategory> cats = request.primaryId
        ? repo_.findByPrimaryId(*request.primaryId)
        : repo_.findAll();

    std::vector<PfcDetailedCategoryDto> dtos;
    dtos.reserve(cats.size());
    for (const auto& cat : cats)
        dtos.emplace_back(cat);
    return dtos;
}
