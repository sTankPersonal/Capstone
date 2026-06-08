#include "presentation/controllers/UserChatController.h"
#include "application/chatSessions/dtos/ChatSessionDto.h"
#include "application/chatSessions/dtos/ChatMessageDto.h"
#include "application/users/dtos/UserProfileDto.h"
#include "application/users/queries/GetUserProfileQuery.h"

#include "application/chatSessions/commands/CreateChatSessionCommand.h"
#include "application/chatSessions/commands/DeleteChatSessionCommand.h"
#include "application/chatSessions/commands/SendChatMessageCommand.h"
#include "application/chatSessions/commands/UpdateChatSessionCommand.h"
#include "application/chatSessions/queries/GetChatHistoryQuery.h"
#include "application/chatSessions/queries/GetChatSessionQuery.h"
#include "application/chatSessions/queries/ListChatSessionsQuery.h"

UserChatController::UserChatController(const CreateChatSession& createChatSession, const UpdateChatSession& updateChatSession, const DeleteChatSession& deleteChatSession, const GetChatSession& getChatSession, const ListChatSessions& listChatSessions, const GetChatHistory& getChatHistory, const SendChatMessage& sendChatMessage, const GetUserProfile& getUserProfile) : createChatSession_(createChatSession), updateChatSession_(updateChatSession), deleteChatSession_(deleteChatSession), getChatSession_(getChatSession), listChatSessions_(listChatSessions), getChatHistory_(getChatHistory), sendChatMessage_(sendChatMessage), getUserProfile_(getUserProfile) {}

void UserChatController::registerRoutes(RoboDadApp& app) {
    CROW_ROUTE(app, "/user/chats").methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req) {
        return getChatSessions(req, UserId(app.get_context<AuthMiddleware>(req).userId));
    });

    CROW_ROUTE(app, "/user/chats/<string>").methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req, const std::string& chat_session_id) {
        return getChatSessionDetails(req, UserId(app.get_context<AuthMiddleware>(req).userId), ChatSessionId(chat_session_id));
    });

    CROW_ROUTE(app, "/user/chats/new").methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req) {
        return getNewChatSessionPage(req, UserId(app.get_context<AuthMiddleware>(req).userId));
    });

    CROW_ROUTE(app, "/user/chats/<string>/edit").methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req, const std::string& chat_session_id) {
        return getEditChatSessionPage(req, UserId(app.get_context<AuthMiddleware>(req).userId), ChatSessionId(chat_session_id));
    });

    CROW_ROUTE(app, "/user/chats/<string>/delete").methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req, const std::string& chat_session_id) {
        return getDeleteChatSessionPage(req, UserId(app.get_context<AuthMiddleware>(req).userId), ChatSessionId(chat_session_id));
    });

    CROW_ROUTE(app, "/user/chats/new").methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req) {
        return postNewChatSession(req, UserId(app.get_context<AuthMiddleware>(req).userId));
    });

    CROW_ROUTE(app, "/user/chats/<string>/edit").methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req, const std::string& chat_session_id) {
        return postEditChatSession(req, UserId(app.get_context<AuthMiddleware>(req).userId), ChatSessionId(chat_session_id));
    });

    CROW_ROUTE(app, "/user/chats/<string>/delete").methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req, const std::string& chat_session_id) {
        return postDeleteChatSession(req, UserId(app.get_context<AuthMiddleware>(req).userId), ChatSessionId(chat_session_id));
    });

    CROW_ROUTE(app, "/user/chats/<string>/messages").methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req, const std::string& chat_session_id) {
        return getMessages(req, UserId(app.get_context<AuthMiddleware>(req).userId), ChatSessionId(chat_session_id));
    });

    CROW_ROUTE(app, "/user/chats/<string>/messages").methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req, const std::string& chat_session_id) {
        return postNewMessage(req, UserId(app.get_context<AuthMiddleware>(req).userId), ChatSessionId(chat_session_id));
    });
}

crow::response UserChatController::getChatSessions(const crow::request& req, UserId user_id) {
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    std::vector<ChatSessionDto> chatSessions = listChatSessions_.execute(ListChatSessionsQuery(user_id));
    crow::json::wvalue::list sessionList;
    for (const ChatSessionDto& session : chatSessions) {
        sessionList.push_back(static_cast<crow::json::wvalue>(session));
    }
    crow::mustache::context ctx;
    ctx["chat_sessions"] = std::move(sessionList);
    if (userOpt) ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);
    return crow::response(crow::mustache::load("user_chat_sessions.html").render(ctx));
}

crow::response UserChatController::getChatSessionDetails(const crow::request& req, UserId user_id, ChatSessionId chat_session_id) {
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    std::optional<ChatSessionDto> chatSessionOpt = getChatSession_.execute(GetChatSessionQuery(chat_session_id));
    if (!chatSessionOpt || chatSessionOpt->getUserId() != user_id.getId()) {
        return crow::response(404, "Chat session not found");
    }
    crow::mustache::context ctx;
    ctx["chat_session"] = static_cast<crow::json::wvalue>(*chatSessionOpt);
    if (userOpt) ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);
    return crow::response(crow::mustache::load("user_chat_session_details.html").render(ctx));
}

crow::response UserChatController::getNewChatSessionPage(const crow::request& req, UserId user_id) {
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    crow::mustache::context ctx;
    if (userOpt) ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);
    return crow::response(crow::mustache::load("new_chat_session.html").render(ctx));
}

crow::response UserChatController::getEditChatSessionPage(const crow::request& req, UserId user_id, ChatSessionId chat_session_id) {
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    std::optional<ChatSessionDto> chatSessionOpt = getChatSession_.execute(GetChatSessionQuery(chat_session_id));
    if (!chatSessionOpt || chatSessionOpt->getUserId() != user_id.getId()) {
        return crow::response(404, "Chat session not found");
    }
    crow::mustache::context ctx;
    ctx["chat_session"] = static_cast<crow::json::wvalue>(*chatSessionOpt);
    if (userOpt) ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);
    return crow::response(crow::mustache::load("edit_chat_session.html").render(ctx));
}

crow::response UserChatController::getDeleteChatSessionPage(const crow::request& req, UserId user_id, ChatSessionId chat_session_id) {
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    std::optional<ChatSessionDto> chatSessionOpt = getChatSession_.execute(GetChatSessionQuery(chat_session_id));
    if (!chatSessionOpt || chatSessionOpt->getUserId() != user_id.getId()) {
        return crow::response(404, "Chat session not found");
    }
    crow::mustache::context ctx;
    ctx["chat_session"] = static_cast<crow::json::wvalue>(*chatSessionOpt);
    if (userOpt) ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);
    return crow::response(crow::mustache::load("delete_chat_session.html").render(ctx));
}

crow::response UserChatController::postNewChatSession(const crow::request& req, UserId user_id) {
    crow::query_string params("?" + req.body);
    std::string personaId   = params.get("persona_id")   ? params.get("persona_id")   : "";
    std::string description = params.get("description")  ? params.get("description")  : "";

    if (personaId.empty() || description.empty()) {
        return crow::response(400, "Missing required fields");
    }

    CreateChatSessionCommand createRequest(user_id, LlmPersonaId(personaId), description);
    ChatSessionDto newSession = createChatSession_.execute(createRequest);
    return crow::response(201, "Chat session created successfully");
}

crow::response UserChatController::postEditChatSession(const crow::request& req, UserId user_id, ChatSessionId chat_session_id) {
    crow::query_string params("?" + req.body);
    std::string description = params.get("description") ? params.get("description") : "";

    if (description.empty()) {
        return crow::response(400, "Missing required fields");
    }

    std::optional<std::string> additionalInfo = params.get("additional_info")
        ? std::optional<std::string>{params.get("additional_info")}
        : std::nullopt;

    UpdateChatSessionCommand updateRequest(chat_session_id, description, additionalInfo);
    updateChatSession_.execute(updateRequest);
    return crow::response(200, "Chat session updated successfully");
}

crow::response UserChatController::postDeleteChatSession(const crow::request& req, UserId user_id, ChatSessionId chat_session_id) {
    DeleteChatSessionCommand deleteRequest(chat_session_id);
    deleteChatSession_.execute(deleteRequest);
    return crow::response(200, "Chat session deleted successfully");
}

crow::response UserChatController::getMessages(const crow::request& req, UserId user_id, ChatSessionId chat_session_id) {
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    std::vector<ChatMessageDto> messages = getChatHistory_.execute(GetChatHistoryQuery(chat_session_id));
    crow::json::wvalue::list messageList;
    for (const ChatMessageDto& message : messages) {
        messageList.push_back(static_cast<crow::json::wvalue>(message));
    }
    crow::mustache::context ctx;
    ctx["messages"] = std::move(messageList);
    ctx["chat_session_id"] = chat_session_id.getId();
    if (userOpt) ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);
    return crow::response(crow::mustache::load("chat.html").render(ctx));
}

crow::response UserChatController::postNewMessage(const crow::request& req, UserId user_id, ChatSessionId chat_session_id) {
    crow::query_string params("?" + req.body);
    std::string message = params.get("message") ? params.get("message") : "";

    if (message.empty()) {
        return crow::response(400, "Missing required fields");
    }

    SendChatMessageCommand sendRequest(chat_session_id, message);
    std::string assistantReply = sendChatMessage_.execute(sendRequest);
    crow::json::wvalue responseBody;
    responseBody["reply"] = assistantReply;
    return crow::response(200, responseBody);
}
