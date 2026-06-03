#pragma once
#include "IUseCase.h"
#include "GetLanguageQuery.h"
#include "ILanguageRepository.h"
#include "LanguageDto.h"
#include <optional>

class GetLanguage : public IUseCase<GetLanguageQuery, std::optional<LanguageDto>> {
    ILanguageRepository& repo_;
public:
    explicit GetLanguage(ILanguageRepository& repo);
    std::optional<LanguageDto> execute(const GetLanguageQuery& request) override;
};
