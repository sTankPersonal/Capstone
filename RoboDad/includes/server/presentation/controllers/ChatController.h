#pragma once

#include <crow.h>
#include <optional>
#include <string>
#include "application/chatSession/CreateChatSession.h"
#include "application/chatSession/DeleteChatSession.h"
#include "application/chatSession/GetChatHistory.h"
#include "application/chatSession/GetChatSession.h"
#include "application/chatSession/ListChatSessions.h"
#include "application/chatSession/SendChatMessage.h"
#include "application/chatSession/UpdateChatSession.h"
#include "IChatSessionRepository.h"
#include "IChatMessageRepository.h"
#include "ILlmPersonaRepository.h"
#include "ITransactionRepository.h"
#include "infrastructureServices/apiClient/ILlmClient.h"
#include "infrastructureServices/apiClient/IPromptBuilder.h"
#include "infrastructureServices/security/IJwtService.h"

class ChatController {
    CreateChatSession  createSession_;
    DeleteChatSession  deleteSession_;
    GetChatHistory     getChatHistory_;
    GetChatSession     getSession_;
    ListChatSessions   listSessions_;
    SendChatMessage    sendMessage_;
    UpdateChatSession  updateSession_;
    IJwtService&       jwt_;

    std::optional<std::string> extractUserId(const crow::request& req) const;

public:
    ChatController(IChatSessionRepository& sessionRepo,
                   IChatMessageRepository& messageRepo,
                   ILlmPersonaRepository&  personaRepo,
                   ITransactionRepository& transactionRepo,
                   ILlmClient&             llmClient,
                   IPromptBuilder&         promptBuilder,
                   IJwtService&            jwt);

    void registerRoutes(crow::SimpleApp& app);
};
