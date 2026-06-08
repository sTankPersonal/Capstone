#include "application/references/languages/services/ListLanguages.h"

ListLanguages::ListLanguages(ILanguageRepository& repo) : repo_(repo) {}

std::vector<LanguageDto> ListLanguages::execute(const ListLanguagesQuery&) {
    auto items = repo_.findAll();
    std::vector<LanguageDto> dtos;
    dtos.reserve(items.size());
    for (const auto& l : items) dtos.emplace_back(l);
    return dtos;
}
