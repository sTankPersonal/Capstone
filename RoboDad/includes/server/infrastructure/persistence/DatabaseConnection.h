#pragma once
#include "IDatabaseConnection.h"
#include <pqxx/pqxx>
#include <memory>
#include <string>

class DatabaseConnection : public IDatabaseConnection {
    std::unique_ptr<pqxx::connection> conn_;

public:
    explicit DatabaseConnection(const std::string& connStr);

    explicit DatabaseConnection(
        const std::string& user,
        const std::string& password,
        const std::string& dbname,
        const std::string& host = "localhost",
        unsigned int port = 5432
    );

    ~DatabaseConnection() = default;
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;
    DatabaseConnection(DatabaseConnection&&) = default;
    DatabaseConnection& operator=(DatabaseConnection&&) = default;

    pqxx::connection& getConnection() noexcept { return *conn_; }
    const pqxx::connection& getConnection() const noexcept { return *conn_; }

    bool isConnected() const noexcept override;

    static DatabaseConnection fromEnv();
};
