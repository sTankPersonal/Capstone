#pragma once
#include <libpq-fe.h>
#include <memory>
#include <stdexcept>
#include <string>

namespace pg {

struct ResultDeleter {
    void operator()(PGresult* r) const noexcept { if (r) PQclear(r); }
};

using Result = std::unique_ptr<PGresult, ResultDeleter>;

inline void check(PGresult* res, ExecStatusType expected, PGconn* conn, const char* ctx) {
    if (PQresultStatus(res) != expected)
        throw std::runtime_error(std::string(ctx) + ": " + PQerrorMessage(conn));
}

// Returns true if at least one row was affected by the last command.
inline bool rowsAffected(PGresult* res) noexcept {
    const char* n = PQcmdTuples(res);
    return n && n[0] != '\0' && n[0] != '0';
}

} // namespace pg
