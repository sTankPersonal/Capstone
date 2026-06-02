#pragma once
#include "IChatSessionRepository.h"
#include "ChatSession.h"
#include "UserId.h"
#include <vector>

class ListChatSessions {
    IChatSessionRepository& repo_;
public:
    explicit ListChatSessions(IChatSessionRepository& repo);
    std::vector<ChatSession> execute(const UserId& userId);
};
