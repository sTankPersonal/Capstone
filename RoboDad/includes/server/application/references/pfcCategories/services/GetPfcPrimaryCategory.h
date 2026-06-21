#pragma once
#include "IUseCase.h"
#include "GetPfcPrimaryCategoryQuery.h"
#include "IPfcPrimaryCategoryRepository.h"
#include "PfcPrimaryCategoryDto.h"
#include <optional>

class GetPfcPrimaryCategory : public IUseCase<GetPfcPrimaryCategoryQuery, std::optional<PfcPrimaryCategoryDto>> {
    IPfcPrimaryCategoryRepository& repo_;
public:
    explicit GetPfcPrimaryCategory(IPfcPrimaryCategoryRepository& repo);
    std::optional<PfcPrimaryCategoryDto> execute(const GetPfcPrimaryCategoryQuery& request) override;
};
