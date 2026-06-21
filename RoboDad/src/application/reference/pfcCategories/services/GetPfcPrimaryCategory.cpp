#include "application/references/pfcCategories/services/GetPfcPrimaryCategory.h"

GetPfcPrimaryCategory::GetPfcPrimaryCategory(IPfcPrimaryCategoryRepository& repo) : repo_(repo) {}

std::optional<PfcPrimaryCategoryDto> GetPfcPrimaryCategory::execute(const GetPfcPrimaryCategoryQuery& request) {
    auto cat = repo_.findById(request.id);
    if (!cat) return std::nullopt;
    return PfcPrimaryCategoryDto(*cat);
}
