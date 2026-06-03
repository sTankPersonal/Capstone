#pragma once
#include "IUseCase.h"
#include "ListEmploymentStatusesQuery.h"
#include "IEmploymentStatusRepository.h"
#include "EmploymentStatusDto.h"
#include <vector>

class ListEmploymentStatuses : public IUseCase<ListEmploymentStatusesQuery, std::vector<EmploymentStatusDto>> {
    IEmploymentStatusRepository& repo_;
public:
    explicit ListEmploymentStatuses(IEmploymentStatusRepository& repo);
    std::vector<EmploymentStatusDto> execute(const ListEmploymentStatusesQuery& request) override;
};
