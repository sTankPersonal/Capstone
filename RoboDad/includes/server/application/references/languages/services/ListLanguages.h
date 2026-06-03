#pragma once
#include "IUseCase.h"
#include "ListLanguagesQuery.h"
#include "ILanguageRepository.h"
#include "LanguageDto.h"
#include <vector>

class ListLanguages : public IUseCase<ListLanguagesQuery, std::vector<LanguageDto>> {
    ILanguageRepository& repo_;
public:
    explicit ListLanguages(ILanguageRepository& repo);
    std::vector<LanguageDto> execute(const ListLanguagesQuery& request) override;
};
