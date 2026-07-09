#pragma once
#include "IUseCase.h"
#include "GetPfcDetailedCategoryQuery.h"
#include "IPfcDetailedCategoryRepository.h"
#include "PfcDetailedCategoryDto.h"
#include <optional>

class GetPfcDetailedCategory : public IUseCase<GetPfcDetailedCategoryQuery, std::optional<PfcDetailedCategoryDto>> {
    IPfcDetailedCategoryRepository& repo_;
public:
    explicit GetPfcDetailedCategory(IPfcDetailedCategoryRepository& repo);
    std::optional<PfcDetailedCategoryDto> execute(const GetPfcDetailedCategoryQuery& request) override;
};
