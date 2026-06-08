#pragma once
#include "LanguageId.h"

struct GetLanguageQuery {
    LanguageId id;

    explicit GetLanguageQuery(LanguageId id)
        : id(std::move(id)) {}
};
