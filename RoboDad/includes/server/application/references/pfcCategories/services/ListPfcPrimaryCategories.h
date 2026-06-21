#pragma once
#include "IUseCase.h"
#include "ListPfcPrimaryCategoriesQuery.h"
#include "IPfcPrimaryCategoryRepository.h"
#include "PfcPrimaryCategoryDto.h"
#include <vector>

class ListPfcPrimaryCategories : public IUseCase<ListPfcPrimaryCategoriesQuery, std::vector<PfcPrimaryCategoryDto>> {
    IPfcPrimaryCategoryRepository& repo_;
public:
    explicit ListPfcPrimaryCategories(IPfcPrimaryCategoryRepository& repo);
    std::vector<PfcPrimaryCategoryDto> execute(const ListPfcPrimaryCategoriesQuery& request) override;
};
