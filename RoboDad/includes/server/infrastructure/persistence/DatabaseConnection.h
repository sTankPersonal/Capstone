#pragma once
#include "IDatabaseConnection.h"
#include <pqxx/pqxx>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

// Thread-safe pool of pqxx connections. Each caller checks out a connection for the
// duration of a single unit of work (via acquire()) so concurrent requests on Crow's
// worker threads never share one pqxx::connection, which is not safe for concurrent use.
class DatabaseConnection : public IDatabaseConnection {
public:
    static constexpr unsigned int kDefaultPoolSize = 8;

    // RAII handle for a single checked-out connection. Returns the connection to the
    // pool when destroyed.
    class ConnectionHandle {
    public:
        ConnectionHandle(DatabaseConnection& owner, std::unique_ptr<pqxx::connection> conn) noexcept
            : owner_(&owner), conn_(std::move(conn)) {}
        ~ConnectionHandle() { if (conn_) owner_->release(std::move(conn_)); }

        ConnectionHandle(const ConnectionHandle&) = delete;
        ConnectionHandle& operator=(const ConnectionHandle&) = delete;
        ConnectionHandle(ConnectionHandle&&) noexcept = default;
        ConnectionHandle& operator=(ConnectionHandle&&) noexcept = default;

        pqxx::connection& operator*() noexcept { return *conn_; }
        pqxx::connection* operator->() noexcept { return conn_.get(); }

    private:
        DatabaseConnection* owner_;
        std::unique_ptr<pqxx::connection> conn_;
    };

    explicit DatabaseConnection(const std::string& connStr, unsigned int poolSize = kDefaultPoolSize);

    explicit DatabaseConnection(
        const std::string& user,
        const std::string& password,
        const std::string& dbname,
        const std::string& host = "localhost",
        unsigned int port = 5432,
        unsigned int poolSize = kDefaultPoolSize
    );

    ~DatabaseConnection() = default;
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;
    DatabaseConnection(DatabaseConnection&&) = delete;
    DatabaseConnection& operator=(DatabaseConnection&&) = delete;

    // Blocks until a pooled connection is free, then hands out exclusive ownership of it
    // until the returned handle is destroyed.
    ConnectionHandle acquire();

    bool isConnected() const noexcept override;

    static DatabaseConnection fromEnv();

private:
    void release(std::unique_ptr<pqxx::connection> conn);

    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::vector<std::unique_ptr<pqxx::connection>> pool_;
};
