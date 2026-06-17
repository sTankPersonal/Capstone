#include "application/chatSessions/services/SendChatMessage.h"
#include "ChatMessageId.h"
#include "ChatMessageContent.h"
#include "MessageSenderId.h"
#include "GetFinancialInsights.h"
#include "UuidGenerator.h"
#include <chrono>

SendChatMessage::SendChatMessage(IChatSessionRepository& sessionRepo,
    IChatMessageRepository& messageRepo,
                                 ILlmPersonaRepository&  personaRepo,
    ITransactionRepository& transactionRepo,
                                 ILlmClient&             llmClient,
                                 IPromptBuilder&         promptBuilder,
    int historyLimit)
    : sessionRepo_(sessionRepo)
    , messageRepo_(messageRepo)
    , personaRepo_(personaRepo)
    , transactionRepo_(transactionRepo)
    , llmClient_(llmClient)
    , promptBuilder_(promptBuilder)
    , defaultHistoryLimit_(historyLimit) {}

std::string SendChatMessage::execute(const SendChatMessageCommand& request) {
    auto session = sessionRepo_.findById(request.sessionId);
    if (!session) return {};

    auto persona = personaRepo_.findById(session->getPersonaId());
    const std::string systemPrompt = persona ? persona->getSystemPrompt() : std::string{};

    auto history = messageRepo_.findByChatSessionId(request.sessionId, defaultHistoryLimit_);

    // Build insights locally (no interface required)
    GetFinancialInsights insightsService(transactionRepo_);
    auto insightsOpt = insightsService.execute(GetFinancialInsightsQuery(session->getUserId()));

    const std::string enriched = promptBuilder_
        .withUserMessage(request.userMessage)
        .withInsights(insightsOpt.value_or(FinancialInsightsDto{}))
        .build();


    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };

    // "user" and "assistant" are the seeded message_sender_id values.
    messageRepo_.create(ChatMessage(
        ChatMessageId(UuidGenerator::generate()),
        request.sessionId,
        MessageSenderId("user"),
        ChatMessageContent(request.userMessage),
        today
    ));

    std::string response;
    try {
        response = llmClient_.generate(systemPrompt, history, enriched);
    }
    catch (const std::exception& ex) {
        response = "I'm running into an issue and can't generate a full response right now.";
    }

    //Save assistant message (real or fallback)
    messageRepo_.create(ChatMessage(
        ChatMessageId(UuidGenerator::generate()),
        request.sessionId,
        MessageSenderId("assistant"),
        ChatMessageContent(response),
        today
    ));

    // Bump the session's updatedAt so the list view shows recency.
    ChatSession updated(
        request.sessionId,
        session->getUserId(),
        session->getPersonaId(),
        session->getSessionDescription(),
        session->getCreatedAt(),
        today
    );
    sessionRepo_.update(updated);

    return response;
}
