#pragma once
#include "IUseCase.h"
#include "SendChatMessageCommand.h"
#include "IChatSessionRepository.h"
#include "IChatMessageRepository.h"
#include "ILlmPersonaRepository.h"
#include "ITransactionRepository.h"
#include "ILlmClient.h"
#include "IPromptBuilder.h"
#include <string>

class SendChatMessage : public IUseCase<SendChatMessageCommand, std::string> {
    IChatSessionRepository& sessionRepo_;
    IChatMessageRepository& messageRepo_;
    ILlmPersonaRepository&  personaRepo_;
    ITransactionRepository& transactionRepo_;
    ILlmClient&             llmClient_;
    IPromptBuilder&         promptBuilder_;
    int defaultHistoryLimit_;
public:
    SendChatMessage(IChatSessionRepository& sessionRepo,
                    IChatMessageRepository& messageRepo,
                    ILlmPersonaRepository&  personaRepo,
                    ITransactionRepository& transactionRepo,
                    ILlmClient&             llmClient,
                    IPromptBuilder&         promptBuilder,
                    int historyLimit = 5);

    std::string execute(const SendChatMessageCommand& request) override;
};
