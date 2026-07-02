#pragma once

#include "IEntity.h"
#include "PlaidItemId.h"
#include "UserId.h"
#include <string>
#include <chrono>

class PlaidItem : public IEntity<PlaidItemId, std::chrono::year_month_day> {
    PlaidItemId id_;
    UserId userId_;
    std::string accessToken_;
    std::string syncCursor_;
    std::chrono::year_month_day createdAt_;

public:
    explicit PlaidItem(
        const PlaidItemId& id,
        const UserId& userId,
        const std::string& accessToken,
        const std::chrono::year_month_day& createdAt,
        const std::string& syncCursor = ""
    );

    const PlaidItemId& getId() const noexcept override;
    const UserId& getUserId() const noexcept;
    const std::string& getAccessToken() const noexcept;
    const std::string& getSyncCursor() const noexcept;
    void setSyncCursor(const std::string& cursor);
    const std::chrono::year_month_day& getCreatedAt() const noexcept override;
};
