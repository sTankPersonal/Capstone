#pragma once
#include "IUseCase.h"
#include "ListPfcDetailedCategoriesQuery.h"
#include "IPfcDetailedCategoryRepository.h"
#include "PfcDetailedCategoryDto.h"
#include <vector>

class ListPfcDetailedCategories : public IUseCase<ListPfcDetailedCategoriesQuery, std::vector<PfcDetailedCategoryDto>> {
    IPfcDetailedCategoryRepository& repo_;
public:
    explicit ListPfcDetailedCategories(IPfcDetailedCategoryRepository& repo);
    std::vector<PfcDetailedCategoryDto> execute(const ListPfcDetailedCategoriesQuery& request) override;
};
