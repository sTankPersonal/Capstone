#pragma once
#include "IRepository.h"
#include "Language.h"
#include "LanguageId.h"
#include <optional>
#include <string>

class ILanguageRepository : public IRepository<Language, LanguageId> {
public:
    virtual std::optional<Language> findByValue(const std::string& value) = 0;
};
