#pragma once

#include "IEmploymentStatusRepository.h"
#include "DatabaseConnection.h"
#include <optional>
#include <string>
#include <vector>

class PostgresEmploymentStatusRepository : public IEmploymentStatusRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresEmploymentStatusRepository(DatabaseConnection& db);

    EmploymentStatus create(const EmploymentStatus& entity) override;
    std::optional<EmploymentStatus> findById(EmploymentStatusId id) override;
    std::vector<EmploymentStatus> findAll() override;
    bool update(const EmploymentStatus& entity) override;
    bool remove(EmploymentStatusId id) override;

    std::optional<EmploymentStatus> findByValue(const std::string& value);
};
