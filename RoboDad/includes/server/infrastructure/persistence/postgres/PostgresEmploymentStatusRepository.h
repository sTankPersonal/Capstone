#pragma once

#include "IRepository.h"
#include "EmploymentStatus.h"
#include "EmploymentStatusId.h"
#include "DatabaseConnection.h"
#include <optional>
#include <string>
#include <vector>

class PostgresEmploymentStatusRepository : public IRepository<EmploymentStatus, EmploymentStatusId> {
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
