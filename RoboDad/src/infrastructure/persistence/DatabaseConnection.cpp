#include "infrastructure/persistence/DatabaseConnection.h"
#include <cstdlib>
#include <stdexcept>

DatabaseConnection::DatabaseConnection(const std::string& connStr) {
    conn_ = PQconnectdb(connStr.c_str());
    if (PQstatus(conn_) != CONNECTION_OK) {
        std::string err = PQerrorMessage(conn_);
        PQfinish(conn_);
        conn_ = nullptr;
        throw std::runtime_error("Database connection failed: " + err);
    }
}

DatabaseConnection::~DatabaseConnection() {
    if (conn_) PQfinish(conn_);
}

DatabaseConnection::DatabaseConnection(DatabaseConnection&& other) noexcept
    : conn_(other.conn_) {
    other.conn_ = nullptr;
}

DatabaseConnection& DatabaseConnection::operator=(DatabaseConnection&& other) noexcept {
    if (this != &other) {
        if (conn_) PQfinish(conn_);
        conn_ = other.conn_;
        other.conn_ = nullptr;
    }
    return *this;
}

bool DatabaseConnection::isConnected() const noexcept {
    return conn_ && PQstatus(conn_) == CONNECTION_OK;
}

DatabaseConnection DatabaseConnection::fromEnv() {
    const char* url = std::getenv("DATABASE_URL");
    if (!url) throw std::runtime_error("DATABASE_URL environment variable is not set");
    return DatabaseConnection(url);
}
