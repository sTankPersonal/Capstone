#pragma once

#include "crow.h"

class IDto {
public:
    virtual ~IDto() = default;
    virtual explicit operator crow::json::wvalue() const = 0;
};