#pragma once
#include "IUseCase.h"
#include "GetEmploymentStatusQuery.h"
#include "IEmploymentStatusRepository.h"
#include "EmploymentStatusDto.h"
#include <optional>

class GetEmploymentStatus : public IUseCase<GetEmploymentStatusQuery, std::optional<EmploymentStatusDto>> {
    IEmploymentStatusRepository& repo_;
public:
    explicit GetEmploymentStatus(IEmploymentStatusRepository& repo);
    std::optional<EmploymentStatusDto> execute(const GetEmploymentStatusQuery& request) override;
};
