#include "application/references/employmentStatuses/services/ListEmploymentStatuses.h"

ListEmploymentStatuses::ListEmploymentStatuses(IEmploymentStatusRepository& repo) : repo_(repo) {}

std::vector<EmploymentStatusDto> ListEmploymentStatuses::execute(const ListEmploymentStatusesQuery&) {
    auto items = repo_.findAll();
    std::vector<EmploymentStatusDto> dtos;
    dtos.reserve(items.size());
    for (const auto& s : items) dtos.emplace_back(s);
    return dtos;
}
