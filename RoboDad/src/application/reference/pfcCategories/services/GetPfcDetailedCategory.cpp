#include "application/references/pfcCategories/services/GetPfcDetailedCategory.h"

GetPfcDetailedCategory::GetPfcDetailedCategory(IPfcDetailedCategoryRepository& repo) : repo_(repo) {}

std::optional<PfcDetailedCategoryDto> GetPfcDetailedCategory::execute(const GetPfcDetailedCategoryQuery& request) {
    auto cat = repo_.findById(request.id);
    if (!cat) return std::nullopt;
    return PfcDetailedCategoryDto(*cat);
}
