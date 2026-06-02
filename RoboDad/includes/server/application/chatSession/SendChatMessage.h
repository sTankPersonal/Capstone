#pragma once
#include "IChatSessionRepository.h"
#include "IChatMessageRepository.h"
#include "ILlmPersonaRepository.h"
#include "ITransactionRepository.h"
#include "ILlmClient.h"
#include "IPromptBuilder.h"
#include "ChatSessionId.h"
#include <string>

class SendChatMessage {
    IChatSessionRepository& sessionRepo_;
    IChatMessageRepository& messageRepo_;
    ILlmPersonaRepository&  personaRepo_;
    ITransactionRepository& transactionRepo_;
    ILlmClient&             llmClient_;
    IPromptBuilder&         promptBuilder_;

    int defaultHistoryLimit_; // Limit the number of past messages sent to the LLM for context.
public:
    SendChatMessage(IChatSessionRepository& sessionRepo,
                    IChatMessageRepository& messageRepo,
                    ILlmPersonaRepository&  personaRepo,
                    ITransactionRepository& transactionRepo,
                    ILlmClient&             llmClient,
                    IPromptBuilder&         promptBuilder,
                    int historyLimit = 5);

    // Sends userMessage to the LLM, persists both turns, returns the assistant reply.
    std::string execute(const ChatSessionId& sessionId, const std::string& userMessage);
};
