#include "application/references/employmentStatuses/services/GetEmploymentStatus.h"

GetEmploymentStatus::GetEmploymentStatus(IEmploymentStatusRepository& repo) : repo_(repo) {}

std::optional<EmploymentStatusDto> GetEmploymentStatus::execute(const GetEmploymentStatusQuery& request) {
    auto status = repo_.findById(request.id);
    if (!status) return std::nullopt;
    return EmploymentStatusDto(*status);
}
