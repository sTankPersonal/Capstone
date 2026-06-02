#include "presentation/controllers/ChatController.h"
#include "domain/valueObjects/ids/LlmPersonaId.h"
#include "domain/valueObjects/ids/ChatSessionId.h"
#include "domain/valueObjects/ids/UserId.h"
#include <crow.h>
#include <cstdio>
#include <optional>
#include <string>
#include <vector>

ChatController::ChatController(IChatSessionRepository& sessionRepo,
                               IChatMessageRepository& messageRepo,
                               ILlmPersonaRepository&  personaRepo,
                               ITransactionRepository& transactionRepo,
                               ILlmClient&             llmClient,
                               IPromptBuilder&         promptBuilder,
                               IJwtService&            jwt)
    : createSession_(sessionRepo),
      deleteSession_(sessionRepo),
      getChatHistory_(messageRepo),
      getSession_(sessionRepo),
      listSessions_(sessionRepo),
      sendMessage_(sessionRepo, messageRepo, personaRepo, transactionRepo, llmClient, promptBuilder),
      updateSession_(sessionRepo),
      jwt_(jwt) {}

std::optional<std::string> ChatController::extractUserId(const crow::request& req) const {
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

static crow::json::wvalue sessionToJson(const ChatSession& s) {
    crow::json::wvalue j;
    j["id"]            = s.getId().getId();
    j["userId"]        = s.getUserId().getId();
    j["personaId"]     = s.getPersonaId().getId();
    j["description"]   = s.getSessionDescription().getDescription();
    j["createdAt"]     = formatDate(s.getCreatedAt());
    j["updatedAt"]     = formatDate(s.getUpdatedAt());
    const auto& info = s.getSessionDescription().getAdditionalInfo();
    if (info) j["additionalInfo"] = *info;
    return j;
}

static crow::json::wvalue messageToJson(const ChatMessage& m) {
    crow::json::wvalue j;
    j["id"]      = m.getId().getId();
    j["role"]    = m.getMessageSenderId().getId();
    j["content"] = m.getContent().getContent().value_or(std::string{});
    j["date"]    = formatDate(m.getCreatedAt());
    return j;
}

void ChatController::registerRoutes(crow::SimpleApp& app) {

    CROW_ROUTE(app, "/chat")
    ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/chat.html");
        res.end();
    });

    // POST /api/ai/sessions — create a new chat session
    CROW_ROUTE(app, "/api/ai/sessions").methods("POST"_method)
    ([this](const crow::request& req) {
        auto userId = extractUserId(req);
        if (!userId) return crow::response(401, "Unauthorized");

        auto body = crow::json::load(req.body);
        if (!body || !body.has("personaId") || !body.has("description"))
            return crow::response(400, "Missing 'personaId' or 'description'");

        try {
            auto session = createSession_.execute(
                UserId(*userId),
                LlmPersonaId(body["personaId"].s()),
                body["description"].s()
            );
            return crow::response(201, sessionToJson(session).dump());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    // GET /api/ai/sessions — list sessions for the authenticated user
    CROW_ROUTE(app, "/api/ai/sessions").methods("GET"_method)
    ([this](const crow::request& req) {
        auto userId = extractUserId(req);
        if (!userId) return crow::response(401, "Unauthorized");

        try {
            auto sessions = listSessions_.execute(UserId(*userId));
            std::vector<crow::json::wvalue> items;
            items.reserve(sessions.size());
            for (auto& s : sessions)
                items.push_back(sessionToJson(s));
            return crow::response(200, crow::json::wvalue(std::move(items)).dump());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    // GET /api/ai/sessions/:id
    CROW_ROUTE(app, "/api/ai/sessions/<string>").methods("GET"_method)
    ([this](const crow::request& req, const std::string& id) {
        if (!extractUserId(req)) return crow::response(401, "Unauthorized");

        try {
            auto session = getSession_.execute(ChatSessionId(id));
            if (!session) return crow::response(404, "Session not found");
            return crow::response(200, sessionToJson(*session).dump());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    // DELETE /api/ai/sessions/:id
    CROW_ROUTE(app, "/api/ai/sessions/<string>").methods("DELETE"_method)
    ([this](const crow::request& req, const std::string& id) {
        if (!extractUserId(req)) return crow::response(401, "Unauthorized");

        try {
            bool ok = deleteSession_.execute(ChatSessionId(id));
            return ok ? crow::response(204) : crow::response(404, "Session not found");
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    // PUT /api/ai/sessions/:id — update description
    CROW_ROUTE(app, "/api/ai/sessions/<string>").methods("PUT"_method)
    ([this](const crow::request& req, const std::string& id) {
        if (!extractUserId(req)) return crow::response(401, "Unauthorized");

        auto body = crow::json::load(req.body);
        if (!body || !body.has("description"))
            return crow::response(400, "Missing 'description'");

        std::optional<std::string> additionalInfo;
        if (body.has("additionalInfo"))
            additionalInfo = body["additionalInfo"].s();

        try {
            bool ok = updateSession_.execute(ChatSessionId(id),
                                             body["description"].s(),
                                             additionalInfo);
            return ok ? crow::response(204) : crow::response(404, "Session not found");
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    // POST /api/ai/sessions/:id/messages — send a message, get AI reply
    CROW_ROUTE(app, "/api/ai/sessions/<string>/messages").methods("POST"_method)
    ([this](const crow::request& req, const std::string& id) {
        if (!extractUserId(req)) return crow::response(401, "Unauthorized");

        auto body = crow::json::load(req.body);
        if (!body || !body.has("message"))
            return crow::response(400, "Missing 'message'");

        try {
            std::string reply = sendMessage_.execute(ChatSessionId(id), body["message"].s());
            crow::json::wvalue out;
            out["reply"] = reply;
            return crow::response(200, out.dump());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    // GET /api/ai/sessions/:id/history
    CROW_ROUTE(app, "/api/ai/sessions/<string>/history").methods("GET"_method)
    ([this](const crow::request& req, const std::string& id) {
        if (!extractUserId(req)) return crow::response(401, "Unauthorized");

        try {
            auto messages = getChatHistory_.execute(ChatSessionId(id));
            std::vector<crow::json::wvalue> items;
            items.reserve(messages.size());
            for (auto& m : messages)
                items.push_back(messageToJson(m));
            return crow::response(200, crow::json::wvalue(std::move(items)).dump());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });
}
