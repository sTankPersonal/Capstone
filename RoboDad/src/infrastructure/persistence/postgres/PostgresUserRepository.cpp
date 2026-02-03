#include "infrastructure/persistence/postgres/PostgresUserRepository.h"
#include "DateUtils.h"
#include <pqxx/pqxx>
#include <optional>

PostgresUserRepository::PostgresUserRepository(DatabaseConnection& db) : db_(db) {}

static User rowToUser(const pqxx::row& row) {
    UserId userId{row["user_id"].as<std::string>()};

    std::optional<std::string> pwHash = row["password_hash"].is_null()
        ? std::nullopt
        : std::make_optional(row["password_hash"].as<std::string>());
    UserLogin login{row["email"].as<std::string>(), pwHash};

    auto optStr = [&](const char* col) -> std::optional<std::string> {
        return row[col].is_null() ? std::nullopt : std::make_optional(row[col].as<std::string>());
    };

    std::optional<std::chrono::year_month_day> dob = row["date_of_birth"].is_null()
        ? std::nullopt
        : std::make_optional(dateFromStr(row["date_of_birth"].as<std::string>()));

    auto optId = [&](const char* col) -> std::optional<std::string> {
        return row[col].is_null() ? std::nullopt : std::make_optional(row[col].as<std::string>());
    };

    UserInformation info{
        optStr("first_name"),
        optStr("last_name"),
        dob,
        optId("country_id")           ? std::make_optional(CountryId{*optId("country_id")})                     : std::optional<CountryId>{},
        optId("currency_id")          ? std::make_optional(CurrencyId{*optId("currency_id")})                   : std::optional<CurrencyId>{},
        optId("language_id")          ? std::make_optional(LanguageId{*optId("language_id")})                   : std::optional<LanguageId>{},
        optId("employment_status_id") ? std::make_optional(EmploymentStatusId{*optId("employment_status_id")}) : std::optional<EmploymentStatusId>{}
    };

    return User{
        userId,
        login,
        info,
        dateFromStr(row["created_at"].as<std::string>()),
        dateFromStr(row["updated_at"].as<std::string>())
    };
}

User PostgresUserRepository::create(const User& user) {
    pqxx::work txn{db_.getConnection()};
    const auto& info = user.getUserInformation();
    const auto& login = user.getUserLogin();

    auto optIdStr = [](const auto& opt) -> std::optional<std::string> {
        return opt.has_value() ? std::make_optional(opt->getId()) : std::nullopt;
    };
    std::optional<std::string> dobStr = info.getDateOfBirth().has_value()
        ? std::make_optional(dateToStr(*info.getDateOfBirth())) : std::nullopt;

    txn.exec_params(
        "INSERT INTO users(user_id, email, password_hash, first_name, last_name, "
        "date_of_birth, country_id, currency_id, language_id, employment_status_id, "
        "created_at, updated_at) VALUES($1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12)",
        user.getId().getId(),
        login.getEmail(),
        login.getPasswordHash(),
        info.getFirstName(),
        info.getLastName(),
        dobStr,
        optIdStr(info.getCountryId()),
        optIdStr(info.getCurrencyId()),
        optIdStr(info.getLanguageId()),
        optIdStr(info.getEmploymentStatusId()),
        dateToStr(user.getCreatedAt()),
        dateToStr(user.getUpdatedAt())
    );
    txn.commit();
    return user;
}

std::optional<User> PostgresUserRepository::findById(UserId id) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT user_id, email, password_hash, first_name, last_name, date_of_birth, "
        "country_id, currency_id, language_id, employment_status_id, created_at, updated_at "
        "FROM users WHERE user_id=$1",
        id.getId());
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToUser(r[0]);
}

std::vector<User> PostgresUserRepository::findAll() {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec(
        "SELECT user_id, email, password_hash, first_name, last_name, date_of_birth, "
        "country_id, currency_id, language_id, employment_status_id, created_at, updated_at "
        "FROM users");
    txn.commit();
    std::vector<User> users;
    for (const auto& row : r)
        users.push_back(rowToUser(row));
    return users;
}

bool PostgresUserRepository::update(const User& user) {
    pqxx::work txn{db_.getConnection()};
    const auto& info = user.getUserInformation();
    const auto& login = user.getUserLogin();

    auto optIdStr = [](const auto& opt) -> std::optional<std::string> {
        return opt.has_value() ? std::make_optional(opt->getId()) : std::nullopt;
    };
    std::optional<std::string> dobStr = info.getDateOfBirth().has_value()
        ? std::make_optional(dateToStr(*info.getDateOfBirth())) : std::nullopt;

    auto res = txn.exec_params(
        "UPDATE users SET email=$2, password_hash=$3, first_name=$4, last_name=$5, "
        "date_of_birth=$6, country_id=$7, currency_id=$8, language_id=$9, "
        "employment_status_id=$10, updated_at=$11 WHERE user_id=$1",
        user.getId().getId(),
        login.getEmail(),
        login.getPasswordHash(),
        info.getFirstName(),
        info.getLastName(),
        dobStr,
        optIdStr(info.getCountryId()),
        optIdStr(info.getCurrencyId()),
        optIdStr(info.getLanguageId()),
        optIdStr(info.getEmploymentStatusId()),
        dateToStr(user.getUpdatedAt())
    );
    txn.commit();
    return res.affected_rows() > 0;
}

bool PostgresUserRepository::remove(UserId id) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params("DELETE FROM users WHERE user_id=$1", id.getId());
    txn.commit();
    return res.affected_rows() > 0;
}

std::optional<std::pair<UserId, std::string>> PostgresUserRepository::lookupCredentials(
    const std::string& email)
{
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT user_id, password_hash FROM users WHERE email=$1", email);
    txn.commit();
    if (r.empty()) return std::nullopt;
    std::string hash = r[0]["password_hash"].is_null() ? "" : r[0]["password_hash"].as<std::string>();
    return std::make_pair(UserId{r[0]["user_id"].as<std::string>()}, std::move(hash));
}
