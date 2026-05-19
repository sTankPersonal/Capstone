#pragma once
#include "infrastructureServices/persistance/IDatabaseConnection.h"
#include <string>
#include <stdexcept>
#include <libpq-fe.h>

class DatabaseConnection : public IDatabaseConnection {
    PGconn* conn_ = nullptr;

public:
    explicit DatabaseConnection(const std::string& connStr);
    ~DatabaseConnection();

    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;
    DatabaseConnection(DatabaseConnection&& other) noexcept;
    DatabaseConnection& operator=(DatabaseConnection&& other) noexcept;

    PGconn* get() const noexcept { return conn_; }
    bool isConnected() const noexcept override;

    // Reads DATABASE_URL from the environment. Throws if unset or connection fails.
    static DatabaseConnection fromEnv();
};
