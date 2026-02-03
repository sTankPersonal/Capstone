#include "application/chatSession/SendChatMessage.h"
#include "ChatMessageId.h"
#include "ChatMessageContent.h"
#include "MessageSenderId.h"
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

std::string SendChatMessage::execute(const ChatSessionId& sessionId,
                                     const std::string&   userMessage) {
    auto session = sessionRepo_.findById(sessionId);
    if (!session) return {};

    auto persona = personaRepo_.findById(session->getPersonaId());
    const std::string systemPrompt = persona ? persona->getSystemPrompt() : std::string{};

    auto history = messageRepo_.findByChatSessionId(sessionId, defaultHistoryLimit_);

    auto transactions = transactionRepo_.findByUserId(session->getUserId());

    const std::string enriched = promptBuilder_
        .withUserMessage(userMessage)
        .withTransactionContext(transactions)
        .build();

    const std::string response = llmClient_.generate(systemPrompt, history, enriched);

    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };

    // "user" and "assistant" are the seeded message_sender_id values.
    messageRepo_.create(ChatMessage(
        ChatMessageId(UuidGenerator::generate()),
        sessionId,
        MessageSenderId("user"),
        ChatMessageContent(userMessage),
        today
    ));
    messageRepo_.create(ChatMessage(
        ChatMessageId(UuidGenerator::generate()),
        sessionId,
        MessageSenderId("assistant"),
        ChatMessageContent(response),
        today
    ));

    // Bump the session's updatedAt so the list view shows recency.
    ChatSession updated(
        sessionId,
        session->getUserId(),
        session->getPersonaId(),
        session->getSessionDescription(),
        session->getCreatedAt(),
        today
    );
    sessionRepo_.update(updated);

    return response;
}
