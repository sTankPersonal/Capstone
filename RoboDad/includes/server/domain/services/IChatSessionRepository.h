#pragma once
#include "IRepository.h"
#include "ChatSession.h"
#include "ChatSessionId.h"
#include "UserId.h"
#include <vector>

class IChatSessionRepository : public IRepository<ChatSession, ChatSessionId> {
public:
    virtual std::vector<ChatSession> findByUserId(const UserId& userId) = 0;
};
