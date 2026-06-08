#pragma once
#include "sharedKernel/presentation/IController.h"
#include "presentation/AppType.h"
#include "domain/valueObjects/ids/ChatSessionId.h"
#include "domain/valueObjects/ids/UserId.h"

#include "application/chatSessions/services/CreateChatSession.h"
#include "application/chatSessions/services/DeleteChatSession.h"
#include "application/chatSessions/services/GetChatHistory.h"
#include "application/chatSessions/services/GetChatSession.h"
#include "application/chatSessions/services/ListChatSessions.h"
#include "application/chatSessions/services/SendChatMessage.h"
#include "application/chatSessions/services/UpdateChatSession.h"
#include "application/users/services/GetUserProfile.h"

/*
    GET: /user/chats - Loads the user chat sessions page
    GET: /user/chats/<chat_session_id> - Loads the chat session details page for the specified chat session

    GET: /user/chats/new - Loads the new chat session page
        - LlmPersonaId
        - ChatSessionDescription
            - description
            - additionalInfo (optional)
    GET: /user/chats/<chat_session_id>/edit - Loads the chat session edit page for the specified chat session
    GET: /user/chats/<chat_session_id>/delete - Loads the chat session deletion confirmation page for the specified chat session

    POST: /user/chats/new - Submits the new chat session form
    POST: /user/chats/<chat_session_id>/edit - Submits the chat session edit form
    POST: /user/chats/<chat_session_id>/delete - Submits the chat session deletion confirmation form

    GET: /user/chats/<chat_session_id>/messages - Loads the messages for the specified chat session
    POST: /user/chats/<chat_session_id>/messages - Submits a new message to the specified chat session
*/

class UserChatController : public IController<RoboDadApp> {
    CreateChatSession createChatSession_;
    UpdateChatSession updateChatSession_;
    DeleteChatSession deleteChatSession_;
    GetChatSession getChatSession_;
    ListChatSessions listChatSessions_;
    GetChatHistory getChatHistory_;
    SendChatMessage sendChatMessage_;
    GetUserProfile getUserProfile_;
public:
    UserChatController(const CreateChatSession& createChatSession, const UpdateChatSession& updateChatSession, const DeleteChatSession& deleteChatSession, const GetChatSession& getChatSession, const ListChatSessions& listChatSessions, const GetChatHistory& getChatHistory, const SendChatMessage& sendChatMessage, const GetUserProfile& getUserProfile);
    void registerRoutes(RoboDadApp& app) override;

    crow::response getChatSessions(const crow::request& req, UserId user_id);
    crow::response getChatSessionDetails(const crow::request& req, UserId user_id, ChatSessionId chat_session_id);

    crow::response getNewChatSessionPage(const crow::request& req, UserId user_id);
    crow::response getEditChatSessionPage(const crow::request& req, UserId user_id, ChatSessionId chat_session_id);
    crow::response getDeleteChatSessionPage(const crow::request& req, UserId user_id, ChatSessionId chat_session_id);

    crow::response postNewChatSession(const crow::request& req, UserId user_id);
    crow::response postEditChatSession(const crow::request& req, UserId user_id, ChatSessionId chat_session_id);
    crow::response postDeleteChatSession(const crow::request& req, UserId user_id, ChatSessionId chat_session_id);

    crow::response getMessages(const crow::request& req, UserId user_id, ChatSessionId chat_session_id);
    crow::response postNewMessage(const crow::request& req, UserId user_id, ChatSessionId chat_session_id);
};