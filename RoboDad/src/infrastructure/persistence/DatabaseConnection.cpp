#include "infrastructure/persistence/DatabaseConnection.h"
#include <cstdlib>
#include <stdexcept>
#include <sstream>

static std::string buildConnStr(
    const std::string& user,
    const std::string& password,
    const std::string& dbname,
    const std::string& host,
    unsigned int port)
{
    std::ostringstream ss;
    ss << "host=" << host
       << " port=" << port
       << " dbname=" << dbname
       << " user=" << user
       << " password=" << password;
    return ss.str();
}

static std::vector<std::unique_ptr<pqxx::connection>> makePool(const std::string& connStr, unsigned int poolSize) {
    if (poolSize == 0)
        throw std::invalid_argument("DatabaseConnection pool size must be at least 1");

    std::vector<std::unique_ptr<pqxx::connection>> pool;
    pool.reserve(poolSize);
    for (unsigned int i = 0; i < poolSize; ++i)
        pool.push_back(std::make_unique<pqxx::connection>(connStr));
    return pool;
}

DatabaseConnection::DatabaseConnection(const std::string& connStr, unsigned int poolSize)
    : pool_(makePool(connStr, poolSize)) {}

DatabaseConnection::DatabaseConnection(
    const std::string& user,
    const std::string& password,
    const std::string& dbname,
    const std::string& host,
    unsigned int port,
    unsigned int poolSize)
    : pool_(makePool(buildConnStr(user, password, dbname, host, port), poolSize)) {}

DatabaseConnection::ConnectionHandle DatabaseConnection::acquire() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !pool_.empty(); });
    auto conn = std::move(pool_.back());
    pool_.pop_back();
    return ConnectionHandle(*this, std::move(conn));
}

void DatabaseConnection::release(std::unique_ptr<pqxx::connection> conn) {
    std::lock_guard<std::mutex> lock(mutex_);
    pool_.push_back(std::move(conn));
    cv_.notify_one();
}

bool DatabaseConnection::isConnected() const noexcept {
    std::lock_guard<std::mutex> lock(mutex_);
    if (pool_.empty()) return true; // every connection is currently checked out
    try {
        return pool_.back()->is_open();
    } catch (...) {
        return false;
    }
}

DatabaseConnection DatabaseConnection::fromEnv() {
    auto optUint = [](const char* name, unsigned int def) -> unsigned int {
        const char* v = std::getenv(name);
        return (v && *v) ? static_cast<unsigned int>(std::stoul(v)) : def;
    };
    const unsigned int poolSize = optUint("DB_POOL_SIZE", DatabaseConnection::kDefaultPoolSize);

    const char* url = std::getenv("DATABASE_URL");
    if (url && *url) return DatabaseConnection(std::string(url), poolSize);

    auto require = [](const char* name) -> std::string {
        const char* v = std::getenv(name);
        if (!v || !*v)
            throw std::runtime_error(std::string(name) + " environment variable is not set");
        return v;
    };

    return DatabaseConnection(
        require("DB_USER"),
        require("DB_PASSWORD"),
        require("DB_NAME"),
        require("DB_HOST"),
        optUint("DB_PORT", 5432),
        poolSize
    );
}
