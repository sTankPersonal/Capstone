#include "presentation/controllers/SettingsController.h"
#include "domain/valueObjects/UserInformation.h"
#include "domain/valueObjects/ids/UserId.h"
#include "domain/valueObjects/ids/LlmPersonaId.h"
#include "domain/valueObjects/ids/EmploymentStatusId.h"
#include "domain/valueObjects/ids/CurrencyId.h"
#include "domain/valueObjects/ids/CountryId.h"
#include "domain/valueObjects/ids/LanguageId.h"
#include <crow.h>
#include <cstdio>
#include <optional>
#include <string>

SettingsController::SettingsController(IUserRepository&       userRepo,
                                       ILlmPersonaRepository& personaRepo,
                                       IPasswordHasher&       hasher,
                                       IJwtService&           jwt)
    : getUserProfile_(userRepo),
      updateUserProfile_(userRepo),
      updatePassword_(userRepo, hasher),
      createPersona_(personaRepo),
      deletePersona_(personaRepo),
      getPersona_(personaRepo),
      listPersonas_(personaRepo),
      updatePersona_(personaRepo),
      jwt_(jwt) {}

std::optional<std::string> SettingsController::extractUserId(const crow::request& req) const {
    std::string cookie = req.get_header_value("Cookie");
    const std::string prefix = "token=";
    auto pos = cookie.find(prefix);
    if (pos == std::string::npos) return std::nullopt;
    pos += prefix.size();
    auto end = cookie.find(';', pos);
    std::string token = (end == std::string::npos)
        ? cookie.substr(pos)
        : cookie.substr(pos, end - pos);
    return jwt_.verify(token);
}

static std::string formatDate(const std::chrono::year_month_day& d) {
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%04d-%02u-%02u",
                  static_cast<int>(d.year()),
                  static_cast<unsigned>(d.month()),
                  static_cast<unsigned>(d.day()));
    return buf;
}

static crow::json::wvalue userToJson(const User& u) {
    crow::json::wvalue j;
    j["id"]    = u.getId().getId();
    j["email"] = u.getUserLogin().getEmail();

    const auto& info = u.getUserInformation();
    if (info.getFirstName())          j["firstName"]          = *info.getFirstName();
    if (info.getLastName())           j["lastName"]           = *info.getLastName();
    if (info.getDateOfBirth())        j["dateOfBirth"]        = formatDate(*info.getDateOfBirth());
    if (info.getCountryId())          j["countryId"]          = info.getCountryId()->getId();
    if (info.getCurrencyId())         j["currencyId"]         = info.getCurrencyId()->getId();
    if (info.getLanguageId())         j["languageId"]         = info.getLanguageId()->getId();
    if (info.getEmploymentStatusId()) j["employmentStatusId"] = info.getEmploymentStatusId()->getId();

    j["createdAt"] = formatDate(u.getCreatedAt());
    j["updatedAt"] = formatDate(u.getUpdatedAt());
    return j;
}

static crow::json::wvalue personaToJson(const LlmPersona& p) {
    crow::json::wvalue j;
    j["id"]           = p.getId().getId();
    j["name"]         = p.getName();
    j["description"]  = p.getDescription();
    j["systemPrompt"] = p.getSystemPrompt();
    j["createdAt"]    = formatDate(p.getCreatedAt());
    return j;
}

void SettingsController::registerRoutes(crow::SimpleApp& app) {

    CROW_ROUTE(app, "/settings")
    ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/settings.html");
        res.end();
    });

    // --- User Profile ---

    CROW_ROUTE(app, "/api/user/profile").methods("GET"_method)
    ([this](const crow::request& req) {
        auto userId = extractUserId(req);
        if (!userId) return crow::response(401, "Unauthorized");

        try {
            auto user = getUserProfile_.execute(UserId(*userId));
            if (!user) return crow::response(404, "User not found");
            return crow::response(200, userToJson(*user).dump());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    CROW_ROUTE(app, "/api/user/profile").methods("PUT"_method)
    ([this](const crow::request& req) {
        auto userId = extractUserId(req);
        if (!userId) return crow::response(401, "Unauthorized");

        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");

        std::optional<std::string> firstName;
        std::optional<std::string> lastName;
        std::optional<std::string> employmentStatusId;

        if (body.has("firstName"))          firstName          = body["firstName"].s();
        if (body.has("lastName"))           lastName           = body["lastName"].s();
        if (body.has("employmentStatusId")) employmentStatusId = body["employmentStatusId"].s();

        UserInformation info(
            firstName,
            lastName,
            std::nullopt, // dateOfBirth — not yet supported from PUT
            std::nullopt, // countryId
            std::nullopt, // currencyId
            std::nullopt, // languageId
            employmentStatusId
                ? std::optional<EmploymentStatusId>(EmploymentStatusId(*employmentStatusId))
                : std::nullopt
        );

        try {
            bool ok = updateUserProfile_.execute(UserId(*userId), info);
            return ok ? crow::response(204) : crow::response(404, "User not found");
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    CROW_ROUTE(app, "/api/user/password").methods("PUT"_method)
    ([this](const crow::request& req) {
        auto userId = extractUserId(req);
        if (!userId) return crow::response(401, "Unauthorized");

        auto body = crow::json::load(req.body);
        if (!body || !body.has("currentPassword") || !body.has("newPassword"))
            return crow::response(400, "Missing 'currentPassword' or 'newPassword'");

        try {
            bool ok = updatePassword_.execute(
                UserId(*userId),
                body["currentPassword"].s(),
                body["newPassword"].s()
            );
            return ok ? crow::response(204) : crow::response(400, "Incorrect current password");
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    // --- LLM Personas ---

    CROW_ROUTE(app, "/api/personas").methods("GET"_method)
    ([this](const crow::request& req) {
        if (!extractUserId(req)) return crow::response(401, "Unauthorized");

        try {
            auto personas = listPersonas_.execute();
            std::vector<crow::json::wvalue> items;
            items.reserve(personas.size());
            for (auto& p : personas)
                items.push_back(personaToJson(p));
            return crow::response(200, crow::json::wvalue(std::move(items)).dump());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    CROW_ROUTE(app, "/api/personas").methods("POST"_method)
    ([this](const crow::request& req) {
        if (!extractUserId(req)) return crow::response(401, "Unauthorized");

        auto body = crow::json::load(req.body);
        if (!body || !body.has("name") || !body.has("description") || !body.has("systemPrompt"))
            return crow::response(400, "Missing 'name', 'description', or 'systemPrompt'");

        try {
            auto persona = createPersona_.execute(
                body["name"].s(),
                body["description"].s(),
                body["systemPrompt"].s()
            );
            return crow::response(201, personaToJson(persona).dump());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    CROW_ROUTE(app, "/api/personas/<string>").methods("GET"_method)
    ([this](const crow::request& req, const std::string& id) {
        if (!extractUserId(req)) return crow::response(401, "Unauthorized");

        try {
            auto persona = getPersona_.execute(LlmPersonaId(id));
            if (!persona) return crow::response(404, "Persona not found");
            return crow::response(200, personaToJson(*persona).dump());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    CROW_ROUTE(app, "/api/personas/<string>").methods("DELETE"_method)
    ([this](const crow::request& req, const std::string& id) {
        if (!extractUserId(req)) return crow::response(401, "Unauthorized");

        try {
            bool ok = deletePersona_.execute(LlmPersonaId(id));
            return ok ? crow::response(204) : crow::response(404, "Persona not found");
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    CROW_ROUTE(app, "/api/personas/<string>").methods("PUT"_method)
    ([this](const crow::request& req, const std::string& id) {
        if (!extractUserId(req)) return crow::response(401, "Unauthorized");

        auto body = crow::json::load(req.body);
        if (!body || !body.has("name") || !body.has("description") || !body.has("systemPrompt"))
            return crow::response(400, "Missing 'name', 'description', or 'systemPrompt'");

        try {
            bool ok = updatePersona_.execute(
                LlmPersonaId(id),
                body["name"].s(),
                body["description"].s(),
                body["systemPrompt"].s()
            );
            return ok ? crow::response(204) : crow::response(404, "Persona not found");
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });
}
