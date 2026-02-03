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

DatabaseConnection::DatabaseConnection(const std::string& connStr)
    : conn_(std::make_unique<pqxx::connection>(connStr)) {}

DatabaseConnection::DatabaseConnection(
    const std::string& user,
    const std::string& password,
    const std::string& dbname,
    const std::string& host,
    unsigned int port)
    : conn_(std::make_unique<pqxx::connection>(buildConnStr(user, password, dbname, host, port))) {}

bool DatabaseConnection::isConnected() const noexcept {
    if (!conn_) return false;
    try {
        return conn_->is_open();
    } catch (...) {
        return false;
    }
}

DatabaseConnection DatabaseConnection::fromEnv() {
    const char* url = std::getenv("DATABASE_URL");
    if (url && *url) return DatabaseConnection(std::string(url));

    auto require = [](const char* name) -> std::string {
        const char* v = std::getenv(name);
        if (!v || !*v)
            throw std::runtime_error(std::string(name) + " environment variable is not set");
        return v;
    };
    auto optUint = [](const char* name, unsigned int def) -> unsigned int {
        const char* v = std::getenv(name);
        return (v && *v) ? static_cast<unsigned int>(std::stoul(v)) : def;
    };

    return DatabaseConnection(
        require("DB_USER"),
        require("DB_PASSWORD"),
        require("DB_NAME"),
        require("DB_HOST"),
        optUint("DB_PORT", 5432)
    );
}
