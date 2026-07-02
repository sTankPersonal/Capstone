#include "PlaidItem.h"

PlaidItem::PlaidItem(
    const PlaidItemId& id,
    const UserId& userId,
    const std::string& accessToken,
    const std::chrono::year_month_day& createdAt,
    const std::string& syncCursor)
    : id_(id), userId_(userId), accessToken_(accessToken), syncCursor_(syncCursor), createdAt_(createdAt) {}

const PlaidItemId& PlaidItem::getId() const noexcept { return id_; }
const UserId& PlaidItem::getUserId() const noexcept { return userId_; }
const std::string& PlaidItem::getAccessToken() const noexcept { return accessToken_; }
const std::string& PlaidItem::getSyncCursor() const noexcept { return syncCursor_; }
void PlaidItem::setSyncCursor(const std::string& cursor) { syncCursor_ = cursor; }
const std::chrono::year_month_day& PlaidItem::getCreatedAt() const noexcept { return createdAt_; }
