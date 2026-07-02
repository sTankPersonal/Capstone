#pragma once

#include "IPlaidItemRepository.h"
#include "DatabaseConnection.h"
#include <optional>
#include <vector>

class PostgresPlaidItemRepository : public IPlaidItemRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresPlaidItemRepository(DatabaseConnection& db);

    PlaidItem create(const PlaidItem& item) override;
    std::optional<PlaidItem> findById(PlaidItemId id) override;
    std::vector<PlaidItem> findAll() override;
    bool update(const PlaidItem& item) override;
    bool remove(PlaidItemId id) override;
    std::optional<PlaidItem> findByUserId(const UserId& userId) override;
};
