#include "application/references/languages/services/GetLanguage.h"

GetLanguage::GetLanguage(ILanguageRepository& repo) : repo_(repo) {}

std::optional<LanguageDto> GetLanguage::execute(const GetLanguageQuery& request) {
    auto lang = repo_.findById(request.id);
    if (!lang) return std::nullopt;
    return LanguageDto(*lang);
}
